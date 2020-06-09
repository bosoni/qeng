/*
Area2 is an area where x,y is area's center point.
Can check if areas are colliding.

*/
#pragma once
#include <math.h>

class Area2D
{
	float x, y;
	float half_w, half_h;

public:
	int id = 0; // userdata
	
	Area2D(float x, float y, float width, float height, int id = 0)
	{
		SetArea(x, y, width, height, id);
	}

	void SetArea(float x, float y, float width, float height, int id = 0)
	{
		this->x = x;
		this->y = y;
		half_w = width / 2;
		half_h = height / 2;
		this->id = id;
	}

	bool IsColliding(const Area2D& other)
	{
		float dx = fabs(x - other.x);
		float dy = fabs(y - other.y);
		float minx = half_w + other.half_w;
		float miny = half_h + other.half_h;
		if (dx <= minx && dy <= miny)
		{
			//printf(" %f %f :: %f %f \n", dx, dy, minx, miny);  // DEBUG
			return true;
		}
		return false;
	}

	float Length(const Area2D& other)
	{
		float dx = fabs(x - other.x);
		float dy = fabs(y - other.y);
		return sqrtf(dx * dx + dy * dy);
	}

};
