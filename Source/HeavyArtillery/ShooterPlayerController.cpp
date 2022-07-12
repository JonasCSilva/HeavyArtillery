// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Json.h"
#include "UObject/ConstructorHelpers.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetPause(true);

    HUD = CreateWidget(this, HUDClass);

    if (HUD != nullptr)
    {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::UnPause()
{
    SetPause(false);
}

void AShooterPlayerController::Tick(float DeltaSeconds)
{
    if (bGameStarted)
    {
        Seconds += DeltaSeconds;
    }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();

    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }

    bGameStarted = false;

    FString TheFloatStr = FString::SanitizeFloat(Seconds);

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow, *TheFloatStr);
    }

    ExecuteRequest();

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::ExecuteRequest()
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
    RequestObj->SetNumberField("seconds", Seconds);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestObj, Writer);

    Request->OnProcessRequestComplete().BindUObject(this, &AShooterPlayerController::OnResponseReceived);
    Request->SetURL("https://zlfpjbenpgattmgznehs.supabase.co/rest/v1/leaderboard");
    Request->SetVerb("POST");
    Request->SetHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");
    Request->SetHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Prefer", "return=representation");
    Request->SetContentAsString(RequestBody);

    Request->ProcessRequest();
}

void AShooterPlayerController::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    FString responseText = (TEXT("Response: %s"), *Response->GetContentAsString());

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, responseText);
    }
}