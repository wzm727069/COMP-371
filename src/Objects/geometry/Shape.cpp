//to be Shape

#include<iostream>
#include "Shape.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Opengl_a/VertexComponent.h"

//Default Shape 
Shape::Shape()
{
	vc = new VertexController(true, false, false);
}

//Shape constructor that passes a vec3 as the original position--> CUBE CONSTRUCTOR
Shape::Shape(glm::vec3 origin_a)
{
	setupCube();
	origin = origin_a;
	translate_fromOrigin();
}

//Shape constructor that passes a the parsed info from the obj file --> LOADED OBJ CONSTRUCTOR
Shape::Shape(glm::vec3 origin_a,
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals)
{

	setUpLoadedObj(in_vertices, in_uvs, in_normals);
	origin = origin_a;
	translate_fromOrigin();
}

void Shape::setUpLoadedObj(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals)
{
	vc = new VertexController(true, false, false);
	for (int i = 0; i < in_vertices.size(); i++)
	{
		Vertex * v = new Vertex();
		v->addVertexComponent(VertexComponent(POSITION, in_vertices.at(i)));
		v->addVertexComponent(VertexComponent(TEXTURE, in_uvs.at(i)));
		v->addVertexComponent(VertexComponent(NORMAL, in_normals.at(i)));
		vc->appendVertex(*v);
	}
}

//Method that sets up the vertices that build a single Shape
void Shape::setupCube()
{

	vc = new VertexController(true, false, false);

	// a -> model's back faces
	Vertex a1;
	a1.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, -0.5f)));
	a1.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	a1.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, -1.0f)));

	Vertex a2;
	a2.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, -0.5f)));
	a2.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	a2.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, -1.0f)));

	Vertex a3;
	a3.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, -0.5f)));
	a3.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	a3.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, -1.0f)));

	Vertex a4;
	a4.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, -0.5f)));
	a4.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	a4.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, -1.0f)));

	Vertex a5;
	a5.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, -0.5f)));
	a5.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	a5.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, -1.0f)));

	Vertex a6;
	a6.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, -0.5f)));
	a6.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	a6.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, -1.0f)));

	vc->appendVertex(a3);
	vc->appendVertex(a2);
	vc->appendVertex(a1);
	vc->appendVertex(a6);
	vc->appendVertex(a5);
	vc->appendVertex(a4);


	// l -> model's front faces
	Vertex l1;
	l1.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, 0.5f)));
	l1.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	l1.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, 1.0f)));

	Vertex l2;
	l2.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, 0.5f)));
	l2.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	l2.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, 1.0f)));

	Vertex l3;
	l3.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, 0.5f)));
	l3.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	l3.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, 1.0f)));

	Vertex l4;
	l4.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, 0.5f)));
	l4.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	l4.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, 1.0f)));

	Vertex l5;
	l5.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, 0.5f)));
	l5.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	l5.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, 1.0f)));

	Vertex l6;
	l6.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, 0.5f)));
	l6.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	l6.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 0.0f, 1.0f)));

	vc->appendVertex(l1);
	vc->appendVertex(l2);
	vc->appendVertex(l3);
	vc->appendVertex(l4);
	vc->appendVertex(l5);
	vc->appendVertex(l6);

	// p -> model's right faces
	Vertex p1;
	p1.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, 0.5f)));
	p1.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	p1.addVertexComponent(VertexComponent(NORMAL, glm::vec3(-1.0f, 0.0f, 0.0f)));

	Vertex p2;
	p2.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, -0.5f)));
	p2.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	p2.addVertexComponent(VertexComponent(NORMAL, glm::vec3(-1.0f, 0.0f, 0.0f)));

	Vertex p3;
	p3.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, -0.5f)));
	p3.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	p3.addVertexComponent(VertexComponent(NORMAL, glm::vec3(-1.0f, 0.0f, 0.0f)));

	Vertex p4;
	p4.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, -0.5f)));
	p4.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	p4.addVertexComponent(VertexComponent(NORMAL, glm::vec3(-1.0f, 0.0f, 0.0f)));

	Vertex p5;
	p5.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, 0.5f)));
	p5.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	p5.addVertexComponent(VertexComponent(NORMAL, glm::vec3(-1.0f, 0.0f, 0.0f)));

	Vertex p6;
	p6.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, 0.5f)));
	p6.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	p6.addVertexComponent(VertexComponent(NORMAL, glm::vec3(-1.0f, 0.0f, 0.0f)));

	vc->appendVertex(p1);
	vc->appendVertex(p2);
	vc->appendVertex(p3);
	vc->appendVertex(p4);
	vc->appendVertex(p5);
	vc->appendVertex(p6);


	// o -> model's left faces
	Vertex o1;
	o1.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, 0.5f)));
	o1.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	o1.addVertexComponent(VertexComponent(NORMAL, glm::vec3(1.0f, 0.0f, 0.0f)));

	Vertex o2;
	o2.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, -0.5f)));
	o2.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	o2.addVertexComponent(VertexComponent(NORMAL, glm::vec3(1.0f, 0.0f, 0.0f)));

	Vertex o3;
	o3.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, -0.5f)));
	o3.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	o3.addVertexComponent(VertexComponent(NORMAL, glm::vec3(1.0f, 0.0f, 0.0f)));

	Vertex o4;
	o4.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, -0.5f)));
	o4.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	o4.addVertexComponent(VertexComponent(NORMAL, glm::vec3(1.0f, 0.0f, 0.0f)));

	Vertex o5;
	o5.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, 0.5f)));
	o5.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	o5.addVertexComponent(VertexComponent(NORMAL, glm::vec3(1.0f, 0.0f, 0.0f)));

	Vertex o6;
	o6.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, 0.5f)));
	o6.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	o6.addVertexComponent(VertexComponent(NORMAL, glm::vec3(1.0f, 0.0f, 0.0f)));

	vc->appendVertex(o3);
	vc->appendVertex(o2);
	vc->appendVertex(o1);
	vc->appendVertex(o6);
	vc->appendVertex(o5);
	vc->appendVertex(o4);


	// k -> model's bottom faces
	Vertex k1;
	k1.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, -0.5f)));
	k1.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	k1.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, -1.0f, 0.0f)));

	Vertex k2;
	k2.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, -0.5f)));
	k2.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	k2.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, -1.0f, 0.0f)));

	Vertex k3;
	k3.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, 0.5f)));
	k3.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	k3.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, -1.0f, 0.0f)));

	Vertex k4;
	k4.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, -0.5f, 0.5f)));
	k4.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	k4.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, -1.0f, 0.0f)));

	Vertex k5;
	k5.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, 0.5f)));
	k5.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	k5.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, -1.0f, 0.0f)));

	Vertex k6;
	k6.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, -0.5f, -0.5f)));
	k6.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	k6.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, -1.0f, 0.0f)));

	vc->appendVertex(k1);
	vc->appendVertex(k2);
	vc->appendVertex(k3);
	vc->appendVertex(k4);
	vc->appendVertex(k5);
	vc->appendVertex(k6);

	// u -> model's top faces
	Vertex u1;
	u1.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, -0.5f)));
	u1.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	u1.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 1.0f, 0.0f)));

	Vertex u2;
	u2.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, -0.5f)));
	u2.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 1.0f)));
	u2.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 1.0f, 0.0f)));

	Vertex u3;
	u3.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, 0.5f)));
	u3.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	u3.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 1.0f, 0.0f)));

	Vertex u4;
	u4.addVertexComponent(VertexComponent(POSITION, glm::vec3(0.5f, 0.5f, 0.5f)));
	u4.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(1.0f, 0.0f)));
	u4.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 1.0f, 0.0f)));

	Vertex u5;
	u5.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, 0.5f)));
	u5.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 0.0f)));
	u5.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 1.0f, 0.0f)));

	Vertex u6;
	u6.addVertexComponent(VertexComponent(POSITION, glm::vec3(-0.5f, 0.5f, -0.5f)));
	u6.addVertexComponent(VertexComponent(TEXTURE, glm::vec2(0.0f, 1.0f)));
	u6.addVertexComponent(VertexComponent(NORMAL, glm::vec3(0.0f, 1.0f, 0.0f)));

	vc->appendVertex(u3);
	vc->appendVertex(u2);
	vc->appendVertex(u1);
	vc->appendVertex(u6);
	vc->appendVertex(u5);
	vc->appendVertex(u4);

}

