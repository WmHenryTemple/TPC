
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

  G4double preE=prePoint->GetKineticEnergy();
  G4double postE=postPoint->GetKineticEnergy();
  //  if(parentID==0)	G4cout <<"Energy pre, post, and post-pre"<<"\t"<<preE<<"\t"<<postE<<"\t"<<postE-preE<<G4endl;

  G4double zStart=preZvec.getZ();
  G4double yStart=preZvec.getY();
  
  G4double dz=abs(postZ-preZ);//z along beam line here
  G4int copyNum=0;
  G4int parentID = aStep->GetTrack()->GetParentID();
  /////////////////////////////////////////////////////////////////////
  G4bool prim=false;  //is primary particle?
  G4bool start=false;  // "init" step
  if(parentID==0)prim=true;
  if(postPoint->GetProcessDefinedStep()->GetProcessName()=="initStep")start==true;
  if(start && prim)G4cout << "Start point (y,z) z in G4 coordinates" << yStart <<", "<<zStart<<G4endl;

  /////////////////////////////////////////////////////////////////////
  /////////////////  Ebeam after shutter  /////////////////////////////
  if(prim && volume!=volume2 && volume== fDetector->GetShutter()){
    //    G4cout<<"Left shutter:" <<preE<<G4endl;
    fHistoManager->FillHisto(18, postE);
  /////////////////////////////////////////////////////////////////////
  /////////////////  Ebeam after patient  /////////////////////////////
  }
  if(prim && volume!=volume2 && volume== fDetector->GetPatient()){
    //    G4cout<<"Left Patient:" <<preE<<G4endl;
    fHistoManager->FillHisto(20, postE);
  }
  /////////////////////////////////////////////////////////////////////
  /////////////////  Ebeam after annulus  /////////////////////////////
  if(prim && volume2==fDetector->GetphysiWorld() && volume== fDetector->GetAnnulusMother()){
    //    G4cout<<"Left Annulus:" <<preE<<G4endl;
    fHistoManager->FillHisto(24, postE);
  }

  





  

  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
    stepl = aStep->GetStepLength();
  
  // 
  if (volume == fDetector->GetAbsorber()){
    fEventAction->AddAbs(edep,stepl);
    // add energy deposited in array indexed by copy number
    copyNum = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);
    if(parentID==0)fEventAction->absEnergy(edep, copyNum);
    //    G4cout << edep <<"\t"<<copyNum<<G4endl;
  }

  G4String material= aStep -> GetTrack() -> GetMaterial() -> GetName();
  G4bool inGap1=(volume == fDetector->GetGap1());
  G4bool inGap2=(volume == fDetector->GetGap2());
  G4bool inGap3=(volume == fDetector->GetGap3());
  G4bool inGap=(inGap1||inGap2||inGap3);

  // post E of primary particle leaving gap
  if(prim && volume!=volume2 && inGap){
    //    G4cout<<"Left shutter:" <<preE<<G4endl;
    //    G4cout << volume <<"\t"<<volume2<<"\t"<<postE<<G4endl;
    fEventAction->eLastAbs(postE);
  }
  
  G4Track* theTrack = aStep -> GetTrack();

  // check if track is not alive and primary particle
  G4double volumeId=0;
  if(!(theTrack->GetTrackStatus() == fAlive)&&parentID==0){
    //  if(postE<0.25 && parentID==0){
    if(inGap)volumeId=1;
    else if(volume == fDetector->GetAbsorber())volumeId=2;
    else volumeId=3;
    // Cemetary histogram
    fHistoManager->FillHisto(17, volumeId);
  }

  // process = hIoni, primary particle, in Gas chamber   
  if (postPoint->GetProcessDefinedStep()->GetProcessName()=="hIoni"){
      if (inGap && parentID==0){
	//if(volume != volume2)G4cout << "different volumes"<<G4endl;
	copyNum = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);

	// This is wrong need new chamber size
	G4double preGasZ=257.5 + preZ - (copyNum)*10.3-.15*2;
	G4double postGasZ=257.5 + postZ - (copyNum)*10.3-.15*2;
	G4double stripNumber=0.;
	G4double stripOff=1.;

	// TODO distribute energy across strips 
	//diffusion as a function of vertical position
	if(inGap1)stripOff=1;
	if(inGap2)stripOff=2;
	if(inGap3)stripOff=3;
	stripNumber=3*copyNum+stripOff;
	////////////////////////////////////////////////
	fHistoManager->FillHisto(5, preGasZ, 1);    
	fHistoManager->FillHisto(6, preGasZ, edep);    
	fHistoManager->FillHisto(7, postGasZ, 1);    
	fHistoManager->FillHisto(8, postGasZ, edep);    
	fHistoManager->FillHisto(9, preZ, 1);    
	fHistoManager->FillHisto(10, preZ, edep);    
	fHistoManager->FillHisto(11, postZ, 1);    
	fHistoManager->FillHisto(12, postZ, edep);    
	fEventAction->AddGap(edep,stepl);// accumulates edep/step
	fEventAction->gapEnergy(edep, copyNum);// fills array per event
	fEventAction->stripEnergy(edep, stripNumber);// fills array per event
	/////////////////////////////////////////////////
	////////  Find what chamber the step started
	////////  and ended.  Then split up E deposited
	////////  amongst strips.  Not working?!
	G4double start=0;
	if(preGasZ>0. and preGasZ<=3.33)start=1; 
	if(preGasZ>3.33 and preGasZ<=6.66)start=2; 
	if(preGasZ>6.66 and preGasZ<=10.)start=3; 
	if(preGasZ>10. and preGasZ<0.)G4cout <<"check geometry"<<G4endl; 
	G4double stop=0;
	if((abs(preGasZ)+dz)>0. and (abs(preGasZ)+dz)<=3.33)stop=1; 
	if((abs(preGasZ)+dz)>3.33 and (abs(preGasZ)+dz)<=6.66)stop=2; 
	if((abs(preGasZ)+dz)>6.66 and (abs(preGasZ)+dz)<=10.)stop=3; 
	if((abs(preGasZ)+dz)>10. and (abs(preGasZ)+dz)<0.)G4cout <<"check geometry"<<G4endl; 
	if(stop == 1 && start == 1)fHistoManager->FillHisto(13, 3*copyNum+1, edep);        
	if(stop == 2 && start == 2)fHistoManager->FillHisto(13, 3*copyNum+2, edep);    
	if(stop == 3 && start == 3)fHistoManager->FillHisto(13, 3*copyNum+3, edep);    
	G4double edep1,edep2,edep3;
	if(stop == 3 && start == 1)
	  {
	    edep1=(10/3.-preGasZ)/stepl*edep;
	    edep2=10/3./stepl*edep;
	    edep3=(abs(preGasZ)+dz-20/3.)/stepl*edep;
	    //	    G4cout <<edep1<< "   "<<edep2<< "   "<<edep3<< "   "<<edep<< G4endl;
	    //G4cout << edep-edep1-edep2-edep3<<G4endl;
	    fHistoManager->FillHisto(13, 3*copyNum+1, edep1);    
	    fHistoManager->FillHisto(13, 3*copyNum+2, edep2);    
	    fHistoManager->FillHisto(13, 3*copyNum+3, edep3);    
	  }
	if(stop == 2 && start == 1)
	  {
	    edep1=(10/3.-preGasZ)/stepl*edep;
	    edep2=(abs(preGasZ)+dz-10./3.)/stepl*edep;
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
	    edep3=(abs(preGasZ)+dz-20./3)/stepl*edep;
	    //	    G4cout <<edep1<< "   "<<edep2<< "   "<<edep3<< "   "<<edep<< "   "<<preGasZ<<G4endl;
	    //	    G4cout << edep-edep1-edep2-edep3<<G4endl;
	    fHistoManager->FillHisto(13, 3*copyNum+1, edep1);    
	    fHistoManager->FillHisto(13, 3*copyNum+2, edep2);    
	    fHistoManager->FillHisto(13, 3*copyNum+3, edep3);    
	  }

		//ePerStrip(postz,edep);
	//fEventAction->lastPos(stripNumber,inGap);
	/*
	  G4cout <<  material <<G4endl;
	  G4cout<<std::setw(8)<<std::setprecision(5) <<"  layer: "<<   copyNum;
	  G4cout<<std::setw(10)<<std::setprecision(5) <<"preGasZ:"<<preGasZ;
	  G4cout<<std::setw(8)<<std::setprecision(5) <<"  edep: "<<   edep;
	  G4cout<<std::setw(8)<<std::setprecision(5) <<"  preZ: "<<   preZ;
	  G4cout<<std::setw(8)<<std::setprecision(5) <<"  postZ: "<<   postZ;
	  G4cout<<std::setw(8)<<std::setprecision(5) <<"  stepL: "<<   stepl;
	  G4cout<<std::setw(8)<<std::setprecision(5) <<"   stripOff: "<<   stripOff  <<G4endl;
	  G4cout << inGap1 <<"  "<<inGap2 <<"  "<<inGap3 <<G4endl;
	*/

      }
      
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
