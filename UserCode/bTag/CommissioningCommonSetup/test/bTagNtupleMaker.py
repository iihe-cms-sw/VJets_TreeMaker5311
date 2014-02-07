#########################
#####    Settings   #####
#########################
DATALIST=[
#"/QCD_Pt-15to30_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v3/AODSIM",
#"/QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v3/AODSIM",
#"/QCD_Pt-50to80_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM"
#"/QCD_Pt-80to120_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v3/AODSIM",
#"/QCD_Pt-120to170_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
#"/QCD_Pt-170to300_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
#"/QCD_Pt-300to470_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
#"/QCD_Pt-470to600_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
#"/QCD_Pt-600to800_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
#"/QCD_Pt-800to1000_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM"
#"/QCD_Pt-15to30_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-50to80_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-80to120_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-120to170_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-170to300_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-300to470_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-470to600_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-600to800_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG",
#"/QCD_Pt-800to1000_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START44_V5-v1/GEN-SIM-RECODEBUG"
#"/QCD_Pt-120to170_TuneZ2_7TeV_pythia6/Fall11-PU_S6-START44_V5-v1/AODSIM",
#"/QCD_Pt-170to300_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM",
#"/QCD_Pt-300to470_TuneZ2_7TeV_pythia6/Fall11-PU_S6-START44_V5-v1/AODSIM",
#"/QCD_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/Fall11-Peak32PU_START42_V14B-v1/GEN-SIM-RECO",
#"/Jet/Run2011B-PromptReco-v1/AOD",
#"/Jet/Run2012A-PromptReco-v1/AOD",
#"/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/Summer12-428SLHCstd_DESIGN42_V17-v3/AODSIM",
"/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/Summer12-UpgradeStdGeom2_DR428-PU50-DESIGN42_V17S-v1/GEN-SIM-DIGI-RECO"
#"/BTag/Run2012A-PromptReco-v1/AOD",
#"/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-NoPileUp_START52_V9-v5/AODSIM",
#"/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V5-v1/AODSIM"
          ]

import os,sys
CMSSW_Version_=os.getenv("CMSSW_VERSION")
Version_Number_=CMSSW_Version_.split('_')
castorhome_=os.getenv("CASTOR_HOME")

if sys.argv[-1] == "setup":
    os.system( 'rfchmod 775 %s' % castorhome_ )
    CMSSW_home_=os.getenv("CMSSW_BASE")
    if ( int(Version_Number_[1])<4 ) or ( int(Version_Number_[1])==4 and int(Version_Number_[2])<1 ) or ( int(Version_Number_[1])==4 and int(Version_Number_[2])==1 and int(Version_Number_[3])<2 ):
        os.system( 'sed -i "s/.*#define GetPUFromEarlyThan_4_1_2/#define GetPUFromEarlyThan_4_1_2/g" %s/src/bTag/CommissioningCommonSetup/plugins/TagNtupleProducer.cc' % CMSSW_home_ )
    else:
        os.system( 'sed -i "s/.*#define GetPUFromEarlyThan_4_1_2/\/\/#define GetPUFromEarlyThan_4_1_2/g" %s/src/bTag/CommissioningCommonSetup/plugins/TagNtupleProducer.cc' % CMSSW_home_ )
    if ( int(Version_Number_[1])<4 ) or ( int(Version_Number_[1])==4 and int(Version_Number_[2])<4 ):
        os.system( 'sed -i "s/.*#define GetPUFromEarlyThan_4_4_0/#define GetPUFromEarlyThan_4_4_0/g" %s/src/bTag/CommissioningCommonSetup/plugins/TagNtupleProducer.cc' % CMSSW_home_ )
    else:
        os.system( 'sed -i "s/.*#define GetPUFromEarlyThan_4_4_0/\/\/#define GetPUFromEarlyThan_4_4_0/g" %s/src/bTag/CommissioningCommonSetup/plugins/TagNtupleProducer.cc' % CMSSW_home_ )
    if ( int(Version_Number_[1])<4 ) or ( int(Version_Number_[1])==4 and int(Version_Number_[2])<2 ) or ( int(Version_Number_[1])==4 and int(Version_Number_[2])==2 and int(Version_Number_[3])<3 ):
        os.system( 'cd %s/src/;cvs co -r V01-06-00 RecoBTag/SecondaryVertex;cvs co -r CMSSW_4_2_3 RecoVertex/PrimaryVertexProducer;cvs co -r CMSSW_4_2_3 TrackingTools/TrajectoryState' % CMSSW_home_ )
    if ( int(Version_Number_[1])>5 ) or ( int(Version_Number_[1])==5 and int(Version_Number_[2])>=2 ):
        os.system( 'rm %s/src/bTag/CommissioningCommonSetup/plugins/PFJetSelector.cc' % CMSSW_home_ )

    print "Current main code settings:"
    os.system( 'head %s/src/bTag/CommissioningCommonSetup/plugins/TagNtupleProducer.cc' % CMSSW_home_ )
    os.system( 'cd %s/src/bTag/CommissioningCommonSetup/;scramv1 b;cd test' % CMSSW_home_ )
    sys.exit()

