#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4Tubs.hh"

#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // materials
    auto nist = G4NistManager::Instance();
    auto matAir = nist->FindOrBuildMaterial("G4_AIR");
    auto matWater = nist->FindOrBuildMaterial("G4_WATER");

    // World
    auto worldSize = 3. * m;
    auto worldSol = new G4Box("World", .5 * worldSize, .5 * worldSize, .5 * worldSize);
    auto worldLog = new G4LogicalVolume(worldSol, matAir, "World");
    auto worldPhy = new G4PVPlacement(nullptr, G4ThreeVector(), worldLog, "World", nullptr, false, 0);

    // Waterphantom1
    auto phantom1Pos = G4ThreeVector(0 *cm, 0.*cm, 100.0*cm);

    auto phantom1Diameter = 5.0 * cm;
    auto phantom1Height = 5.0 * cm;
    auto phantom1Sol = new G4Tubs("phantom1", 0., 0.5 * phantom1Diameter, .5 * phantom1Height, 0., 360. * deg);
    auto phantom1Log = new G4LogicalVolume(phantom1Sol, matWater, "phantom1");
    new G4PVPlacement(nullptr, phantom1Pos, phantom1Log, "phantom1", worldLog, false, 0);
    
    return worldPhy;
    
}

void DetectorConstruction::ConstructSDandField()
{
    // MFD 등록
    auto mfd = new G4MultiFunctionalDetector("Detector");
    G4SDManager::GetSDMpointer()->AddNewDetector(mfd);

    // PS 1 등록 -> 여러개의 PS를 등록할 때도 마찬가지로 사용하면 된다.
    auto psEDep = new G4PSEnergyDeposit("EDep"); 
    mfd->RegisterPrimitive(psEDep);
    
    // Logical Volume에 SD setting
    SetSensitiveDetector("phantom1", mfd);
   
   }
