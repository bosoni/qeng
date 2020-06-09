#pragma once
#pragma warning(disable : 4244)
#pragma warning(disable : 4838)
#include <string>
#include <vector>
#include "Core.h"

class Camera;
class Environment;

class QNode
{
protected:
	std::vector<QNode*> children;
	QNode* parent;
	Vector3 pos, rot, scale = { 1, 1, 1 };
	Matrix worldMatrix = MatrixIdentity();
	Matrix rotationMatrix = MatrixIdentity();
	bool rotDirty = true; // calc rotationMatrix if rotation changed
	bool dirty = true; // calc worldMatrix only if pos/scale changed

public:
	std::string name = "";

	void virtual Render(QCamera* camera, Environment* env);
	std::vector<QNode*> GetChildren();
	void Add(QNode* node);
	void Remove(QNode* node);
	QNode* Find(std::string name);
	Matrix CalcWorldMatrix();

	Vector3 GetPosition();
	Vector3 GetWorldPosition();
	void PrintTree(bool info = true, int depth = 0);

	// Set world matrix directly.
	// Note that pos,rot and scale vectors aren't used now.
	void SetWorldMatrix(Matrix world);

	// Set rotation matrix directly.
	// Note that pos,rot and scale vectors aren't used now.
	void SetRotationMatrix(Matrix matrix);

	void SetPosition(float x, float y, float z);

	// Add xyz to current position.
	void AddToPosition(float dx, float dy, float dz);

	// Sets the rotation. Angles in degrees.
	void SetRotation(float x, float y, float z);

	void SetScale(float x, float y, float z);
	void SetScale(float sc);

	void LookAt(Vector3 lookAtTarget);

	// Rotate around axis. Angle in degrees
	void RotateX(float angle);

	// Rotate around axis. Angle in degrees
	void RotateY(float angle);

	// Rotate around axis. Angle in degrees
	void RotateZ(float angle);

	void Forward(float amount);
	void Right(float amount);
	void Up(float amount);

	// If debug, one can setup node's pos. TAB shows infos.
	void SetNodeWithKeyboard()
	{
#if DEBUG
		float spd = deltaTime * 5;
		if (inputGetKeyState(entry::Key::Up)) AddToPosition(0, spd, 0);
		if (inputGetKeyState(entry::Key::Down)) AddToPosition(0, -spd, 0);
		if (inputGetKeyState(entry::Key::Left)) AddToPosition(-spd, 0, 0);
		if (inputGetKeyState(entry::Key::Right)) AddToPosition(spd, 0, 0);
		if (inputGetKeyState(entry::Key::PageUp)) AddToPosition(0, 0, spd);
		if (inputGetKeyState(entry::Key::PageDown)) AddToPosition(0, 0, -spd);
		if (inputGetKeyState(entry::Key::Tab))
		{
			printf("NodePos: %ff, %ff, %ff \t[%s]\n", pos.x, pos.y, pos.z, name.c_str());
		}
#endif
	}
};

#ifdef IMPLEMENTATION
#include "QCamera.h"
#include "QScene.h"

void QNode::Render(QCamera* camera, Environment* env)
{
	//Log(" nothing to render ");
}

std::vector<QNode*> QNode::GetChildren()
{
	return children;
}

void QNode::Add(QNode* node)
{
	node->parent = this;
	children.push_back(node);
}

void QNode::Remove(QNode* node)
{
	Log(" TODO tsekkaa toimiiko. ei testattu!");

	int i = 0;
	for (QNode* n : children)
	{
		if (n->name == name)
		{
			children.erase(children.begin() + i);
			return;
		}
		Remove(node);
		i++;
	}
}

QNode* QNode::Find(std::string name)
{
	for (QNode* n : children)
	{
		if (n->name == name) return n;
		QNode* r = n->Find(name);
		if (r != 0) return r;
	}
	return 0;
}

