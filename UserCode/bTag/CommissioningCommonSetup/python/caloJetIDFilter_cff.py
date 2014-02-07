import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak5JetID_cfi import *
from bTag.CommissioningCommonSetup.caloJetIDFilter_cfi import *

caloJetID = cms.Sequence(ak5JetID * caloJetIDFilter)

