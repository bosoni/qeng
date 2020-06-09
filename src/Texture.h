#pragma once
#include <string>
#include "Core.h"

class Texture
{
public:
	std::string name = "";
	bgfx::TextureHandle texture;

	Texture() { texture.idx = 0; }

	void Load(std::string name);
	void Dispose();
};

#ifdef IMPLEMENTATION
void Texture::Load(std::string filename)
{
	for (Texture* t : allTextures)
		if (t->name == filename)
		{
			//Log("+found " + filename);
			name = t->name;
			texture = t->texture;
			return;
		}
	Log("+load " + filename);
	name = filename;
	texture = loadTexture(name.c_str());
	if (isValid(texture) == false)
	{
		Log("  ERROR: Cannot load texture!");
		throw "Cannot load texture!";
		return;
	}
	allTextures.push_back(this);
}

void Texture::Dispose()
{
	// TODO refcount

	Log("-destroy " + name);
	bgfx::destroy(texture);

	int c = 0;
	for (Texture* s : allTextures)
	{
		if (s->name == name)
		{
			allTextures.erase(allTextures.begin() + c);
			break;
		}
		c++;
	}
}

#endif
