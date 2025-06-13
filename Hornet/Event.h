#pragma once
#include "vector2D.h"
enum EventType { NONE, ANIMATED_OBJECT_CREATED };

class GameObject;

struct Event
{
	GameObject* pSource;
	EventType type;
	Vector2D position;
	double data1;
	double data2;
};

