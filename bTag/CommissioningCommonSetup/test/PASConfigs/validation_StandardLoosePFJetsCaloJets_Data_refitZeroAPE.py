import FWCore.ParameterSet.Config as cms

import RecoBTag.SoftLepton.muonSelection
from DQMOffline.RecoB.bTagCombinedSVVariables_cff import *
#includes added because of block refactoring replacing simple includes by using statements
from DQMOffline.RecoB.bTagTrackIPAnalysis_cff import *
from DQMOffline.RecoB.bTagCombinedSVAnalysis_cff import *
from DQMOffline.RecoB.bTagTrackCountingAnalysis_cff import *
from DQMOffline.RecoB.bTagTrackProbabilityAnalysis_cff import *
from DQMOffline.RecoB.bTagTrackBProbabilityAnalysis_cff import *
from DQMOffline.RecoB.bTagGenericAnalysis_cff import *
from DQMOffline.RecoB.bTagSimpleSVAnalysis_cff import *
from DQMOffline.RecoB.bTagSoftLeptonAnalysis_cff import *
from DQMOffline.RecoB.bTagSoftLeptonByPtAnalysis_cff import *
from DQMOffline.RecoB.bTagSoftLeptonByIPAnalysis_cff import *

process = cms.Process("validation")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("DQMServices.Core.DQM_cfg")

process.load("RecoBTag.Configuration.RecoBTag_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#Global tag for 3_6_1
process.GlobalTag.globaltag = 'GR_R_36X_V12A::All'

process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
#Good Bunch Crossings
process.bptxAnd = hltLevel1GTSeed.clone(L1TechTriggerSeeding = cms.bool(True), L1SeedsLogicalExpression = cms.string('0'))
#BSCNOBEAMHALO
process.bit40 = hltLevel1GTSeed.clone(L1TechTriggerSeeding = cms.bool(True), L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39) AND NOT ((42 AND NOT 43) OR (43 AND NOT 42))'))

#Physics-declared Bit
from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
process.physDecl = hltHighLevelDev.clone(HLTPaths = ['HLT_PhysicsDeclared'], HLTPathsPrescales = [1])

#Select events based on the HLT triggers (HLT_L1Jet6U || HLT_L1Jet10U || HLT_Jet15U)
# Use the instructions provided at:
# http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HLTrigger/HLTfilters/python/hltHighLevel_cfi.py?hideattic=1&revision=1.5&view=markup
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.singleJetHLTFilter = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.singleJetHLTFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.singleJetHLTFilter.HLTPaths = ["HLT_L1Jet6U", "HLT_L1Jet10U", "HLT_Jet15U"]
process.singleJetHLTFilter.andOr = cms.bool(True) # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.HLT_Jet15U = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.HLT_Jet15U.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.HLT_Jet15U.HLTPaths = ["HLT_Jet15U"]
process.HLT_Jet15U.andOr = cms.bool(True) # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true

#Require a good vertex
process.oneGoodVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"),
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)

############################################################################################
##refit tracks with new alignment geometry and set APEs to zero from Jula Draeger
############################################################################################
#####################################################################
### Apply corrections to local reco - needed only if you run on TK-DECO data (i.e., MinBias) !!!! For cosmics in PEAK mode, comment them out
#####################################################################
process.load("RecoLocalTracker.SiStripRecHitConverter.OutOfTime_cff")
process.OutOfTime.TOBlateBP=0.071
process.OutOfTime.TIBlateBP=0.036

from CondCore.DBCommon.CondDBSetup_cfi import *
##
process.stripLorentzAngle = cms.ESSource("PoolDBESSource",CondDBSetup,
                                         connect = cms.string('sqlite_file:SiStripLorentzAngle_Deco.db'),
                                         toGet = cms.VPSet(cms.PSet(record = cms.string('SiStripLorentzAngleRcd'),
                                                                    tag = cms.string('SiStripLorentzAngle_Deco') ))
                                         )
process.es_prefer_stripLorentzAngle = cms.ESPrefer("PoolDBESSource", "stripLorentzAngle")

### Load the new geometry
process.trackerAlignmentICHEP2010 = cms.ESSource("PoolDBESSource",
                                        CondDBSetup,
                                        toGet = cms.VPSet(cms.PSet(
                                                record = cms.string('TrackerAlignmentRcd'),
                                                tag = cms.string('Alignments')
                                                )),
                                        connect = cms.string('sqlite_file:TOBCenteredObjectICHEP2010.db')
                                        )
process.es_prefer_trackerAlignment = cms.ESPrefer("PoolDBESSource", "trackerAlignmentICHEP2010")

###for the APE:
### Load the Tracker APE=0
process.trackerAPEzero = cms.ESSource("PoolDBESSource",CondDBSetup,
                          connect = cms.string('frontier://FrontierProd/CMS_COND_31X_FROM21X'),
                          toGet = cms.VPSet(cms.PSet(record = cms.string('TrackerAlignmentErrorRcd'),
                          tag = cms.string('TrackerIdealGeometryErrors210_mc') 
                          ))
                          )

process.es_prefer_trackerAPE = cms.ESPrefer("PoolDBESSource", "trackerAPEzero")

####################################################
# refitting
####################################################
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.generalTracks = process.TrackRefitter.clone(    
    src ='generalTracks',#'ALCARECOTkAlCosmicsCTF0T',
    TrajectoryInEvent = True,
    NavigationSchool = "",
    TTRHBuilder = "WithAngleAndTemplate" #default
)


