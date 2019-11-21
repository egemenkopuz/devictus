#ifndef AABB_H
#define AABB_H

//#include "intersection.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tuple>
#include <vector>

const glm::vec3 COMPASS_3D[] = {
	glm::vec3(0.f,0.f,1.f),		// forward
	glm::vec3(0.f,0.f,-1.f),	// backward
	glm::vec3(1.f,0.f,0.f),		// right
	glm::vec3(-1.f,0.f,0.f),	// left
	glm::vec3(0.f,1.f,0.f),		// up
	glm::vec3(0.f,-1.f,0.f)		// down
};

enum CollisionDirection {
	C_FORWARD, 
	C_BACKWARD,
	C_RIGHT, 
	C_LEFT,
	C_UP, 
	C_DOWN
};

typedef std::tuple<bool, CollisionDirection, glm::vec3> Collision;

class AABB {
private:
	glm::vec3 minExtent;
	glm::vec3 maxExtent;
public:
	float height;
	float widthX;
	float widthZ;

	glm::vec3 getMinExtent() { return this->minExtent; }
	glm::vec3 getMaxExtent() { return this->maxExtent; }

	void updateAABB(glm::mat4 transformMatrix, glm::vec3 pos)
	{
		std::vector<glm::vec3> corners;

		// can be [i*SizeX+j], lighter
		float** current = new float*[2];
		for (int i = 0; i < 2; ++i)
			current[i] = new float[3];

		current[0][0] = this->minExtent.x;
		current[0][1] = this->minExtent.y;
		current[0][2] = this->minExtent.z;

		current[1][0] = this->maxExtent.x;
		current[1][1] = this->maxExtent.y;
		current[1][2] = this->maxExtent.z;

		glm::vec3 center(0.f, 0.f, 0.f);

		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					center.x += current[x][0];
					center.y += current[y][1];
					center.z += current[z][2];
					
					corners.push_back(glm::vec3(current[x][0], current[y][1], current[z][2]));
				}
			}
		}

		for (int i = 0; i < 2; ++i) 
			delete[] current[i];
		delete[] current;

	/*	center /= 8.f;

		float minX = center.x, minY = center.y, minZ = center.z;
		float maxX = center.x, maxY = center.y, maxZ = center.z;*/

		float minX = pos.x, minY = pos.y, minZ = pos.z;
		float maxX = pos.x, maxY = pos.y, maxZ = pos.z;

		for (auto& iter : corners)
		{

			iter = transformMatrix * glm::vec4(iter,1.f);

			if (iter.x < minX)
				minX = iter.x;
			if (iter.x > maxX)
				maxX = iter.x;
			if (iter.y < minY)
				minY = iter.y;
			if (iter.y > maxY)
				maxY = iter.y;
			if (iter.z < minZ)
				minZ = iter.z;
			if (iter.z > maxZ)
				maxZ = iter.z;
		}

		this->height = pos.y - minY;
		this->widthX = pos.x - minX;
		this->widthZ = pos.z - minZ;

		this->minExtent = glm::vec3(minX, minY, minZ);
		this->maxExtent = glm::vec3(maxX, maxY, maxZ);
	}

	Collision intersectAABB(AABB& other)
	{
		glm::vec3 distance1 = other.getMinExtent() - this->maxExtent;
		glm::vec3 distance2 = this->minExtent - other.getMaxExtent();

		glm::vec3 distance = glm::max(distance1, distance2);

		float maxDistance = glm::compMax(distance);

		//// experimental

		return std::make_tuple(maxDistance < 0, C_UP, distance);

		//float max = 0.f;
		//int best = -1;

		//for (int i = 0; i < 6; i++)
		//{
		//	float dotProduct = glm::dot(glm::normalize(distance), COMPASS_3D[i]);
		//	if (dotProduct > max)
		//	{
		//		max = dotProduct;
		//		best = i;
		//	}
		//}
		//return std::make_tuple(true, (CollisionDirection)best, distance);
	}

	AABB() {}
	AABB(glm::vec3 minEx, glm::vec3 maxEx) : minExtent(minEx), maxExtent(maxEx) {}
};


#endif