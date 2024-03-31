#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomTools.hh"
#include "G4Gamma.hh"
#include "G4Neutron.hh"

#include "PrimaryGeneratorAction.hh"

// 생성자
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
    fPrimary = new G4ParticleGun();
}

// 소멸자
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fPrimary;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    fPrimary->SetParticleDefinition(G4Gamma::Definition());
    fPrimary->SetParticleEnergy(0.662*MeV);
    fPrimary->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    fPrimary->SetParticleMomentumDirection(G4RandomDirection());
    fPrimary->GeneratePrimaryVertex(anEvent);
}