if ( int(Version_Number_[1])<5 ) or ( int(Version_Number_[1])==5 and int(Version_Number_[2])<2 ):
    usePFnoPUdefault=False
    print "I am going to calculate the Rho parameter for PFnoPU because the CMSSW version is ",CMSSW_Version_
else:
    usePFnoPUdefault=True

if ( int(Version_Number_[1])<4 ) or ( int(Version_Number_[1])==4 and int(Version_Number_[2])<3 ):
    newerthan430=False
else:
    newerthan430=True

HLTProc="RECO"
runOnMC = True
usePFnoPU =True

import FWCore.ParameterSet.Config as cms

JetCut=cms.string("pt > 30.0 && abs(eta) < 2.4 && neutralHadronEnergyFraction < 0.99 && neutralEmEnergyFraction < 0.99 && nConstituents > 1 && chargedHadronEnergyFraction > 0.0 && chargedMultiplicity > 0.0 && chargedEmEnergyFraction < 0.99")

	
# It will select the events based on the TriggerSelections. The ntuple will only save the triggers whose name matches any entry in TriggerSelections
TriggerSelections=cms.vstring("HLT_*Jet*","HLT_HT*")

############################
###    Main Program   ######
############################
process = cms.Process("validation")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("RecoBTag.Configuration.RecoBTag_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

if newerthan430:
    from Configuration.AlCa.autoCond import autoCond # for >=CMSSW_4_3_0
else:
    from Configuration.PyReleaseValidation.autoCond import autoCond  # for <CMSSW_4_3_0

if runOnMC:
  process.GlobalTag.globaltag = autoCond['startup']
else:
  process.GlobalTag.globaltag = autoCond['com10']

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("rfio:/castor/cern.ch/user/z/zhangjin/Test_TTbar_UpgradeStdGeom2_DR428-PU50-DESIGN42_V17S-v1.root")
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

########### Trigger selection ###########
# Select events based on the HLT triggers....singleJet and BTag triggers
# Use the instructions provided at:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/TriggerResultsFilter
# This eases the trigger selection for different HLT menus and also takes care of wildcard and trigger versioning
#######

import HLTrigger.HLTfilters.triggerResultsFilter_cfi as hlt
process.JetHLTFilter = hlt.triggerResultsFilter.clone(
   triggerConditions = TriggerSelections,
   hltResults = cms.InputTag("TriggerResults","",HLTProc),
   l1tResults = cms.InputTag( "" ),
   throw = cms.bool( False) #set to false to deal with missing triggers while running over different trigger menus
)


########### Event cleaning ###########
#Require a good vertex

from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector

process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )
    
    
process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi")
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                      vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                      minimumNDOF = cms.uint32(4) ,
 		      maxAbsZ = cms.double(24), 
 		      maxd0 = cms.double(2)	
                     )


#Filter for removing scraping events
process.noscraping = cms.EDFilter("FilterOutScraping",
                                applyfilter = cms.untracked.bool(True),
                                debugOn = cms.untracked.bool(False),
                                numtrack = cms.untracked.uint32(10),
                                thresh = cms.untracked.double(0.25)
                                )
				
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

process.ak5PFJetsJEC = process.ak5PFJetsL2L3.clone(
    src = 'ak5PFJets', 
    correctors = ['ak5PFL2L3']
)

