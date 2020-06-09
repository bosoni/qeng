/*

* StateManager test, press SPACE to change scene

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

StateManager stateManager;

class TestState : public BaseGameState
{
	Font font;
	QScene scene;
	QModel3D* mdl;

public:
	void Init()
	{
		font.LoadTtf("fonts/visitor1.ttf", 20);

		mdl = new QModel3D();
		mdl->Load("models/floor1.obj.mesh", "models/floor1.mtl");
		scene.Add(mdl);

		scene.camera.SetPosition(0, 1, -4);
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

		if (inputGetKeyState(entry::Key::Space))
		{
			stateManager.Remove(this);
			stateManager.Add(new Test1(), true);
			return;
		}

		mdl->RotateY(deltaTime*20);

		cameraUpdate(deltaTime * 0.6f, mouseState);
		scene.Render();

		SetOrthoMatrix(screenWidth, screenHeight);
		PrintText();
	}

	void PrintText()
	{
		char txt[512];
		sprintf(txt, "Visible meshes: %d\nPress F1 to see stats.\nPress SPACE to change scene.", visibleMeshes);
		font.CreateText(5, 40, WChar(txt));
		font.Submit();
	}

};

class Test8 : public BaseGameState
{
public:
	void Init()
	{
		stateManager.Add(new TestState(), true);
	}

	void Update(float deltaTime)
	{
		stateManager.Update(deltaTime);
	}
};
