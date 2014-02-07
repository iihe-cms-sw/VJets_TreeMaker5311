import FWCore.ParameterSet.Config as cms

process = cms.Process("validation")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("RecoBTag.Configuration.RecoBTag_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#Global tag for 3_8_X data reprocessing
process.GlobalTag.globaltag = 'GR_R_311_V2::All'

########### Event cleaning ###########
#Require a good vertex
process.oneGoodVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)

#Filter for removing scraping events
process.noscraping = cms.EDFilter("FilterOutScraping",
                                applyfilter = cms.untracked.bool(True),
                                debugOn = cms.untracked.bool(False),
                                numtrack = cms.untracked.uint32(10),
                                thresh = cms.untracked.double(0.25)
                                )


########### Trigger selection ###########
# Select events based on the HLT triggers....singleJet and BTag triggers
# Use the instructions provided at:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/TriggerResultsFilter
# This eases the trigger selection for different HLT menus and also takes care of wildcard and trigger versioning
#######

import HLTrigger.HLTfilters.triggerResultsFilter_cfi as hlt
process.JetHLTFilter = hlt.triggerResultsFilter.clone(
   triggerConditions = cms.vstring(
        "HLT_L1SingleJet36_v*",
        "HLT_Jet30_v*",
        "HLT_Jet60_v*",
        "HLT_Jet80_v*",
        "HLT_Jet110_v*",
        "HLT_Jet150_v*",
        "HLT_Jet190_v*",
        "HLT_Jet240_v*",
        "HLT_Jet370_v*",
        "HLT_Jet370_NoJetID_v*",
        "HLT_DiJetAve15U_v*",
        "HLT_DiJetAve30U_v*",
        "HLT_DiJetAve50U_v*",
        "HLT_DiJetAve70U_v*",
        "HLT_DiJetAve100U_v*",
        "HLT_DiJetAve140U_v*",
        "HLT_DiJetAve180U_v*",
        "HLT_DiJetAve300U_v*",
        "HLT_BTagMu_DiJet20_Mu5_v*",
        "HLT_BTagMu_DiJet40_Mu5_v*",
        "HLT_BTagMu_DiJet70_Mu5_v*",
        "HLT_BTagMu_DiJet110_Mu5_v*",
        "HLT_BTagMu_DiJet60_Mu7_v*",
        "HLT_BTagMu_DiJet80_Mu9_v*",
#        "HLT_BTagMu_DiJet100_Mu9_v*",
        "HLT_Mu17_CentralJet40_BTagIP_v*",
        "HLT_IsoMu17_CentralJet40_BTagIP_v*"
        ),
   hltResults = cms.InputTag("TriggerResults","","HLT"),
   l1tResults = cms.InputTag( "" ),
   throw = cms.bool( False) #set to false to deal with missing triggers while running over different trigger menus
)

########### Jet selection and JEC ###########
# Apply "loose PF JetID" first
process.PFJetsFilter = cms.EDFilter("PFJetSelector",
  src = cms.InputTag("ak5PFJets"),
  cut = cms.string("pt > 10.0 && abs(eta) < 2.4 && neutralHadronEnergyFraction < 0.99 && neutralEmEnergyFraction < 0.99 && nConstituents > 1 && chargedHadronEnergyFraction > 0.0 && chargedMultiplicity > 0.0 && chargedEmEnergyFraction < 0.99"),
  filter = cms.bool(True)
)

# Then apply the JEC
#...........................................
# JEC in 38X....switch off use of confDB
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

##------------------  PF JETS --------------
process.ak5PFJetsJEC = process.ak5PFJetsL2L3.clone(
    src = 'PFJetsFilter', 
    correctors = ['ak5PFL2L3'])

#............................................


########### Rerun b-tagging for PF Jets ###########
process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")

process.ak5PFJetTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
   process.j2tParametersVX,
   jets = cms.InputTag("ak5PFJetsJEC")
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
  jets = "ak5PFJetsJEC"
)

