#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raymath.h"
#include <bgfx_utils.h>
#include <common.h>
#include <camera.h>
#include <entry/input.h>
#include <font/font_manager.h>
#include <font/text_buffer_manager.h>
#include "Array.h"
#include "InlineFunctions.h"

static std::string VERSION = "0.20.6.09"; // major.year.month.day

// TODO kun exporttaa niin vaihda "Data/"
static std::string ASSETSDIR = "../Data/";
constexpr auto MAX_TEXTURES = 5;
constexpr auto RENDER3D_VIEW_ID = 0; // view 0
constexpr auto ORTHO_VIEW_ID = 1; // view 1
constexpr auto loadNormalAndSpecMap = true;

extern bool renderFirstTime;
extern int loadedModels;
extern int visibleMeshes;
extern float deltaTime;

extern uint32_t screenWidth, screenHeight;
extern entry::MouseState mouseState;

static uint64_t noCullState = 0
| BGFX_STATE_WRITE_RGB
| BGFX_STATE_WRITE_A
| BGFX_STATE_WRITE_Z
| BGFX_STATE_DEPTH_TEST_LESS
//| BGFX_STATE_CULL_CCW
| BGFX_STATE_MSAA
;

class Vector4
{
public:
	float x, y, z, w;
};

class QScene;
class QModel3D;
class Material;
class Texture;
class Shader;
extern std::vector<QModel3D*> allModels;
extern std::vector<Material*> allMaterials;
extern std::vector<Texture*> allTextures;
extern std::vector<Shader*> allShaders;

void DisposeAll();
wchar_t* WChar(const char* txt);
void SetShader(QModel3D* mdl, int shaderType, bool instanced);
void AddToScene(QScene* scene, std::string fileName, std::string mtlName, int shaderType,
	std::vector<Matrix> mtx, std::vector<Vector4> col,
	int numModels, bool instanced);
void SetOrthoMatrix(int width, int height);

#ifdef IMPLEMENTATION
#include "QModel3D.h"
#include "Material.h"
#include "Texture.h"

// caches
std::vector<QModel3D*> allModels;
std::vector<Material*> allMaterials;
std::vector<Texture*> allTextures;
std::vector<Shader*> allShaders;

bool renderFirstTime = true;
int loadedModels = 0;
int visibleMeshes = 0;
float deltaTime = 0;

uint32_t screenWidth, screenHeight;
entry::MouseState mouseState;

void DisposeAll()
{
	// dispose datas, last first
	for (int c = allModels.size() - 1; c >= 0; c--) allModels[c]->Dispose();
	for (int c = allTextures.size() - 1; c >= 0; c--) allTextures[c]->Dispose();
	for (int c = allMaterials.size() - 1; c >= 0; c--) allMaterials[c]->Dispose();
	for (int c = allShaders.size() - 1; c >= 0; c--) allShaders[c]->Dispose();

	// ---- DEBUG ----
	printf("   debug: DisposeAll sizes:  %d %d %d %d\n", (int)allModels.size(), (int)allMaterials.size(), (int)allTextures.size(), (int)allShaders.size());
	for (Material* m : allMaterials) printf("  mat not disposed: %s\n", m->name.c_str());
	for (Texture* m : allTextures) printf("  tex not disposed: %s\n", m->name.c_str());
	for (QModel3D* m : allModels) printf("  model not disposed: %s\n", m->name.c_str());
	for (Shader* m : allShaders) printf("  shader not disposed: %s\n", m->name.c_str());


	allModels.clear();
	allMaterials.clear();
	allTextures.clear();
	allShaders.clear();
}

wchar_t wtxt[1000];
/*
Convert char array (string) to wide char array (string),
so one can use ִײהצ characters in Font class' writing functions.
*/
wchar_t* WChar(const char* txt)
{
	size_t tmp;
	mbstowcs_s(&tmp, wtxt, txt, strlen(txt));
	return wtxt;
}

/*
Set shader to model.
shaderType: 0=TextureShader, 1=TextureLightingShader, 2=BumpShqder
*/
void SetShader(QModel3D* mdl, int shaderType, bool instanced)
{
	mdl->SetShader(new Shader(shaderType, instanced));
}

/*
Adds numModels count of models to the scene (instanced or not).
shaderType: 0=TextureShader, 1=TextureLightingShader, 2=BumpShader
*/
void AddToScene(QScene* scene, std::string fileName, std::string mtlName, int shaderType,
	std::vector<Matrix> mtx, std::vector<Vector4> col,
	int numModels, bool instanced)
{
	if (instanced)
	{
		QModel3D* mdl = new QModel3D();
		mdl->Load(fileName, mtlName);
		SetShader(mdl, shaderType, instanced);
		scene->Add(mdl); // Add model only once

		// 80 bytes stride = 64 bytes for 4x4 matrix + 16 bytes for RGBA color.
		int stride = 80, ins = numModels;
		uint8_t* ptr = mdl->CreateInstanceDataBuffer(ins, stride);
		for (int q = 0; q < ins; q++)
		{
			float* mtxptr = (float*)ptr;
			Matrix matrix = mtx[q];
			matrix = MatrixTranspose(matrix);
			float* mptr = (float*)&matrix;
			for (int w = 0; w < 16; w++) mtxptr[w] = mptr[w];
			float* color0 = (float*)&ptr[64]; // 4*16=64 (1float=4bytes, 1row=4float=16bytes, matrix4x4=4*16=64)
			color0[0] = col[q].x;
			color0[1] = col[q].y;
			color0[2] = col[q].z;
			color0[3] = 1;
			ptr += stride;
		}
	}
	else
	{
		for (int q = 0; q < numModels; q++)
		{
			QModel3D* mdl = new QModel3D();
			mdl->Load(fileName, mtlName);
			SetShader(mdl, shaderType, instanced);

			mdl->SetWorldMatrix(mtx[q]);
			for (int w = 0; w < mdl->materials.size(); w++) mdl->materials[w]->diffCol = col[q];
			scene->Add(mdl); // Add all models
		}
	}
}

void SetOrthoMatrix(int width, int height)
{
	const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -1.0f };
	float view[16];
	bx::mtxLookAt(view, eye, at);
	const float centering = 0.5f;

	// Setup a top-left ortho matrix for screen space drawing.
	const bgfx::Caps* caps = bgfx::getCaps();
	{
		float ortho[16];
		bx::mtxOrtho(ortho, centering, width + centering, height + centering, centering, 0.0f, 100.0f, 0.0f, caps->homogeneousDepth);
		bgfx::setViewTransform(ORTHO_VIEW_ID, view, ortho);
		bgfx::setViewRect(ORTHO_VIEW_ID, 0, 0, uint16_t(width), uint16_t(height));
	}
}

#endif // IMPLEMENTATION
