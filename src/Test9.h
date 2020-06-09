/*

* tex, tex_lit, bump test ;  with and without instancing

* first setup scene like in  CreateTowers()
* then AddToScene("models/rock.obj_nm.mesh", "models/rock_nm.mtl", mtx, col, numModels, false/true);
  which adds models to scene (using instancing or not)

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

class Test9 : public BaseGameState
{
	Font font;
	QScene scene;
	QModel3D* mdl;
	QLight light[4];

public:
	void Init()
	{
		font.LoadTtf("fonts/visitor1.ttf", 20);

		mdl = new QModel3D();
		mdl->Load("models/floor1.obj.mesh", "models/floor1.mtl");
		mdl->SetScale(20);
		mdl->SetShader(new Shader(2, false));
		scene.Add(mdl);

		scene.camera.SetPosition(0, 6, -15);
		scene.camera.Init(screenWidth, screenHeight);

		for (int q = 0; q < 1; q++)
		{
			light[q].SetPosition(-2 + q, 10, 0);
			light[q].radius = 10;
			light[q].enabled = true; // lights are disabled by default
			scene.AddLight(&light[q]);
		}

		scene.PrintTree();

		const bgfx::Caps* caps = bgfx::getCaps();
		if ((BGFX_CAPS_INSTANCING & caps->supported) == 0)
		{
			Log("Instancing is not supported by GPU. ");
		}

		CreateTowers();

	}

	void Dispose()
	{
		mdl->Dispose();
	}


	void CreateTowers()
	{
		int numModels = 25;
		std::vector<Matrix> mtx;
		std::vector<Vector4> col;
		bool instanced = false;
		int shaderType = 0;

		// towers:
		for (int z = 0; z < 2; z++)
			for (int x = 0; x < 3; x++)
			{
				shaderType = x; // TextureShader, TextureLightingShader, BumpShader
				instanced = (int)z; // false, true

				mdl = new QModel3D();
				mdl->Load("models/rock.obj.mesh", "models/rock.mtl");
				for (int q = 0; q < numModels; q++)
				{
					//mdl->SetPosition(rand() % 2 - x * 8, q * 2, z * 8);
					mdl->SetPosition(2 - x * 8, q * 2, z * 8);
					mdl->SetRotation(0, q * 5, 0);
					Matrix matrix = mdl->CalcWorldMatrix();
					mtx.push_back(matrix);

					col.push_back({ (float)q * 0.1f,1,1, 1 });
				}
				AddToScene(&scene, "models/rock.obj_nm.mesh", "models/rock_nm.mtl", shaderType, mtx, col, numModels, instanced);
				mtx.clear();
				col.clear();
			}
	}

	void Update(float deltaTime)
	{
		visibleMeshes = 0;
		renderFirstTime = true;

		cameraUpdate(deltaTime * 0.6f, mouseState);

		light[0].SetLightWithKeyboard();

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