process.PFJetsFilter = cms.EDFilter("PFJetSelector",
  src = cms.InputTag("ak5PFJetsJEC"),
  cut = JetCut,
  filter = cms.bool(True)
)

#Filter to remove pthat>15 events in MinBias samples
process.pthat_filter = cms.EDFilter("MCProcessFilter",
	  MaxPthat = cms.untracked.vdouble(15.0),
	  ProcessID = cms.untracked.vint32(0),
	  MinPthat = cms.untracked.vdouble(1.0)
)

process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")


#######calculate Rho for PFnoPU (<CMSSW_5_2_0)
if not usePFnoPUdefault:
    from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
    process.kt6PFJets = kt4PFJets.clone(
        src = cms.InputTag( 'particleFlow'),
        rParam = cms.double(0.6),
        doRhoFastjet = cms.bool(True),
        Rho_EtaMax = cms.double(2.5)
        )
    # CV: need to rerun 'ak5PFJets' module with jet area computation enabled,
    #     since it has not been enabled per default in CMSSW_4_2_x
    #    (if the jet area of 'ak5PFJets' is zero, the L1FastjetCorrector::correction function always returns 1.0)
#from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
    process.load("RecoJets.JetProducers.ak5PFJets_cfi")
    process.ak5PFJets.doAreaFastjet = cms.bool(True)
    process.redoJets = cms.Sequence( process.kt6PFJets * process.ak5PFJets )
else:
    process.redoJets = cms.Sequence()
#######end of calculation of Rho

########### Jet selection and JEC use PFtoPAT and PFnoPU ###########
#JetID

if usePFnoPU :
    process.out = cms.OutputModule("PoolOutputModule",
                                   outputCommands = cms.untracked.vstring('drop *'),
                                   fileName = cms.untracked.string('EmptyFile.root')
                                   )
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    from PhysicsTools.PatAlgos.tools.pfTools import *
    postfix="PF2PAT"
    usePF2PAT(process,runPF2PAT=True, jetAlgo="AK5", runOnMC=runOnMC, postfix=postfix) 
    applyPostfix(process,"patJetCorrFactors",postfix).payload = cms.string('AK5PFchs')
    process.pfPileUpPF2PAT.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
    process.pfPileUpPF2PAT.checkClosestZVertex = cms.bool(False)
    if not usePFnoPUdefault:
        process.pfPileUpPF2PAT.Enable = True
        process.pfJetsPF2PAT.doAreaFastjet = True
        process.pfJetsPF2PAT.doRhoFastjet = False
        process.patJetCorrFactorsPF2PAT.rho = cms.InputTag("kt6PFJets", "rho")
    process.selectedPatJetsPF2PAT.cut = JetCut
    process.JECAlgo = cms.Sequence( getattr(process,"patPF2PATSequence"+postfix) )
    JetID=cms.InputTag("selectedPatJetsPF2PAT")
else:
    process.JECAlgo = cms.Sequence( process.ak5PFJetsJEC*process.PFJetsFilter )
    JetID=cms.InputTag("PFJetsFilter")

 





########### Rerun b-tagging for PF Jets ###########
process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")

process.ak5PFJetTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
   process.j2tParametersVX,
   jets = JetID
)


process.standardImpactParameterPFTagInfos = process.impactParameterTagInfos.clone(
  jetTracks = "ak5PFJetTracksAssociatorAtVertex"
)


process.standardSecondaryVertexPFTagInfos = process.secondaryVertexTagInfos.clone(
  trackIPTagInfos = "standardImpactParameterPFTagInfos"
)

process.standardSecondaryVertex3TrkPFTagInfos = process.standardSecondaryVertexPFTagInfos.clone()
process.standardSecondaryVertex3TrkPFTagInfos.vertexCuts.multiplicityMin = 3

process.standardGhostTrackVertexPFTagInfos = process.standardSecondaryVertexPFTagInfos.clone()
process.standardGhostTrackVertexPFTagInfos.vertexReco = process.ghostTrackVertexRecoBlock.vertexReco
process.standardGhostTrackVertexPFTagInfos.vertexCuts.multiplicityMin = 1

process.standardCombinedSecondaryVertexPF = process.combinedSecondaryVertex.clone()
process.standardCombinedSecondaryVertex3TrkPF = process.combinedSecondaryVertex.clone()

