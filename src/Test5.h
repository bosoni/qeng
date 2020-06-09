/*

* scenegraph test

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

class Test5 : public BaseGameState
{
	Font font;
	QScene scene;
	QModel3D* mdl;

	QModel3D* body;
	QModel3D* earth;

public:
	void Init()
	{
		font.LoadTtf("fonts/visitor1.ttf", 20);

		scene.camera.SetPosition(0, 4, -25);
		scene.camera.Init(screenWidth, screenHeight);

		// "car"
		{
			// body
			body = new QModel3D();
			body->Load("models/cube.obj.mesh", "models/cube.mtl");
			body->SetPosition(0, 0.5f, 0);
			body->SetScale(1, 0.2f, 1);
			scene.Add(body);

			QModel3D* upper = new QModel3D();
			upper->Load("models/cube.obj.mesh", "models/cube.mtl");
			upper->SetPosition(0, 1, 0);
			upper->SetScale(1, 1, 0.5f);
			body->Add(upper);

			QModel3D* wheel;
			// wheels
			wheel = new QModel3D();
			wheel->Load("models/earth.obj.mesh", "models/earth.mtl");
			wheel->SetPosition(-1, 0, -1);
			wheel->SetScale(0.5f, 2, 0.5f); // y=2 because of parent's scale
			body->Add(wheel);

			wheel = new QModel3D();
			wheel->Load("models/earth.obj.mesh", "models/earth.mtl");
			wheel->SetPosition(1, 0, -1);
			wheel->SetScale(0.5f, 2, 0.5f);
			body->Add(wheel);

			wheel = new QModel3D();
			wheel->Load("models/earth.obj.mesh", "models/earth.mtl");
			wheel->SetPosition(1, 0, 1);
			wheel->SetScale(0.5f, 2, 0.5f);
			body->Add(wheel);

			wheel = new QModel3D();
			wheel->Load("models/earth.obj.mesh", "models/earth.mtl");
			wheel->SetPosition(-1, 0, 1);
			wheel->SetScale(0.5f, 2, 0.5f);
			body->Add(wheel);

			earth = new QModel3D();
			earth->Load("models/earth.obj.mesh", "models/earth.mtl");
			earth->SetPosition(0, 5, 0);
			earth->SetScale(0.5f, 2, 0.5f);
			body->Add(earth);
		}

		// create tower
		QNode* parent = new QNode();
		parent->SetPosition(10, 0, 0);
		scene.Add(parent);
		for (int q = 0; q < 10; q++)
		{
			QModel3D* child = new QModel3D();
			child->Load("models/cube.obj.mesh", "models/cube.mtl");
			child->SetPosition(0, 2, 0);
			child->RotateZ(-5);
			parent->Add(child);
			parent = child;
		}

		// create tower
		parent = new QNode();
		parent->SetPosition(-10, 0, 0);
		scene.Add(parent);
		for (int q = 0; q < 10; q++)
		{
			QModel3D* child = new QModel3D();
			child->Load("models/cube.obj.mesh", "models/cube.mtl");
			child->SetPosition(0, 2, 0);
			child->RotateZ(5);
			parent->Add(child);
			parent = child;
		}

		mdl = new QModel3D();
		mdl->Load("models/floor1.obj.mesh", "models/floor1.mtl");
		mdl->SetScale(20);
		scene.Add(mdl);

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

		body->AddToPosition(0, 0, deltaTime);
		earth->RotateY(deltaTime * 50);

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
