// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Json.h"
#include "UObject/ConstructorHelpers.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    ExecuteGetRequest();

    SetInputMode(FInputModeUIOnly());

    SetPause(true);

    bSent = false;

    LoadingScreen = CreateWidget(this, LoadingScreenClass);

    if (LoadingScreen != nullptr)
    {
        LoadingScreen->AddToViewport();
    }
}

void AShooterPlayerController::UnPause()
{
    SetPause(false);

    SetInputMode(FInputModeGameOnly());

    bGameStarted = true;

    if (LeaderboardScreen != nullptr)
    {
        LeaderboardScreen->RemoveFromViewport();
    }

    HUD = CreateWidget(this, HUDClass);

    if (HUD != nullptr)
    {
        HUD->AddToViewport();
    }
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

        if (!bSent) 
        {
            bSent = true;

            bGameStarted = false;

            FString TheFloatStr = FString::SanitizeFloat(Seconds, 0);

            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow, *TheFloatStr);
            }

            auto* Egg = MyArray.FindByPredicate([this](FMyArrayStruct& Str) {
                return Name.Equals(Str.Name);
            });

            if (Egg != nullptr)
            {
                TArray<FString> arrayOfString;
                arrayOfString.Add("https://zlfpjbenpgattmgznehs.supabase.co/rest/v1/leaderboard?id=eq.");
                arrayOfString.Add(FString::SanitizeFloat(Egg->Id, 0));

                FString Url = FString::Join(arrayOfString, TEXT(""));

                ExecutePatchRequest(Url);
            }
            else
            {
                ExecutePostRequest();
            }

        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }

        GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    }
}

void AShooterPlayerController::ChangeName(FString NewName)
{
    Name = NewName;
}

void AShooterPlayerController::ExecutePostRequest()
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
    RequestObj->SetNumberField("seconds", Seconds);
    RequestObj->SetStringField("name", Name);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestObj, Writer);

    Request->OnProcessRequestComplete().BindUObject(this, &AShooterPlayerController::OnResponseReceivedLog);
    Request->SetURL("https://zlfpjbenpgattmgznehs.supabase.co/rest/v1/leaderboard");
    Request->SetVerb("POST");
    Request->SetHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");
    Request->SetHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Prefer", "return=representation");
    Request->SetContentAsString(RequestBody);

    Request->ProcessRequest();
}

void AShooterPlayerController::OnResponseReceivedLog(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    FString responseText = (TEXT("Response: %s"), *Response->GetContentAsString());

    // if (GEngine) {
    //     GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, responseText);
    // }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::ExecuteGetRequest()
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &AShooterPlayerController::OnGetResponseReceived);
    Request->SetURL("https://eoii60hzua5opus.m.pipedream.net");
    Request->SetVerb("GET");

    Request->ProcessRequest();
}

void AShooterPlayerController::OnGetResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    TSharedPtr<FJsonObject> ResponseObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    FJsonSerializer::Deserialize(Reader, ResponseObj);

    const TArray<TSharedPtr<FJsonValue>>& LEArray = ResponseObj->GetArrayField("response");

    for (TSharedPtr<FJsonValue> LEEntry: LEArray)
    {
        if (LEEntry != nullptr)
        {
            auto Test = LEEntry->AsObject();

            if (Test != nullptr)
            {
                FMyArrayStruct LocalStruct;
                LocalStruct.Seconds = Test->GetNumberField("seconds");
                LocalStruct.Id = Test->GetNumberField("id");
                LocalStruct.Name = Test->GetStringField("name");

                MyArray.Add(LocalStruct);
            }
                
        }
    }

    // FString responseText = (TEXT("Response: %s"), *Response->GetContentAsString());

    // if (GEngine) {
    //    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, responseText);
    // }

    if (LoadingScreen != nullptr)
    {
        LoadingScreen->RemoveFromViewport();
    }

    LeaderboardScreen = CreateWidget(this, LeaderboardScreenClass);

    if (LeaderboardScreen != nullptr)
    {
        LeaderboardScreen->AddToViewport();
    }
}

void AShooterPlayerController::ExecutePatchRequest(FString Url)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
    RequestObj->SetNumberField("seconds", Seconds);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestObj, Writer);

    // if (GEngine) {
    //     GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Purple, *Url);
    // }

    Request->OnProcessRequestComplete().BindUObject(this, &AShooterPlayerController::OnResponseReceivedLog);
    Request->SetURL(Url);
    Request->SetVerb("PATCH");
    Request->SetHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");
    Request->SetHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("Prefer", "return=representation");
    Request->SetContentAsString(RequestBody);

    Request->ProcessRequest();
}

void AShooterPlayerController::ExecuteDeleteRequest()
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    // Request->OnProcessRequestComplete().BindUObject(this, &AShooterPlayerController::OnDeleteResponseReceived);
    Request->SetURL("https://zlfpjbenpgattmgznehs.supabase.co/rest/v1/leaderboard?id=not.is.null");
    Request->SetVerb("DELETE");
    Request->SetHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");
    Request->SetHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY1MzQzODI2MywiZXhwIjoxOTY5MDE0MjYzfQ.88g16dG1eOSUXUy_vB0qaTS3RNKharuLPPCKsax3m_I");

    Request->ProcessRequest();
}