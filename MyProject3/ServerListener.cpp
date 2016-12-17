#include "MyProject3.h"
#include "ServerListener.h"
#include <string>
#include "Runtime/Core/Public/Misc/Base64.h"


// Sets default values
AServerListener::AServerListener(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AServerListener::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//~~~~~~~~~~~~~~~~
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void AServerListener::BeginPlay()
{
	Super::BeginPlay();

	FIPv4Endpoint Endpoint(FIPv4Address::Any, 5000);

	ListenSocket = FUdpSocketBuilder(TEXT("SomeDescription"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(2 * 1024 * 1024);

	int32 SendSize = 2 * 1024 * 1024;
	ListenSocket->SetSendBufferSize(SendSize, SendSize);
	ListenSocket->SetReceiveBufferSize(SendSize, SendSize);

}

void AServerListener::Callback(const FArrayReaderPtr& data, const FIPv4Endpoint&) {

	const auto encoded = FBase64::Encode(*data);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, encoded);
	UE_LOG(LogTemp, Verbose, TEXT("Received: %s"), *encoded);
}

void AServerListener::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Dostalem wiadomosc yaay ");
	TArray<uint8> ReceivedData;

	uint32 Size;
	while (ListenSocket->HasPendingData(Size))
	{
		uint8 *Recv = new uint8[Size];
		int32 BytesRead = 0;

		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));

		ListenSocket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);
		//Do something with the received data

		char ansiiData[1024]; //A temp buffer for the data

		memcpy(ansiiData, ReceivedData.GetData(), BytesRead); //Assumes bytesRead is always smaller than 1024 bytes

		ansiiData[BytesRead] = 0; //Add null terminator

		FString debugData = ANSI_TO_TCHAR(ansiiData);

		//ListenSocket->RecvFrom(Recv, Size, BytesRead, *targetAddr);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Message by UDP: " + debugData);
	}
}