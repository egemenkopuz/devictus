#ifndef AABB_H
#define AABB_H

//#include "intersection.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tuple>
#include <vector>

//const unsigned int AABB_INDICES[] = {
//	//front
//	0,1,2,
//	2,3,0,
//	// right
//	1,5,6,
//	5,2,1,
//	//back
//	7,6,5,
//	5,4,7,
//	//left
//	4,0,3,
//	3,7,4,
//	//bottom
//	4,5,1,
//	1,0,4,
//	//top
//	3,2,6,
//	6,7,3
//};
//
//const float AABB_VERTICES[] = {
//	// front
//	-0.5,-0.5,0.5,
//	0.5,-0.5,0.5,
//	0.5,0.5,0.5,
//	-0.5,0.5,0.5,
//	// back
//	-0.5,-0.5,-0.5,
//	0.5,-0.5,-0.5,
//	0.5,0.5,-0.5,
//	-0.5,0.5,-0.5
//};

const glm::vec3 COMPASS_3D[] = {
	glm::vec3(-1.f,  0.f,  0.f), // 'left' face normal (-x direction)
	glm::vec3(1.f,  0.f,  0.f), // 'right' face normal (+x direction)
	glm::vec3(0.f, -1.f,  0.f), // 'bottom' face normal (-y direction)
	glm::vec3(0.f,  1.f,  0.f), // 'top' face normal (+y direction)
	glm::vec3(0.f,  0.f, -1.f), // 'far' face normal (-z direction)
	glm::vec3(0.f,  0.f,  1.f), // 'close' face normal (+z direction)
};

enum CollisionDirection {
	C_LEFT,
	C_RIGHT,
	C_BOTTOM,
	C_TOP,
	C_FAR,
	C_CLOSE
};

typedef std::tuple<bool, CollisionDirection, glm::vec3> Collision;

class AABB {
private:

	glm::vec3 oMinExtent;
	glm::vec3 oMaxExtent;

	glm::vec3 minExtent;
	glm::vec3 maxExtent;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> indices;

	bool collided = false;

	unsigned int VBO, VAO, EBO;
public:
	float height;
	float widthX;
	float widthZ;

	bool isCollided() { return collided; }

	glm::vec3 getMinExtent() { return this->minExtent; }
	glm::vec3 getMaxExtent() { return this->maxExtent; }
	std::vector<glm::vec3> getVertices() { return this->vertices; }

