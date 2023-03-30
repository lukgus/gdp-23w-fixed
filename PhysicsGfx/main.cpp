#include "Engine.h"
#include "World.h"
#include "PhysicsDemoWorld.h"
#include "AIDemoWorld.h"
#include "AIPathFindingDemoWorld.h"
#include "AnimationDemoWorld.h"
#include "PhysicsDemoWorld.h"
#include "BoneDemoWorld.h"
#include "CharacterAnimationDemoWorld.h"
#include "NavMeshDemoWorld.h"
#include "VehicleDemoWorld.h"
#include "RagdollDemoWorld.h"

#include <string>
#include <fstream>

using namespace gdp;

World* g_World;
void Update(float dt)
{
	g_World->Update(dt);
}

void Render()
{
	g_World->Render();
}

void LoadWorld(const std::string& worldName)
{
	if (strcmp(worldName.c_str(), "PhysicsDemoWorld") == 0)
	{
		g_World = new PhysicsDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "AIDemoWorld") == 0)
	{
		g_World = new AIDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "AIPathFindingDemoWorld") == 0)
	{
		g_World = new AIPathFindingDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "AnimationDemoWorld") == 0)
	{
		g_World = new AnimationDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "BoneDemoWorld") == 0)
	{
		g_World = new BoneDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "CharacterAnimationDemoWorld") == 0)
	{
		g_World = new CharacterAnimationDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "NavMeshDemoWorld") == 0)
	{
		g_World = new NavMeshDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "VehicleDemoWorld") == 0)
	{
		g_World = new VehicleDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "RagdollDemoWorld") == 0)
	{
		g_World = new RagdollDemoWorld();
	}
}

void InitializeWorld()
{
	g_World->Initialize();
}
//
//struct BitmapFileHeader
//{
//	unsigned char headerField[2];
//	unsigned char sizeOfBmp[4];
//	unsigned char reserved1[2];
//	unsigned char reserved2[2];
//	unsigned char dataOffset[4];
//};
//
//struct BITMAPINFOHEADER
//{
//
//}
//
//struct Color
//{
//	unsigned char r;
//	unsigned char g;
//	unsigned char b;
//};
//
//void BitmapReadingSample()
//{
//	std::ifstream bmpStream;
//	bmpStream.open("samplemap.bmp");
//	if (!bmpStream.is_open())
//	{
//		bmpStream.close();
//		return;
//	}
//
//	bmpStream.seekg(0);
//
//	BitmapFileHeader fileHeader;
//
//
//
//	int size = sizeof(fileHeader);
//	bmpStream.read((char*)&fileHeader.headerField, size);
//
//	int breakpoint = 0;
//
//	bmpStream.seekg(fileHeader.dataOffset[0]);
//
//
//
//	Color color;
//	for (int i = 0; i < 300; i++)
//	{
//		bmpStream.read((char*)&color, 3);
//
//		printf("%d %d %d\n", color.r, color.g, color.b);
//	}
//
//	bmpStream.close();
//}


int main(int argc, char** argv)
{
	//BitmapReadingSample();


	if (argc != 3)
		LoadWorld("PhysicsDemoWorld");
	else
		LoadWorld(argv[2]);

	GDP_Initialize();
	GDP_CreateWindow("Test", 1200, 800);
	GDP_UpdateCallback(&Update);
	GDP_RenderCallback(&Render);

	InitializeWorld();
	GDP_Run();

	g_World->Destroy();
	GDP_Destroy();
	return 0;
}