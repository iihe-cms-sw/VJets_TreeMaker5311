#!/usr/bin/env python
# T.Seva 17 May first try to have an automatic scipt for submiting jobs
# run it like: python resubmitCRABJObs.py multicrab_ALL_MC_NTUPLES &
# multicrab_ALL_MC_NTUPLES == is multicrab directory
# you can also give:  multicrab_ALL_MC_NTUPLES/DMu_2012A then it runs crab
# hopefull useful !
# for N-rechecks you need to set it in the external loop

import os, sys, string
import re


#---------------------------------------------------------------------------------------
# colors for printing purposes
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

#---------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------
# print errorMsg in red
def printErr(errorMsg):
    sys.exit(bcolors.FAIL + errorMsg + bcolors.ENDC)

#---------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------
# print infoMsg in green
def printInfo(infoMsg):
    print bcolors.OKGREEN + infoMsg + bcolors.ENDC

#---------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------
# print execMsg in green
def printExec(execMsg):
    print bcolors.OKBLUE + execMsg + bcolors.ENDC

#---------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------
def resubmission(crabDirectory):
    # look for subdirectories indicating if it is a multicrab or not
    dirPath = sys.argv[1].strip("/")
    isMultiCrab = (len(dirPath.split("/")) == 1)

    printInfo("is multicrab: " + str(isMultiCrab))

    # output name for the status file
    status = dirPath + ".status"

    # create some commands for later use (or not)
    cmdCrabGet = ""
    cmdCrabSta = ""
    cmdCrabRep = ""

    if (isMultiCrab):
    	cmdCrabGet = "multicrab -get -c " + dirPath
    	cmdCrabSta = "multicrab -status -c " + dirPath + " > " + status
    	cmdCrabRep = "multicrab -report -c " + dirPath
    else:
    	cmdCrabGet = "crab -get -c " + dirPath
    	cmdCrabSta = "crab -status -c " + dirPath + " > " + status
    	cmdCrabRep = "crab -report -c " + dirPath

    # little magic
    cmdport = "export GLOBUS_TCP_PORT_RANGE=\"22236,25000\""
    printExec("Executing " + cmdport)
    os.system(cmdport)

    # get the status
    cmdRm = "rm -f " + status
    printExec("Executing " + cmdRm)
    os.system(cmdRm)
    printExec(cmdCrabSta)
    os.system(cmdCrabSta)


    statusFile = open(status, "r")
    error = False
    Aborted = False
    Cancelled = False
    #cmd = 'crab -resubmit '
    cmd = 'crab -forceResubmit '
    dirCrab = ''

    for line in statusFile:

        # Get the working directory. Second passage is the right one.
        if "working directory" in line:
            dirCrab = line.split("working directory")[1].strip()
            continue

        # Quit if erver is unable to connect
        if "Server status decoding problem" in line:
            printErr("Server status decoding problem, lets skip resubmiting")
            break

        # Fetch the summary line, exit code, and number of jobs with this exit code
        # set the error flag to true
        if "Jobs with Wrapper Exit Code" in line:
            print line.strip()
            splitter = re.compile(r'\d+')
            match1 = splitter.findall(line)
            if int(match1[0]) > 0:
                if int(match1[1]) > 0:
                    print "Following error found ", match1[1], " total ", match1[0], " times"
                    error = True

        # When error exit code have been read, proceed to resubmission
        if error:
            jobsRes = ""
            if "List" in line:
                jobsRes = line.split(":")[1].strip()
                printExec("Executing: " + cmd + jobsRes + " -c " + dirCrab)
                os.system(cmd + jobsRes + " -c " + dirCrab)
                error = False


        # Check the list of aborted jobs
        if "Jobs Aborted" in line:
            splitter = re.compile(r'\d+')
            match1 = splitter.findall(line)
            if int(match1[0]) > 0:
                print "Aborted jobs ", match1[0], " times"
                Aborted = True

        # and resubmit
        if Aborted:
            jobsRes = ""
            if "List" in line:
                if "resubmit" in line:
    		        print "\n"
                else:
                    jobsRes = line.split(":")[1].strip()
                    printExec("Executing " + cmd + jobsRes + " -c " + dirCrab)
                    os.system(cmd + jobsRes + " -c " + dirCrab)
                    Aborted = False


        if "Jobs Cancelled"  in line:
            print line.strip()
            splitter = re.compile(r'\d+')
            match1 = splitter.findall(line)
            print match1
            if int(match1[0]) > 0:
                print "Cancelled jobs ", match1[0], " times"
                Cancelled = True

        if Cancelled:
            jobsRes = ""
            if "List" in line:
                if "resubmit" in line:
                    print "\n"
                else:
                    jobsRes = line.split(":")[1].strip()
                    printExec("Executing " + cmd + jobsRes + " -c " + dirCrab)
                    os.system(cmd + jobsRes + " -c " + dirCrab)
                    Cancelled = False


    print "\nOne more resubmition done"

#---------------------------------------------------------------------------------------


# check the crab directory
if len(sys.argv) < 2:
    errorMsg = "Error: no argument has been provided\n"
    errorMsg += "Please give the multicrab directory you want to run on\n"
    printErr(errorMsg)
elif not os.path.exists(sys.argv[1]):
    errorMsg = "Error: crab directory does not exist\n"
    printErr(errorMsg)
else:
    printInfo("Start resubmission for directory " + sys.argv[1])

resubmission(sys.argv[1])
