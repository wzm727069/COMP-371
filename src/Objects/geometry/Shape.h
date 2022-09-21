#pragma once
/*
Shape
Depends on: Polygon.h
Description: handles the generation on any shape:
costum:A 3d Shape that forms the basis for amy Shapes  we may want to construct in the future
loaded obj file: using objloader
origin: the point of origin for this particular polygon
vaComponentCount: the number of vertex components in this Shape
vaByteSize: the number Bytes required to contain the vertices of this Shape
*/

#include "Polygon.h"

class VertexComponent;

#include "../../Opengl_a/VertexController.h"

class Shape : public Polygon {

public:
	Shape();
	Shape(glm::vec3 origin_a);
	Shape(glm::vec3 origin_a,
		std::vector<glm::vec3> & in_vertices,
		std::vector<glm::vec2> & in_uvs,
		std::vector<glm::vec3> & in_normals);
	virtual void transform(glm::mat4 transmat);
	void translate_fromOrigin();
	virtual float* getVertexArray();
	virtual int getVAVertexCount();
	virtual int getVAByteSize();
	virtual int getVAFloatCount();
	virtual int getVertexByteSize();
	virtual void setVertexController(bool position, bool texture, bool color, bool normal);
	virtual void setBoolean(bool position, bool texture, bool color, bool normal);
	virtual Vertex getSampleVertex();
	virtual std::map<std::string, glm::vec3> getMinMax();
	virtual void print();




	glm::vec3 origin;
	int vaComponentCount;
	int vaByteSize;

	void setupCube();
	void setUpLoadedObj(
		std::vector<glm::vec3> & in_vertices,
		std::vector<glm::vec2> & in_uvs,
		std::vector<glm::vec3> & in_normals);

	VertexController* vc;
};