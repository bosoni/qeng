#pragma once
#include <stdio.h>
#include <bgfx_utils.h>
#include "Core.h"

inline void Log(std::string txt)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	printf("[%d:%d:%d]: %s\n",
		ltm->tm_hour,
		ltm->tm_min,
		ltm->tm_sec,
		txt.c_str());
}

inline bool FileExists(const char* file)
{
	FILE* f = fopen(file, "r");
	if (!f) return false;
	fclose(f);
	return true;
}

inline void PrintAttributes(Mesh* mesh)
{
	char* attrnames[] =
	{
			"Position",  //!< a_position
			"Normal",    //!< a_normal
			"Tangent",   //!< a_tangent
			"Bitangent", //!< a_bitangent
			"Color0",    //!< a_color0
			"Color1",    //!< a_color1
			"Color2",    //!< a_color2
			"Color3",    //!< a_color3
			"Indices",   //!< a_indices
			"Weight",    //!< a_weight
			"TexCoord0", //!< a_texcoord0
			"TexCoord1", //!< a_texcoord1
			"TexCoord2", //!< a_texcoord2
			"TexCoord3", //!< a_texcoord3
			"TexCoord4", //!< a_texcoord4
			"TexCoord5", //!< a_texcoord5
			"TexCoord6", //!< a_texcoord6
			"TexCoord7", //!< a_texcoord7
	};
	printf("--------------\n");
	for (int q = 0; q < 18; q++) printf("--debug  attr: %d   %s\n", mesh->m_layout.m_attributes[q],
		mesh->m_layout.m_attributes[q] < 65535 ? attrnames[q] : "");
}

inline float Vector3LengthSquared(const Vector3 v)
{
	float result = (v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
}

// -- str string --
inline std::string str_replace(std::string n, char a, char b)
{
	for (int q = 0; q < n.length(); q++)
		if (n[q] == a) n[q] = b;
	return n;
}
inline int str_findLast(std::string n, char c)
{
	for (int q = (int)n.length() - 1; q > 0; q--)
		if (n[q] == c)
			return q;
	return -1;
}
inline char* str_subStr(std::string s, int start, int end)
{
	static char temp[1000];
	int c = 0;
	for (int q = start; q < end; q++)
		temp[c++] = s[q];
	temp[c] = 0;
	return temp;
}

// ie models/house.png  returns house
inline std::string str_getBaseName(std::string n)
{
	n = str_replace(n, '\\', '/');
	int st = str_findLast(n, '/');
	int end = str_findLast(n, '.');
	char* ff = str_subStr(n, st + 1, end);
	return std::string(ff);
}
// ie models/house.png  returns house.png
inline std::string str_getFileName(std::string n)
{
	n = str_replace(n, '\\', '/');
	int st = str_findLast(n, '/');
	int end = (int)n.length();
	char* ff = str_subStr(n, st + 1, end);
	return std::string(ff);
}