void QNode::PrintTree(bool info, int depth)
{
	if (info) Log("PrintTree():");
	for (QNode* c : children)
	{
		for (int q = 0; q < depth; q++) printf("  ");
		printf("> %s \n", c->name.c_str());
		if (c->children.size() > 0) c->PrintTree(false, depth + 1);
	}
	if (info) printf("-----------------------\n");
}

Matrix QNode::CalcWorldMatrix()
{
	if (rotDirty)
	{
		rotationMatrix = MatrixRotateXYZ({ -rot.x * (float)DEG2RAD, rot.y * (float)DEG2RAD, rot.z * (float)DEG2RAD });
		rotDirty = false;
	}

	if (dirty)
	{
		Matrix m1 = MatrixMultiply(MatrixScale(scale.x, scale.y, scale.z), rotationMatrix);
		worldMatrix = MatrixMultiply(m1, MatrixTranslate(pos.x, pos.y, pos.z));
		dirty = false;
	}

	if (parent != NULL)
	{
		return MatrixMultiply(worldMatrix, parent->CalcWorldMatrix());
	}
	return worldMatrix;
}

Vector3 QNode::GetPosition()
{
	return pos;
}

Vector3 QNode::GetWorldPosition()
{
	CalcWorldMatrix();
	Vector3 pos = { worldMatrix.m12, worldMatrix.m13, worldMatrix.m14 };
	return pos;
}

// Set world matrix directly.
// NOTE: pos,rot and scale vectors aren't used now.
void QNode::SetWorldMatrix(Matrix world)
{
	worldMatrix = world;
	rotDirty = dirty = false;
}

// Set rotation matrix directly.
// NOTE: pos,rot and scale vectors aren't used now.
void QNode::SetRotationMatrix(Matrix matrix)
{
	rotationMatrix = matrix;
	rotDirty = dirty = false;
}

void QNode::SetPosition(float x, float y, float z)
{
	pos = { x,y,z };
	dirty = true;
}

// Add xyz to current position.
void QNode::AddToPosition(float dx, float dy, float dz)
{
	pos.x += dx;
	pos.y += dy;
	pos.z += dz;
	dirty = true;
}

// Sets the rotation. Angles in degrees.
void QNode::SetRotation(float x, float y, float z)
{
	rot = { x,y,z };
	rotDirty = true;
}

void QNode::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
	dirty = true;
}

void QNode::SetScale(float sc)
{
	scale = { sc, sc, sc };
	dirty = true;
}

void QNode::LookAt(Vector3 lookAtTarget)
{
	rotationMatrix = MatrixLookAt(pos, lookAtTarget, { 0,1,0 });
	rotDirty = dirty = false;
}

// Rotate around axis. Angle in degrees
void QNode::RotateX(float angle)
{
	angle = -angle;
	rot.x += angle;
	rotDirty = dirty = true;
}

// Rotate around axis. Angle in degrees
void QNode::RotateY(float angle)
{
	angle = -angle;
	rot.y += angle;
	rotDirty = dirty = true;
}

// Rotate around axis. Angle in degrees
void QNode::RotateZ(float angle)
{
	rot.z += angle;
	rotDirty = dirty = true;
}

void QNode::Forward(float amount)
{
	float xRotation = -rot.y * DEG2RAD;
	float yRotation = -rot.x * DEG2RAD;
	Vector3 direction =
	{
		cos(yRotation) * sin(xRotation),
		sin(yRotation),
		cos(yRotation) * cos(xRotation),
	};
	direction = Vector3Scale(direction, amount);
	pos = Vector3Add(pos, direction);
	dirty = true;
}

void QNode::Right(float amount)
{
	float xRotation = -rot.y * DEG2RAD;
	float yRotation = -rot.x * DEG2RAD;
	Vector3 right =
	{
		sin(xRotation - DEG2RAD * 90.0f),
		0,
		cos(xRotation - DEG2RAD * 90.0f),
	};
	right = Vector3Scale(right, amount);
	pos = Vector3Add(pos, right);
	dirty = true;
}

void QNode::Up(float amount)
{
	pos.y += amount;
	dirty = true;
}

#endif
