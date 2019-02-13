#pragma once

struct Time
{
	static Time& get()
	{
		static Time time;
		return time;
	}

	double time;
	double deltaTime;
	double elapsed;
	double lag;
};