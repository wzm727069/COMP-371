
#include "binder.h"
#include <iostream>
#include "../../Opengl_a/vertexComponent.h"
#include "../../Opengl_a/vertex.h"
#include "model.h"

/* USED FOR DEBUGGING - Every OpenGL function call we use should be wrapped in a GLCall()
*
*The following 20 lines of code are not our own!
*
*Source: https://www.youtube.com/watch?v=FBbPWSOQ0-w&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=10
*
*/
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include <GL/glew.h>    

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[openGL error] (" << error << ") " << function <<
			" " << file << ":" << std::endl;
		return false;
	}
	return true;
}

/* The above 20 lines of code are not our own!
*
*Source: https://www.youtube.com/watch?v=FBbPWSOQ0-w&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=10
*
*/

/*
*Default constructor with default boolean values
*/
Binder::Binder()
{
	Binder::position = true;
	Binder::texture = false;
	Binder::color = false;
	Binder::normal = false;
}

/*
*Default constructor specifying boolean values for each 
*@param : position a boolean indicating whether or not to load position vertex components
*@param : color a boolean indicating whether or not to load color vertex components
*@param : texture a boolean indicating whether or not to load texture vertex components
*/
Binder::Binder(bool position, bool texture, bool color, bool normal)
{
	Binder::position = position;
	Binder::texture = texture;
	Binder::color = color;
	Binder::normal = normal;
}



/*
* Description: bind a vertex array for a certain component
*
*@param unbind : boolean indicating whether or not to unbind the vao after creating it 
*@param model : the model for which to create a vao
*/
void Binder::bindArrayBuffer(bool unbind, Model* model)
{

	bool* arr = new bool[4];
	arr[POSITION] = model->position;
	arr[TEXTURE] = model->texture;
	arr[COLOR] = model->color;
	arr[NORMAL] = model->normal;

	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glGenBuffers(1, &vbo));

	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));

	GLCall(glBufferData(GL_ARRAY_BUFFER, model->getVAByteSize(), model->getVertexArray(), GL_STATIC_DRAW));
	int vertexAttribCount = 0;
	int vertexByteOffset = 0;
	Vertex tempVert = model->getSampleVertex();
	for (std::vector<VertexComponent>::iterator it = tempVert.vertexComponents.begin(); it < tempVert.vertexComponents.end(); it++)
	{
		if (arr[(int)it->type]) 
		{
			GLCall(glVertexAttribPointer(vertexAttribCount, it->getFloatCount(), GL_FLOAT, GL_FALSE, model->getVertexByteSize(), (void*)(vertexByteOffset)));
			GLCall(glEnableVertexAttribArray(vertexAttribCount));
			vertexAttribCount += 1;
			vertexByteOffset += it->getByteSize();
		}
	}
	
	if (unbind)
		Binder::unbind();
}


/*
* Description: binds the already created vao
*/
void Binder::bind() 
{
	GLCall(glBindVertexArray(vao));
}

/*
* Description: unbinds the vao
*/
void Binder::unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void Binder::deallocate() 
{
	GLCall(glDeleteVertexArrays(1, &vao));
	GLCall(glDeleteBuffers(1, &vbo));
}