#pragma once
#include "QNode.h"

class QLight : public QNode
{
public:
	bool enabled = false;
	Vector3 color = { 1,1,1 };
	float radius = 8, innerRadius = 1;

	// If debug, one can setup light's pos, radius&innerradius. TAB shows infos.
	void SetLightWithKeyboard()
	{
#if DEBUG
		SetNodeWithKeyboard();

		if (inputGetKeyState(entry::Key::KeyR)) radius -= 0.1f;
		if (inputGetKeyState(entry::Key::KeyT)) radius += 0.1f;
		if (inputGetKeyState(entry::Key::KeyF)) innerRadius -= 0.1f;
		if (inputGetKeyState(entry::Key::KeyG)) innerRadius += 0.1f;
		radius = Clamp(radius, 0, 1000);
		innerRadius = Clamp(innerRadius, 0, 1);

		if (inputGetKeyState(entry::Key::Tab))
			printf(" Radius: %ff  InnerRadius: %ff\n", radius, innerRadius);
#endif
	}

};
