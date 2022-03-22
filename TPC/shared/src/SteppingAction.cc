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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "G4Step.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det,EventAction* evt, HistoManager* histo)                                         
: G4UserSteppingAction(), 
  fDetector(det), fEventAction(evt), fHistoManager(histo)                                         
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the current step
  G4VPhysicalVolume* volume 
  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4StepPoint* postPoint = aStep->GetPostStepPoint();
  G4ThreeVector preZvec=prePoint->GetPosition();
  G4ThreeVector postZvec=postPoint->GetPosition();

  G4double preZ=preZvec.getX();
  G4double postZ=preZvec.getX();

  //  G4cout << preZ <<"\t" << postZ << G4endl;
  //  G4Touchable* touch = aStep->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber();;

  G4int copyNum=0;
  if (volume == fDetector->GetGap()){
  copyNum = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);
  //  G4cout << "Copy Number" << copyNum << G4endl;
  //  G4cout <<"Name:" << volume->GetName() << G4endl<<G4endl;
  }
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double stepl = 0.;

  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
    stepl = aStep->GetStepLength();
  
  if (volume == fDetector->GetAbsorber()){
    fEventAction->AddAbs(edep,stepl);
  }

  if (volume == fDetector->GetGap()){
    fHistoManager->FillHisto(5, preZ, edep);    
  //ePerStrip(postz,edep);
    fEventAction->AddGap(edep,stepl);
    fEventAction->gapEnergy(edep, copyNum);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
