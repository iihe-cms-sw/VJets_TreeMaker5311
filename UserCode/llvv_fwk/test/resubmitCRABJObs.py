#!/usr/bin/env python
# T.Seva 17 May first try to have an automatic scipt for submiting jobs
# run it like: python resubmitCRABJObs.py multicrab_ALL_MC_NTUPLES &
# multicrab_ALL_MC_NTUPLES == is multicrab directory
# you can also give:  multicrab_ALL_MC_NTUPLES/DMu_2012A then it runs crab
# hopefull useful !
# for N-rechecks you need to set it in the external loop
import os, sys, string
import re

import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)
if  len(sys.argv) < 2 :
	sys.exit("give me multicrab directory you want to run on")
if not os.path.exists(sys.argv[1]):
	sys.exit("this crab directory does not exist")
TempIn = sys.argv[1].strip("/").split("/")
print TempIn
print len(TempIn)
isMultiCrab = 'False'
if "multicrab" in sys.argv[1]:
	isMultiCrab = 'True'
if len(TempIn) > 1 :
	isMultiCrab = 'False'
print isMultiCrab
CMDcrabGet=""
CMDcrabSta=""
CMDcrabRep=""
fileOut = "Status_multicrab_data.txt"
if "MC" in sys.argv[1]:
	fileOut = "Status_multicrab_MC.txt"
#if isMultiCrab:
#	CMDcrabGet = "multicrab -get -c " + sys.argv[1]
#	CMDcrabSta = "multicrab -status -c " + sys.argv[1] + " > " + fileOut
#	CMDcrabRep = "multicrab -report -c " + sys.argv[1] 
#else:
CMDcrabGet = "crab -get -c " + sys.argv[1]
#CMDcrabSta = "crab -status -c " + sys.argv[1] + " > Status_multicrab_data.txt"
CMDcrabSta = "crab -status -c " + sys.argv[1] + " > " + fileOut
CMDcrabRep = "crab -report -c " + sys.argv[1] 

cmdRm = "rm  " + fileOut
os.system(cmdRm)
print CMDcrabGet
os.system(CMDcrabGet)
print CMDcrabSta
os.system(CMDcrabSta)
#print CMDcrabRep
#os.system(CMDcrabRep)



print " WE HAVE REPORT"
#ins = open( "Status_multicrab_data.txt", "r" )
ins = open( fileOut, "r" )
array = []
exist = 'False'
exist = 'True'
error ='False'
Aborted ='False'
Cancelled ='False'
#cmd = 'crab -resubmit '
cmd = 'crab -forceResubmit '
for line in ins:
    array.append( line.rstrip('\n') )
#    print line.rstrip('\n')
    if "working directory" in line:
	print " lets get our  directory"
	a = line.split("working directory")
	print a[1].strip()
	dirCrab = a[1].strip()
        exist = 'True'
#    print exist
    if "Server status decoding problem" in line:
	exist = 'False'
	print "Server status decoding problem  lets skip resubmiting"
    if exist == 'True' :
        if "Jobs with Wrapper Exit Code"  in line:
#		print line.strip()
		splitter = re.compile(r'\d+')
		match1 = splitter.findall(line)
#		print match1
		if int(match1[0]) > 0: 
			if int(match1[1]) > 0: 
				print "Following error found ", match1[1] , " total " , match1[0] ," times"
    				error ='True'	
    if exist == 'True' :
        if "Jobs Aborted"  in line:
#               print line.strip()
                splitter = re.compile(r'\d+')
                match1 = splitter.findall(line)
                print match1
                if int(match1[0]) > 0: 
#                        if int(match1[1]) > 0: 
                                print "Aborted jobs ", match1[0] ," times"
                                Aborted ='True'   


    if exist == 'True' :
        if "Jobs Cancelled"  in line:
#               print line.strip()
                splitter = re.compile(r'\d+')
                match1 = splitter.findall(line)
                print match1
                if int(match1[0]) > 0:
#                        if int(match1[1]) > 0:
                                print "Cancelled jobs ", match1[0] ," times"
                                Cancelled ='True'

    if error =='True':
       jobsRes = ""
       if exist == 'True' :
	#if "List of jobs:" in line:
	   if "List" in line:
		bErrors = line.split("List of jobs:")
		jobsRes = bErrors[1].strip()
		cmd += jobsRes 
		cmd += " -c  " + dirCrab  
     		print cmd
		os.system(cmd)
		#exist = 'False'
		error ='False'
		#cmd = 'crab -resubmit '
		cmd = 'crab -forceResubmit '
		print "\n"
		print "\n"
		print "\n"
    if Aborted =='True':
       jobsRes = ""
       if exist == 'True' :
        #if "List of jobs:" in line:
           if "List" in line:
	      if "resubmit" in line:
		print "\n"
	      else:
                bErrors = line.split("List of jobs:")
                jobsRes = bErrors[1].strip()
                cmd += jobsRes
                cmd += " -c  " + dirCrab
                print cmd
		os.system(cmd)
                #exist = 'False'
                error ='False'
		Aborted ='False'
                #cmd = 'crab -resubmit '
                cmd = 'crab -forceResubmit '
                print "\n"
                print "\n"
                print "\n"

    if Cancelled =='True':
       jobsRes = ""
       if exist == 'True' :
	#if "List of jobs:" in line:
	   if "List" in line:
		bErrors = line.split("List of jobs Cancelled:")
		jobsRes = bErrors[1].strip()
		cmd += jobsRes 
		cmd += " -c  " + dirCrab  
     		print cmd
		os.system(cmd)
		#exist = 'False'
		Cancelled ='False'
		#cmd = 'crab -resubmit '
		cmd = 'crab -forceResubmit '
		print "\n"
		print "\n"
		print "\n"
print "\n"
print "\n"
print "ONE MORE RESUBMITION DONE"
print "\n"
print "\n"
