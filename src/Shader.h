#pragma once
#include "Core.h"

class Environment;
class MeshMaterial;

#define MAX_UNIFORMS 5
#define MAX_TEXTURES 5
#define NUM_LIGHTS   4

class Shader
{

public:
	bool instanced = false;
	std::string name = "";
	bgfx::ProgramHandle program;
	bgfx::UniformHandle uniforms[MAX_UNIFORMS];
	bgfx::UniformHandle samplers[MAX_TEXTURES];

	/*
	Load shaders, vertex shader and fragment shader.
	To load tex_fs.bin file, use
		new Shader("tex", "tex", useInstancing); //dont write _vs and _fs after filenames, they are added automatically.
	If using instancing, put instanced=true.
	*/
	Shader(std::string vs, std::string fs, bool instanced)
	{
		Load(vs, fs, instanced);
	}

	/*
	Set shader to model.
	shaderType: 0=TextureShader, 1=TextureLightingShader, 2=BumpShader
	If using instancing, put instanced=true.
	*/
	Shader(int shaderType, bool instanced)
	{
		switch (shaderType)
		{
		case 0:  Load("tex", "tex", instanced); break;
		case 1:  Load("tex_lit", "tex_lit", instanced); break;
		case 2:  Load("bump", "bump", instanced); break;
		}
	}

	/*
	Load shaders, vertex shader and fragment shader.
	To load tex_fs.bin file, use
		new Shader("tex", "tex", useInstancing); //dont write _vs and _fs after filenames, they are added automatically.
	If using instancing, put instanced=true.
	*/
	void Load(std::string vs, std::string fs, bool instanced);

	void Dispose();

	void GetUniforms();
	void SetUniforms(const MeshMaterial* mat, const Environment* environment);
};


#ifdef IMPLEMENTATION
void Shader::Load(std::string vs, std::string fs, bool instanced)
{
	for (int q = 0; q < MAX_UNIFORMS; q++) uniforms[q].idx = bx::kInvalidHandle;
	for (int q = 0; q < MAX_TEXTURES; q++) samplers[q].idx = bx::kInvalidHandle;

	if (instanced) vs += "_instanced_vs";
	else vs += "_vs";
	fs += "_fs";
	std::string shadername = vs + ":" + fs;

	this->instanced = instanced;

	// Create program from shaders
	for (Shader* s : allShaders)
		if (s->name == shadername)
		{
			//Log("+found " + shadername);
			name = s->name;
			program = s->program;
			GetUniforms();
			return;
		}
	Log("+load " + shadername);
	name = shadername;
	program = loadProgram(vs.c_str(), fs.c_str());

	if (!isValid(program))
	{
		Log("   ERROR: Cannot load " + shadername);
		throw "Cannot load shader!";
	}
	allShaders.push_back(this);

	GetUniforms();
}

void Shader::GetUniforms()
{
	bgfx::UniformHandle tex = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
	bgfx::UniformHandle tex2 = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
	samplers[0] = tex;
	samplers[1] = tex2;

	bgfx::UniformHandle color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
	uniforms[0] = color;

	// take lights if any
	bgfx::UniformHandle lightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4, NUM_LIGHTS);
	bgfx::UniformHandle lightRgbInnerR = bgfx::createUniform("u_lightRgbInnerR", bgfx::UniformType::Vec4, NUM_LIGHTS);
	uniforms[1] = lightPosRadius;
	uniforms[2] = lightRgbInnerR;
}

void Shader::Dispose()
{
	// TODO refcount

	Log("-destroy " + name);

	for (int q = 0; q < MAX_UNIFORMS; q++) if (isValid(uniforms[q])) bgfx::destroy(uniforms[q]);
	for (int q = 0; q < MAX_TEXTURES; q++) if (isValid(samplers[q])) bgfx::destroy(samplers[q]);

	bgfx::destroy(program);

	int c = 0;
	for (Shader* s : allShaders)
	{
		if (s->name == name)
		{
			allShaders.erase(allShaders.begin() + c);
			break;
		}
		c++;
	}
}

void Shader::SetUniforms(const MeshMaterial* mat, const Environment* environment)
{
	if (instanced == false)
		bgfx::setUniform(uniforms[0], &mat->diffCol);

	float lightPosRadius[4][4];
	float lightRgbInnerR[4][4];
	int s = environment->lights.size();

	// set lights if there are any
	if (s > 0 && isValid(uniforms[1]) && isValid(uniforms[2]))
	{
		if (s > NUM_LIGHTS) s = NUM_LIGHTS;
		for (int q = 0; q < s; q++)
		{
			if (environment->lights[q]->enabled)
			{
				lightPosRadius[q][0] = environment->lights[q]->GetPosition().x;
				lightPosRadius[q][1] = environment->lights[q]->GetPosition().y;
				lightPosRadius[q][2] = environment->lights[q]->GetPosition().z;
				lightPosRadius[q][3] = environment->lights[q]->radius;

				lightRgbInnerR[q][0] = environment->lights[q]->color.x;
				lightRgbInnerR[q][1] = environment->lights[q]->color.y;
				lightRgbInnerR[q][2] = environment->lights[q]->color.z;
				lightRgbInnerR[q][3] = environment->lights[q]->innerRadius;
			}
		}
		bgfx::setUniform(uniforms[1], lightPosRadius, NUM_LIGHTS);
		bgfx::setUniform(uniforms[2], lightRgbInnerR, NUM_LIGHTS);
	}
}

#endif
