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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"


class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;
class F02ElectricFieldSetup;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction();
  virtual ~DetectorConstruction();
  
public:
     
  void SetAbsorberMaterial (G4String);     
  void SetAbsorberThickness(G4double);

  void SetShutterThickness(G4double);
  
  void SetGapMaterial (G4String);     
  void SetGapThickness(G4double);
  
  void SetCalorSizeY(G4double);          
  void SetCalorSizeZ(G4double);          
  void SetNbOfLayers (G4int);   
     
  virtual G4VPhysicalVolume* Construct();

public:
  
     void PrintCalorParameters(); 
                    
     G4double GetWorldSizeX()           {return fWorldSizeX;}; 
     G4double GetWorldSizeZ()          {return fWorldSizeZ;};
     G4double GetWorldSizeY()          {return fWorldSizeY;};

     G4double GetStripSizeX()           {return fStripSizeX;}; 
     G4double GetStripSizeZ()          {return fStripSizeZ;};
     G4double GetStripSizeY()          {return fStripSizeY;};
     
     G4double GetCalorThickness()       {return fCalorThickness;}; 
     G4double GetCalorSizeY()          {return fCalorSizeY;};
     G4double GetCalorSizeZ()          {return fCalorSizeZ;};
      
     G4int GetNbOfLayers()              {return fNbOfLayers;}; 
     G4int GetNbOfStrips()              {return fNbOfStrips;}; 
     
     G4Material* GetAbsorberMaterial()  {return fAbsorberMaterial;};
     G4double    GetAbsorberThickness() {return fAbsorberThickness;};      
     
     G4Material* GetGapMaterial()       {return fGapMaterial;};
     G4double    GetGapThickness()      {return fGapThickness;};

     const G4VPhysicalVolume* GetphysiWorld() {return fPhysiWorld;};           
     const G4VPhysicalVolume* GetAbsorber()   {return fPhysiAbsorber;};
     const G4VPhysicalVolume* GetGap1()        {return fPhysiGap1;};
     const G4VPhysicalVolume* GetGap2()        {return fPhysiGap2;};
     const G4VPhysicalVolume* GetGap3()        {return fPhysiGap3;};
     const G4VPhysicalVolume* GetShutter()        {return fPhysiShutter;};
     const G4VPhysicalVolume* GetAnnulusMother()        {return fPhysiAnnulusMother;};
     const G4VPhysicalVolume* GetPatient()        {return fPhysiPatient;};    
     const G4VPhysicalVolume* GetStrip()        {return fPhysiStrip;};
                 
