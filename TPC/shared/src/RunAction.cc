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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager* histo)
: G4UserRunAction(),
  fHistoManager(histo),
  fSumEAbs(0.), fSum2EAbs(0.),
  fSumEGap(0.), fSum2EGap(0.),
  fSumLAbs(0.), fSum2LAbs(0.),
  fSumLGap(0.), fSum2LGap(0.)    
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    
  //initialize cumulative quantities
  //
  fSumEAbs = fSum2EAbs =fSumEGap = fSum2EGap = 0.;
  fSumLAbs = fSum2LAbs =fSumLGap = fSum2LGap = 0.;
  
  //histograms
  //
  fHistoManager->Book(); 
  outTxt.open("ePerStrip.txt",std::ios::out);

  outTxt <<"Gap#"<<"\t";
    outTxt <<std::setw(7)<<std::setprecision(4)<<std::fixed;
    outTxt << "Ebeam" <<"\t";
    outTxt <<"E_post_samp" <<"\t";
    outTxt <<"E_dep" <<"\t";
    outTxt <<"E_dep_sec"<< "\t"; 
    outTxt << "x0" << "\t"; 
    outTxt << "y0" << G4endl;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::WritePerEvent(G4double arr[], G4int size){

  for(G4int i=0; i<size; i++){
    G4cout <<i<<"\t"<< arr[i] << G4endl;
    outTxt <<i<<"\t"<< arr[i] << G4endl;    
  }
}

void RunAction::WritePerEvent(G4double arr[], G4int size, G4double arr2[]){

  for(G4int i=0; i<size; i++){
    //    G4cout <<i<<"\t"<< arr[i] <<"\t"<<arr2[i]<< G4endl;
    outTxt <<i<<"\t"<< arr[i] <<"\t"<<arr2[i]<< G4endl;    
  }
}

void RunAction::WritePerEvent(G4double arr[], G4int size, G4double arr2[], G4double ebeam, G4double x0, G4double y0, G4double ePostSamp){

  for(G4int i=0; i<size; i++){
    //    G4cout <<i<<"\t"<< arr[i] <<"\t"<<arr2[i]<< G4endl;
    /*
    outTxt <<std::setw(3)<<std::setprecision(6)<<std::fixed;
    outTxt <<i<<"\t"<< ebeam <<"\t";
    outTxt <<std::setw(6)<<std::setprecision(4)<<std::fixed<<ePostSamp <<"\t";
    outTxt <<std::setw(6)<<std::setprecision(4)<<std::fixed<<arr[i] <<"\t";
    outTxt <<std::setw(6)<<std::setprecision(4)<<std::fixed<<arr2[i]<< "\t"; 
    outTxt <<std::setw(6)<<std::setprecision(4)<<std::fixed<< x0 << "\t"; 
    outTxt <<std::setw(6)<<std::setprecision(4)<<std::fixed<< y0 << G4endl;    
    */
    outTxt <<i<<"\t";
    outTxt <<std::setw(7)<<std::setprecision(3)<<std::fixed;
    outTxt << ebeam <<"\t";
    outTxt <<ePostSamp <<"\t";
    outTxt <<arr[i] <<"\t";
    outTxt <<arr2[i]<< "\t"; 
    outTxt << x0 << "\t"; 
    outTxt << y0 << G4endl;    
    //    G4cout <<std::setw(9)<<std::setprecision(4)<<std::fixed;
  }
}
void RunAction::FillPerEvent(G4double EAbs, G4double EGap,
                                  G4double LAbs, G4double LGap)
{
  //accumulate statistic
  //
  fSumEAbs += EAbs;  fSum2EAbs += EAbs*EAbs;
  fSumEGap += EGap;  fSum2EGap += EGap*EGap;
  
  fSumLAbs += LAbs;  fSum2LAbs += LAbs*LAbs;
  fSumLGap += LGap;  fSum2LGap += LGap*LGap;  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  outTxt.close();
  //compute statistics: mean and rms
  //
  fSumEAbs /= NbOfEvents; fSum2EAbs /= NbOfEvents;
  G4double rmsEAbs = fSum2EAbs - fSumEAbs*fSumEAbs;
  if (rmsEAbs >0.) rmsEAbs = std::sqrt(rmsEAbs); else rmsEAbs = 0.;
  
  fSumEGap /= NbOfEvents; fSum2EGap /= NbOfEvents;
  G4double rmsEGap = fSum2EGap - fSumEGap*fSumEGap;
  if (rmsEGap >0.) rmsEGap = std::sqrt(rmsEGap); else rmsEGap = 0.;
  
  fSumLAbs /= NbOfEvents; fSum2LAbs /= NbOfEvents;
  G4double rmsLAbs = fSum2LAbs - fSumLAbs*fSumLAbs;
  if (rmsLAbs >0.) rmsLAbs = std::sqrt(rmsLAbs); else rmsLAbs = 0.;
  
  fSumLGap /= NbOfEvents; fSum2LGap /= NbOfEvents;
  G4double rmsLGap = fSum2LGap - fSumLGap*fSumLGap;
  if (rmsLGap >0.) rmsLGap = std::sqrt(rmsLGap); else rmsLGap = 0.;
  
  //print
  //
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << "\n mean Energy in Absorber : " << G4BestUnit(fSumEAbs,"Energy")
     << " +- "                          << G4BestUnit(rmsEAbs,"Energy")  
     << "\n mean Energy in Gap      : " << G4BestUnit(fSumEGap,"Energy")
     << " +- "                          << G4BestUnit(rmsEGap,"Energy")
     << G4endl;
     
  G4cout
     << "\n mean trackLength in Absorber : " << G4BestUnit(fSumLAbs,"Length")
     << " +- "                               << G4BestUnit(rmsLAbs,"Length")  
     << "\n mean trackLength in Gap      : " << G4BestUnit(fSumLGap,"Length")
     << " +- "                               << G4BestUnit(rmsLGap,"Length")
     << "\n------------------------------------------------------------\n"
     << G4endl;
     
  //save histograms
  //
  fHistoManager->PrintStatistic();
  fHistoManager->Save();   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