############################################################################
##Produce Primary Vertex Collection needed for later analysis
############################################################################
process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi")
process.offlinePrimaryVertices.TrackLabel = 'generalTracks'


############################################################################################################
##end of refitting inlcude
############################################################################################################

# JEC for both ak5PF and Calo jets
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.ak5CaloJetsL2L3.src = "caloJetIDFilter"

#Filter for PFJets
process.PFJetsFilter = cms.EDFilter("PFJetSelector",
  src = cms.InputTag("ak5PFJetsL2L3"),
  cut = cms.string("pt > 10.0 && abs(eta) < 2.5 && neutralHadronEnergyFraction < 1.0 && neutralEmEnergyFraction < 1.0 && nConstituents > 1 && chargedHadronEnergyFraction > 0.0 && chargedMultiplicity > 0.0 && chargedEmEnergyFraction < 1.0"),
  filter = cms.bool(True)
)

#Filter for CaloJets
process.load("bTag.CommissioningCommonSetup.caloJetIDFilter_cff")

#Filter for removing scraping events
process.noscraping = cms.EDFilter("FilterOutScraping",
                                applyfilter = cms.untracked.bool(True),
                                debugOn = cms.untracked.bool(False),
                                numtrack = cms.untracked.uint32(10),
                                thresh = cms.untracked.double(0.25)
                                )

process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")

process.ak5CaloJetTracksAssociatorAtVertex = cms.EDProducer("JetTracksAssociatorAtVertex",
   process.j2tParametersVX,
   jets = cms.InputTag("ak5CaloJetsL2L3")
)

process.ak5PFJetTracksAssociatorAtVertex = process.ak5CaloJetTracksAssociatorAtVertex.clone(
  jets = "PFJetsFilter"
)

process.standardImpactParameterCaloTagInfos = process.impactParameterTagInfos.clone(
  jetTracks = "ak5CaloJetTracksAssociatorAtVertex"
)

process.standardImpactParameterPFTagInfos = process.standardImpactParameterCaloTagInfos.clone(
  jetTracks = "ak5PFJetTracksAssociatorAtVertex"
)

process.standardSecondaryVertexCaloTagInfos = process.secondaryVertexTagInfos.clone(
  trackIPTagInfos = "standardImpactParameterCaloTagInfos"
)

process.standardSecondaryVertex3TrkCaloTagInfos = process.standardSecondaryVertexCaloTagInfos.clone()
process.standardSecondaryVertex3TrkCaloTagInfos.vertexCuts.multiplicityMin = 3

process.standardSecondaryVertexPFTagInfos = process.standardSecondaryVertexCaloTagInfos.clone(
  trackIPTagInfos = "standardImpactParameterPFTagInfos"
)

process.standardSecondaryVertex3TrkPFTagInfos = process.standardSecondaryVertexPFTagInfos.clone()
process.standardSecondaryVertex3TrkPFTagInfos.vertexCuts.multiplicityMin = 3

process.standardGhostTrackVertexCaloTagInfos = process.standardSecondaryVertexCaloTagInfos.clone()
process.standardGhostTrackVertexCaloTagInfos.vertexReco = process.ghostTrackVertexRecoBlock.vertexReco
process.standardGhostTrackVertexCaloTagInfos.vertexCuts.multiplicityMin = 1

process.standardGhostTrackVertexPFTagInfos = process.standardSecondaryVertexPFTagInfos.clone()
process.standardGhostTrackVertexPFTagInfos.vertexReco = process.ghostTrackVertexRecoBlock.vertexReco
process.standardGhostTrackVertexPFTagInfos.vertexCuts.multiplicityMin = 1

process.standardCombinedSecondaryVertexCalo = process.combinedSecondaryVertex.clone()

process.standardCombinedSecondaryVertex3TrkCalo = process.combinedSecondaryVertex.clone()

process.standardCombinedSecondaryVertexPF = process.standardCombinedSecondaryVertexCalo.clone()

process.standardCombinedSecondaryVertex3TrkPF = process.standardCombinedSecondaryVertexCalo.clone()

process.standardSecondaryVertexV0CaloTagInfos = process.standardSecondaryVertexCaloTagInfos.clone()
process.standardSecondaryVertexV0CaloTagInfos.vertexCuts.v0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0))

process.standardSecondaryVertexV0PFTagInfos = process.standardSecondaryVertexPFTagInfos.clone()
process.standardSecondaryVertexV0PFTagInfos.vertexCuts.v0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0))

process.standardCombinedSecondaryVertexV0Calo = process.standardCombinedSecondaryVertexCalo.clone()
process.standardCombinedSecondaryVertexV0Calo.trackPairV0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0))
process.standardCombinedSecondaryVertexV0Calo.pseudoVertexV0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0)) 
process.standardCombinedSecondaryVertexV0Calo.trackMultiplicityMin = 2 

process.standardCombinedSecondaryVertexV0PF = process.standardCombinedSecondaryVertexPF.clone()
process.standardCombinedSecondaryVertexV0PF.trackPairV0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0))
process.standardCombinedSecondaryVertexV0PF.pseudoVertexV0Filter = cms.PSet(k0sMassWindow = cms.double(-1.0)) 
process.standardCombinedSecondaryVertexV0PF.trackMultiplicityMin = 2

process.standardTrackCountingHighEffCaloBJetTags = process.trackCountingHighEffBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterCaloTagInfos"))
)

process.standardTrackCountingHighEffPFBJetTags = process.trackCountingHighEffBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"))
)

