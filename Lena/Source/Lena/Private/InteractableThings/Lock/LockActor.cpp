
#include "Lena/Public/InteractableThings/Lock/LockActor.h"

#include "Kismet/GameplayStatics.h"

ALockActor::ALockActor()
{
	
}

void ALockActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ALockActor::CheckPassword(const FString& InputPassword)
{
	return InputPassword==Password;
}

void ALockActor::Unlock(AActor* ActorToUnlock)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), RightAnswerSound, GetActorLocation());
}
