#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "glm/glm.hpp"

class Grid
{
public:
	/* Attributes */
	std::vector<glm::vec3> meshVertices;

	float floorVertices[48] = {
		// Positions        Textures    Normals        
		 0.5f,  0.5f, 0.0f, 50.0f, 50.0f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, -1.0f,
											  	
		 0.5f, -0.5f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, -1.0f
	};

	float axisVertices[36] = {
		// Positions       // Colors
		0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.5f,  0.0f, 0.0f, 1.0f,
	};

	/* Constructor */
	Grid()
	{
		// Add all the vertical lines
		for (int i = -100; i <= 100; i += 2)
		{
			meshVertices.push_back(glm::vec3(float(i) / 100, 10.0f, 0.0f));
			meshVertices.push_back(glm::vec3(float(i) / 100, -15.0f, 10.0f));
		}

		// Add all the horizontal lines
		for (int i = 100; i >= -100; i -= 2)
		{
			meshVertices.push_back(glm::vec3(-1.0f, float(i) / 100, 0.0f));
			meshVertices.push_back(glm::vec3(1.0f, float(i) / 100, 0.0f));
		}
	}

	~Grid() {};
};

#endif