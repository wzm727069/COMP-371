#include "VertexComponent.h"
#include <iostream>
#include <string>
#include <sstream>

/*
*@param tp : the type of Vertex component we wish to create
*@param dat : the floats that define this vertex component
*/
VertexComponent::VertexComponent(Components tp, glm::vec2 dat) 
{
	type = tp;
	data = new float[2];
	data[0] = dat.x;
	data[1] = dat.y;
	floatCount = 2;
}
VertexComponent::VertexComponent(Components tp, glm::vec3 dat)
{
	type = tp;
	data = new float[3];
	data[0] = dat.x;
	data[1] = dat.y;
	data[2] = dat.z;
	floatCount = 3;
}
VertexComponent::VertexComponent(Components tp, glm::vec4 dat)
{
	type = tp;
	data = new float[4];
	data[0] = dat.x;
	data[1] = dat.y;
	data[2] = dat.z;
	data[3] = dat.w;
	floatCount = 4;
}

std::string VertexComponent::toString()
{
	std::ostringstream ss;
		
	for (int i = 0; i < floatCount; i++) 
	{
		if(i == floatCount-1)
			ss << data[i];
		else
			ss << data[i]<<", ";
	}
		
	std::string s(ss.str());
	return comps[type] + " (" + s + ")";
}

float* VertexComponent::getData()
{
	return data;
}

