import FWCore.ParameterSet.Config as cms

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

process = cms.Process("harvest")
process.load("DQMServices.Components.DQMEnvironment_cfi")

#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("DQMServices.Core.DQM_cfg")
process.DQMStore.collateHistograms = True

process.load("RecoBTag.Configuration.RecoBTag_cff")

#For harvesting of data
process.load("DQMOffline.RecoB.bTagAnalysisData_cfi")
#For harvesting of MC
#process.load("Validation.RecoB.bTagAnalysis_harvesting_cfi")
#For harvesting of data
process.bTagAnalysisHarvest = process.bTagAnalysis.clone()
#For harvesting of MC
#process.bTagAnalysisHarvest = process.bTagValidationHarvest.clone()
process.bTagAnalysisHarvest.finalizePlots = True
process.bTagAnalysisHarvest.finalizeOnly = True
process.bTagAnalysisHarvest.differentialPlots = cms.bool(False)
process.bTagAnalysisHarvest.ptRecJetMin = 30.0
process.bTagAnalysisHarvest.etaMax = 2.5
process.bTagAnalysisHarvest.ptRanges = cms.vdouble(30.0, 80.0)
process.bTagAnalysisHarvest.etaRanges = cms.vdouble(0.0, 1.5, 2.5)
process.bTagAnalysisHarvest.tagConfig = cms.VPSet(
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
        ), 
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
process.bTagAnalysisHarvest.tagConfig[2].parameters.categories[0].vertexMass.min = 0.3
process.bTagAnalysisHarvest.tagConfig[2].parameters.categories[0].vertexMass.max = 0.8
process.bTagAnalysisHarvest.tagConfig[2].parameters.categories[1].vertexMass.min = 0.3
process.bTagAnalysisHarvest.tagConfig[2].parameters.categories[1].vertexMass.max = 0.8
process.bTagAnalysisHarvest.tagConfig[2].parameters.categories[2].vertexMass.min = 0.3
process.bTagAnalysisHarvest.tagConfig[2].parameters.categories[2].vertexMass.max = 0.8
process.bTagAnalysisHarvest.tagConfig[22].parameters.categories[0].vertexMass.min = 0.3
process.bTagAnalysisHarvest.tagConfig[22].parameters.categories[0].vertexMass.max = 0.8
process.bTagAnalysisHarvest.tagConfig[22].parameters.categories[1].vertexMass.min = 0.3
process.bTagAnalysisHarvest.tagConfig[22].parameters.categories[1].vertexMass.max = 0.8
process.bTagAnalysisHarvest.tagConfig[22].parameters.categories[2].vertexMass.min = 0.3
process.bTagAnalysisHarvest.tagConfig[22].parameters.categories[2].vertexMass.max = 0.8
for i in range(4, 17):
  for j in range(i + 1, 17):
    process.bTagAnalysisHarvest.tagConfig.append(
      cms.PSet(
        type = cms.string("TagCorrelation"),
        label1 = process.bTagAnalysisHarvest.tagConfig[i].label,
        label2 = process.bTagAnalysisHarvest.tagConfig[j].label,
        parameters = cms.PSet(
          CreateProfile = cms.bool(True),
          Discr1Start = process.bTagAnalysisHarvest.tagConfig[i].parameters.discriminatorStart,
          Discr1End = process.bTagAnalysisHarvest.tagConfig[i].parameters.discriminatorEnd,
          Discr2Start = process.bTagAnalysisHarvest.tagConfig[j].parameters.discriminatorStart,
          Discr2End = process.bTagAnalysisHarvest.tagConfig[j].parameters.discriminatorEnd
        )
      )
    )
for i in range(24, 36):
  for j in range(i + 1, 36):
    process.bTagAnalysisHarvest.tagConfig.append(
      cms.PSet(
        type = cms.string("TagCorrelation"),
        label1 = process.bTagAnalysisHarvest.tagConfig[i].label,
        label2 = process.bTagAnalysisHarvest.tagConfig[j].label,
        parameters = cms.PSet(
          CreateProfile = cms.bool(True),
          Discr1Start = process.bTagAnalysisHarvest.tagConfig[i].parameters.discriminatorStart,
          Discr1End = process.bTagAnalysisHarvest.tagConfig[i].parameters.discriminatorEnd,
          Discr2Start = process.bTagAnalysisHarvest.tagConfig[j].parameters.discriminatorStart,
          Discr2End = process.bTagAnalysisHarvest.tagConfig[j].parameters.discriminatorEnd
        )
      )
    )

## both the EDM FWK and the DQM FWK (with collate=on) add up the histograms: the below setting should switch off the EDM one
## https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideEDMParametersForModules
process.options = cms.untracked.PSet(
    fileMode = cms.untracked.string('NOMERGE')
  )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(),
    inputCommands = cms.untracked.vstring("drop *",
      "keep *_MEtoEDMConverter_*_validation"
    )
)

process.load("DQMServices.Components.EDMtoMEConverter_cfi")


process.plots = cms.Path(process.EDMtoMEConverter* process.bTagAnalysisHarvest*process.dqmSaver)
#process.plots = cms.Path(process.EDMtoMEConverter* process.dqmSaver)
process.dqmEnv.subSystemFolder = 'BTAG'
process.dqmSaver.producer = 'DQM'
process.dqmSaver.workflow = '/POG/BTAG/BJET'
process.dqmSaver.convention = 'Offline'
process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd =cms.untracked.bool(True) 
process.dqmSaver.forceRunNumber = cms.untracked.int32(1)

## Dijet pT 0 to 15
#for i in range(1,52):
#  process.PoolSource.fileNames.append("rfio:///castor/cern.ch/user/a/alschmid/btagApr30/BTagCommissioning2010_April20_7TeV_MC_"+str(i)+"_1.root")

## Dijet pT 15 to 20
#for i in range(1,52):
#  process.PoolSource.fileNames.append("rfio:///castor/cern.ch/user/a/alschmid/btagApr30_mid/BTagCommissioning2010_April20_7TeV_MC_"+str(i)+"_1.root")

process.PoolSource.fileNames.append(
  "file:/uscms_data/d2/jkeller7/bTagDevelopment/CMSSW_3_5_8/src/BTagCommissioning2010_April20_7TeV_MC.root"
)

#process.PoolSource.fileNames.append(
#  "file:/uscms_data/d2/jkeller7/bTagDevelopment/CMSSW_3_5_8/src/BTagCommissioning2010_April20_7TeV_Data_PromptReco_v8.root"
#)


