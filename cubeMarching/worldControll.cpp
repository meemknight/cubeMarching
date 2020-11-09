#include "worldControll.h"
#include <cmath>
#include <GL/glew.h>
#include <cstring>

void World3d::popultate(std::function<bool(int, int, int)> func)
{
	int tempCount = 0;
	for(int x=0; x<size.x; x++)
	{
		for(int y=0; y<size.y; y++)
		{
			for(int z=0; z<size.z; z++)
			{				
				bool b = func(x, y, z);
				set(x, y, z, b);
				if (b) 
				{
					tempCount++; 
				}
			}
		}
	}
	trueCount = tempCount;
}

bool World3d::get(int x, int y, int z)
{
	return world[x + (size.x * y) + ((size.x * size.y) * z)];
}

void World3d::set(int x, int y, int z, bool val)
{
	if(world[x + (size.x * y) + ((size.x * size.y) * z)])
	{
		if (val == false)
			trueCount--;
	}else
	{
		if (val == true)
			trueCount++;
	}

	world[x + (size.x * y) + ((size.x * size.y) * z)] = val;
}

void World3d::create()
{
	world = new bool[size.x * size.y * size.z];
	memset(world, 0, size.x * size.y * size.z * sizeof(bool));
}

void World3d::cleanup()
{
	if(world)
	delete[] world;

	if(gpuBufferId)
	{
		glDeleteBuffers(1, &gpuBufferId);
	}
}

void World3d::calculateGpuData()
{
	if(!gpuBufferId)
	{
		glGenBuffers(1, &gpuBufferId);
	}

	glBindBuffer(GL_ARRAY_BUFFER, gpuBufferId);
	float *data = new float[3 * sizeof(float) * trueCount];

	int i = 0;

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			for (int z = 0; z < size.z; z++)
			{
				if(get(x,y,z))
				{
					data[i] = x * scale;
					data[i+1] = y * scale;
					data[i+2] = z * scale;
					i += 3;
				}
			}
		}
	}

	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * trueCount, data, GL_STATIC_DRAW);
	delete[] data;
}

void World3d::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, gpuBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
	glEnableVertexAttribArray(0);
}

