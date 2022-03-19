{
  // Draw histos filled by Geant4 simulation 
  //   
  TFile *f = new TFile("AnaEx02.root");  
  TCanvas* c1 = new TCanvas("c1", "  ");
  
  TH1D* hist1 = (TH1D*)f->Get("EAbs");
  TH1D* hist2 = (TH1D*)f->Get("EGap");
  TH1D* hist3 = (TH1D*)f->Get("LAbs");
  TH1D* hist4 = (TH1D*)f->Get("LGap");
  c1->SetLogy(1);
  c1->Divide(2,2);
  c1->cd(1);
  hist1->Draw("HIST");    
  c1->cd(2);
  hist2->Draw("HIST");    
  c1->cd(3);
  hist3->Draw("HIST");    
  c1->cd(4);
  hist4->Draw("HIST");    
}  
