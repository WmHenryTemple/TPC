//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file analysis/AnaEx02/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
:fRootFile(0), 
 fNtuple1(0), fNtuple2(0), 
 fEabs(0.), fEgap(0.) ,fLabs(0.), fLgap(0.)
{
      
  // histograms
  for (G4int k=0; k<kMaxHisto; k++) fHisto[k] = 0;
    
  // ntuple
  fNtuple1 = 0;
  fNtuple2 = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  if (fRootFile) delete fRootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{ 
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  //
  G4String fileName = "AnaEx02.root";
  fRootFile = new TFile(fileName,"RECREATE");
  if (! fRootFile) {
    G4cout << " HistoManager::Book :" 
           << " problem creating the ROOT TFile "
           << G4endl;
    return;
  }
  
  // id = 0
  fHisto[0] = new TH1D("EAbs", "Edep in absorber (MeV)", 100, 20., 60*CLHEP::MeV);
  // id = 1
  fHisto[1] = new TH1D("EGap", "Edep in gap (MeV)", 100, 0., 5*CLHEP::MeV);
  // id = 2
  fHisto[2] = new TH1D("LAbs", "trackL in absorber (mm)", 100, 0., 20*CLHEP::mm);
  // id = 3
  fHisto[3] = new TH1D("LGap", "trackL in gap (mm)", 100, 0., 60*CLHEP::cm);

  fHisto[4] = new TH1D("dedx", "Energy Deposited vs Chamber # ", 60, -0.5, 59.5);
  fHisto[25] = new TH1D("dedx_hIoni", "Energy Deposited vs Chamber # ", 60, -0.5, 59.5);
  fHisto[26] = new TH1D("dedx_sec", "Energy Deposited vs Chamber # ", 60, -0.5, 59.5);
  fHisto[27] = new TH1D("dedx_prim_other", "Energy Deposited vs Chamber # ", 60, -0.5, 59.5);  

  //  fHisto[5] = new TH1D("dedxZ", "Energy Deposited ", 166, -257.3, 257.3)
  fHisto[5] = new TH1D("evPre", "# steps vs chamber pre-step position ", 1000, -.1, 12);
  fHisto[6] = new TH1D("dePre", "Energy deposited vs chamber pre-step position ", 1000, -.1, 12);
  fHisto[7] = new TH1D("evPost", "# steps vs chamber post-step position ", 1000, -.1, 12);
  fHisto[8] = new TH1D("dePost", "Energy deposited vs chamber post-step position ", 1000, -.1, 12);

  fHisto[9] = new TH1D("evPreZ", "# steps vs pre-step Z position ", 1000, 300, -300);
  fHisto[10] = new TH1D("dePreZ", "Energy deposited vs pre-step Z position ", 1000, 300, -300);
  fHisto[11] = new TH1D("evPostZ", "# steps vs post-step Z position ", 1000, 300, -300);
  fHisto[12] = new TH1D("dePostZ", "Energy deposited vs post-step Z position ", 1000, 300, -300);

  fHisto[13] = new TH1D("deStrip_shared", "Energy deposited vs Strip number  ", 200, 0.5, 200.5);
  fHisto[14] = new TH1D("deStrip", "Energy deposited vs Strip number  ", 200, 0.5, 200.5);

  fHisto[15] = new TH1D("lastStrip", "Last Strip number Hit  ", 200, 0.5, 200.5);

  fHisto[16] = new TH1D("lastGap", "Last Gap Hit  ", 60, 0.5, 60.5);
  fHisto[17] = new TH1D("stopVolume", "Last Volume Hit  ", 4, -0.5, 3.5);
  fHisto[18] = new TH1D("stragglingShutter", "Straggling from Shutter;MeV  ", 300, 0, 100);
  fHisto[19] = new TH1D("eLastAbs", "Energy in Last Absorber;MeV  ", 500, 0, 15);
  fHisto[20] = new TH1D("stragglingPatient", "Straggling  Patient; MeV  ", 600, 0, 200);
  fHisto[21] = new TH1D("deAbs", "Energy deposited by Absorber; MeV  ", 600, 0, 200);
  fHisto[23] = new TH1D("numAbs", "No Events by Absorber; MeV  ", 600, 0, 200);  
  fHisto[22] = new TH1D("lastAbs", "Energy deposited in Last Absorber; MeV  ", 600, 0, 10);
  fHisto[24] = new TH1D("eBeamB4Det", "Beam Energy After Annulus; MeV  ", 600, 0, 200);             


  

  for ( G4int i=0; i<kMaxHisto; ++i ) {
    if (! fHisto[i]) G4cout << "\n can't create histo " << i << G4endl;
  }  

  // create 1st ntuple
  fNtuple1 = new TTree("Ntuple1", "Edep");
  fNtuple1->Branch("Eabs", &fEabs, "Eabs/D");
  fNtuple1->Branch("Egap", &fEgap, "Egap/D");
  fNtuple1->Branch("y0", &fy0, "y0/D");
  fNtuple1->Branch("z0", &fz0, "z0/D");  
  fNtuple1->Branch("ebeam", &feb, "eb/D");
  fNtuple1->Branch("lastGap", &fLastGap, "eb/D");
  fNtuple1->Branch("ePostSamp", &fEpostSamp, "eb/D");

  // create 2nd ntuple 
  fNtuple2 = new TTree("Ntuple2", "TrackL");
  fNtuple2->Branch("Labs", &fLabs, "Labs/D");
  fNtuple2->Branch("Lgap", &fLgap, "Lgap/D");
 
  G4cout << "\n----> Output file is open in " << fileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{ 
  if (! fRootFile) return;
  
  fRootFile->Write();       // Writing the histograms to the file
  fRootFile->Close();       // and closing the tree (and the file)
  
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetLastGap( G4double lastG){
  fLastGap=lastG;
}
void HistoManager::SetePostSamp( G4double ePostSamp){
  fEpostSamp=ePostSamp;
}
void HistoManager::SetInitPos( G4double yy, G4double zz){
  fy0=yy;
  fz0=zz;  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::SetInitEbeam( G4double eb){
  feb=eb;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double HistoManager::GetePostSamp(){
  return fEpostSamp;
}

G4double HistoManager::GetEbeam(){
  return feb;
}

G4double HistoManager::GetXinit(){
  return fz0;
}

G4double HistoManager::GetYinit(){
  return fy0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= kMaxHisto) {
    G4cerr << "---> warning from HistoManager::FillHisto() : histo " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (fHisto[ih]) { fHisto[ih]->Fill(xbin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void HistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= kMaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (fHisto[ih]) fHisto[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillNtuple(G4double energyAbs, G4double energyGap,
                              G4double trackLAbs , G4double trackLGap )
{
  fEabs = energyAbs;
  fEgap = energyGap;
  fLabs = trackLAbs;
  fLgap = trackLGap;

  if (fNtuple1) fNtuple1->Fill();
  if (fNtuple2) fNtuple2->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  G4cout << "\n ----> print histograms statistic \n" << G4endl;
  for ( G4int i=0; i<4; ++i ) {
    TH1D* h1 = fHisto[i];
    const G4String name = h1->GetName();  

    G4String unitCategory;
    if (name[0] == 'E' ) unitCategory = "Energy"; 
    if (name[0] == 'L' ) unitCategory = "Length";

    G4cout << name
           << ": mean = " << G4BestUnit(h1->GetMean(), unitCategory) 
           << " rms = " << G4BestUnit(h1->GetRMS(), unitCategory ) 
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
