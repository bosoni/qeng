/*

* rotate model (arrow keys, Z,X, R,T, F,G, space)

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

class Test3 : public BaseGameState
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
		mdl->Load("models/car.obj.mesh", "models/car.mtl");
		scene.Add(mdl);

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

		if (inputGetKeyState(entry::Key::Up)) mdl->Forward(deltaTime * 5);
		if (inputGetKeyState(entry::Key::Down)) mdl->Forward(-deltaTime * 5);

		if (inputGetKeyState(entry::Key::KeyZ)) mdl->Right(-deltaTime * 5);
		if (inputGetKeyState(entry::Key::KeyX)) mdl->Right(deltaTime * 5);

		if (inputGetKeyState(entry::Key::Left)) mdl->RotateY(-deltaTime * 100);
		if (inputGetKeyState(entry::Key::Right)) mdl->RotateY(deltaTime * 100);

		if (inputGetKeyState(entry::Key::KeyR)) mdl->RotateX(-deltaTime * 100);
		if (inputGetKeyState(entry::Key::KeyT)) mdl->RotateX(deltaTime * 100);


		if (inputGetKeyState(entry::Key::KeyF)) mdl->RotateZ(-deltaTime * 100);
		if (inputGetKeyState(entry::Key::KeyG)) mdl->RotateZ(deltaTime * 100);


		if (inputGetKeyState(entry::Key::Space)) mdl->LookAt(Vector3());

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
