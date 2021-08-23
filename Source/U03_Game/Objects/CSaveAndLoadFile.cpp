#include "CSaveAndLoadFile.h"
#include "Global.h"
#include "Serialization/BufferArchive.h"

ACSaveAndLoadFile::ACSaveAndLoadFile()
{
	CHelpers::CreateActorComponent(this, &InputComponent, "InputComponent");
}

void ACSaveAndLoadFile::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	EnableInput(controller);

	InputComponent->BindAction("Save", EInputEvent::IE_Pressed, this, &ACSaveAndLoadFile::OnSave);
	InputComponent->BindAction("Load", EInputEvent::IE_Pressed, this, &ACSaveAndLoadFile::OnLoad);
}

void ACSaveAndLoadFile::OnSave()
{
	FSaveData data;
	data.Name = "someddk";
	data.Health = 999.99f;

	FBufferArchive buffer;
	buffer << data;
	

	CheckTrue(buffer.Num() < 1);
	buffer.FlushCache();
	FString path = FPaths::ProjectDir() + FString("Test.bin");
	bool result = FFileHelper::SaveArrayToFile(buffer, *path);
	buffer.Empty();
	CLog::Print(result ? "Saved" : "Save Faild");
}

void ACSaveAndLoadFile::OnLoad()
{
	FString path = FPaths::ProjectDir() + FString("Test.bin");

	FBufferArchive buffer;
	FFileHelper::LoadFileToArray(buffer, *path);

	FMemoryReader reader = FMemoryReader(buffer, true);
	reader.Seek(0);

	FSaveData saveData;

	reader << saveData;

	CLog::Print(saveData.Name.ToString(), -1, 10.0f, FColor::Green);
	CLog::Print(saveData.Health, -1, 10.0f, FColor::Green);

	reader.FlushCache();
	reader.Close();
}