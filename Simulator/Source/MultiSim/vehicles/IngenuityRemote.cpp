/*
* Class implementation for Ingenuity pawn in MultiSim
*
* Copyright (C) 2018 Simon D. Levy
*
* MIT License
*/

#include "IngenuityRemote.h"
#include "../threads/RemoteThread.hpp"

AIngenuityRemote::AIngenuityRemote()
{
    // Build the frame
    vehicle.buildFull(this, BodyStatics.mesh.Get()); // Restore for cameras, audio

    // Add rotors
    addRotor(RotorTopStatics.mesh.Get(), .250);
    addRotor(RotorBottomStatics.mesh.Get(), .170);

    // Add mast, solar panel, antenna
    vehicle.addComponent(MastStatics.mesh.Get(), makeName("Mast", 1, "Mesh"));
    vehicle.addComponent(SolarPanelStatics.mesh.Get(), makeName("SolarPanel", 1, "Mesh"),
            0, 0, 0.34);
    vehicle.addComponent(AntennaStatics.mesh.Get(), makeName("Antenna", 1, "Mesh"));

    // Add legs
    addLeg(1, Leg1BracketStatics.mesh.Get(), Leg1TopStatics.mesh.Get(),
            Leg1BottomStatics.mesh.Get());
    addLeg(2, Leg2BracketStatics.mesh.Get(), Leg2TopStatics.mesh.Get(),
            Leg2BottomStatics.mesh.Get());
    addLeg(3, Leg3BracketStatics.mesh.Get(), Leg3TopStatics.mesh.Get(),
            Leg3BottomStatics.mesh.Get());
    addLeg(4, Leg4BracketStatics.mesh.Get(), Leg4TopStatics.mesh.Get(),
            Leg4BottomStatics.mesh.Get());
}

// Called when the game starts or when spawned
void AIngenuityRemote::BeginPlay()
{
    vehicle.beginPlay(new FRemoteThread(&dynamics));

    Super::BeginPlay();
}

void AIngenuityRemote::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    vehicle.endPlay();

    Super::EndPlay(EndPlayReason);
}

void AIngenuityRemote::PostInitializeComponents()
{
    vehicle.postInitializeComponents();

    Super::PostInitializeComponents();
}

// Called automatically on main thread
void AIngenuityRemote::Tick(float DeltaSeconds)
{
    vehicle.tick(DeltaSeconds);

    Super::Tick(DeltaSeconds);
}

void AIngenuityRemote::addLeg(
        uint8_t index,
        UStaticMesh * bracketMesh,
        UStaticMesh * topMesh,
        UStaticMesh * bottomMesh)
{
    vehicle.addComponent(bracketMesh, makeName("LegBracket", index, "Mesh"));
    vehicle.addComponent(topMesh,     makeName("LegTop", index, "Mesh"));
    vehicle.addComponent(bottomMesh,  makeName("LegBottom", index, "Mesh"));
}

void AIngenuityRemote::addRotor(UStaticMesh* propMesh, float z)
{
    vehicle.addRotor(propMesh, 0, 0, z);
}