process.standardSoftElectronPFTagInfos = process.softElectronTagInfos.clone(
  jets = "ak5PFJetsJEC"
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

########### Dump contents into ntuple ###########
process.load("bTag.CommissioningCommonSetup.tagntupleproducer_cfi")

process.standardPFBTagNtuple = process.bTagNtuple.clone()
process.standardPFBTagNtuple.jetSrc = cms.InputTag( "ak5PFJetsJEC" )
process.standardPFBTagNtuple.svComputer = cms.InputTag( "standardCombinedSecondaryVertexPF" )
process.standardPFBTagNtuple.TriggerTag = cms.InputTag( "TriggerResults::HLT")
process.standardPFBTagNtuple.getMCTruth = cms.bool(False)
process.standardPFBTagNtuple.getMCPUInfo = cms.bool(False)
process.standardPFBTagNtuple.jetTracks = cms.InputTag( "ak5PFJetTracksAssociatorAtVertex" )
process.standardPFBTagNtuple.SVTagInfos = cms.InputTag( "standardSecondaryVertexPFTagInfos" )
process.standardPFBTagNtuple.IPTagInfos = cms.InputTag( "standardImpactParameterPFTagInfos" )
process.standardPFBTagNtuple.ElectronTagInfos = cms.InputTag( "standardSoftElectronPFTagInfos" )
process.standardPFBTagNtuple.MuonTagInfos = cms.InputTag( "standardSoftMuonPFTagInfos" )
process.standardPFBTagNtuple.Label = cms.string("standardPF")
process.standardPFBTagNtuple.filename = cms.string("standardPFNtuple.root")
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


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:E2428E6C-A954-E011-8F8A-001D09F34488.root'
#     '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/954/2EF6D49C-2355-E011-BE0F-001617C3B70E.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/943/58A09E66-FD54-E011-9AC1-0016177CA7A0.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/942/947D60CA-F054-E011-A7AB-003048F1183E.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/940/F0D5C5D5-EE54-E011-82C6-001617DBCF6A.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/939/E670AC30-F954-E011-9515-003048F1C420.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/938/4EADCC18-F754-E011-A70E-003048F11942.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/937/709C33E1-F254-E011-8CDF-001617C3B5E4.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/937/3A7C6AA1-1A55-E011-9709-001617E30F48.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/936/96C8B24B-EE54-E011-A082-003048D37456.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/935/964F0173-F154-E011-872B-001D09F2A49C.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/935/2687B169-F654-E011-ACB1-001D09F231C9.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/916/22A24F34-A954-E011-815E-0030487CD184.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/915/CC358858-9654-E011-AA61-0030487C778E.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/915/7E8EC303-9154-E011-8118-0030487CD76A.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/914/3C6CFC53-9454-E011-B491-001617C3B706.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/913/04F396ED-9854-E011-81F7-0030487CD162.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/911/E87873A4-7354-E011-B801-003048F1C424.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/911/E64C09CD-8854-E011-BA64-0030487CD7B4.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/907/C27F2E90-7854-E011-A82A-00304879FBB2.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/898/564A650C-F953-E011-979F-003048D375AA.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/894/5450EDBC-4354-E011-9B72-000423D94494.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/890/F23CB99E-4754-E011-AAE3-000423D33970.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/890/762AD04D-4254-E011-94DA-001D09F291D7.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/890/30614F67-6854-E011-84A2-001617E30CE8.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/888/BCAAEB40-3B54-E011-851F-003048F11C5C.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/888/9C31F599-3554-E011-862C-001617C3B706.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/888/5C033F97-3E54-E011-B299-001D09F24DDF.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/877/626D250F-E453-E011-8E65-000423D987E0.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/876/36B905CE-D453-E011-8939-003048F117EC.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/875/669E7ED4-F953-E011-A0CC-001617C3B6C6.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/875/4825A855-1854-E011-851B-0030487CD906.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/874/7A7BD15A-0C54-E011-9143-001617E30CC8.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/873/3080B6D3-DF53-E011-B00B-001617C3B778.root',
#        '/store/data/Run2011A/METBTag/RECO/PromptReco-v1/000/160/872/00D23A36-EB53-E011-BFF8-003048F11CF0.root'
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


process.plots = cms.Path(
  process.JetHLTFilter +
  process.noscraping +
  process.oneGoodVertexFilter +
  process.PFJetsFilter *
  process.ak5PFJetsJEC *
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
