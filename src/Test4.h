/*

* instancing

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

class Test4 : public BaseGameState
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

		scene.camera.SetPosition(0, 2, -5);
		scene.camera.Init(screenWidth, screenHeight);

		scene.PrintTree();

		const bgfx::Caps* caps = bgfx::getCaps();
		if ((BGFX_CAPS_INSTANCING & caps->supported) == 0)
		{
			Log("Instancing is not supported by GPU. ");
		}

		CreateInstances();

	}

	void Dispose()
	{
		mdl->Dispose();
	}

	void CreateInstances()
	{
		mdl = new QModel3D();
		mdl->Load("models/tree1.obj.mesh", "models/tree1.mtl");
		//mdl->Load("models/cube.obj.mesh", "models/cube.mtl"); 
		//mdl->Load("models/obj1.obj.mesh", "models/obj1.mtl");
		mdl->SetShader(new Shader(0, true)); // true==instanced

		// 80 bytes stride = 64 bytes for 4x4 matrix + 16 bytes for RGBA color.
		int stride = 80, ins = 100;
		uint8_t* ptr = mdl->CreateInstanceDataBuffer(ins, stride);
		for (int q = 0; q < ins; q++)
		{
			Vector3 v = { rand() % 60 - 30, 3, rand() % 60 - 30 };

			float* mtx = (float*)ptr;
			for (int q = 0; q < 16; q++) mtx[q] = 0;
			mtx[0] = mtx[5] = mtx[10] = mtx[15] = 1;
			mtx[12] = v.x;
			mtx[13] = v.y;
			mtx[14] = v.z;
			float* color = (float*)&ptr[64]; // 4*16=64
			color[0] = bx::sin(q * 0.01f);
			color[1] = 0;
			color[2] = 0.1f;
			color[3] = 1.0f;
			ptr += stride;
		}
	}


	void Update(float deltaTime)
	{
		visibleMeshes = 0;
		renderFirstTime = true;

		cameraUpdate(deltaTime * 0.6f, mouseState);

		mdl->RenderInstanced(&scene.camera, &scene.environment);

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

