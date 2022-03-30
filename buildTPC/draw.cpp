{
  gStyle->SetTitleW(.65);
  gStyle->SetTitleX(.1);
  gStyle->SetTitleH(.08);
  
  
  TCanvas *c1=new TCanvas();
  c1->Divide(2,2);
  c1->cd(1);
  LAbs->Draw();
  c1->cd(2);
  EAbs->Draw();
  c1->cd(3);
  LGap->Draw();
  c1->cd(4);
  EGap->Draw();
  c1->SaveAs("PDFs/fourPan.pdf");
  
  c1->Clear();
  dedx->GetYaxis()->SetTitle("(Energy deposited)/event (MeV)");
  dedx->GetYaxis()->SetTitle("Ar_C02 Gap Layer #");
  dedx->SetMarkerStyle(20);
  dedx->SetMarkerSize(.5);
  dedx->Scale(1./100000);
  dedx->Draw("HIST p");
  c1->SaveAs("PDFs/edepVsChamberNumber.pdf");

  c1->Clear();
  deStrip->GetYaxis()->SetTitle("(Energy deposited)/event (MeV)");
  deStrip->GetXaxis()->SetTitle("Strip Number");
  deStrip->SetMarkerStyle(20);
  deStrip->SetMarkerSize(.5);
  deStrip->Scale(1./100000);
  deStrip->Draw("HIST p");
  c1->SaveAs("PDFs/edepVsStripNumber.pdf");

  c1->Clear();
  gPad->SetLogy();
  lastStrip->GetYaxis()->SetTitle("Counts");
  lastStrip->GetXaxis()->SetTitle("LastStrip Hit");
  lastStrip->SetMarkerStyle(20);
  lastStrip->SetMarkerSize(.5);
  lastStrip->Draw("HIST");
  c1->SaveAs("PDFs/lastStrip.pdf");

}
