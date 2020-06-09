#pragma once
#include <string>
#include "Core.h"
#include "Texture.h"

class Shader;

// shader, colors and textures
class MeshMaterial
{
public:
	std::string name = "";
	Shader* shader = NULL;
	Vector4 diffCol = { 1,1,1,1 }, ambCol = { 0.2f, 0.2f, 0.2f, 0.2f }, specCol = { 0.2f, 0.2f, 0.2f, 0.2f };
	Texture* textures[MAX_TEXTURES];
	bool frustumCulling = true;

	float alpha = 1;
	float ns = 1; // specular

	MeshMaterial();

	// Load texturemap. type: albedomap=0  normalmap=1  specmap=2
	void LoadTexture(std::string fileName, int type = 0, bool doNotUseModelsDir = false);
	void Dispose();
};

// List of materials of .mtl file
class Material
{
public:
	std::string name = "";
	std::vector<MeshMaterial> materials;

	// Load .mtl file
	void Load(std::string filename);

	// find material from materials list
	MeshMaterial* GetMeshMaterial(std::string name);

	// set shader to all materials
	void SetShader(Shader* shader);

	void Dispose();
};

#ifdef IMPLEMENTATION
#include "InlineFunctions.h"
#include "Shader.h"

#if true
Texture* whiteTex = NULL;
MeshMaterial::MeshMaterial()
{
	if (whiteTex == NULL)
	{
		std::string s = ASSETSDIR + "textures/white.png";
		whiteTex = new Texture();
		whiteTex->Load(s);
	}
	for (int q = 0; q < MAX_TEXTURES; q++) textures[q] = whiteTex;
}
#else
MeshMaterial::MeshMaterial() // old version
{
	std::string s = ASSETSDIR + "textures/white.png";
	Texture* whiteTex = new Texture();
	whiteTex->Load(s);
	for (int q = 0; q < MAX_TEXTURES; q++) textures[q] = whiteTex;
}
#endif


// Load texturemap. type: albedomap=0  normalmap=1  specmap=2
void MeshMaterial::LoadTexture(std::string fileName, int type, bool doNotUseModelsDir)
{
	std::string fname;

	if (doNotUseModelsDir)
		fname = ASSETSDIR + fileName;
	else
		fname = ASSETSDIR + "models/" + str_getFileName(fileName);

	// reset colors
	diffCol = { 1,1,1,1 };
	ambCol = { 0.2f, 0.2f, 0.2f, 0.2f };
	specCol = { 0.2f, 0.2f, 0.2f, 0.2f };

	// create new texture and load texture

	if (type == 0)
	{
		//textures[0]->Load(fname);
		textures[0] = new Texture();
		textures[0]->Load(fname);

		if (loadNormalAndSpecMap) // automatic normalmap+specmap loading
		{
			//printf("*find  %s\n", tmp.c_str());
			std::string tmp = fname + "_nm";
			if (FileExists(tmp.c_str()))
			{
				textures[1] = new Texture();
				textures[1]->Load(tmp);
			}

			//printf("*find  %s\n", tmp.c_str());
			tmp = fname + "_sm";
			if (FileExists(tmp.c_str()))
			{
				textures[2] = new Texture();
				textures[2]->Load(tmp);
			}
		}
	}
	else
	{
		//textures[type]->Load(fname); // TODO tää ei toiminu mut miks taas toimiis?
		textures[type] = new Texture();
		textures[type]->Load(fname);
	}

}

void MeshMaterial::Dispose()
{
	/*
	Log("-destroy " + name);
	shader->Dispose(); // TODOFIX crash
	*/

	/* NOTE: not like this
	for (int q = 0; q < MAX_TEXTURES; q++)
		if (textures->texture.idx != 0)
			textures[q].Dispose();
	*/
}

// --- Material ---

// Load .mtl file
void Material::Load(std::string filename)
{
	for (Material* s : allMaterials)
		if (s->name == filename)
		{
			//Log("+found " + filename);
			name = s->name;
			materials = s->materials;
			return;
		}
	Log("+load " + filename);
	name = filename;

	FILE* in = fopen(filename.c_str(), "r");
	if (!in)
	{
		Log("  ERROR: Cannot load " + filename);
		throw "Cannot load material file!";
		return;
	}

	MeshMaterial mat;
	char line[500], str[500];
	float r = 1, g = 1, b = 1, alpha = 1;
	float ns = 1;
	while (true)
	{
		if (fgets(line, 100, in) == NULL)
			break;

		if (line[0] == '#' || line[0] == '\n')
			continue;

		if (strstr(line, "newmtl "))
		{
			if (mat.name != "")
			{
				materials.push_back(mat);

				MeshMaterial* m = new MeshMaterial();
				mat = *m;
			}
			sscanf(line, "newmtl %s", str);
			mat.name = std::string(str);
			continue;
		}

		if (strstr(line, "map_Kd ")) // texturemap
		{
			sscanf(line, "map_Kd %s", str);
			mat.LoadTexture(str);
			continue;
		}

		if (strstr(line, "map_Kn ")) // normalmap
		{
			sscanf(line, "map_Kn %s", str);
			mat.LoadTexture(str, 1);
			continue;
		}

		if (strstr(line, "Map_Ks ")) // specularmap
		{
			sscanf(line, "map_Ks %s", str);
			mat.LoadTexture(str, 2);
			continue;
		}

		if (strstr(line, "Ns "))
		{
			sscanf(line, "Ns %f", &ns);
			mat.ns = ns;
			continue;
		}
		if (strstr(line, "Ka "))
		{
			sscanf(line, "Ka %f %f %f", &r, &g, &b);
			mat.ambCol = { r,g,b };
			continue;
		}
		if (strstr(line, "Kd "))
		{
			sscanf(line, "Kd %f %f %f", &r, &g, &b);
			mat.diffCol = { r,g,b };
			continue;
		}
		if (strstr(line, "Ks "))
		{
			sscanf(line, "Ks %f %f %f", &r, &g, &b);
			mat.specCol = { r,g,b };
			continue;
		}
		if (strstr(line, "Alpha "))
		{
			sscanf(line, "Alpha %f", &alpha);
			mat.alpha = alpha;
			continue;
		}
	}

	if (mat.name != "") // last one
	{
		materials.push_back(mat);
	}

	fclose(in);

	allMaterials.push_back(this);
}

void Material::Dispose()
{
	Log("-destroy " + name);

	for (int q = 0; q < materials.size(); q++) materials[q].Dispose();

	int c = 0;
	for (Material* s : allMaterials)
	{
		if (s->name == name)
		{
			allMaterials.erase(allMaterials.begin() + c);
			break;
		}
		c++;
	}
}

MeshMaterial* Material::GetMeshMaterial(std::string name)
{
	for (int q = 0; q < materials.size(); q++)
	{
		if (materials[q].name == name) return &materials[q];
	}
	return NULL;
}

void Material::SetShader(Shader* shader)
{
	for (int q = 0; q < materials.size(); q++)
		materials[q].shader = shader;
}

#endif
