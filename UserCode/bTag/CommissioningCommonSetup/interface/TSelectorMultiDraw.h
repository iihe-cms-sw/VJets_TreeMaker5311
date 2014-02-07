//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 11 00:27:16 2010 by ROOT version 5.22/00d
// from TTree /
// found on file: Memory Directory
//////////////////////////////////////////////////////////

#ifndef bTag_CommissioningCommonSetup_TSelectorMultiDraw_h
#define bTag_CommissioningCommonSetup_TSelectorMultiDraw_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/Information1D.h"
#include "bTag/CommissioningCommonSetup/interface/Information2D.h"
#include "bTag/CommissioningCommonSetup/interface/InformationCutComp.h"
#include "bTag/CommissioningCommonSetup/interface/InformationTrackCuts.h"
#include "bTag/CommissioningCommonSetup/interface/InformationMuonCuts.h"
#include "bTag/CommissioningCommonSetup/interface/InformationQuality.h"
#include "bTag/CommissioningCommonSetup/interface/InformationPtHat.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <vector>
#include <string>

class TSelectorMultiDraw : public TSelector {
  public :
   TSelectorMultiDraw() : TSelector() { }
   virtual ~TSelectorMultiDraw();
   inline virtual Int_t Version() const { return 2; }
   virtual void Begin(TTree *tree);
   virtual void Init(TTree *tree);
   inline virtual Bool_t Notify()
   {
     for(unsigned int i = 0; i != fSelectors.size(); ++i)
       fSelectors[i]->Notify();

     return kTRUE;
   }
   virtual Bool_t Process(Long64_t entry);
   inline virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) {
     return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
   }
   inline virtual void SetOption(const char *option) {
     for(unsigned int i = 0; i != fSelectors.size(); ++i)
       fSelectors[i]->SetOption(option);  
   }
   inline virtual void Terminate()
   {
     for(unsigned int i = 0; i != fSelectors.size(); ++i) fSelectors[i]->Terminate();
   }
   inline virtual void SetIsData(const bool isData)
   {
     for(unsigned int i = 0; i != fSelectors.size(); ++i) fSelectors[i]->SetIsData(isData);
   }
   virtual void    AddGenericSelector(const Information1D& info);
   virtual void    AddGenericSelector2D(const Information2D& info);
   virtual void    AddGenericTrackSelector(const Information1D& info);
   virtual void    AddGenericTrackSelector2D(const Information2D& info);
   virtual void    AddGenericTrackQualitySelector(const InformationQuality& info);
   virtual void    AddGenericJetTrackQualitySelector(const InformationQuality& info);
   virtual void    AddTrackSelector(const InformationTrackCuts& info);
   virtual void    AddMuonSelector(const InformationMuonCuts& info);
   virtual void    AddCutCompSelector(const InformationCutComp& info);
   virtual void    AddPtHatSelector(const InformationPtHat& info);

   virtual std::vector<BaseSelector*> const getSelectors() const {return fSelectors;}

  protected:
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   std::vector<BaseSelector*> fSelectors; //vector of TSelectors
};

#endif