//Method that sets up the position, texture and color components
void Shape::setVertexController(bool position, bool texture, bool color, bool normal)
{
	vc->position = position;
	vc->texture = texture;
	vc->color = color;
	vc->normal = normal;
}

void Shape::setBoolean(bool position, bool texture, bool color, bool normal)
{
	setVertexController(position, texture, color, normal);
}

//Method that returns the vertices of the Shape 
Vertex Shape::getSampleVertex()
{
	return vc->vertices.front();
}

//Method that applies a transformation matrix to the vertices of the Shape
void Shape::transform(glm::mat4 transmat)
{
	vc->transform(transmat);
}

//Method that translates the Shape from origin (used for when moving the model so it is according to the origin of the model)
void Shape::translate_fromOrigin()
{
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0 + origin.x, 0.0 + origin.y, 0.0 + origin.z));
	vc->transform(translation);
}

//Method that returns the vertex array count
int Shape::getVAVertexCount()
{
	return vc->getVAVertexCount();
}

//Method that returns the vertex array float count
int Shape::getVAFloatCount()
{
	return vc->getVAFloatCount();
}

//Method that returns the vertex array byte size
int Shape::getVAByteSize()
{
	return vc->getVAByteSize();
}
//Method that returns the vertex byte size
int Shape::getVertexByteSize()
{

	return vc->getVertexByteSize(vc->vertices.front());
}

//Method that returns the vertex array
float* Shape::getVertexArray()
{
	return vc->getVertexArray();
}

//Method that returns the origin, as a vec3 coordinate, of the combination of the vertices
std::map<std::string, glm::vec3> Shape::getMinMax()
{
	return vc->getMinMax();
}

void Shape::print() 
{
	vc->print();
}