
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

#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"

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

  G4VPhysicalVolume* volume2 
  = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double stepl = 0.;

  G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4StepPoint* postPoint = aStep->GetPostStepPoint();

  G4ThreeVector preZvec=prePoint->GetPosition();
  G4ThreeVector postZvec=postPoint->GetPosition();

  G4double preZ=preZvec.getX();
  G4double postZ=postZvec.getX();

  G4int copyNum=0;
  G4int parentID = aStep->GetTrack()->GetParentID();

  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
    stepl = aStep->GetStepLength();
  
  if (volume == fDetector->GetAbsorber()){
    fEventAction->AddAbs(edep,stepl);
  }
  G4String material= aStep -> GetTrack() -> GetMaterial() -> GetName();
  if (postPoint->GetProcessDefinedStep()->GetProcessName()=="hIoni")
    {
      if (volume == fDetector->GetGap() && parentID==0){
	if(volume != volume2)G4cout << "different volumes"<<G4endl;
	copyNum = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);
	//	G4cout <<  material <<G4endl;
	//	G4double vals=257.5 + (preZ+postZ)/2. - (copyNum)*10.3-.15*2;
	G4double preGasZ=257.5 + preZ - (copyNum)*10.3-.15*2;
	G4double postGasZ=257.5 + postZ - (copyNum)*10.3-.15*2;

	G4double stripNumber=0.;
	G4double stripOff=1.;
	if(preGasZ>0. and preGasZ<=3.33)stripOff=1; 
	if(preGasZ>3.33 and preGasZ<=6.66)stripOff=2; 
	if(preGasZ>6.66 and preGasZ<=10.)stripOff=3; 
	if(preGasZ>10. and preGasZ<0.)G4cout <<"check geometry"<<G4endl; 
	/*
	G4cout<<std::setw(8)<<std::setprecision(5) <<"  layer: "<<   copyNum;
	G4cout<<std::setw(10)<<std::setprecision(5) <<"preGasZ:"<<preGasZ;
	G4cout<<std::setw(8)<<std::setprecision(5) <<"  edep: "<<   edep;
	G4cout<<std::setw(8)<<std::setprecision(5) <<"  preZ: "<<   preZ;
	G4cout<<std::setw(8)<<std::setprecision(5) <<"  postZ: "<<   postZ;
	G4cout<<std::setw(8)<<std::setprecision(5) <<"  stepL: "<<   stepl;
	G4cout<<std::setw(8)<<std::setprecision(5) <<"   stripOff: "<<   stripOff  <<G4endl;
	*/
	stripNumber=3*copyNum+stripOff;
	fHistoManager->FillHisto(5, preGasZ, 1);    
	fHistoManager->FillHisto(6, preGasZ, edep);    
	fHistoManager->FillHisto(7, postGasZ, 1);    
	fHistoManager->FillHisto(8, postGasZ, edep);    

	fHistoManager->FillHisto(9, preZ, 1);    
	fHistoManager->FillHisto(10, preZ, edep);    
	fHistoManager->FillHisto(11, postZ, 1);    
	fHistoManager->FillHisto(12, postZ, edep);    

	G4double start=0;
	if(preGasZ>0. and preGasZ<=3.33)start=1; 
	if(preGasZ>3.33 and preGasZ<=6.66)start=2; 
	if(preGasZ>6.66 and preGasZ<=10.)start=3; 
	if(preGasZ>10. and preGasZ<0.)G4cout <<"check geometry"<<G4endl; 


	G4double stop=0;
	if((preGasZ+stepl)>0. and (preGasZ+stepl)<=3.33)stop=1; 
	if((preGasZ+stepl)>3.33 and (preGasZ+stepl)<=6.66)stop=2; 
	if((preGasZ+stepl)>6.66 and (preGasZ+stepl)<=10.)stop=3; 
	if((preGasZ+stepl)>10. and (preGasZ+stepl)<0.)G4cout <<"check geometry"<<G4endl; 

	if(stop == 1 && start == 1)fHistoManager->FillHisto(13, 3*copyNum+1, edep);        
	if(stop == 2 && start == 2)fHistoManager->FillHisto(13, 3*copyNum+2, edep);    
	if(stop == 3 && start == 3)fHistoManager->FillHisto(13, 3*copyNum+3, edep);    

	G4double edep1,edep2,edep3;


	if(stop == 3 && start == 1)
	  {

	    edep1=(10/3.-preGasZ)/stepl*edep;
	    edep2=10/3./stepl*edep;
	    edep3=(preGasZ+stepl-20/3.)/stepl*edep;
	    //	    G4cout <<edep1<< "   "<<edep2<< "   "<<edep3<< "   "<<edep<< G4endl;
	    //G4cout << edep-edep1-edep2-edep3<<G4endl;
	    fHistoManager->FillHisto(13, 3*copyNum+1, edep1);    
	    fHistoManager->FillHisto(13, 3*copyNum+2, edep2);    
	    fHistoManager->FillHisto(13, 3*copyNum+3, edep3);    
	  }

	if(stop == 2 && start == 1)
	  {

	    edep1=(10/3.-preGasZ)/stepl*edep;
	    edep2=(preGasZ+stepl-10./3.)/stepl*edep;
	    edep3=0;
	    //	    G4cout <<edep1<< "   "<<edep2<< "   "<<edep3<< "   "<<edep<< G4endl;
	    //	    G4cout << edep-edep1-edep2-edep3<<G4endl;
	    fHistoManager->FillHisto(13, 3*copyNum+1, edep1);    
	    fHistoManager->FillHisto(13, 3*copyNum+2, edep2);    
	    fHistoManager->FillHisto(13, 3*copyNum+3, edep3);    

	  }

	if(stop == 3 && start == 2)
	  {

	    edep1=0;
	    edep2=(20/3.-preGasZ)/stepl*edep;
	    edep3=(preGasZ+stepl-20./3)/stepl*edep;
	    //	    G4cout <<edep1<< "   "<<edep2<< "   "<<edep3<< "   "<<edep<< "   "<<preGasZ<<G4endl;
	    //	    G4cout << edep-edep1-edep2-edep3<<G4endl;
	    fHistoManager->FillHisto(13, 3*copyNum+1, edep1);    
	    fHistoManager->FillHisto(13, 3*copyNum+2, edep2);    
	    fHistoManager->FillHisto(13, 3*copyNum+3, edep3);    
	  }

		//ePerStrip(postz,edep);
	fEventAction->AddGap(edep,stepl);
	fEventAction->gapEnergy(edep, copyNum);
      }
      
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
