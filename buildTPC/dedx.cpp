TGraph *dedx(){
  TFile *f=new TFile("AnaEx02.root");
  TTree * t1=(TTree*)f->Get("Ntuple1");
  TTree * t2=(TTree*)f->Get("Ntuple2");

  Double_t Eabs, Egap, Labs, Lgap;
  t1->SetBranchAddress("Eabs",&Eabs);
  t1->SetBranchAddress("Egap",&Egap);
  t2->SetBranchAddress("Labs",&Labs);
  t2->SetBranchAddress("Lgap",&Lgap);

  Double_t Lgap_last=0;
  vector <double> x,y;
  Int_t n=t1->GetEntries();
  for(int i=0;i<n;i++){
    t1->GetEntry(i);
    t2->GetEntry(i);
    Double_t dex=Egap/(Lgap-Lgap_last);

    x.push_back(Lgap);
    y.push_back(dex);
    Lgap_last=Lgap;
    cout<<Lgap<<"    "<<dex<<endl;
  }

  TGraph *g=new TGraph(x.size(),&x[0],&y[0]);
  g->SetMarkerStyle(33);
  return g;
}