process.standardTrackCountingHighPurCaloBJetTags = process.trackCountingHighPurBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterCaloTagInfos"))
)

process.standardTrackCountingHighPurPFBJetTags = process.trackCountingHighPurBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"))
)

process.standardJetProbabilityCaloBJetTags = process.jetProbabilityBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterCaloTagInfos")) 
)

process.standardJetProbabilityPFBJetTags = process.jetProbabilityBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos")) 
)

process.standardJetBProbabilityCaloBJetTags = process.jetBProbabilityBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterCaloTagInfos")) 
)

process.standardJetBProbabilityPFBJetTags = process.jetBProbabilityBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos")) 
)

process.standardSimpleSecondaryVertexHighEffCaloBJetTags = process.simpleSecondaryVertexHighEffBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSecondaryVertexCaloTagInfos"))
)

process.standardSimpleSecondaryVertexHighEffPFBJetTags = process.simpleSecondaryVertexHighEffBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardSimpleSecondaryVertexHighPurCaloBJetTags = process.simpleSecondaryVertexHighPurBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSecondaryVertexCaloTagInfos"))
)

process.standardSimpleSecondaryVertexHighPurPFBJetTags = process.simpleSecondaryVertexHighPurBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardGhostTrackCaloBJetTags = process.ghostTrackBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterCaloTagInfos"),
	                         cms.InputTag("standardGhostTrackVertexCaloTagInfos"))
)

process.standardGhostTrackPFBJetTags = process.ghostTrackBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"),
	                         cms.InputTag("standardGhostTrackVertexPFTagInfos"))
)

process.standardCombinedSecondaryVertexCaloBJetTags = process.combinedSecondaryVertexBJetTags.clone(
  jetTagComputer = cms.string('standardCombinedSecondaryVertexCalo'),
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterCaloTagInfos"),
                           cms.InputTag("standardSecondaryVertexCaloTagInfos"))
)

process.standardCombinedSecondaryVertexPFBJetTags = process.combinedSecondaryVertexBJetTags.clone(
  jetTagComputer = cms.string('standardCombinedSecondaryVertexPF'),
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"),
                           cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardCombinedSecondaryVertexMVACaloBJetTags = process.combinedSecondaryVertexMVABJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterCaloTagInfos"),
                           cms.InputTag("standardSecondaryVertexCaloTagInfos"))
)

process.standardCombinedSecondaryVertexMVAPFBJetTags = process.combinedSecondaryVertexMVABJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardImpactParameterPFTagInfos"),
                           cms.InputTag("standardSecondaryVertexPFTagInfos"))
)

process.standardSoftMuonCaloTagInfos = process.softMuonTagInfos.clone(
  jets = "ak5CaloJetsL2L3"
)

process.standardSoftMuonPFTagInfos = process.standardSoftMuonCaloTagInfos.clone(
  jets = "PFJetsFilter"
)

process.standardSoftElectronCaloTagInfos = process.softElectronTagInfos.clone(
  jets = "ak5CaloJetsL2L3"
)

process.standardSoftElectronPFTagInfos = process.standardSoftElectronCaloTagInfos.clone(
  jets = "PFJetsFilter"
)

process.standardSoftMuonCaloBJetTags = process.softMuonBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonCaloTagInfos"))
)

process.standardSoftMuonPFBJetTags = process.softMuonBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonPFTagInfos"))
)

process.standardSoftMuonByPtCaloBJetTags = process.softMuonByPtBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonCaloTagInfos"))
)

process.standardSoftMuonByPtPFBJetTags = process.softMuonByPtBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonPFTagInfos"))
)

process.standardSoftMuonByIP3dCaloBJetTags = process.softMuonByIP3dBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonCaloTagInfos"))
)

process.standardSoftMuonByIP3dPFBJetTags = process.softMuonByIP3dBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftMuonPFTagInfos"))
)

process.standardSoftElectronByPtCaloBJetTags = process.softElectronByPtBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftElectronCaloTagInfos"))
)

process.standardSoftElectronByPtPFBJetTags = process.softElectronByPtBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftElectronPFTagInfos"))
)

process.standardSoftElectronByIP3dCaloBJetTags = process.softElectronByIP3dBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftElectronCaloTagInfos"))
)

process.standardSoftElectronByIP3dPFBJetTags = process.softElectronByIP3dBJetTags.clone(
  tagInfos = cms.VInputTag(cms.InputTag("standardSoftElectronPFTagInfos"))
)

