#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "vertex.h"
#include <map>
#include <string>



/*
* Vertex Controller
*
*A container of multiple opengl vertices that contains methods to manipulate them 
*
*
*@member vertices : a vector containing a list of multiple vertices 
*@member position : a boolean to determine whether the corresponding vertex component should be accounted for
*@member color : a boolean to determine whether the corresponding vertex component should be accounted for
*@member texture : a boolean to determine whether the corresponding vertex component should be accounted for
*
*/
class VertexController{

public: 
	std::vector<Vertex> vertices;
	bool position;
	bool texture;
	bool color;
	bool normal;


	VertexController(bool position = true, bool texture = false, bool color = false, bool normal = false);
	VertexController(std::vector<Vertex> verts, bool position = true, bool texture = false, bool color = false, bool normal = false);

	void appendVertex(Vertex& ver);
	void appendVertices(std::vector<Vertex> newVertices);

	int getVertexByteSize(Vertex vert);
	int getVertexFloatCount(Vertex vert);
	int getVAFloatCount();
	int getVAByteSize();
	int getVAVertexCount();
	float* getVertex(Vertex vert);
	float* getVertexArray();
	std::map<std::string, glm::vec3> getMinMax();

	void transform(glm::mat4 transmat);
	void transformOne(glm::mat4 transmat, VertexComponent& vc);
	void print();

	

	
	

};