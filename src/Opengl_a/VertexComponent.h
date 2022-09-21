#pragma once
#include <glm/glm.hpp>
#include <string>


/*
* Components
*
*This enum makes the types of components more readable
*
*/
enum Components
{

	POSITION = 0,
	TEXTURE ,
	COLOR,
	NORMAL
};

//overload == for the above enum
static bool operator==(Components& a, Components& b)  
{
	return (int)a == (int)b;
}


/*
* Vertex Component
*
*An abstraction of one component of an opengl vertex,
*this class may represent a position, color, texture, etc..
*
*@member data : holds the values of this particular vertex component 
*
*
*/
class VertexComponent 
{

	public:

		std::string comps[4] = {"Position","Texture","Color","Normal"};

		Components type;
		float * data;
		int floatCount = 0;

		inline int getFloatCount() { return floatCount; }
		inline int getByteSize() { return floatCount*sizeof(float); }

		std::string toString();
		VertexComponent(Components tp, glm::vec2 dat);
		VertexComponent(Components tp, glm::vec3 dat);
		VertexComponent(Components tp, glm::vec4 dat);
		float* getData();
	
};