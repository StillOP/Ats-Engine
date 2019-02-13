#pragma once

struct Vector2
{
	Vector2(float x, float y)
		: x{ x }, y{ y } 
	{}

	Vector2()
		: x{ 0.0f }, y{ 0.0f }
	{}

	//Vector2 operator +(Vector2 &other) { return Vector2(x + other.x, y + other.y); }

	//Vector2 operator -(Vector2 &other) { return Vector2(x - other.x, y - other.y); }

	/*Vector2& operator +=(Vector2 &other)
	{
		x =  x + other.x;
		y =  y + other.y;

		return *this;
	}*/


	float x, y;
};

struct Rect
{
	Rect(float left, float top, float right, float bottom)
		: left{ left }, top{ top }, right{ right }, bottom{ bottom }
	{}

	Rect()
		: left{ 0.0f }, top{ 0.0f }, right{ 0.0f }, bottom{ 0.0f }
	{}


	float left, top, right, bottom;
};

struct Math
{
	static struct::Vector2 Vector2(float x, float y) { return Vector2::Vector2(x, y); }
	static struct::Vector2 Vector2() { return Vector2::Vector2(); }
};