	void updateAABB(glm::mat4 transformMatrix, glm::vec3 pos)
	{
		// can be [i*SizeX+j], lighter
		float** current = new float*[2];
		for (int i = 0; i < 2; ++i)
			current[i] = new float[3];

		current[0][0] = this->oMinExtent.x;
		current[0][1] = this->oMinExtent.y;
		current[0][2] = this->oMinExtent.z;

		current[1][0] = this->oMaxExtent.x;
		current[1][1] = this->oMaxExtent.y;
		current[1][2] = this->oMaxExtent.z;

		//glm::vec3 center(0.f, 0.f, 0.f);

		vertices.clear();

		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					/*center.x += current[x][0];
					center.y += current[y][1];
					center.z += current[z][2];*/

					vertices.push_back(glm::vec3(current[x][0], current[y][1], current[z][2]));
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

		//float minX, minY, minZ, maxX, maxY, maxZ;
		//bool firstCheck = true;

		for (auto& iter : vertices)
		{

			iter = transformMatrix * glm::vec4(iter, 1.f);

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

		this->height = abs(pos.y - minY);
		this->widthX = abs(pos.x - minX);
		this->widthZ = abs(pos.z - minZ);

		this->minExtent = glm::vec3(minX, minY, minZ);
		this->maxExtent = glm::vec3(maxX, maxY, maxZ);
	}

	Collision intersectAABB(AABB& other)
	{
		CollisionDirection dir;
		collided = false;
		other.collided = false;

		//glm::vec3 distance1 = other.getMinExtent() - this->maxExtent;
		//glm::vec3 distance2 = this->minExtent - other.getMaxExtent();

		//glm::vec3 distance = glm::max(distance1, distance2);

		//float maxDistance = glm::compMax(distance);

		glm::vec3 maxa = this->maxExtent;
		glm::vec3 maxb = other.maxExtent;
		glm::vec3 mina = this->minExtent;
		glm::vec3 minb = other.minExtent;


		float distances[6] =
		{
			(maxb.x - mina.x), // distance of box 'b' to face on 'left' side of 'a'.
			(maxa.x - minb.x), // distance of box 'b' to face on 'right' side of 'a'.
			(maxb.y - mina.y), // distance of box 'b' to face on 'bottom' side of 'a'.
			(maxa.y - minb.y), // distance of box 'b' to face on 'top' side of 'a'.
			(maxb.z - mina.z), // distance of box 'b' to face on 'far' side of 'a'.
			(maxa.z - minb.z), // distance of box 'b' to face on 'close' side of 'a'.
		};

		glm::vec3 distance;
		collided = true;
		unsigned int best = 0;
		float dcoll = NULL;

		for (auto& iter : distances)
		{
			if (iter < 0.0f)
			{
				collided = false;
				break;
			}
		}

		if (collided)
		{
			for (int i = 0; i < 6; i++)
			{

				// face of least intersection depth. That's our candidate.
				if ((dcoll == NULL) || (distances[i] < dcoll))
				{
					best = i;
					distance = COMPASS_3D[i];
					dcoll = distances[i];
				}
			}
		}
		other.collided = collided;
		return std::make_tuple(collided, (CollisionDirection)best, distance);

		//// experimental

		/*collided = maxDistance < 0;
		other.collided = collided;

		if (collided)
		{
			int best = -1;
			float max = 0.f;

			for (int i = 0; i < 6; i++)
			{
				float dot_product = glm::dot(glm::normalize(distance), COMPASS_3D[i]);
				if (dot_product > max)
				{
					max = dot_product;
					best = i;
				}
			}

			intersectedFaces[best] = true;
			return std::make_tuple(collided, (CollisionDirection)best, distance);
		}*/

		dir = C_TOP;
		return std::make_tuple(collided, dir, distance);


		// finding direction

		//float distances[6] =
		//{
		//	(other.getMaxExtent().x - minExtent.x), // distance of box 'b' to face on 'left' side of 'this'.
		//	(maxExtent.x - other.getMinExtent().x), // distance of box 'b' to face on 'right' side of 'this'.
		//	(other.getMaxExtent().y - minExtent.y), // distance of box 'b' to face on 'bottom' side of 'this'.
		//	(maxExtent.y - other.getMinExtent().y), // distance of box 'b' to face on 'top' side of 'this'.
		//	(other.getMaxExtent().z - minExtent.z), // distance of box 'b' to face on 'far' side of 'this'.
		//	(maxExtent.z - other.getMinExtent().z), // distance of box 'b' to face on 'close' side of 'this'.
		//};
		//
		//for (int i = 0; i < 6; i++)
		//{
		//	// box does not intersect face. So boxes don't intersect at all.
		//	if (distances[i] < 0.0f)
		//		collided = false;

		//	// face of least intersection depth. That's our candidate.
		//	if ((i == 0) || (distances[i] < dcoll))
		//	{
		//		fcoll = i;
		//		ncoll = faces;
		//		dcoll = distances;
		//	}
		//}



	}

	AABB() { setup(); }

	~AABB()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	AABB(glm::vec3 minEx, glm::vec3 maxEx) : minExtent(minEx), maxExtent(maxEx), oMaxExtent(maxEx), oMinExtent(minEx) { setup(); }

	void setup()
	{
		vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));

		vertices.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));

		//indices.push_back();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float) * 3, &vertices[0], GL_STATIC_DRAW);

		GLushort cube_elements[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
*/
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


		glBindVertexArray(0);
	}

	void draw(Shader shader)
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 12);
	}
};


#endif