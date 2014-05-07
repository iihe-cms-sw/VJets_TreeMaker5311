VJets_TreeMaker5311
===================

Installation
------------

Before cloning this repository, you need to set up your CMSSW environment.
For this, execute the 4 following commands:

    mkdir VJets_ntuples
    cd VJets_ntuples
    cmsrel CMSSW_5_3_11
    cd CMSSW_5_3_11/src
    cmsenv
    scram b -j4

If every thing went fine, you have a working CMSSW environment. Now you can
get the git repository into the src directory by typing:

    git clone git@github.com:iihe-cms-sw/VJets_TreeMaker5311.git ./

(this can take several minutes depending on your internet connection).

and finally, you can compile the masta-piece:

    scram b -j4

(this can take 15 to 20 minutes).


Usage
-----

The main (and in principle the only) directory of interest for the user willing to 
produce ntuples is at the following path:

    cd UserCode/llvv_fwk/test/grid/

This directory contains all the necessary files to create and submit jobs for V + jets
analysis (Z + jets or W + jets analysis, the code is the same).

But before submitting anything, you need to configure the crab files to your needs. Let
us consider the crab.cfg file.

For iihe users (m machines), this can remain as it is (it is a copy of crab_iihe.cfg).
Your output root files will be stored on your storage element in a directory called VJets_ntuples.

For cern users (lxplus machines), you can have a look at the file called crab_pedro.cfg, edit it
to suit your needs and copy it to the a file called crab.cfg (because it is really the crab.cfg 
that is used).

Now you can submit, for example the TTJets sample. For this, you can have a look at the file
called multicrab_VJets_TTJets.cfg. It contains the configuration for what you want to do.
When you are happy with it, do the following:

    multicrab -create -cfg multicrab_VJets_TTJets

This will create the jobs to be submitted to create the TTJets ntuples. Once this is done,
you can submit the jobs by group of 500 by typing:

    multicrab -submit 1-500 -c multicrab_VJets_TTJets
    multicrab -submit 501-1000 -c multicrab_VJets_TTJets

etc...

Now you must check, from time to time the status of the jobs by typing:

    multicrab -status -c multicrab_VJets_TTJets

If you see jobs with exit code different than 0, you will have to resubmit them by typing:

    multicrab -forceResubmit 1,405 -c multicrab_VJets_TTJets

to resubmit jobs 1 and 405 (for example).

Once all the jobs are done, you must check that you have indeed all the corresponding root
files saved in your storage element. Check that you have the same number of root files as
the number of successful jobs. Check for any duplicate file (remove the older one if any).

When everything is consistent, it is time to get the output:

    multicrab -get -c multicrab_VJets_TTJets

And finally, retrieve the information about the number of events proceed:

    multicrab -report -c multicrab_VJets_TTJets

If you arrive at this point, then you have successfully produced the ntuples for the TTJets
sample. You can proceed with the other background, the DY sample, and the data.
Note that for the data you must compute the luminosity after the report command. For this, 
use the following command:

    pixelLumiCalc.py -i path/to/lumiSummary.json overview




Good luck