process.standardSecondaryVertexV0PFTagInfos = process.standardSecondaryVertexPFTagInfos.clone()
process.standardSecondaryVertexV0PFTagInfos.vertexCuts.v0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0))

process.standardCombinedSecondaryVertexV0PF = process.standardCombinedSecondaryVertexPF.clone()
process.standardCombinedSecondaryVertexV0PF.trackPairV0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0))
process.standardCombinedSecondaryVertexV0PF.pseudoVertexV0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0)) 
process.standardCombinedSecondaryVertexV0PF.trackMultiplicityMin = 2


process.standardTrackCountingHighEffPFBJetTags = process.trackCountingHighEffBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"))
)


process.standardTrackCountingHighPurPFBJetTags = process.trackCountingHighPurBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"))
)

process.standardJetProbabilityPFBJetTags = process.jetProbabilityBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos")) 
)

process.standardJetBProbabilityPFBJetTags = process.jetBProbabilityBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos")) 
)

process.standardSimpleSecondaryVertexHighEffPFBJetTags = process.simpleSecondaryVertexHighEffBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardSimpleSecondaryVertexHighPurPFBJetTags = process.simpleSecondaryVertexHighPurBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardGhostTrackPFBJetTags = process.ghostTrackBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"),
	                         cms.InputTag("standardGhostTrackVertexPFTagInfos"))
)

process.standardCombinedSecondaryVertexPFBJetTags = process.combinedSecondaryVertexBJetTags.clone(
  jetTagComputer = cms.string('standardCombinedSecondaryVertexPF'),
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"),
                           cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardCombinedSecondaryVertexMVAPFBJetTags = process.combinedSecondaryVertexMVABJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"),
                           cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardSoftMuonPFTagInfos = process.softMuonTagInfos.clone(
  jets = JetID
)

process.standardSoftElectronPFTagInfos = process.softElectronTagInfos.clone(
  jets = JetID
)

process.standardSoftMuonPFBJetTags = process.softMuonBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonPFTagInfos"))
)

process.standardSoftMuonByPtPFBJetTags = process.softMuonByPtBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonPFTagInfos"))
)

process.standardSoftMuonByIP3dPFBJetTags = process.softMuonByIP3dBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonPFTagInfos"))
)

process.standardSoftElectronByPtPFBJetTags = process.softElectronByPtBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftElectronPFTagInfos"))
)

process.standardSoftElectronByIP3dPFBJetTags = process.softElectronByIP3dBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftElectronPFTagInfos"))
)

########### MC truth flavour matching ###########
process.load("PhysicsTools.JetMCAlgos.CaloJetsMCFlavour_cfi")

process.AK5PFbyRef = process.AK5byRef.clone(
  jets = JetID
)

process.AK5PFbyValAlgo = process.AK5byValAlgo.clone(
  srcByReference = "AK5PFbyRef"
)

process.flavourSeq = cms.Sequence(
    process.myPartons *
    process.AK5PFbyRef *
    process.AK5PFbyValAlgo )
########### Generation Level Jet Matching ###########
#stole from PAT
process.load("PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi")
process.patJetGenJetMatch.src = JetID

########### Dump contents into ntuple ###########
process.load("bTag.CommissioningCommonSetup.tagntupleproducer_cfi")

