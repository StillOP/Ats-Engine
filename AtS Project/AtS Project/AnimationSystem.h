#pragma once

class AnimationSystem
{
public:
	~AnimationSystem();

	static AnimationSystem& get()
	{
		static AnimationSystem animSys;
		return animSys;
	}

	void Init();
	void Update(double elapsed);

private:
	AnimationSystem();
};