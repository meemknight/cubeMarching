#pragma once
#include <glm/vec3.hpp>
#include <functional>

auto circleFunctionCreator(float r, float cx =0, float cy=0, float cz=0)
{
	return[r, cx, cy, cz](int x, int y, int z) -> bool
	{
		float x2 = x - cx;
		float y2 = y - cy;
		float z2 = z - cz;
		return sqrtf(x2*x2 + y2 * y2 + z2 * z2) < r;
	};
}

struct World3d
{
	struct Vec3
	{
		int x, y, z;
	}size = { 0 };

	float scale = 1;

	bool *world = nullptr;

	void popultate(std::function<bool(int, int, int)> func);

	bool get(int x, int y, int z);
	void set(int x, int y, int z, bool val);

	void create();
	void create(int x, int y, int z) { size = { x,y,z }; create(); };
	void cleanup();
	void calculateGpuData();
	unsigned int gpuBufferId = 0;
	
	int getTrueCount() { return trueCount; }

	void bind();

private:
	int trueCount = 0;

};