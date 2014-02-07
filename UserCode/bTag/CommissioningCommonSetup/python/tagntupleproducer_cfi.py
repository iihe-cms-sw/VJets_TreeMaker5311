import FWCore.ParameterSet.Config as cms

bTagNtuple = cms.EDAnalyzer('TagNtupleProducer',
                            getMCTruth = cms.bool(True),
                            getMCPUInfo = cms.bool(True),
                            jetSrc = cms.InputTag( "ak5CaloJets" ),
                            svComputer = cms.InputTag( "combinedSecondaryVertex" ),
                            TriggerTag = cms.InputTag("TriggerResults::HLT8E29"),
                            jetMCSrc = cms.InputTag( "AK5byValAlgo" ),
                            jetTracks = cms.InputTag( "ak5JetTracksAssociatorAtVertex" ),
                            primaryVertex = cms.InputTag("offlinePrimaryVertices"),
                            SVTagInfos = cms.InputTag( "secondaryVertexTagInfos" ),
                            IPTagInfos = cms.InputTag( "impactParameterTagInfos" ),
                            filename = cms.string("testfile.root"),
                            ElectronTagInfos = cms.InputTag( "softElectronTagInfos" ),
                            MuonTagInfos = cms.InputTag( "softMuonTagInfos" ),
                            getSharedHitInfo = cms.bool(False),
                            Label = cms.string(""),
                            bTagConfig = cms.VPSet(
                                cms.PSet(
                                    alias = cms.string("trackCountingHighEffBJetTags"),
                                    label = cms.InputTag("trackCountingHighEffBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("trackCountingHighPurBJetTags"),
                                    label = cms.InputTag("trackCountingHighPurBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("jetProbabilityBJetTags"),
                                    label = cms.InputTag("jetProbabilityBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("jetBProbabilityBJetTags"),
                                    label = cms.InputTag("jetBProbabilityBJetTags")
                                    ),
                                
                                cms.PSet(
                                    alias = cms.string("simpleSecondaryVerteBJetTags"),
                                    label = cms.InputTag("simpleSecondaryVertexBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("combinedSecondaryVertexBJetTags"),
                                    label = cms.InputTag("combinedSecondaryVertexBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("combinedSecondaryVertexMVABJetTags"),
                                    label = cms.InputTag("combinedSecondaryVertexMVABJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("softMuonBJetTags"),
                                    label = cms.InputTag("softMuonBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("softMuonByIP3dBJetTags"),
                                    label = cms.InputTag("softMuonByIP3dBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("softMuonByPtBJetTags"),
                                    label = cms.InputTag("softMuonByPtBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("softElectronByIP3dBJetTags"),
                                    label = cms.InputTag("softElectronByIP3dBJetTags")
                                    ), 
                                cms.PSet(
                                    alias = cms.string("softElectronByPtBJetTags"),
                                    label = cms.InputTag("softElectronByPtBJetTags")
                                    )
                                )
                            )

