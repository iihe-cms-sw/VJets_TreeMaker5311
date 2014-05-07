#!/usr/bin/env python
# T.Seva 17 May first try to have an automatic scipt for submiting jobs
# run it like: python resubmitCRABJObs.py multicrab_ALL_MC_NTUPLES &
# multicrab_ALL_MC_NTUPLES == is multicrab directory
# you can also give:  multicrab_ALL_MC_NTUPLES/DMu_2012A then it runs crab
# hopefull useful !
# for N-rechecks you need to set it in the external loop
import os, sys, string
import re


print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)

if len(sys.argv) < 2 :
	sys.exit("give me multicrab directory you want to run on")

if not os.path.exists(sys.argv[1]):
	sys.exit("this crab directory does not exist")

TempIn = sys.argv[1].strip("/").split("/")
print TempIn
print "lenght of input:", len(TempIn)
isMultiCrab = 0

if "multicrab" in sys.argv[1]:
	isMultiCrab = 1

if len(TempIn) > 1 :
	print " found longer input"
	isMultiCrab = 0

print "is multicrab:", isMultiCrab

CMDcrabGet = ""
CMDcrabSta = ""
CMDcrabRep = ""

fileOut = TempIn[0] + ".txt"
cmdRm = "rm -f " + fileOut
os.system(cmdRm)

if (isMultiCrab):
	print "lets do multicrab"
	CMDcrabGet = "multicrab -get -c " + sys.argv[1]
	CMDcrabSta = "multicrab -status -c " + sys.argv[1] + " > " + fileOut
	CMDcrabRep = "multicrab -report -c " + sys.argv[1]
else:
	print "lets do crab"
	CMDcrabGet = "crab -get -c " + sys.argv[1]
	CMDcrabSta = "crab -status -c " + sys.argv[1] + " > " + fileOut
	CMDcrabRep = "crab -report -c " + sys.argv[1]

cmdport = "export GLOBUS_TCP_PORT_RANGE=\"22236,25000\""
os.system(cmdport)

print CMDcrabGet
#os.system(CMDcrabGet)
print CMDcrabSta
os.system(CMDcrabSta)
print " WE HAVE STATUS"
print CMDcrabRep
#os.system(CMDcrabRep)
#print " WE HAVE REPORT"

statusFile = open(fileOut, "r")
error = False
Aborted = False
Cancelled = False
#cmd = 'crab -resubmit '
cmd = 'crab -forceResubmit '
dirCrab = ''

for line in statusFile:

    # Get the working directory. Second passage is the right one.
    if "working directory" in line:
        print " lets get our directory"
        dirCrab = line.split("working directory")[1].strip()
        continue

    # Quit if erver is unable to connect
    if "Server status decoding problem" in line:
        print "Server status decoding problem, lets skip resubmiting"
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
            print cmd + jobsRes + " -c " + dirCrab
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
                print cmd + jobsRes + " -c " + dirCrab
                #os.system(cmd + jobsRes + " -c " + dirCrab)
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
                print cmd + jobsRes + " -c " + dirCrab
                os.system(cmd + jobsRes + " -c " + dirCrab)
                Cancelled = False


print "\nONE MORE RESUBMITION DONE"

