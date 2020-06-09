/*

* render ground
* mushrooms rendered in Update() (they are not in scene)

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

class Test2 : public BaseGameState
{
	Font font;
	QScene scene;
	QModel3D* mdl;

public:
	void Init()
	{
		font.LoadTtf("fonts/visitor1.ttf", 20);

		mdl = new QModel3D();
		mdl->Load("models/ground2.obj.mesh", "models/ground2.mtl");
		scene.Add(mdl);

		mdl = new QModel3D();
		mdl->Load("models/mushroom.obj.mesh", "models/mushroom.mtl");
		mdl->SetScale(0.2f);

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

		// one way to create scene (render queue)
		for (int x = 0; x < 10; x++)
			for (int y = 0; y < 10; y++)
			{
				mdl->SetPosition(x, 1, y);
				mdl->Render(&scene.camera, &scene.environment);
			}

		scene.Render();

		SetOrthoMatrix(screenWidth, screenHeight);
		PrintText();
	}

	void PrintText()
	{
		char txt[512];
		sprintf(txt, "Visible meshes: %d\nPress F1 to see stats.", visibleMeshes);
		font.CreateText(5, 40, WChar(txt));
		font.Submit();
	}

};
