#pragma once

struct Vector2
{
	Vector2(float x, float y)
		: x{ x }, y{ y } 
	{}

	Vector2()
		: x{ 0.0f }, y{ 0.0f }
	{}
	
	Vector2 operator +(const Vector2& other) const  { return Vector2(x + other.x, y + other.y); }
	
	Vector2 operator -(const Vector2& other) const  { return Vector2(x - other.x, y - other.y); }

	Vector2 operator =(const Vector2& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	Vector2& operator +=(const Vector2& other)
	{
		this->x += other.x;
		this->y += other.y;

		return *this;
	}

	Vector2& operator -=(const Vector2& other)
	{
		x = x + other.x;
		y = y + other.y;

		return *this;
	}

	Vector2 operator *(float value)
	{
		return Vector2(x * value, y * value);
	}
	
	float dot(const Vector2& vector) const
	{
		return (x * vector.x) + (y * vector.y);
	}

	float squaredMagnitude()
	{
		return (x * x) + (y * y);
	}

	Vector2 normal()
	{
		Vector2 normal = { y, -x };

		if (dot(Vector2(-normal.y, normal.x)) > 0) { return normal; }

		return Vector2(-y, x);
	}

	void transform(const Vector2& translation, float rotation = 0.0f, const Vector2& center = Vector2(0.0f, 0.0f))
	{
		float cx = x;
		float cy = y;
		
		x = ((cx - center.x) * cos(rotation)) - ((cy - center.y) * sin(rotation)) + (center.x + translation.x);
		y = ((cx - center.x) * sin(rotation)) + ((cy - center.y) * cos(rotation)) + (center.y + translation.y);
	}

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


	static constexpr double PI = 3.141592653589793238462643383279502884;
};