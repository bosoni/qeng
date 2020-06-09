#pragma once
#include "Core.h"
#include "QNode.h"
#include "Material.h"

class QModel3D : public QNode
{
	Material mtl;
	void SetMaterials();
	uint8_t* instanceData = 0;
	int numInstances, instanceStride;

public:
	Mesh* mesh = NULL;
	std::vector<MeshMaterial*> materials;
	uint64_t state = BGFX_STATE_MASK;
	bgfx::ViewId viewId = 0;

	void Load(std::string name, std::string mtlfile = "");
	void LoadMaterial(std::string mtlfile = "");
	void SetShader(Shader* shader);
	void Dispose();

	void Render(QCamera* camera, Environment* env) override;
	void RenderInstanced(QCamera* camera, Environment* env);

	uint8_t* CreateInstanceDataBuffer(int numOfInstances, int stride)
	{
		numInstances = numOfInstances;
		instanceStride = stride;
		if (instanceData != 0) delete[] instanceData;
		instanceData = new uint8_t[(int)(numOfInstances * stride)];
		return instanceData;
	}
};

#ifdef IMPLEMENTATION
#include "Shader.h"

void QModel3D::SetMaterials()
{
	int c = -1;
	for (GroupArray::iterator it = mesh->m_groups.begin(), itEnd = mesh->m_groups.end(); it != itEnd; ++it)
	{
		c++;
		Group& group = *it;
		// if material found, set it
		MeshMaterial* mat = mtl.GetMeshMaterial(group.m_prims[0].materialName);
		if (mat != NULL)
			materials.push_back(mat);
		else
		{
			// no material, create random color and texture shader
			MeshMaterial* newmat = new MeshMaterial();
			newmat->diffCol.x = (float)c * 0.01f;
			newmat->diffCol.y = (float)c * 0.001f;
			newmat->diffCol.z = (float)c * 0.001f;
			Shader* shader = new Shader(0, false);
			newmat->shader = shader;
			materials.push_back(newmat);
		}
	}
}

void QModel3D::Load(std::string filename, std::string mtlfile)
{
	if (mesh != NULL)
		throw "Create new object and then load model!  ie QModel3D *newObj=new QModel3D(); newObj->Load(\"...\");";

	std::string nname = ASSETSDIR + filename;
	for (QModel3D* s : allModels)
		if (s->name == filename)
		{
			//Log("+found " + filename);
			name = s->name;
			mesh = s->mesh;
			LoadMaterial(mtlfile);
			return;
		}

	Log("+load " + filename);

	name = filename;
	mesh = meshLoad(nname.c_str());

	allModels.push_back(this);

	if (mesh == NULL)
	{
		Log("  ERROR: Cannot load " + nname);
		throw "Cannot load model!";
		return;
	}

	LoadMaterial(mtlfile);
}

void QModel3D::LoadMaterial(std::string mtlfile)
{
	if (mtlfile != "")
	{
		std::string nname = ASSETSDIR + mtlfile;
		mtl.Load(nname);
	}
	SetMaterials();
	SetShader(new Shader(0, false));
}

void QModel3D::SetShader(Shader* shader)
{
	mtl.SetShader(shader);
}

void QModel3D::Dispose()
{
	// TODO refcount

	Log("-destroy " + name);
	meshUnload(mesh);

	int c = 0;
	for (QModel3D* s : allModels)
	{
		if (s->name == name)
		{
			allModels.erase(allModels.begin() + c);
			break;
		}
		c++;
	}
}

void QModel3D::Render(QCamera* camera, Environment* env)
{
	if (mesh == NULL)
	{
		Log("   ERROR: mesh==null");
		throw "   ERROR: mesh==null";
	}

	if (numInstances > 0)
	{
		RenderInstanced(camera, env);
		return;
	}

	Matrix worldPos = CalcWorldMatrix();
	Matrix mtx = MatrixTranspose(worldPos);
	mesh->submitWithMaterials(camera, env, materials, viewId, (float*)&mtx, state);
}

void QModel3D::RenderInstanced(QCamera* camera, Environment* env)
{
	if (mesh == NULL)
	{
		Log("   ERROR: mesh==null");
		throw "   ERROR: mesh==null";
	}
	mesh->submitWithMaterialsInstanced(env, materials, viewId, state, instanceData, numInstances, instanceStride);
}

#endif