process.load("DQMOffline.RecoB.bTagAnalysisData_cfi")
#process.bTagAnalysis.allHistograms = True
process.caloBTagAnalysis = process.bTagAnalysis.clone() 
process.caloBTagAnalysis.finalizePlots = False 
process.caloBTagAnalysis.finalizeOnly = False 
process.caloBTagAnalysis.ptRecJetMin = 30.0
process.caloBTagAnalysis.etaMax = 2.5
process.caloBTagAnalysis.ptRanges = cms.vdouble(30.0, 80.0, 99999.0)
process.caloBTagAnalysis.etaRanges = cms.vdouble(0.0, 1.5, 2.5)
process.caloBTagAnalysis.tagConfig = cms.VPSet(
        cms.PSet(
            bTagTrackIPAnalysisBlock,
            type = cms.string('TrackIP'),
            label = cms.InputTag("standardImpactParameterCaloTagInfos")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            ipTagInfos = cms.InputTag("standardImpactParameterCaloTagInfos"),
            type = cms.string('GenericMVA'),
            svTagInfos = cms.InputTag("standardSecondaryVertexCaloTagInfos"),
            label = cms.InputTag("standardCombinedSecondaryVertexCalo")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            ipTagInfos = cms.InputTag("standardImpactParameterCaloTagInfos"),
            type = cms.string('GenericMVA'),
            svTagInfos = cms.InputTag("standardSecondaryVertexV0CaloTagInfos"),
            label = cms.InputTag("standardCombinedSecondaryVertexV0Calo")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            ipTagInfos = cms.InputTag("standardImpactParameterCaloTagInfos"),
            type = cms.string('GenericMVA'),
            svTagInfos = cms.InputTag("standardSecondaryVertex3TrkCaloTagInfos"),
            label = cms.InputTag("standardCombinedSecondaryVertex3TrkCalo")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("standardTrackCountingHighEffCaloBJetTags")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("standardTrackCountingHighPurCaloBJetTags")
        ), 
        cms.PSet(
            bTagProbabilityAnalysisBlock,
            label = cms.InputTag("standardJetProbabilityCaloBJetTags")
        ), 
        cms.PSet(
            bTagBProbabilityAnalysisBlock,
            label = cms.InputTag("standardJetBProbabilityCaloBJetTags")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("standardSimpleSecondaryVertexHighEffCaloBJetTags")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("standardSimpleSecondaryVertexHighPurCaloBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("standardGhostTrackCaloBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("standardCombinedSecondaryVertexCaloBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("standardCombinedSecondaryVertexMVACaloBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            label = cms.InputTag("standardSoftMuonCaloBJetTags")
        ),
        cms.PSet(
            bTagSoftLeptonByIPAnalysisBlock,
            label = cms.InputTag("standardSoftMuonByIP3dCaloBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByPtAnalysisBlock,
            label = cms.InputTag("standardSoftMuonByPtCaloBJetTags")
        ),
        cms.PSet(
            bTagSoftLeptonByIPAnalysisBlock,
            label = cms.InputTag("standardSoftElectronByIP3dCaloBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByPtAnalysisBlock,
            label = cms.InputTag("standardSoftElectronByPtCaloBJetTags")
        ),
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            type = cms.string("SoftLepton"),
            label = cms.InputTag("standardSoftMuonCaloTagInfos")
        ),
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            type = cms.string("SoftLepton"),
            label = cms.InputTag("standardSoftElectronCaloTagInfos")
        ) 
)
process.caloBTagAnalysis.tagConfig[2].parameters.categories[0].vertexMass.min = 0.3
process.caloBTagAnalysis.tagConfig[2].parameters.categories[0].vertexMass.max = 0.8
process.caloBTagAnalysis.tagConfig[2].parameters.categories[1].vertexMass.min = 0.3
process.caloBTagAnalysis.tagConfig[2].parameters.categories[1].vertexMass.max = 0.8
process.caloBTagAnalysis.tagConfig[2].parameters.categories[2].vertexMass.min = 0.3
process.caloBTagAnalysis.tagConfig[2].parameters.categories[2].vertexMass.max = 0.8
for i in range(4, 17):
  for j in range(i + 1, 17):
    process.caloBTagAnalysis.tagConfig.append(
      cms.PSet(
        type = cms.string("TagCorrelation"),
        label1 = process.caloBTagAnalysis.tagConfig[i].label,
        label2 = process.caloBTagAnalysis.tagConfig[j].label,
        parameters = cms.PSet(
          CreateProfile = cms.bool(True),
          Discr1Start = process.caloBTagAnalysis.tagConfig[i].parameters.discriminatorStart,
          Discr1End = process.caloBTagAnalysis.tagConfig[i].parameters.discriminatorEnd,
          Discr2Start = process.caloBTagAnalysis.tagConfig[j].parameters.discriminatorStart,
          Discr2End = process.caloBTagAnalysis.tagConfig[j].parameters.discriminatorEnd
        )
      )
    )

process.pfBTagAnalysis = process.caloBTagAnalysis.clone()
process.pfBTagAnalysis.tagConfig = cms.VPSet(
        cms.PSet(
            bTagTrackIPAnalysisBlock,
            type = cms.string('TrackIP'),
            label = cms.InputTag("standardImpactParameterPFTagInfos")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            ipTagInfos = cms.InputTag("standardImpactParameterPFTagInfos"),
            type = cms.string('GenericMVA'),
            svTagInfos = cms.InputTag("standardSecondaryVertexPFTagInfos"),
            label = cms.InputTag("standardCombinedSecondaryVertexPF")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            ipTagInfos = cms.InputTag("standardImpactParameterPFTagInfos"),
            type = cms.string('GenericMVA'),
            svTagInfos = cms.InputTag("standardSecondaryVertexV0PFTagInfos"),
            label = cms.InputTag("standardCombinedSecondaryVertexV0PF")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            ipTagInfos = cms.InputTag("standardImpactParameterPFTagInfos"),
            type = cms.string('GenericMVA'),
            svTagInfos = cms.InputTag("standardSecondaryVertex3TrkPFTagInfos"),
            label = cms.InputTag("standardCombinedSecondaryVertex3TrkPF")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("standardTrackCountingHighEffPFBJetTags")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("standardTrackCountingHighPurPFBJetTags")
        ), 
        cms.PSet(
            bTagProbabilityAnalysisBlock,
            label = cms.InputTag("standardJetProbabilityPFBJetTags")
        ), 
        cms.PSet(
            bTagBProbabilityAnalysisBlock,
            label = cms.InputTag("standardJetBProbabilityPFBJetTags")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("standardSimpleSecondaryVertexHighEffPFBJetTags")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("standardSimpleSecondaryVertexHighPurPFBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("standardGhostTrackPFBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("standardCombinedSecondaryVertexPFBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("standardCombinedSecondaryVertexMVAPFBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            label = cms.InputTag("standardSoftMuonPFBJetTags")
        ),
        cms.PSet(
            bTagSoftLeptonByIPAnalysisBlock,
            label = cms.InputTag("standardSoftMuonByIP3dPFBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByPtAnalysisBlock,
            label = cms.InputTag("standardSoftMuonByPtPFBJetTags")
        ),
        cms.PSet(
            bTagSoftLeptonByIPAnalysisBlock,
            label = cms.InputTag("standardSoftElectronByIP3dPFBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByPtAnalysisBlock,
            label = cms.InputTag("standardSoftElectronByPtPFBJetTags")
        ),
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            type = cms.string("SoftLepton"),
            label = cms.InputTag("standardSoftMuonPFTagInfos")
        ),
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            type = cms.string("SoftLepton"),
            label = cms.InputTag("standardSoftElectronPFTagInfos")
        ) 
)
process.pfBTagAnalysis.tagConfig[2].parameters.categories[0].vertexMass.min = 0.3
process.pfBTagAnalysis.tagConfig[2].parameters.categories[0].vertexMass.max = 0.8
process.pfBTagAnalysis.tagConfig[2].parameters.categories[1].vertexMass.min = 0.3
process.pfBTagAnalysis.tagConfig[2].parameters.categories[1].vertexMass.max = 0.8
process.pfBTagAnalysis.tagConfig[2].parameters.categories[2].vertexMass.min = 0.3
process.pfBTagAnalysis.tagConfig[2].parameters.categories[2].vertexMass.max = 0.8
for i in range(4, 17):
  for j in range(i + 1, 17):
    process.pfBTagAnalysis.tagConfig.append(
      cms.PSet(
        type = cms.string("TagCorrelation"),
        label1 = process.pfBTagAnalysis.tagConfig[i].label,
        label2 = process.pfBTagAnalysis.tagConfig[j].label,
        parameters = cms.PSet(
          CreateProfile = cms.bool(True),
          Discr1Start = process.pfBTagAnalysis.tagConfig[i].parameters.discriminatorStart,
          Discr1End = process.pfBTagAnalysis.tagConfig[i].parameters.discriminatorEnd,
          Discr2Start = process.pfBTagAnalysis.tagConfig[j].parameters.discriminatorStart,
          Discr2End = process.pfBTagAnalysis.tagConfig[j].parameters.discriminatorEnd
        )
      )
    )

process.load("bTag.CommissioningCommonSetup.tagntupleproducer_cfi")

process.standardCaloBTagNtuple = process.bTagNtuple.clone()
process.standardCaloBTagNtuple.getMCTruth = cms.bool(False)
process.standardCaloBTagNtuple.jetSrc = cms.InputTag( "ak5CaloJetsL2L3" )
process.standardCaloBTagNtuple.svComputer = cms.InputTag( "standardCombinedSecondaryVertexCalo" )
process.standardCaloBTagNtuple.TriggerTag = cms.InputTag( "TriggerResults::HLT")
process.standardCaloBTagNtuple.jetMCSrc = cms.InputTag( "" )
process.standardCaloBTagNtuple.jetTracks = cms.InputTag( "ak5CaloJetTracksAssociatorAtVertex" )
process.standardCaloBTagNtuple.SVTagInfos = cms.InputTag( "standardSecondaryVertexCaloTagInfos" )
process.standardCaloBTagNtuple.IPTagInfos = cms.InputTag( "standardImpactParameterCaloTagInfos" )
process.standardCaloBTagNtuple.ElectronTagInfos = cms.InputTag( "standardSoftElectronCaloTagInfos" )
process.standardCaloBTagNtuple.MuonTagInfos = cms.InputTag( "standardSoftMuonCaloTagInfos" )
process.standardCaloBTagNtuple.Label = cms.string("standardCalo")
process.standardCaloBTagNtuple.filename = cms.string("standardCaloNtuple.root")
process.standardCaloBTagNtuple.bTagConfig = cms.VPSet(
    cms.PSet(
    alias = cms.string("standardTrackCountingHighEffCaloBJetTags"),
    label = cms.InputTag("standardTrackCountingHighEffCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardTrackCountingHighPurCaloBJetTags"),
    label = cms.InputTag("standardTrackCountingHighPurCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardJetProbabilityCaloBJetTags"),
    label = cms.InputTag("standardJetProbabilityCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardJetBProbabilityCaloBJetTags"),
    label = cms.InputTag("standardJetBProbabilityCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSimpleSecondaryVertexHighEffCaloBJetTags"),
    label = cms.InputTag("standardSimpleSecondaryVertexHighEffCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSimpleSecondaryVertexHighPurCaloBJetTags"),
    label = cms.InputTag("standardSimpleSecondaryVertexHighPurCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardGhostTrackCaloBJetTags"),
    label = cms.InputTag("standardGhostTrackCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardCombinedSecondaryVertexCaloBJetTags"),
    label = cms.InputTag("standardCombinedSecondaryVertexCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardCombinedSecondaryVertexMVACaloBJetTags"),
    label = cms.InputTag("standardCombinedSecondaryVertexMVACaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSoftMuonCaloBJetTags"),
    label = cms.InputTag("standardSoftMuonCaloBJetTags")
    ),
    cms.PSet(
    alias = cms.string("standardSoftMuonByIP3dCaloBJetTags"),
    label = cms.InputTag("standardSoftMuonByIP3dCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSoftMuonByPtCaloBJetTags"),
    label = cms.InputTag("standardSoftMuonByPtCaloBJetTags")
    ),
    cms.PSet(
    alias = cms.string("standardSoftElectronByIP3dCaloBJetTags"),
    label = cms.InputTag("standardSoftElectronByIP3dCaloBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardSoftElectronByPtCaloBJetTags"),
    label = cms.InputTag("standardSoftElectronByPtCaloBJetTags")
    )
    )

process.standardPFBTagNtuple = process.bTagNtuple.clone()
process.standardPFBTagNtuple.getMCTruth = cms.bool(False)
process.standardPFBTagNtuple.jetSrc = cms.InputTag( "PFJetsFilter" )
process.standardPFBTagNtuple.svComputer = cms.InputTag( "standardCombinedSecondaryVertexPF" )
process.standardPFBTagNtuple.TriggerTag = cms.InputTag( "TriggerResults::HLT")
process.standardPFBTagNtuple.jetMCSrc = cms.InputTag( "" )
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
    alias = cms.string("standardGhostTrackPFBJetTags"),
    label = cms.InputTag("standardGhostTrackPFBJetTags")
    ), 
    cms.PSet(
    alias = cms.string("standardCombinedSecondaryVertexPFBJetTags"),
    label = cms.InputTag("standardCombinedSecondaryVertexPFBJetTags")
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

process.bTagNtuples = cms.Sequence(
    process.standardCaloBTagNtuple +
    process.standardPFBTagNtuple
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2C198163-1F4E-DF11-9A9A-00248C0BE013.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2AE90310-1D4E-DF11-804E-0026189437FD.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2AE1AEB5-244E-DF11-AE92-0026189438C9.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2A8264E5-1B4E-DF11-9670-0018F3D0965C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2A6C0888-1D4E-DF11-B136-002618943953.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2A5A108C-194E-DF11-80FB-0026189438C1.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2A50D2BD-1B4E-DF11-97F1-002618943849.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2A22581F-264E-DF11-AF1D-002618943870.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/28CD50F7-244E-DF11-AD87-00261894396D.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/286EE7A9-214E-DF11-A783-00304867920C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/28664E47-204E-DF11-8989-00261894387E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/28513F3D-1C4E-DF11-BE06-002354EF3BDD.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2804503F-1A4E-DF11-ABD8-002618943849.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26E44FBA-204E-DF11-82AD-003048678E6E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26D61DF9-214E-DF11-AE87-00261894389C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26D2533F-184E-DF11-B998-0030486792BA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26BE3D8D-264E-DF11-8A26-0026189437FE.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26A2E7A7-234E-DF11-B124-00304867920C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/267EE6D4-254E-DF11-A81E-0026189437F0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2638E268-184E-DF11-8167-0026189438C2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/26322135-184E-DF11-BE50-00248C0BE018.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/261EFDF7-234E-DF11-8BD7-002618943886.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2606792D-264E-DF11-B339-0026189438D7.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/24AC4F9C-1E4E-DF11-AD4E-00261894390C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/24A4E53A-1F4E-DF11-9F70-003048D3FC94.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/247EC35B-234E-DF11-83B2-002618943946.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2478109C-1E4E-DF11-BB39-00261894386F.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/243B75E7-1E4E-DF11-822D-00261894398C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/22A50C5C-234E-DF11-A4AC-00248C0BE018.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/22751D0F-214E-DF11-8393-0026189437F0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2249B835-184E-DF11-AB18-00304867929E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/224960EA-244E-DF11-8C9C-00261894390C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2246224C-224E-DF11-A523-002618943894.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2201B385-1F4E-DF11-BA2B-00248C0BE018.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/20CDF0E9-1C4E-DF11-9A1F-002354EF3BDD.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/208C8987-1D4E-DF11-BCA8-00261894388F.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/204CF3FF-1B4E-DF11-8AFD-0030486792B8.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/2008A93F-1A4E-DF11-B284-00261894390E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1EDE2E60-184E-DF11-94E3-00261894390E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1E148CF4-184E-DF11-A0AA-0030486792BA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1CFDFB4C-1E4E-DF11-AB24-00261894387E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1CD7644D-214E-DF11-8581-0026189438BD.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1CD5BC3F-1A4E-DF11-AA7C-0026189438E9.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1C9E989B-1E4E-DF11-BB88-002354EF3BDA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1C845817-1B4E-DF11-B19D-00248C0BE018.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1C5868E7-204E-DF11-872B-00248C0BE018.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1C2635D5-1F4E-DF11-8C64-0018F3D09678.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1C24CCBF-224E-DF11-A8F7-00261894388F.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1AF881F9-1F4E-DF11-9012-0026189438BD.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1AE72863-1B4E-DF11-AFCA-00261894388D.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1ABCE44A-254E-DF11-BF3B-0026189438B4.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1AB9E24A-254E-DF11-8062-0026189438B4.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1A77E874-1E4E-DF11-8251-003048D3FC94.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1A5239C2-224E-DF11-A1F3-0030486792BA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1A08D3F4-184E-DF11-99B2-003048679076.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/18F93316-1B4E-DF11-8C35-002618943868.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/18E896FD-1D4E-DF11-AC77-002354EF3BD2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/18CFAED3-254E-DF11-9E90-00261894393B.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/188B531A-244E-DF11-920C-0026189438B0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/180C554D-1E4E-DF11-A9F1-0026189438D7.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1802C942-194E-DF11-A89D-0026189437F0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/16E45EF8-244E-DF11-8A02-002618943956.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/16CF6416-254E-DF11-A9B4-003048678B36.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/16BF2470-204E-DF11-8CB9-002618943849.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1698CA2F-234E-DF11-82AC-002618943800.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1687262D-1C4E-DF11-A750-00261894389C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1678E73E-264E-DF11-BDB4-002618943908.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1644FCC6-1C4E-DF11-B9E3-002618943899.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/163A6E37-214E-DF11-8F20-0026189438B0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/16299296-224E-DF11-8C3A-003048679076.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/16250CEA-1C4E-DF11-BCFD-00304867929E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/14FAD48F-264E-DF11-A5BF-002618943833.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/14885C71-204E-DF11-A8FA-002618943953.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/14849AF6-1F4E-DF11-BDE1-002618943868.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/145FA242-194E-DF11-BDB9-00261894390C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1451B526-264E-DF11-BE22-002618943946.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/14039404-264E-DF11-B214-002618943868.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/14034D6B-244E-DF11-A9FA-002618943800.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/12FDBFBD-204E-DF11-8933-001BFCDBD160.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/12EDCEA9-214E-DF11-8780-0026189438C2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/12A312FF-1D4E-DF11-9B7B-00261894386F.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/129BA230-234E-DF11-80BF-0026189438E9.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1295E8E9-1A4E-DF11-A007-002618943915.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/128BD4BB-204E-DF11-8A00-00261894390C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/125C5088-1B4E-DF11-BF19-00261894390E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/10EFE361-1D4E-DF11-B266-002618943899.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/10E0B4BA-204E-DF11-A2DB-002618943915.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/10C00DE4-224E-DF11-BC44-0026189438C1.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/10B7982A-1E4E-DF11-97B0-002618943953.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/108066C1-224E-DF11-A6E3-003048D3FC94.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/10548FD1-1F4E-DF11-827B-0026189438EF.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/10443EB8-264E-DF11-84B2-002618943896.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/1041B8D2-214E-DF11-8BCD-0026189438D7.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0EF57E2B-1E4E-DF11-BC6C-0026189438FF.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0EF0CAAF-1B4E-DF11-BD51-003048679076.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0EEEAB44-204E-DF11-814A-0026189438B0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0EB00112-1D4E-DF11-A4F2-003048D3FC94.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0EA50038-214E-DF11-9E45-0026189438F4.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0E6AB045-264E-DF11-8A23-002618943879.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0E47347B-1C4E-DF11-AA91-00304867918A.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0E0AE963-1B4E-DF11-8094-0026189438D7.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0CDC9413-1D4E-DF11-AA13-001BFCDBD160.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0CD32FCB-234E-DF11-B44E-0026189438D7.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0CD0E6B6-244E-DF11-9F37-0030486792B8.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0CC2B978-254E-DF11-BC1B-00261894389C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0CB172D1-244E-DF11-B4BE-001BFCDBD11E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0C6E1DEA-1C4E-DF11-A20F-002618943886.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0C23A4A9-214E-DF11-8549-00304867929E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0AFD1228-254E-DF11-A932-002618943838.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0AF925D1-214E-DF11-9084-002618943800.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0AC0E797-184E-DF11-B808-00261894398C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0A8250F4-184E-DF11-8323-0026189438A9.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0A7AE63B-1B4E-DF11-8EBA-002354EF3BD2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0A3170AF-1B4E-DF11-9E9C-002618943868.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0A273380-234E-DF11-8BC1-0026189438C1.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0A0FD8A7-234E-DF11-9A64-003048678B92.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/08EEB45E-214E-DF11-AC0C-002618943953.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/08E1EB1F-224E-DF11-93B9-00248C0BE018.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/08AFAA71-204E-DF11-898C-0018F3D0965C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/08A705AC-1F4E-DF11-A33D-001BFCDBD160.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/08942371-224E-DF11-A7F7-00304867929E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/084FF170-1C4E-DF11-BB3B-003048678B92.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0824071A-244E-DF11-A7E3-002618943896.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/08208F44-204E-DF11-B8F1-00304867920C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/06DD2885-214E-DF11-8F26-002618943946.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/06CE72FF-1B4E-DF11-AB6F-00248C0BE013.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/06C65130-234E-DF11-ABB9-00261894387E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/067D06DC-244E-DF11-B816-0026189438EF.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/06747462-1B4E-DF11-A803-0030486792BA.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/065AD944-204E-DF11-B4DE-002618943896.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/065277D2-1F4E-DF11-BC1A-002618943821.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/060D7AE0-264E-DF11-9DF7-0030486792B6.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/04EBFCCA-234E-DF11-967A-00248C0BE013.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/04BAD602-254E-DF11-9D5F-002618943800.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/04A859C1-224E-DF11-832A-003048B95B30.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/02E8C471-1E4E-DF11-8292-003048D15DB6.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/02D8F9BF-1E4E-DF11-9545-002354EF3BE2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/028BEB0E-214E-DF11-BEFB-002618943946.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0288E070-224E-DF11-8C45-0026189438E9.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0265DD5B-214E-DF11-9876-002618943886.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0263422B-264E-DF11-AB6B-002618943930.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/02316477-1C4E-DF11-99CB-0026189438B0.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/02126D1E-184E-DF11-9390-00261894396F.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/009F1B68-264E-DF11-AA01-002618943821.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/009EE8EE-194E-DF11-94C1-003048678ED4.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/009A0D7E-234E-DF11-AC3A-0026189438BD.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/Apr20Skim_GOODCOLL-v1/0179/0082F0AA-214E-DF11-9C43-002618943849.root'
     
    )
)

process.EDM = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
                       "keep *_*_*_validation",
                       #"keep recoTracks_generalTracks_*_*",
                       "keep recoTracks_globalMuons_*_*",
                       "keep *_offlineBeamSpot_*_*",
                       "keep *_gsfElectrons_*_*",
                       "keep recoMuons_muons_*_*",
                       "keep *_softPFElectrons_*_*",
                       "keep recoGsfTracks_electronGsfTracks_*_*",
                       "keep *_TriggerResults_*_*",
                       "keep *_offlinePrimaryVertices_*_*"
    ),
    fileName = cms.untracked.string('BTagCommissioning2010_April20_7TeV_Data_PromptReco_v8.root'),
#    SelectEvents = cms.untracked.PSet(
#       SelectEvents = cms.vstring("plots")
#    )
)

process.load("DQMServices.Components.MEtoEDMConverter_cfi")

process.trackAssociation = cms.Sequence(
     process.ak5CaloJetTracksAssociatorAtVertex +
     process.ak5PFJetTracksAssociatorAtVertex 
)

process.ipTagInfos = cms.Sequence(
    process.standardImpactParameterCaloTagInfos +
    process.standardImpactParameterPFTagInfos 
)

process.svTagInfos = cms.Sequence(
    process.standardSecondaryVertexCaloTagInfos +
    process.standardSecondaryVertexPFTagInfos +
    process.standardSecondaryVertexV0CaloTagInfos +
    process.standardSecondaryVertexV0PFTagInfos +
    process.standardSecondaryVertex3TrkCaloTagInfos +
    process.standardSecondaryVertex3TrkPFTagInfos +
    process.standardGhostTrackVertexCaloTagInfos +
    process.standardGhostTrackVertexPFTagInfos 
)

process.ipTaggers = cms.Sequence(
    process.standardTrackCountingHighEffCaloBJetTags +
    process.standardTrackCountingHighEffPFBJetTags +
    process.standardTrackCountingHighPurCaloBJetTags +
    process.standardTrackCountingHighPurPFBJetTags +
    process.standardJetProbabilityCaloBJetTags +
    process.standardJetProbabilityPFBJetTags +
    process.standardJetBProbabilityCaloBJetTags +
    process.standardJetBProbabilityPFBJetTags 
)

process.svTaggers = cms.Sequence(
    process.standardSimpleSecondaryVertexHighEffCaloBJetTags +
    process.standardSimpleSecondaryVertexHighEffPFBJetTags +
    process.standardSimpleSecondaryVertexHighPurCaloBJetTags +
    process.standardSimpleSecondaryVertexHighPurPFBJetTags +
    process.standardGhostTrackCaloBJetTags +
    process.standardGhostTrackPFBJetTags +
    process.standardCombinedSecondaryVertexCaloBJetTags +
    process.standardCombinedSecondaryVertexPFBJetTags +
    process.standardCombinedSecondaryVertexMVACaloBJetTags +
    process.standardCombinedSecondaryVertexMVAPFBJetTags 
)

process.slTagInfos = cms.Sequence(
    process.standardSoftMuonCaloTagInfos +
    process.standardSoftMuonPFTagInfos +
    process.softElectronCands * (
    process.standardSoftElectronCaloTagInfos +
    process.standardSoftElectronPFTagInfos 
    )
)

process.slTaggers = cms.Sequence(
    process.standardSoftMuonCaloBJetTags +
    process.standardSoftMuonPFBJetTags +
    process.standardSoftMuonByPtCaloBJetTags +
    process.standardSoftMuonByPtPFBJetTags +
    process.standardSoftMuonByIP3dCaloBJetTags +
    process.standardSoftMuonByIP3dPFBJetTags +
    process.standardSoftElectronByPtCaloBJetTags +
    process.standardSoftElectronByPtPFBJetTags +
    process.standardSoftElectronByIP3dCaloBJetTags +
    process.standardSoftElectronByIP3dPFBJetTags 
)

process.bTagValidation = cms.Sequence(
    process.caloBTagAnalysis +
    process.pfBTagAnalysis 
)

process.plots = cms.Path(
  process.bit40 +
  process.bptxAnd +
#  process.physDecl +
  process.singleJetHLTFilter +
  process.noscraping +
  ################################################
  ####include refitting
  #################################################
  process.generalTracks*
  process.offlinePrimaryVertices*
  ##################################################   
  process.oneGoodVertexFilter +
  process.ak5PFJetsL2L3 *
  cms.ignore(process.PFJetsFilter) *
  process.ak5JetID *
  cms.ignore(process.caloJetIDFilter) *
  process.ak5CaloJetsL2L3 *
  process.trackAssociation *
  process.ipTagInfos *
  process.svTagInfos *
  process.ipTaggers *
  process.svTaggers *
  process.slTagInfos *
  process.slTaggers *
  process.bTagNtuples *
  process.HLT_Jet15U *
  process.bTagValidation * 
  process.MEtoEDMConverter
)

process.outpath = cms.EndPath(process.EDM)

## Added for getting the L1 and HLT summary report
process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

process.MessageLogger.categories.append('L1GtTrigReport')
process.MessageLogger.categories.append('HLTrigReport')