process.standardPFBTagNtuple = process.bTagNtuple.clone()
process.standardPFBTagNtuple.jetSrc = JetID
process.standardPFBTagNtuple.svComputer = cms.InputTag( "standardCombinedSecondaryVertexPF" )
process.standardPFBTagNtuple.TriggerTag = cms.InputTag( "TriggerResults::"+HLTProc)
process.standardPFBTagNtuple.TriggerSelection = TriggerSelections
process.standardPFBTagNtuple.flavorHistory = cms.InputTag("flavorHistoryFilter")
process.standardPFBTagNtuple.getSharedHitInfo = cms.bool(False)
process.standardPFBTagNtuple.jetMCSrc = cms.InputTag( "AK5PFbyValAlgo" )
#set this to FALSE for 39X samples, set it to TRUE for 3_11 samplesstandardPFNtuple.root
process.standardPFBTagNtuple.HepMCTag = cms.untracked.InputTag( "generator" )
process.standardPFBTagNtuple.BSTag = cms.InputTag( "offlineBeamSpot" )
process.standardPFBTagNtuple.SimTruthTag = cms.untracked.InputTag( "g4SimHits" )
process.standardPFBTagNtuple.jetTracks = cms.InputTag( "ak5PFJetTracksAssociatorAtVertex" )
process.standardPFBTagNtuple.SVTagInfos = cms.InputTag( "standardSecondaryVertexPFTagInfos" )
process.standardPFBTagNtuple.IPTagInfos = cms.InputTag( "standardImpactParameterPFTagInfos" )
process.standardPFBTagNtuple.ElectronTagInfos = cms.InputTag( "standardSoftElectronPFTagInfos" )
process.standardPFBTagNtuple.MuonTagInfos = cms.InputTag( "standardSoftMuonPFTagInfos" )
process.standardPFBTagNtuple.Label = cms.string("standardPF")
process.standardPFBTagNtuple.GenJetMatcher = cms.InputTag("patJetGenJetMatch")
process.standardPFBTagNtuple.filename = cms.string("standardPFNtuple.root")
process.standardPFBTagNtuple.TrackMCTruthMatchChi2Cut = cms.double(25)
process.standardPFBTagNtuple.bTagConfig = cms.VPSet(
    cms.PSet(
    alias = cms.string("standardTrackCountingHighEffPFBJetTags"),
    label = cms.InputTag("standardTrackCountingHighEffPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardTrackCountingHighPurPFBJetTags"),
    label = cms.InputTag("standardTrackCountingHighPurPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardJetProbabilityPFBJetTags"),
    label = cms.InputTag("standardJetProbabilityPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardJetBProbabilityPFBJetTags"),
    label = cms.InputTag("standardJetBProbabilityPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSimpleSecondaryVertexHighEffPFBJetTags"),
    label = cms.InputTag("standardSimpleSecondaryVertexHighEffPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSimpleSecondaryVertexHighPurPFBJetTags"),
    label = cms.InputTag("standardSimpleSecondaryVertexHighPurPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardCombinedSecondaryVertexPFBJetTags"),
    label = cms.InputTag("standardCombinedSecondaryVertexPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardGhostTrackPFBJetTags"),
    label = cms.InputTag("standardGhostTrackPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardCombinedSecondaryVertexMVAPFBJetTags"),
    label = cms.InputTag("standardCombinedSecondaryVertexMVAPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSoftMuonPFBJetTags"),
    label = cms.InputTag("standardSoftMuonPFBJetTags")
    ),
    cms.PSet(
    alias = cms.string("standardSoftMuonByIP3dPFBJetTags"),
    label = cms.InputTag("standardSoftMuonByIP3dPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSoftMuonByPtPFBJetTags"),
    label = cms.InputTag("standardSoftMuonByPtPFBJetTags")
    ),
    cms.PSet(
    alias = cms.string("standardSoftElectronByIP3dPFBJetTags"),
    label = cms.InputTag("standardSoftElectronByIP3dPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSoftElectronByPtPFBJetTags"),
    label = cms.InputTag("standardSoftElectronByPtPFBJetTags")
    )
    )

process.svTagInfos = cms.Sequence(
    process.standardSecondaryVertexPFTagInfos +
    process.standardSecondaryVertexV0PFTagInfos + 
    process.standardSecondaryVertex3TrkPFTagInfos +
    process.standardGhostTrackVertexPFTagInfos 
)

process.ipTaggers = cms.Sequence(
    process.standardTrackCountingHighEffPFBJetTags +
    process.standardTrackCountingHighPurPFBJetTags +
    process.standardJetProbabilityPFBJetTags +
    process.standardJetBProbabilityPFBJetTags 
)

process.svTaggers = cms.Sequence(
    process.standardSimpleSecondaryVertexHighEffPFBJetTags +
    process.standardSimpleSecondaryVertexHighPurPFBJetTags +
    process.standardGhostTrackPFBJetTags +
    process.standardCombinedSecondaryVertexPFBJetTags +
    process.standardCombinedSecondaryVertexMVAPFBJetTags 
)

process.slTagInfos = cms.Sequence(
    process.standardSoftMuonPFTagInfos +
    process.softElectronCands+
    process.standardSoftElectronPFTagInfos
)

