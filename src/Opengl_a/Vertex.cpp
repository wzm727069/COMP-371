#include "Vertex.h"


Vertex::Vertex() {}

void Vertex::addVertexComponent(VertexComponent vc) 
{
	vertexComponents.push_back(vc);
}

std::string Vertex::toString() 
{
	std::string s;
	for (std::vector<VertexComponent>::iterator it = vertexComponents.begin(); it < vertexComponents.end(); it++) {
		s += (*it).toString();
		s += " ";
	}
	return s;
}


