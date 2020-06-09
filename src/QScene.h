#pragma once
#include <vector>
#include "QNode.h"
#include "QCamera.h"
#include "QLight.h"

class Environment
{
public:
	float fogStart = 1, fogEnd = 200, fogY = 0;
	Vector3 fogColor = { 0.1f, 0.1f, 0.3f };
	bool fogEnabled = false;

	std::vector<QLight*> lights;
};

class QScene : public QNode
{
	std::vector<QNode*> allNodes;

public:
	QCamera camera;
	Environment environment;

	QScene() { name = "root"; }

	void AddLight(QLight* light)
	{
		environment.lights.push_back(light);
	}

	void Render();
	void RenderLists(QCamera camera);
	void CreateLists(QNode* node);
};

#ifdef IMPLEMENTATION
void QScene::Render()
{
	camera.Update();

	allNodes.clear();
	CreateLists(this);
	RenderLists(camera);

	renderFirstTime = false;
}

void QScene::RenderLists(QCamera camera)
{
	for (QNode* n : allNodes)
		n->Render(&camera, &environment);
}

void QScene::CreateLists(QNode* node)
{
	for (QNode* n : node->GetChildren())
	{
		allNodes.push_back(n);
		CreateLists(n);
	}
}

#endif
