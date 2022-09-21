#pragma once
#include "Shader.h"
#include <vector>
#include "vertexComponent.h"


/*
*Vertex
*
*Abstracts the concept of an openGL vertex, allowing for
*future additions of components without compromising the API
*
*Yes, its is just a container of vertex components
*
*@member vertexComponents: a vector of vertex components
*
*/
class Vertex
{

public:
	std::vector<VertexComponent> vertexComponents;
	Vertex();
	void addVertexComponent(VertexComponent vc);
	std::string toString();

	
};
