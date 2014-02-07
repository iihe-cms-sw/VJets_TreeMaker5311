#ifndef bTag_CommissioningCommonSetup_BaseSelector_h
#define bTag_CommissioningCommonSetup_BaseSelector_h

#include <TSelector.h>
#include <TChain.h>
#include <TBranch.h>
#include <TLeaf.h>

#include <string> 

class BaseSelector : public TSelector
{
  public:
    BaseSelector() : TSelector(), fChain(0), treeWeight(0.0), isData(false) {};
    virtual ~BaseSelector() {};

    inline virtual Int_t Version() {return 2;}
    virtual void Init(TTree* tree) {};
    inline virtual Bool_t Notify() {return kFALSE;}
    inline virtual Bool_t Process(Long64_t entry) {return kFALSE;}
    inline virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0)
    {
      return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
    }
    inline virtual void SetIsData(const bool data) {isData = data;}
    inline virtual bool const GetIsData() const {return isData;}
    template <class T>
    T* SetPtrToValue(TBranch*& branch, const std::string& branchName)
    {
      branch = fChain->GetBranch(branchName.c_str());
      return static_cast<T*>(branch->GetLeaf(branchName.c_str())->GetValuePointer());
    }

  protected:
    TTree* fChain;
    double treeWeight;
    bool isData;
};

#endif
