// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSign.h"

// Sets default values
AMainSign::AMainSign() {
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainSign::BeginPlay() {
    Super::BeginPlay();

    Radius = 100.0f;
    AngularVelocity = 3.0f;
    RelativeHeight = 1.0f;
    StartAngle = 0.0f;
}

// Called every frame
void AMainSign::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();

    FVector CharacterLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    //Generate the new location of the pawn relative to the Character Location
    NewLocation.X = CharacterLocation.X + FMath::Cos(StartAngle + AngularVelocity * RunningTime) * Radius;
    NewLocation.Y = CharacterLocation.Y + FMath::Sin(StartAngle + AngularVelocity * RunningTime) * Radius;
    NewLocation.Z = CharacterLocation.Z + RelativeHeight;
    RunningTime += DeltaTime;

    SetActorLocation(NewLocation);
}

// Called to bind functionality to input
void AMainSign::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

