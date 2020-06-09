#pragma once
#include "QNode.h"
#include "Frustum.h"

// QCamera class. Dont use QNode's functions, instead use bgfx camera*() functions.
class QCamera : public QNode
{
	float m_view[16];
	float m_proj[16];
	int width, height; // viewport
public:
	Frustum frustum;
	float near = 1, far = 1000;
	float fovY = 45;

	// If debug, show cam pos.
	void Print()
	{
#if DEBUG
		printf("CamPos: %ff, %ff, %ff\n", cameraGetPosition().x, cameraGetPosition().y, cameraGetPosition().z);
#endif
	}

	void Init(int width_, int height_)
	{
		width = width_;
		height = height_;
		cameraCreate();
		cameraSetPosition({ pos.x, pos.y, pos.z });
		//cameraSetVerticalAngle(-0.3f);

		cameraUpdate(0, mouseState);
	}

	void Dispose()
	{
		cameraDestroy();
	}

	void Update()
	{
		cameraGetViewMtx(m_view);
		bx::mtxProj(m_proj, fovY, float(width) / float(height), near, far, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(RENDER3D_VIEW_ID, m_view, m_proj);
		bgfx::setViewRect(RENDER3D_VIEW_ID, 0, 0, uint16_t(width), uint16_t(height)); // Set view 0 default viewport.

		float view[16];
		float proj[16];
		bx::mtxTranspose(view, m_view);
		bx::mtxTranspose(proj, m_proj);
		frustum.CalcFrustum(m_view, m_proj);

		pos = { cameraGetPosition().x, cameraGetPosition().y, cameraGetPosition().z };
	}

};
