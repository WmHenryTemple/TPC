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
/// \file EventAction.hh
/// \brief Definition of the EventAction class
//
//
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, HistoManager*);
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event*);
  virtual void    EndOfEventAction(const G4Event*);
    
  void AddAbs(G4double de, G4double dl) {fEnergyAbs += de; fTrackLAbs += dl;};
  void AddGap(G4double de, G4double dl) {fEnergyGap += de; fTrackLGap += dl;};
  void gapEnergy(G4double de, G4int chamber) {fGapE[chamber]+=de;};
  void gapEnergy_hIon(G4double de, G4int chamber) {fGapE_hIon[chamber]+=de;};
  void gapEnergy_sec(G4double de, G4int chamber) {fGapE_sec[chamber]+=de;};
  void gapEnergy_prim_other(G4double de, G4int chamber) {fGapE_prim_other[chamber]+=de;};  
  void stripEnergy(G4double de, G4int strip) {fStripE[strip]+=de;};

  void absEnergy(G4double de, G4int abs) {
    //    G4cout << "In absEnergy, fAbsE["<<abs<<"]: "<<fAbsE[abs];
    fAbsE[abs]+=de;
    //    G4cout << "    after  absEnergy, fAbsE["<<abs<<"]: "<<fAbsE[abs]<<G4endl;    
  };  

  void eLastAbs(G4double de){fLastAbsEnergy+=de;};
  //void lastPos(G4double pos, G4bool inGap){if(inGap)fLastPos=pos;else fLastPos=-1;}
private:
   RunAction*    fRunAct;
   HistoManager* fHistoManager;
   G4double fGapE[60];
   G4double fGapE_sec[60];
   G4double fGapE_hIon[60];
   G4double fGapE_prim_other[60];     
   G4double fStripE[200];
   G4double fAbsE[60];     
   G4double  fEnergyAbs, fEnergyGap;
   G4double  fTrackLAbs, fTrackLGap;
   G4double  fLastPos;
  G4double  fLastAbsEnergy;
                     
   G4int     fPrintModulo;                             
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
