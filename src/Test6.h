/*

* multilighting
* normalmapping

1,2,3,4: change light
arrowkeys, pgup, pgdown: move light
r,t: light radius
f,t: inner radius


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

class Test6 : public BaseGameState
{
	Font font;
	QScene scene;
	QModel3D* mdl;
	QLight light[4];
	int selectedLight = 0;

public:
	void Init()
	{
		font.LoadTtf("fonts/visitor1.ttf", 20);

		for (int q = 0; q < 10; q++)
		{
			mdl = new QModel3D();
			mdl->Load("models/rock.obj.mesh", "models/rock.mtl");
			mdl->SetPosition(-2, -4 + q, 0);
			mdl->SetRotation(q * 5, q * 20, q * 10);
			mdl->SetShader(new Shader(1, false));
			scene.Add(mdl);
		}

		for (int q = 0; q < 10; q++)
		{
			mdl = new QModel3D();
			mdl->Load("models/rock.obj_nm.mesh", "models/rock_nm.mtl"); // .mesh with tangents
			mdl->SetPosition(2, -4 + q, 0);
			mdl->SetRotation(q * 5, q * 20, q * 10);
			mdl->SetShader(new Shader(2, false));
			scene.Add(mdl);
		}

		mdl = new QModel3D();
		mdl->Load("models/floor1.obj.mesh", "models/floor1.mtl");
		mdl->SetScale(20);
		mdl->SetShader(new Shader(1, false));
		scene.Add(mdl);

		scene.camera.SetPosition(0, 2, -10);
		scene.camera.Init(screenWidth, screenHeight);

		for (int q = 0; q < 4; q++)
		{
			light[q].SetPosition(-2 + q, 3, -5);
			if (q == 0) light[q].enabled = true; // lights are disabled by default
			scene.AddLight(&light[q]);
		}
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

		deltaTime *= 4;

		if (inputGetKeyState(entry::Key::Key1))  selectedLight = 0;
		if (inputGetKeyState(entry::Key::Key2)) selectedLight = 1;
		if (inputGetKeyState(entry::Key::Key3)) selectedLight = 2;
		if (inputGetKeyState(entry::Key::Key4)) selectedLight = 3;

		light[selectedLight].enabled = true;

		if (inputGetKeyState(entry::Key::Up)) light[selectedLight].AddToPosition(0, deltaTime, 0);
		if (inputGetKeyState(entry::Key::Down)) light[selectedLight].AddToPosition(0, -deltaTime, 0);
		if (inputGetKeyState(entry::Key::Left)) light[selectedLight].AddToPosition(-deltaTime, 0, 0);
		if (inputGetKeyState(entry::Key::Right)) light[selectedLight].AddToPosition(deltaTime, 0, 0);
		if (inputGetKeyState(entry::Key::PageUp)) light[selectedLight].AddToPosition(0, 0, deltaTime);
		if (inputGetKeyState(entry::Key::PageDown)) light[selectedLight].AddToPosition(0, 0, -deltaTime);

		if (inputGetKeyState(entry::Key::KeyR)) light[selectedLight].radius -= 0.1f;
		if (inputGetKeyState(entry::Key::KeyT)) light[selectedLight].radius += 0.1f;
		if (inputGetKeyState(entry::Key::KeyF)) light[selectedLight].innerRadius -= 0.1f;
		if (inputGetKeyState(entry::Key::KeyG)) light[selectedLight].innerRadius += 0.1f;
		light[selectedLight].radius = Clamp(light[selectedLight].radius, 0, 1000);
		light[selectedLight].innerRadius = Clamp(light[selectedLight].innerRadius, 0, 1);

		scene.Render();

		SetOrthoMatrix(screenWidth, screenHeight);
		PrintText();
	}

	void PrintText()
	{
		char txt[512];
		sprintf(txt, "Visible meshes: %d\nPress F1 to see stats.\n\nPress 1-4 to change(and enable) light.\nR,T  radius: %4.4f\nF,G  innerradius: %4.4f",
			visibleMeshes,
			light[selectedLight].radius, light[selectedLight].innerRadius);

		font.CreateText(5, 40, WChar(txt));
		font.Submit();
	}

};