process.slTaggers = cms.Sequence(
    process.standardSoftMuonPFBJetTags +
    process.standardSoftMuonByPtPFBJetTags +
    process.standardSoftMuonByIP3dPFBJetTags +
    process.standardSoftElectronByPtPFBJetTags +
    process.standardSoftElectronByIP3dPFBJetTags 
)

if runOnMC:
  process.GenHistory = cms.Sequence(
#    process.pthat_filter+
    process.flavorHistorySeq *
    process.flavourSeq *
    process.patJetGenJetMatch
    )
else:
  process.GenHistory = cms.Sequence()

process.plots = cms.Path( 
  process.noscraping + 
#  process.JetHLTFilter + 
  process.offlinePrimaryVertices *
  process.goodOfflinePrimaryVertices *
  process.primaryVertexFilter +
  process.redoJets*
  process.JECAlgo*
  process.GenHistory +
  process.ak5PFJetTracksAssociatorAtVertex  *
  process.standardImpactParameterPFTagInfos  *
  process.svTagInfos *
  process.ipTaggers *
  process.svTaggers *
  process.slTagInfos *
  process.slTaggers *
  process.standardPFBTagNtuple 
  )

## Added for getting the L1 and HLT summary report
process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

process.MessageLogger.categories.append('L1GtTrigReport')
process.MessageLogger.categories.append('HLTrigReport')
#process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.MessageLogger.cerr.threshold = cms.untracked.string('WARNING')

import datetime,re

if sys.argv[0] != "cmsRun" and sys.argv[-1] != "setup":
    castorhome_=castorhome_.replace("/castor/cern.ch/","")
    home_=os.getenv("HOME")
    time=datetime.datetime.now().strftime("%d%b%Y")
    for dataset_ in DATALIST:
        new_py = open('bTagNtupleMaker.py').read()
        crab_py_start_index=new_py.find('HLTProc=')
        crab_py_end_index=new_py.rfind("\nimport datetime,re")
        new_py = new_py[crab_py_start_index:crab_py_end_index-1]
        new_py = "usePFnoPUdefault=%s\n" % str(usePFnoPUdefault) + "newerthan430=%s\n" % str(newerthan430) + new_py
        new_py = re.sub("input = cms.untracked.int32(.*)",
                        "input = cms.untracked.int32(-1)",
                        new_py)
        dirname_=dataset_.split("/")
        if "Run20" not in dataset_:
            print 'Submitting %s as MC' % dataset_
            jobcontrol_='''
total_number_of_events = -1
number_of_jobs = 100
'''
            dirname_=time+"_"+dirname_[1]+"_"+dirname_[2]
            new_py = re.sub("runOnMC = .*\n",
                            "runOnMC = True\n",
                            new_py)
        else:
            print 'Submitting %s as DATA' % dataset_
            jobcontrol_='''
lumi_mask=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-193557_8TeV_PromptReco_Collisions12_JSON.txt
#/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt
total_number_of_lumis = -1
number_of_jobs = 240
'''
            dirname_=time+"_"+dirname_[1]+"_"+dirname_[2]
            new_py = re.sub("runOnMC = .*\n",
                            "runOnMC = False\n",
                            new_py)
        crab_cfg = '''
[CRAB]
jobtype = cmssw
scheduler = glite

[CMSSW]
datasetpath = %(dataset_)s
pset = bTagNtupleMaker_crab.py
output_file = standardPFNtuple.root
%(jobcontrol_)s

[GRID] 
ce_black_list=T2_BR_SPRACE

[USER]
ui_working_dir = %(home_)s/scratch0/crab_bTaggingNtuple_%(dirname_)s
return_data = 0
copy_data = 1
storage_element=srm-cms.cern.ch
storage_path=/srm/managerv2?SFN=/castor/cern.ch
user_remote_dir = %(castorhome_)s/bTaggingNtuple_%(dirname_)s
publish_data=0
''' % locals()
        open('crab.cfg', 'wt').write(crab_cfg)
        open('bTagNtupleMaker_crab.py', 'wt').write(new_py)
        if 'submit' in sys.argv:
            os.system('crab -create -submit all')
            os.system('rm crab.cfg')
            os.system('rm bTagNtupleMaker_crab.py')
