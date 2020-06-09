/*

* fonts, static and dynamic text
* models to scene
* render scene

*/
#include "Core.h"
#include "QNode.h"
#include "QModel3D.h"
#include "QScene.h"
#include "Material.h"
#include "Shader.h"
#include "Font.h"
#include "InlineFunctions.h"
#include "StateManager.h"

class Test1 : public BaseGameState
{
	Font font, font1;
	QScene scene;
	QModel3D* mdl;

public:
	void Init()
	{
		font.LoadTtf("fonts/visitor1.ttf", 20);

		font1.LoadTtf("fonts/signika-regular.ttf", 20);
		font1.CreateStaticText(5, 10, L"Test statictext toimiiks ääkköset?");

		mdl = new QModel3D();
		mdl->Load("models/skene1M_no_textured.obj.mesh", "models/skene1M_no_textured.mtl");
		mdl->SetPosition(-14, 0, 0);
		scene.Add(mdl);
		
		mdl = new QModel3D();
		mdl->Load("models/skene2M_textured.obj.mesh", "models/skene2M_textured.mtl");
		scene.Add(mdl);
		
		mdl = new QModel3D();
		mdl->Load("models/skene2M_textured.obj.mesh"); // no material
		mdl->SetPosition(14, 0, 0);
		scene.Add(mdl);
		
		// one way to create scene
		for (int q = 0; q < 20; q++)
		{
			mdl = new QModel3D();
			mdl->Load("models/rock.obj.mesh", "models/rock.mtl");
			mdl->SetPosition(rand() % 10 - 5, 4, rand() % 10 - 5);
			scene.Add(mdl);
		}
		
		scene.camera.SetPosition(0, 2, -5);
		scene.camera.Init(screenWidth, screenHeight);

		scene.PrintTree();
	}

	void Dispose()
	{
		mdl->Dispose();
	}

	void Update(float deltaTime)
	{
		visibleMeshes = 0;
		renderFirstTime = true;

		cameraUpdate(deltaTime * 0.6f, mouseState);

		scene.Render();

		SetOrthoMatrix(screenWidth, screenHeight);
		PrintText();
	}

	void PrintText()
	{
		// print statictext which was created in Init()
		font1.Submit();

		char txt[512];
		sprintf(txt, "Visible meshes: %d\nPress F1 to see stats.", visibleMeshes);
		font.CreateText(5, 40, WChar(txt));
		font.AppendToText(L"\n\nTest äääööö...");
		font.AppendToText(WChar("\nÄÄäärgh ÖÖÖRGHHH!"));
		font.Submit();
	}

};
