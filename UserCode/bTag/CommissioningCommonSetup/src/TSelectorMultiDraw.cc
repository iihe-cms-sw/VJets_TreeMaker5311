// The class definition in TSelectorMultiDraw.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("TSelectorMultiDraw.C")
// Root > T->Process("TSelectorMultiDraw.C","some options")
// Root > T->Process("TSelectorMultiDraw.C+")
//

#include "bTag/CommissioningCommonSetup/interface/TSelectorMultiDraw.h"
#include "bTag/CommissioningCommonSetup/interface/GenericSelector.h"
#include "bTag/CommissioningCommonSetup/interface/GenericSelector2D.h"
#include "bTag/CommissioningCommonSetup/interface/GenericTrackSelector.h"
#include "bTag/CommissioningCommonSetup/interface/GenericTrackSelector2D.h"
#include "bTag/CommissioningCommonSetup/interface/GenericTrackQualitySelector.h"
#include "bTag/CommissioningCommonSetup/interface/GenericJetTrackQualitySelector.h"
#include "bTag/CommissioningCommonSetup/interface/TrackSelector.h"
#include "bTag/CommissioningCommonSetup/interface/MuonSelector.h"
#include "bTag/CommissioningCommonSetup/interface/CutCompSelector.h"
#include "bTag/CommissioningCommonSetup/interface/PtHatSelector.h"
#include <TH2.h>
#include <TStyle.h>

using namespace std;

TSelectorMultiDraw::~TSelectorMultiDraw()
{
  for(unsigned int i = 0; i != fSelectors.size(); ++i)
    delete fSelectors[i];
}

void TSelectorMultiDraw::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   for(unsigned int i = 0; i != fSelectors.size(); ++i) fSelectors[i]->Init(tree);  
}

void TSelectorMultiDraw::Begin(TTree * tree)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
 
   for(unsigned int i = 0; i != fSelectors.size(); ++i) fSelectors[i]->Begin(tree);  

}

Bool_t TSelectorMultiDraw::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either TSelectorMultiDraw::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.


//    for(list<TSelectorDraw*>::iterator iSelector = fSelectors.begin(); iSelector!=fSelectors.end(); iSelector++)
//      {
//        if((*iSelector)->ProcessCut(entry)) (*iSelector)->ProcessFill(entry);
//      }
//    for(list<TrackSelector*>::iterator iSelector = fTrackSelectors.begin(); iSelector!=fTrackSelectors.end(); iSelector++)
//      {
//          (*iSelector)->Process(entry);
//      }
  for(unsigned int i = 0; i != fSelectors.size(); ++i) fSelectors[i]->Process(entry);
   
   return kTRUE;
}

void TSelectorMultiDraw::AddGenericSelector(const Information1D& info)
{
  fSelectors.push_back(new GenericSelector(info));
}

void TSelectorMultiDraw::AddGenericSelector2D(const Information2D& info)
{
  fSelectors.push_back(new GenericSelector2D(info));
}

void TSelectorMultiDraw::AddGenericTrackSelector(const Information1D& info)
{
  fSelectors.push_back(new GenericTrackSelector(info));
}

void TSelectorMultiDraw::AddGenericTrackSelector2D(const Information2D& info)
{
  fSelectors.push_back(new GenericTrackSelector2D(info));
}

void TSelectorMultiDraw::AddGenericTrackQualitySelector(const InformationQuality& info)
{
  fSelectors.push_back(new GenericTrackQualitySelector(info));
}

void TSelectorMultiDraw::AddGenericJetTrackQualitySelector(const InformationQuality& info)
{
  fSelectors.push_back(new GenericJetTrackQualitySelector(info));
}

void TSelectorMultiDraw::AddTrackSelector(const InformationTrackCuts& info){
  fSelectors.push_back(new TrackSelector(info));
}

void TSelectorMultiDraw::AddMuonSelector(const InformationMuonCuts& info){
  fSelectors.push_back(new MuonSelector(info));
}

void TSelectorMultiDraw::AddCutCompSelector(const InformationCutComp& info){
  fSelectors.push_back(new CutCompSelector(info));
}

void TSelectorMultiDraw::AddPtHatSelector(const InformationPtHat& info){
  fSelectors.push_back(new PtHatSelector(info));
}
