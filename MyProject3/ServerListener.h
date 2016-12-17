#pragma once

#include "GameFramework/Actor.h"
#include "Networking.h"
#include "ServerListener.generated.h"

UCLASS()
class MYPROJECT3_API AServerListener : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	//====================================================

public:
	FSocket* ListenSocket;

	FUdpSocketReceiver* mySocketReceiver;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	//ScreenMsg
	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const float Value)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %f"), *Msg, Value));
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
	}


public:

	void Callback(const FArrayReaderPtr& data, const FIPv4Endpoint&);

	/** Called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};