// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Http.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMyArrayStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Id;

	UPROPERTY(EditAnywhere)
	float Seconds;

	UPROPERTY(EditAnywhere)
	FString Name;

	FMyArrayStruct()
	{
	}
};

UCLASS()
class HEAVYARTILLERY_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LeaderboardScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoadingScreenClass;

	UPROPERTY()
	UUserWidget* HUD;

	UPROPERTY()
	UUserWidget* LeaderboardScreen;

	UPROPERTY()
	UUserWidget* LoadingScreen;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5;

	FTimerHandle RestartTimer;

	UFUNCTION(BlueprintCallable)
	void UnPause();

	UPROPERTY(EditAnywhere)
	bool bGameStarted = false;

	UPROPERTY(EditAnywhere)
	bool bSent = false;

	UPROPERTY(VisibleAnywhere)
	float Seconds = 0;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY()
	TArray<FMyArrayStruct> MyArray;

	UFUNCTION(BlueprintCallable)
	void ChangeName(FString NewName);

	void ExecutePostRequest();

	void ExecuteGetRequest();

	void OnGetResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ExecutePatchRequest(FString Url);

	void OnResponseReceivedLog(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UFUNCTION(BlueprintCallable)
	void ExecuteDeleteRequest();
};
