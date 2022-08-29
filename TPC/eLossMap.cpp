void *eLossMap(){
  TFile *f=new TFile("AnaEx02.root");
  //  TFile *f=new TFile("annulus_70to110MeV_100k.root");

  TTree * t1=(TTree*)f->Get("Ntuple1");
  TTree * t2=(TTree*)f->Get("Ntuple2");

  Double_t egap, y, z, eb, lg, ePostSamp;

  t1->SetBranchAddress("Egap",&egap);
  t1->SetBranchAddress("ebeam",&eb);
  t1->SetBranchAddress("y0",&y);
  t1->SetBranchAddress("z0",&z);
  t1->SetBranchAddress("lastGap",&lg);
  t1->SetBranchAddress("ePostSamp",&ePostSamp);

  TH1D *h1=new TH1D("h1","h1",300,-20,20);
  int nbin=50;
  double wid=10;
  TH2D *hmap=new TH2D ("hmap","Hit Map (Truth)",nbin,-wid,wid,nbin,-wid,wid);
  TH2D *hmap_predict=new TH2D ("hmap_predict","Hit Map (Predict)",nbin,-wid,wid,nbin,-wid,wid);
  TH2D *hmap_truth=new TH2D ("hmap_truth","Hit Map (Truth)",nbin,-wid,wid,nbin,-wid,wid);
  Int_t n=t1->GetEntries();
  double p0=4.14801;
  double p1=1.70916;
  double p2=-0.0485724;
  double p3=0.00089738;
  double p4=-6.24858e-06;
  
  for(int i=0;i<n;i++){
    t1->GetEntry(i);
    if(lg>0 && lg<60){
    double ebeam_predict=p0 + p1*lg+ p2*lg*lg + p3*lg*lg*lg * p4*lg*lg*lg*lg;
    double howClose=ePostSamp-ebeam_predict;
    double eLoss=eb-ebeam_predict;
    double eLoss_truth=eb-ePostSamp;
    if(howClose<3){
    h1->Fill(howClose);
    hmap_predict->Fill(y,z,eLoss);
    hmap_truth->Fill(y,z,eLoss_truth);
    hmap->Fill(y,z);
    }
    }
  }
  hmap_predict->Divide(hmap);
  hmap_truth->Divide(hmap);
  TCanvas* c1=new TCanvas();
  c1->Divide(3,1);
  c1->cd(1);
  hmap_predict->Draw("COLZ");
  c1->cd(2);
  hmap_truth->Draw("COLZ");
  c1->cd(3);
  h1->Draw();
  return 0;
}
