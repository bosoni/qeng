/*

template

X+ right
Y+ up
Z+ forward

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

namespace Test
{
	class Test : public BaseGameState
	{
		Font font;
		QScene scene;
		QModel3D* mdl;

	public:
		void Init()
		{
			font.LoadTtf("fonts/visitor1.ttf", 20);

			mdl = new QModel3D();
			mdl->Load("models/room1.obj.mesh", "models/room1.mtl");
			//mdl->Load("models/room2.obj.mesh", "models/room2.mtl");
			mdl->SetShader(new Shader(0, false)); // not needed, Load() sets default TextureShader
			scene.Add(mdl);

			// Z+ forward
			float scale = 1;
			for (int q = 0; q < 10; q++)
			{
				mdl = new QModel3D();
				mdl->Load("models/earth.obj.mesh", "models/earth.mtl");
				mdl->SetPosition(5, 0, q); // Z=q
				mdl->SetScale(scale -= 0.08f);
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
			char txt[512];
			sprintf(txt, "Visible meshes: %d\nPress F1 to see stats.", visibleMeshes);
			font.CreateText(5, 40, WChar(txt));
			font.Submit();
		}

	};
}
