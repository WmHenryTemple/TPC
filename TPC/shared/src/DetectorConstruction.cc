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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "F02ElectricFieldSetup.hh"
#include "G4FieldManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"

#include "G4StateManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fAbsorberMaterial(0),fGapMaterial(0),fDefaultMaterial(0),
 fSolidWorld(0),fLogicWorld(0),fPhysiWorld(0),
 fSolidCalor(0),fLogicCalor(0),fPhysiCalor(0),
  fSolidPatient(0),fLogicPatient(0),fPhysiPatient(0),
 fSolidLayer(0),fLogicLayer(0),fPhysiLayer(0),
 fSolidAbsorber(0),fLogicAbsorber(0),fPhysiAbsorber(0),
 fSolidGap (0),fLogicGap (0),fPhysiGap1 (0),fPhysiGap2 (0),fPhysiGap3 (0),
 fAnnulusMother(0), fAnnulusInner(0), fAnnulusOuter(0),
 fLogicAnnulusMother(0), fLogicAnnulusInner(0), fLogicAnnulusOuter(0),
 fPhysiAnnulusMother(0), fPhysiAnnulusInner(0), fPhysiAnnulusOuter(0),
 fDetectorMessenger(0), fEmFieldSetup (0)
{
  // default parameter values of the calorimeter

  // Calculation in DetectorConstruction.hh
  //  fLayerThickness = fAbsorberThickness + fGapThickness;
  //  fCalorThickness = fNbOfLayers*fLayerThickness;

  fNbOfLayers        = 60;
  // July 27th Geo
  //  fAbsorberThickness =  0.254*mm;
  //  fGapThickness      =  7.246*mm;

  // July 28th Test
  fAbsorberThickness =  0.2286*mm;
  fGapThickness      =  8.0214*mm;



  fShutterThickness = 5.*mm;
  fAnnulusPositionX=40.*cm;
  //  fAnnulusPositionY=16.*cm;  
  fAnnulusPositionY=0.*cm;  
  //  fPatientThickness = 120.*mm;
  fPatientThickness = 0.;  

  //fShutterY=0*cm;
  fShutterY=16*cm;  
  fPatientY=0*cm;

  
  fPatientShutterGap = 300.*mm;  
  fPatientSizeY = 120.*mm;
  fPatientSizeZ = 120.*mm;
  fPatientPositionX = 120.*mm;    
  //  fLayerThickness = fAbsorberThickness + fGapThickness;
  //  fCalorThickness = fNbOfLayers*fLayerThickness;
  // July 7th GEM foil is 45 x 25 cm
  // (0.254 + GapThickness)*Nblayers=45

  fCalorSizeY       = 8.*cm;
  fCalorSizeZ       = 25.*cm;

  fNbOfStrips        = 3;//166/3
  //      fCopperSizeX        = (fLayerThickness-(fNbOfStrips+1)*fCuGapSizeX-fAbsorberThickness)/fNbOfStrips;
  fCopperSizeY       = 0.25*mm;
  fCuGapSizeX          = 0.1*mm;

  ComputeCalorParameters();

  DefineMaterials();
  SetAbsorberMaterial("G4_MYLAR");
  SetGapMaterial("ArCO2");
  


  // create commands for interactive definition of the calorimeter
  fDetectorMessenger = new DetectorMessenger(this);
  fEmFieldSetup = new F02ElectricFieldSetup() ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;  delete fEmFieldSetup;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OO0oo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
// use G4-NIST materials data base
//
G4NistManager* man = G4NistManager::Instance();
fDefaultMaterial = man->FindOrBuildMaterial("G4_Galactic");
man->FindOrBuildMaterial("G4_Pb");
fWater= man->FindOrBuildMaterial("G4_WATER");
man->FindOrBuildMaterial("G4_lAr");
man->FindOrBuildMaterial("G4_MYLAR");
 fCopperMaterial=man->FindOrBuildMaterial("G4_Cu");
 fBone=man->FindOrBuildMaterial("G4_B-100_BONE");
 // fTissue=man->FindOrBuildMaterial("G4_B-100_BONE"); 
 //  fBone=man->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");  
 fTissue=man->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP"); 
 G4double density, fractionmass; 
 G4String name;
 G4int ncomponents;
 //80% Ar + 20% CO2, STP
 G4Material* Argon = man->FindOrBuildMaterial("G4_Ar");
 G4Material* CarbonDioxide = man->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
 density = 1.8223*mg/cm3 ;      
  G4Material* Ar_80CO2_20 = new G4Material(name="ArCO2",density,ncomponents=2); 
  Ar_80CO2_20->AddMaterial( Argon,           fractionmass = 0.783 ) ;
  Ar_80CO2_20->AddMaterial( CarbonDioxide,   fractionmass = 0.217 ) ;  


// print table
//
G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter()
{




  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the Calor parameters definition
  ComputeCalorParameters();
   
  //     
  // World
  //
  fSolidWorld = new G4Box("World",                                //its name
                   fWorldSizeX/2,fWorldSizeY/2,fWorldSizeZ/2);  //its size
                         
  fLogicWorld = new G4LogicalVolume(fSolidWorld,            //its solid
                                   fDefaultMaterial,        //its material
                                   "World");                //its name
                                   
  fPhysiWorld = new G4PVPlacement(0,                        //no rotation
                                 G4ThreeVector(),           //at (0,0,0)
                                 fLogicWorld,             //its logical volume
                                 "World",                   //its name
                                 0,                         //its mother  volume
                                 false,                  //no boolean operation
                                 0);                        //copy number
  ///////  Annulus
  G4Tubs*
  fAnnulusMother = new G4Tubs("annulusMother",                        //name
                         0*cm, 3*cm,             //r1, r2
                         3*cm,            //half-length 
                         0., twopi);                      //theta1, theta2

  G4Tubs*
  fAnnulusInner = new G4Tubs("annulusInner",                        //name
                         .25*cm, .5*cm,             //r1, r2
                         2*cm,            //half-length 
                         0., twopi);                      //theta1, theta2

  G4Tubs*
  fAnnulusOuter = new G4Tubs("annulusOuter",                        //name
                         .75*cm, 1*cm,             //r1, r2
                         2*cm,            //half-length 
                         0., twopi);                      //theta1, theta2  

  fLogicAnnulusMother = new G4LogicalVolume(fAnnulusMother,        //its solid
                                        fTissue,   //its material
                                        "AnnulusMother");     //its name

  fLogicAnnulusOuter = new G4LogicalVolume(fAnnulusOuter,        //its solid
                                        fBone,   //its material
                                        "AnnulusOuter");     //its name

  fLogicAnnulusInner = new G4LogicalVolume(fAnnulusInner,        //its solid
					   fBone,   //its material
					   "AnnulusInner");     //its name

  G4RotationMatrix *rot=new G4RotationMatrix();
  rot->rotateY(90.*deg);
  
  fPhysiAnnulusMother = new G4PVPlacement(rot, G4ThreeVector(-fAnnulusPositionX,-fAnnulusPositionY,0.*cm),fLogicAnnulusMother,"Annulus",fLogicWorld, false,0);                         
  //  fPhysiAnnulusMother = new G4PVPlacement(0, G4ThreeVector(-fAnnulusPositionX,0.*cm,0.*cm),fLogicAnnulusMother,"Annulus",fLogicWorld, false,0);                         
  //  fPhysiAnnulusMother = new G4PVPlacement(rot, G4ThreeVector(-40.*cm,30.*cm,0.),fLogicAnnulusMother,"Annulus",fLogicWorld, false,0);                         

    fPhysiAnnulusInner = new G4PVPlacement(0,                    //no rotation
                          G4ThreeVector(0.,0.,0.),  //its position					
                                     fLogicAnnulusInner,           //its logical volume
                                     "AnnulusInner",         //its name
                                     fLogicAnnulusMother,           //its mother  volume
                                     false,              //no boolean operation
                                     0);                    //copy number
	
    fPhysiAnnulusOuter = new G4PVPlacement(0,                    //no rotation
                          G4ThreeVector(0.,0.,0.),  //its position					
                                     fLogicAnnulusOuter,           //its logical volume
                                     "AnnulusOuter",         //its name
                                     fLogicAnnulusMother,           //its mother  volume
                                     false,              //no boolean operation
                                     0);                    //copy number	


//                               
  // Water
  //  
  fSolidPatient=0; fLogicPatient=0; fPhysiPatient=0;

  if (fPatientThickness > 0.)  {                                             
	G4cout<<"fPatientThickness="<<fPatientThickness<<G4endl;
	
     fSolidPatient = new G4Box("Patient",                //its name
                    fPatientThickness/2,fPatientSizeY/2,fPatientSizeZ/2);//size
                                 
      fLogicPatient = new G4LogicalVolume(fSolidPatient,        //its solid
                                        fWater,   //its material
                                        "Patient");     //its name
      /*
      fPhysiPatient = new G4PVPlacement(0,                    //no rotation
                          G4ThreeVector(-fPatientPositionX,fPatientY,0.),  //its position					
                                     fLogicPatient,           //its logical volume
                                     "Patient",         //its name
                                     fLogicWorld,           //its mother  volume
                                     false,              //no boolean operation
                                     0);                    //copy number
      */
    }
      //                               
  // Calorimeter
  //  
  fSolidCalor=0; fLogicCalor=0; fPhysiCalor=0;
  fSolidLayer=0; fLogicLayer=0; fPhysiLayer=0;
  
  if (fCalorThickness > 0.)  
    { fSolidCalor = new G4Box("Calorimeter",                //its name
                    fCalorThickness/2,fCalorSizeY/2,fCalorSizeZ/2);//size
                                 
      fLogicCalor = new G4LogicalVolume(fSolidCalor,        //its solid
                                        fDefaultMaterial,   //its material
                                        "Calorimeter");     //its name
                                           
      fPhysiCalor = new G4PVPlacement(0,                    //no rotation
                                     G4ThreeVector(),       //at (0,0,0)
                                     fLogicCalor,           //its logical volume
                                     "Calorimeter",         //its name
                                     fLogicWorld,           //its mother  volume
                                     false,              //no boolean operation
                                     0);                    //copy number

  //                                 
  // Layer
  //
      fSolidLayer = new G4Box("Layer",                        //its name
                       fLayerThickness/2,fCalorSizeY/2,fCalorSizeZ/2); //size
                       
      fLogicLayer = new G4LogicalVolume(fSolidLayer,        //its solid
                                       fDefaultMaterial,    //its material
                                       "Layer");            //its name
      //      fNbOfLayers=1;
      if (fNbOfLayers > 1){                                      
        fPhysiLayer = new G4PVReplica("Layer",              //its name
                                     fLogicLayer,           //its logical volume
                                     fLogicCalor,           //its mother
                                     kXAxis,              //axis of replication
                                     fNbOfLayers,           //number of replica
                                     fLayerThickness);      //witdth of replica
      }
	/*
	  for(int i=0;i<fNbOfLayers;i++){     
	  G4double pos_x= (i+0.5-fNbOfLayers/2.)*fLayerThickness;
	  fPhysiLayer = new G4PVPlacement(0,                  //no rotation
	  G4ThreeVector(pos_x, 0, 0) ,       //at (0,0,0)
	  fLogicLayer,           //its logical volume
	  "Layer",               //its name
	  fLogicCalor,           //its mother  volume
	  false,               //no boolean operation
	  i);                    //copy number     
	  
	  }
	  }
	*/

      else
        fPhysiLayer = new G4PVPlacement(0,                  //no rotation
                                     G4ThreeVector(),       //at (0,0,0)
                                     fLogicLayer,           //its logical volume
                                     "Layer",               //its name
                                     fLogicCalor,           //its mother  volume
                                     false,               //no boolean operation
                                     0);                    //copy number     
    }                                   
  
  //                               
  // Absorber
  //
  fSolidAbsorber=0; fLogicAbsorber=0; fPhysiAbsorber=0;  
  
  if (fAbsorberThickness > 0.) 
    { fSolidAbsorber = new G4Box("Absorber",                //its name
                          fAbsorberThickness/2,fCalorSizeY/2,fCalorSizeZ/2); 
                          
      fLogicAbsorber = new G4LogicalVolume(fSolidAbsorber,    //its solid
                                            fAbsorberMaterial, //its material
                                            fAbsorberMaterial->GetName());//name
                                                
      fPhysiAbsorber = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(-fGapThickness/2,0.,0.),  //its position
                                        fLogicAbsorber,     //its logical volume
                                        fAbsorberMaterial->GetName(), //its name
                                        fLogicLayer,          //its mother
                                        false,               //no boulean operat
                                        0);                   //copy number
                                        
    }

  fSolidShutter = new G4Box("Shutter",                //its name
                          fShutterThickness/2,fCalorSizeY/2,fCalorSizeZ/2); 
                          
  fLogicShutter = new G4LogicalVolume(fSolidShutter,    //its solid
                                            fAbsorberMaterial, //its material
                                            fAbsorberMaterial->GetName());//name
                                                

    fPhysiShutter = new G4PVPlacement(0,                   //no rotation
                          G4ThreeVector(-fCalorThickness/2-fShutterThickness-10.0,fShutterY,0.),  //its position
                                        fLogicShutter,     //its logical volume
                                        fAbsorberMaterial->GetName(), //its name
                                        fLogicWorld,          //its mother
                                        false,               //no boulean operat
                                        0);                   //copy number
                                        


  //                                 
  // Gap
  //
  fSolidGap=0; fLogicGap=0; fPhysiGap1=0; fPhysiGap2=0; fPhysiGap3=0; 

  
  if (fGapThickness > 0.)
    { fSolidGap = new G4Box("Gap",
                               fGapThickness/6.,fCalorSizeY/2,fCalorSizeZ/2);
                               
      fLogicGap = new G4LogicalVolume(fSolidGap,
                                           fGapMaterial,
                                           fGapMaterial->GetName());
                                           
      fPhysiGap1 = new G4PVPlacement(0,                      //no rotation
               G4ThreeVector(fAbsorberThickness/2-fGapThickness/3.,0.,0.),   //its position
                                   fLogicGap,               //its logical volume
                                   fGapMaterial->GetName(), //its name
                                   fLogicLayer,             //its mother
                                   false,                   //no boulean operat
                                   0);                      //copy number

      fPhysiGap2 = new G4PVPlacement(0,                      //no rotation
               G4ThreeVector(fAbsorberThickness/2,0.,0.),   //its position
                                   fLogicGap,               //its logical volume
                                   fGapMaterial->GetName(), //its name
                                   fLogicLayer,             //its mother
                                   false,                   //no boulean operat
                                   0);                      //copy number

      fPhysiGap3 = new G4PVPlacement(0,                      //no rotation
               G4ThreeVector(fAbsorberThickness/2+fGapThickness/3.,0.,0.),   //its position
                                   fLogicGap,               //its logical volume
                                   fGapMaterial->GetName(), //its name
                                   fLogicLayer,             //its mother
                                   false,                   //no boulean operat
                                   0);                      //copy number
    }

  //     G4double spacing=fCopperSizeX + fCuGapSizeX;
     G4double spacing=fLayerThickness;
     G4double value = -(fCalorSizeY/2+fPcbSizeY/2);

     fSolidPcb = new G4Box("Pcb",fPcbSizeX/2,fPcbSizeY/2,fPcbSizeZ/2);
     fLogicPcb = new G4LogicalVolume(fSolidPcb,fDefaultMaterial,"Pcb");
     fPhysiPcb = new G4PVPlacement(0,G4ThreeVector(-fLayerThickness/2,value,0.),fLogicPcb,"Pcb",fLogicWorld,false,0);

     
     fSolidStrip = new G4Box("Strip",fStripSizeX/2,fStripSizeY/2,fStripSizeZ/2);
     fLogicStrip = new G4LogicalVolume(fSolidStrip,fDefaultMaterial,"Strip");
     fPhysiStrip = new G4PVReplica("Strip",fLogicStrip,fLogicPcb,kXAxis,fNbOfLayers-1,spacing);       

     fSolidCopper = new G4Box("Copper",fCopperSizeX/2,fCopperSizeY/2,fCopperSizeZ/2);
     fLogicCopper = new G4LogicalVolume(fSolidCopper,fDefaultMaterial,"Copper");
     // three strips per absorber
     fPhysiCopper1 = new G4PVPlacement(0, G4ThreeVector( (fAbsorberThickness/2+fCopperSizeX+fCuGapSizeX) , 0. , 0.),fLogicCopper,"Copper1",fLogicStrip,false,0);
     fPhysiCopper2 = new G4PVPlacement(0, G4ThreeVector( fAbsorberThickness/2+0. , 0. , 0.                        ),fLogicCopper,"Copper2",fLogicStrip,false,0);
     fPhysiCopper3 = new G4PVPlacement(0, G4ThreeVector(fAbsorberThickness/2-(fCopperSizeX+fCuGapSizeX) , 0. , 0.),fLogicCopper,"Copper3",fLogicStrip,false,0);




     //     fSolidCuGap = new G4Box("CuGap",fCuGapSizeX/2,fCuGapSizeY/2,fCuGapSizeZ/2);
     //     fLogicCuGap = new G4LogicalVolume(fSolidCuGap,fDefaultMaterial,"CuGap");
     //     fPhysiCuGap = new G4PVPlacement(0, G4ThreeVector( -(fCuGapSizeX-fStripSizeX)/2 , 0. , 0.),fLogicCuGap,"CuGap",fLogicStrip,false,0);


  PrintCalorParameters();     
  //                                        
  // Visualization attributes
  //
     G4VisAttributes* yellowThings= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
     yellowThings->SetVisibility(true);
     G4VisAttributes* greenThings= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
     greenThings->SetVisibility(true);
     
     fLogicWorld->SetVisAttributes (G4VisAttributes::GetInvisible());
     fLogicCalor->SetVisAttributes (G4VisAttributes::GetInvisible());
     fLogicLayer->SetVisAttributes (G4VisAttributes::GetInvisible());
     //     fLogicCuGap->SetVisAttributes (G4VisAttributes::GetInvisible());
     //     fLogicStrip->SetVisAttributes (G4VisAttributes::GetInvisible());
     fLogicGap->SetVisAttributes (G4VisAttributes::GetInvisible());
     fLogicPcb->SetVisAttributes (G4VisAttributes::GetInvisible());

     //     fLogicAbsorber->SetVisAttributes(yellowThings);
     //     fLogicPatient->SetVisAttributes(yellowThings);     
     fLogicCopper->SetVisAttributes(greenThings);
     fLogicShutter->SetVisAttributes(greenThings);
     fLogicAnnulusMother->SetVisAttributes(greenThings);
     fLogicAnnulusInner->SetVisAttributes(yellowThings);
     fLogicAnnulusOuter->SetVisAttributes(yellowThings);
     //green 0 1 0
     //yellow 1 1 0
     //     G4VisAttributes* simpleGapVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
     //     simpleGapVisAtt->SetVisibility(true);
     //     fLogicGap->SetVisAttributes(simpleGapVisAtt);     

  //
  //always return the physical World
  //
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintCalorParameters()
{
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The calorimeter is " << fNbOfLayers << " layers of: [ "
         << fAbsorberThickness/mm << "mm of " << fAbsorberMaterial->GetName() 
         << " + "
         << fGapThickness/mm << "mm of " << fGapMaterial->GetName() << " ]\n " 
	 << " The Calorimeter in cm size is (x,y,z)  ("<<fCalorThickness/cm<<", "
	 <<fCalorSizeY/cm <<", "<<fCalorSizeZ/cm << " ) \n" 
	 << " The World size in cm is (x,y,z)  ("<<fWorldSizeX/cm<<", "
	 <<fWorldSizeY/cm <<", "<<fWorldSizeZ/cm << " ) \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial =
  G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);      
  if (pttoMaterial)
  {
      fAbsorberMaterial = pttoMaterial;
      if ( fLogicAbsorber )
      {
          fLogicAbsorber->SetMaterial(fAbsorberMaterial);
          G4RunManager::GetRunManager()->PhysicsHasBeenModified();
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetGapMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =  
  G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  if (pttoMaterial)
  {
      fGapMaterial = pttoMaterial;
      if ( fLogicGap )
      {
          fLogicGap->SetMaterial(fGapMaterial);
          G4RunManager::GetRunManager()->PhysicsHasBeenModified();
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorberThickness(G4double val)
{
  // change Absorber thickness and recompute the calorimeter parameters
  fAbsorberThickness = val;
  if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}

void DetectorConstruction::SetShutterThickness(G4double val)
{
  // change Absorber thickness and recompute the calorimeter parameters
  fShutterThickness = val;
  if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetGapThickness(G4double val)
{
  // change Gap thickness and recompute the calorimeter parameters
  fGapThickness = val;
  if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCalorSizeZ(G4double val)
{
  // change the transverse size and recompute the calorimeter parameters
  fCalorSizeZ = val;
  if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCalorSizeY(G4double val)
{
  // change the transverse size and recompute the calorimeter parameters
  fCalorSizeY = val;
  if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNbOfLayers(G4int val)
{
  fNbOfLayers = val;
  if ( G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit ) {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
