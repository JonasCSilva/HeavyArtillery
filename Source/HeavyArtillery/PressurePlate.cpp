// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

#include "Components/BoxComponent.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PressureVolume = CreateDefaultSubobject<UBoxComponent>(FName("PressureVolume"));

	if (!ensure(PressureVolume != nullptr)) return;

	RootComponent = PressureVolume;
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	PressureVolume->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);	
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressurePlate::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Activated"));

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pressed!"));
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &APressurePlate::OnResponseReceived);
	Request->SetURL("https://zlfpjbenpgattmgznehs.supabase.co/rest/v1/test-table?select=*");
	Request->SetVerb("GET");
	Request->SetHeader("apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM0MzgyNjMsImV4cCI6MTk2OTAxNDI2M30.0NTVL2NJeB2JZyjv_AIpo08vOghZSSnd1jciPRujkPQ");
	Request->SetHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpsZnBqYmVucGdhdHRtZ3puZWhzIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM0MzgyNjMsImV4cCI6MTk2OTAxNDI2M30.0NTVL2NJeB2JZyjv_AIpo08vOghZSSnd1jciPRujkPQ");
	Request->ProcessRequest();
}

void APressurePlate::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response->GetContentAsString());

	FString responseText = (TEXT("Response: %s"), *Response->GetContentAsString());

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, responseText);
	}
}