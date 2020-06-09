/*

NOTES:
* creates Area2D for player and every block and checks if they are overlapping (collision)
* use arrowkeys


graffabugeja tilejen ala-/yläpuolella.
 korjautuiskoha jos ei käyttäis [0,0]-[1,1] vaan [0.0001,0.0001]-[0.9999,0.9999]  tj?



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
#include "Area2D.h"

namespace UgliGame
{
	// blocks, spikes, water
	char* map[] = {
	{"11111111111111111111111111111111111111"},
	{"10111100000000000000000000000000000001"},
	{"10011100000003000000000000300000000001"},
	{"10000000000011001000010001110101011001"},
	{"10000300000000000000000000000000030001"},
	{"10011111000110011300100001010101011001"},
	{"11111112222221111112221221222121122111"},
	};

	// grass etc
	char* map2[] = {
	{"00000000000000000000000000000000000000"},
	{"00000000000000000000000000000000000000"},
	{"00000000000000000000000000000000000000"},
	{"00000000000000000000000000000000000000"},
	{"00004440000440000000000000000000000000"},
	{"04400000000000000000000000000000000000"},
	{"00000000000000000000000000000000000000"},
	};

	char* tiles[] = {
		"",				// 0=empty
		"block.png",	// 1
		"water.png",	// 2
		"spike.png",	// 3
		"grass.png"		// 4
	};

	class Player
	{
	public:
		QModel3D* img;
		int idx = 0; // image index
		float energy = 10;
	};
	Player player;

	class Test : public BaseGameState
	{
		Font font;
		QScene scene;

		std::vector<Area2D> mapArea2D;
		QNode* mapNode = new QNode();
		float time = 0;
		float middleSound = 110; // TODO

		int WIDTH, HEIGHT;

		float restartTime = 2;

	public:
		void Init()
		{
			Log("Test");
			WIDTH = strlen(map[0]);
			HEIGHT = 7;

			//InitCapture();

			font.LoadTtf("fonts/visitor1.ttf", 20);

			CreateMap();
			scene.Add(mapNode);

			QModel3D* bg = new QModel3D();
			bg->Load("models/plane.obj.mesh", "models/plane.mtl");

			bg->materials[0]->LoadTexture("games/howlingdork/bg.png", 0, true);
			bg->SetScale(50, 20, 1);
			bg->state = noCullState;
			bg->SetPosition(0, -10, 10);
			scene.Add(bg);

			player.img = new QModel3D();
			player.img->Load("models/plane.obj.mesh", "models/plane.mtl");
			player.img->materials[0]->LoadTexture("games/howlingdork/m1.png", 0, true);
			player.img->state = noCullState;
			player.img->SetPosition(3, -6, 0);
			scene.Add(player.img);

			scene.camera.SetPosition(0.7f, -7, -20);
			scene.camera.Init(screenWidth, screenHeight);

			scene.PrintTree();
		}

		void Dispose()
		{
			//CloseCapture();
		}

		bool die = false;
		bool CheckIfColliding(float dx, float dy)
		{
			float x = (-mapNode->GetPosition().x + player.img->GetPosition().x) / 2;
			float y = -(mapNode->GetPosition().y + player.img->GetPosition().y) / 2;

			Area2D playerArea(x + dx, y + dy, 0.8f, 0.9f);
			for (int a = 0; a < mapArea2D.size(); a++)
			{
				if (mapArea2D[a].IsColliding(playerArea))
					return true;
			}
			return false;
		}

		void Update(float deltaTime)
		{
			visibleMeshes = 0;
			renderFirstTime = true;

			{
				float SPD = deltaTime * 2;

				cameraUpdate(deltaTime * 0.6f, mouseState);
				//if (inputGetKeyState(entry::Key::Tab)) scene.camera.Print();
				if (inputGetKeyState(entry::Key::Up))
				{
					if (CheckIfColliding(0, -SPD) == false) // NOTE -SPD. opposite direction because blocks are rendered down to up
						player.img->AddToPosition(0, SPD, 0);
				}
				if (inputGetKeyState(entry::Key::Down))
				{
					if (CheckIfColliding(0, SPD) == false) // NOTE +SPD
						player.img->AddToPosition(0, -SPD, 0);
				}
				if (inputGetKeyState(entry::Key::Left))
				{
					if (CheckIfColliding(-SPD, 0) == false)
						player.img->AddToPosition(-SPD, 0, 0);
				}
				if (inputGetKeyState(entry::Key::Right))
				{
					if (CheckIfColliding(SPD, 0) == false)
						player.img->AddToPosition(SPD, 0, 0);
				}
			}

			time += deltaTime * 2;

// TODO karttaa pitäis liikuttaa että pelaaja pysyy keskellä
//mapNode->SetPosition(-3 - player.img->GetPosition().x, -6 - player.img->GetPosition().y, 0);


			scene.Render();

			SetOrthoMatrix(screenWidth, screenHeight);
			PrintText();

		}

		void PrintText()
		{
			font.CreateText(4, 4, L"Test");
			font.Submit();

			if (player.energy <= 0)
			{
				font.CreateText(screenWidth / 2 - 10, screenHeight / 2, L"*** YOU DIED! ***");
				font.Submit();
			}
		}

		void CreateMap()
		{
			// blocks, spikes, water
			for (int y = 0; y < HEIGHT; y++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					int id = map[y][x] - '0';
					if (id > 0)
					{
						QModel3D* mdl = new QModel3D();
						mdl->Load("models/plane.obj.mesh", "models/plane.mtl");

						mdl->SetPosition(x * 2, -y * 2, -0.001f);

						mdl->state = noCullState;
						std::string fname = tiles[id];
						mdl->materials[0]->LoadTexture("games/howlingdork/" + fname, 0, true);
						mdl->materials[0]->diffCol.x = (float)x * 0.05f;
						mdl->materials[0]->diffCol.y = (float)y * 0.1f;
						mapNode->Add(mdl);

						Area2D area(x, y, 1, 1, id);
						mapArea2D.push_back(area);
					}
				}

				// not blocking planes, grass
				for (int y = 0; y < HEIGHT; y++)
				{
					for (int x = 0; x < WIDTH; x++)
					{
						int id = map2[y][x] - '0';
						if (id > 0)
						{
							QModel3D* mdl = new QModel3D();
							mdl->Load("models/plane.obj.mesh", "models/plane.mtl");

							mdl->SetPosition(x * 2, -y * 2, -0.001f);

							mdl->state = noCullState;
							std::string fname = tiles[id];
							mdl->materials[0]->LoadTexture("games/howlingdork/" + fname, 0, true);
							mdl->materials[0]->diffCol.x = (float)x * 0.05f;
							mdl->materials[0]->diffCol.y = (float)y * 0.1f;
							mapNode->Add(mdl);
						}
					}
				}
			}
		}
	};
}
