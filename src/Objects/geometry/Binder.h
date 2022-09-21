#pragma once

class Model;


/*
*Binder
*
*Description: An interface that provides the necessary methods to bind vertices for a model object
*
*/
class Binder
{

public: 
	Binder(bool position, bool texture, bool color, bool normal = false);
	void bindArrayBuffer(bool unbind, Model* model);
	void deallocate();
	Binder();
	void bind();
	void unbind();

	unsigned int vao, vbo;
	bool position, texture, color, normal;

};