private:
   
  G4Material*        fAbsorberMaterial;
  G4double           fAbsorberThickness;
  G4double           fShutterThickness;

  G4double           fPatientY;
  G4double           fShutterY;
  
  G4Material*        fCopperMaterial;
  G4Material*        fBone;
  G4Material*        fTissue;
     G4Material*        fGapMaterial;
     G4Material*        fWater;
  
     G4double           fGapThickness;
     G4double           fStripSpacing;
     
     G4int              fNbOfLayers;
     G4int              fNbOfStrips;
     G4double           fLayerThickness;
          
     G4double           fCalorSizeZ;
     G4double           fCalorSizeY;
     G4double           fCalorThickness;

     G4double           fPatientSizeZ;
     G4double           fPatientSizeY;
     G4double           fPatientThickness;
     G4double           fPatientPositionX;
       G4double           fAnnulusPositionX;
       G4double           fAnnulusPositionY;      
     G4double         fPatientShutterGap;       
     G4Material*        fDefaultMaterial;
     G4double           fWorldSizeZ;
     G4double           fWorldSizeY;
     G4double           fWorldSizeX;

     G4double           fStripSizeZ;
     G4double           fStripSizeY;
     G4double           fStripSizeX;

     G4double           fCopperSizeZ;
     G4double           fCopperSizeY;
     G4double           fCopperSizeX;

     G4double           fGapSizeZ;
     G4double           fGapSizeY;
     G4double           fGapSizeX;

     G4double           fCuGapSizeZ;
     G4double           fCuGapSizeY;
     G4double           fCuGapSizeX;

     G4double           fPcbSizeZ;
     G4double           fPcbSizeY;
     G4double           fPcbSizeX;

            
     G4Box*             fSolidWorld;    //pointer to the solid World 
     G4LogicalVolume*   fLogicWorld;    //pointer to the logical World
     G4VPhysicalVolume* fPhysiWorld;    //pointer to the physical World

     G4Box*             fSolidCalor;    //pointer to the solid Calor 
     G4LogicalVolume*   fLogicCalor;    //pointer to the logical Calor
     G4VPhysicalVolume* fPhysiCalor;    //pointer to the physical Calor

  G4Tubs*             fAnnulusMother;    //pointer to the solid Annulus
     G4LogicalVolume*   fLogicAnnulusMother;    //pointer to the logical Annulus
     G4VPhysicalVolume* fPhysiAnnulusMother;    //pointer to the physical Annulus

  G4Tubs*             fAnnulusOuter;    //pointer to the solid Annulus
     G4LogicalVolume*   fLogicAnnulusOuter;    //pointer to the logical Annulus
     G4VPhysicalVolume* fPhysiAnnulusOuter;    //pointer to the physical Annulus

  G4Tubs*             fAnnulusInner;    //pointer to the solid Annulus
     G4LogicalVolume*   fLogicAnnulusInner;    //pointer to the logical Annulus
     G4VPhysicalVolume* fPhysiAnnulusInner;    //pointer to the physical Annulus

     G4Box*             fSolidPatient;    //pointer to the solid Patient 
     G4LogicalVolume*   fLogicPatient;    //pointer to the logical Patient
     G4VPhysicalVolume* fPhysiPatient;    //pointer to the physical Patient  
     
     G4Box*             fSolidLayer;    //pointer to the solid Layer 
     G4LogicalVolume*   fLogicLayer;    //pointer to the logical Layer
     G4VPhysicalVolume* fPhysiLayer;    //pointer to the physical Layer
         
     G4Box*             fSolidAbsorber; //pointer to the solid Absorber
  G4LogicalVolume*   fLogicAbsorber; //pointer to the logical Absorber
  G4VPhysicalVolume* fPhysiAbsorber; //pointer to the physical Absorber

  G4Box*             fSolidShutter; //pointer to the solid Shutter
     G4LogicalVolume*   fLogicShutter; //pointer to the logical Shutter
     G4VPhysicalVolume* fPhysiShutter; //pointer to the physical Shutter
     

     G4Box*             fSolidStrip;      //pointer to the solid Strip
     G4LogicalVolume*   fLogicStrip;      //pointer to the logical Strip
     G4VPhysicalVolume* fPhysiStrip;      //pointer to the physical Strip

     G4Box*             fSolidGap;      //pointer to the solid Gap
     G4LogicalVolume*   fLogicGap;      //pointer to the logical Gap
     G4VPhysicalVolume* fPhysiGap1;      //pointer to the physical Gap
     G4VPhysicalVolume* fPhysiGap2;      //pointer to the physical Gap
     G4VPhysicalVolume* fPhysiGap3;      //pointer to the physical Gap

     G4Box*             fSolidCuGap;      //pointer to the solid CuGap
     G4LogicalVolume*   fLogicCuGap;      //pointer to the logical CuGap
     G4VPhysicalVolume* fPhysiCuGap;      //pointer to the physical CuGap

     G4Box*             fSolidCopper;      //pointer to the solid Copper
     G4LogicalVolume*   fLogicCopper;      //pointer to the logical Copper
     G4VPhysicalVolume* fPhysiCopper1;      //pointer to the physical Copper
     G4VPhysicalVolume* fPhysiCopper2;      //pointer to the physical Copper
     G4VPhysicalVolume* fPhysiCopper3;      //pointer to the physical Copper

     G4Box*             fSolidPcb;      //pointer to the solid PCB
     G4LogicalVolume*   fLogicPcb;      //pointer to the logical PCB
     G4VPhysicalVolume* fPhysiPcb;      //pointer to the physical PCB
     
     DetectorMessenger* fDetectorMessenger;  //pointer to the Messenger
     F02ElectricFieldSetup* fEmFieldSetup;
      
  private:
    
     void DefineMaterials();
     void ComputeCalorParameters();
     G4VPhysicalVolume* ConstructCalorimeter();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::ComputeCalorParameters()
{
  // Compute derived parameters of the calorimeter


  fLayerThickness = fAbsorberThickness + fGapThickness;
  fCalorThickness = fNbOfLayers*fLayerThickness;

  fPatientPositionX=fCalorThickness/2 + fShutterThickness + fPatientThickness/2 +   fPatientShutterGap;        
     fWorldSizeX = 10.2*fCalorThickness; 
     fWorldSizeY = 10.2*fCalorSizeY;
     fWorldSizeZ = 10.2*fCalorSizeZ;

     //input copper and gap width and thickness
     //  fNbOfStrips        = 3;
     fPcbSizeX=fCalorThickness-fLayerThickness;
     fPcbSizeY=fCopperSizeY;
     fPcbSizeZ=fCalorSizeZ;

     //     fStripSizeX=fNbOfStrips*(fCopperSizeX+fCuGapSizeX);
     fStripSizeX=fLayerThickness;
     fStripSizeY=fCopperSizeY;
     fStripSizeZ=fCalorSizeZ;

     //     fCuGapSizeX          = 0.1mm;
     fCuGapSizeY=fCopperSizeY;
     fCuGapSizeZ=fCalorSizeZ;

     //     fCopperSizeX        = 3.*mm;
     fCopperSizeX        = (fLayerThickness-(fNbOfStrips+1)*fCuGapSizeX-fAbsorberThickness)/fNbOfStrips;
     //  fCopperSizeY       = 0.25*mm;
     fCopperSizeZ=fCalorSizeZ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

