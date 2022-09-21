#include "../Objects/geometry/Shape.h"
#include "../Objects/geometry/Model.h"
#include "../Objects/geometry/ModelContainer.h"
#include "../Objects/geometry/Shape.h"
#include "../Opengl_a/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "../Common.h"

#include "../utils/SimplexNoise.h"


//this method has for arguments the extracted data from the objloader and creates a model with this data
static void createShape(Model * model,
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals) {
	//create shape with extracted vertices
	Shape * loadedShape = new Shape(glm::vec3(0.0f, 0.0f, 0.0f), in_vertices, in_uvs, in_normals);
	model->addPolygon(loadedShape);
}

/* Static methods to create our Models */
static void createSeansModel(ModelContainer* modelContainer, Shader* shader)
{
	glm::mat4 scale;
	glm::mat4 rotate;
	glm::mat4 translate;
	
	// [Create A Model]
	Model* A = new Model(true, true, false, true, "a", shader, &g_materials[15]);
	Shape* cb1 = new Shape(glm::vec3(-1.0, 0.5, 0.0));
	Shape* cb2 = new Shape(glm::vec3(-2.0, 2.5, 0.0));
	Shape* cb3 = new Shape(glm::vec3(-2.0, 4.5, 0.0));
	Shape* cb4 = new Shape(glm::vec3(-3.0, 0.5, 0.0));

	// Create 1x4 columns
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	cb1->transform(scale);
	cb4->transform(scale);

	A->addPolygon(cb1);
	A->addPolygon(cb2);
	A->addPolygon(cb3);
	A->addPolygon(cb4);
	
	// [Create 7 Model]
	Model* seven = new Model(true, true, false, true, "7", shader, &g_materials[14]);

	Shape * cb5 = new Shape(glm::vec3(1.0f, 2.0f, 0.0f));
	Shape * cb6 = new Shape(glm::vec3(2.0f, 4.5f, 0.0f));
	Shape * cb7 = new Shape(glm::vec3(3.0f, 0.5f, 0.0f));

	// Create 1x4 column
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	cb7->transform(scale);

	// Create 1x2 column
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	cb5->transform(scale);

	seven->addPolygon(cb5);
	seven->addPolygon(cb6);
	seven->addPolygon(cb7);

	// Set initial position
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	A->transform(rotate);
	seven->transform(translate);
	seven->transform(rotate);
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	seven->transform(rotate);
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	A->transform(rotate);

	// Add to model container
	modelContainer->addModel(A);
	modelContainer->addModel(seven);
}

static void createWaynesModel(ModelContainer* modelContainer, Shader* shader)
{
	// [Create Y Model]
	Model* Y = new Model(true, true, false, true, "y", shader, &g_materials[17]);
  
  // Y model
	Shape *cb1 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, 0.5f, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
	glm::mat4 transform = translate * rotate * scale;
	cb1->transform(transform);

	Shape *cb2 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, 1.5f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
	transform = translate * rotate * scale;
	cb2->transform(transform);

	Shape *cb3 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(-2.85f, 3.5f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(65.0f), glm::vec3(0.0f, 0.0f, 1.0));
	transform = translate * rotate * scale;
	cb3->transform(transform);

	Shape *cb4 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(-4.15f, 3.5f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-65.0f), glm::vec3(0.0f, 0.0f, 1.0));
	transform = translate * rotate * scale;
	cb4->transform(transform);

	Shape *cb5 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(-4.75f, 5.0f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
	transform = translate * rotate * scale;
	cb5->transform(transform);

	Shape *cb6 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(-2.25f, 5.0f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
	transform = translate * rotate * scale;
	cb6->transform(transform);

	Y->addPolygon(cb1);
	Y->addPolygon(cb2);
	Y->addPolygon(cb3);
	Y->addPolygon(cb4);
	Y->addPolygon(cb5);
	Y->addPolygon(cb6);

	// [Create 7 Model]
	Model* seven = new Model(true, true, false, true, "7", shader, &g_materials[18]);
  
	// 7 model
	Shape *cb7 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.90f, 2.25f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.5f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
	glm::mat4 skew(1.0f);
	skew = {
		1.0, 0.0, 0.0, 0.0,
		0.46630765, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	transform = skew * translate * rotate * scale;
	cb7->transform(transform);

	Shape *cb8 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 5.0f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
	transform = translate * rotate * scale;
	cb8->transform(transform);

	Shape *cb9 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 4.5f, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0));
	transform = translate * rotate * scale;
	cb9->transform(transform);

	// 7 model
	seven->addPolygon(cb7);
	seven->addPolygon(cb8);
	seven->addPolygon(cb9);

	// Set initial position
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-75.0f), glm::vec3(1.0f, 1.0f, 1.0));;
	Y->transform(translate * rotate);
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 1.0f, 1.0));;
	seven->transform(translate * rotate);

	// Add to model container
	modelContainer->addModel(Y);
	modelContainer->addModel(seven);
}

static void createBensModel(ModelContainer* modelContainer, Shader* shader)
{



	Model* three = new Model(true, true, false, true, "3", shader, &g_materials[2]);

	three->addPolygon(new Shape(glm::vec3(6.0f, 3.5f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(6.5f, 3.5f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(7.0f, 3.5f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(6.0f, 2.0f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(6.5f, 2.0f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(7.0f, 2.0f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(6.0f, 0.5f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(6.5f, 0.5f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(7.0f, 0.5f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(7.5f, 1.0f, 0.0f)));
	three->addPolygon(new Shape(glm::vec3(7.5f, 3.0f, 0.0f)));
	three->transform(glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f)));

	Shape *cb = new Shape(glm::vec3(9.125f, 0.5f, 0.0f));
	Shape *cb1 = new Shape(glm::vec3(6.825f, 0.5f, 0.0f));
	Shape *cb2 = new Shape(glm::vec3(1.0f, 0.0f, 0.0f));

	
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(7.2f, 1.5f, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0));
	glm::mat4 transform = translate * rotate * scale;

	cb->transform(scale);
	cb1->transform(scale);
	cb2->transform(transform);
	Model* N = new Model(true, true, false, true, "n", shader, &g_materials[7]);
	N->addPolygon(cb);
	N->addPolygon(cb1);
	N->addPolygon(cb2);
	N->transform(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f)));

	modelContainer->addModel(N);
	modelContainer->addModel(three);
}

static void createIsabellesModel(ModelContainer* modelContainer, Shader* shader)
{
	glm::mat4 scale;
	glm::mat4 rotate;
	glm::mat4 translate;

	// [Create A Model]
	Model* A = new Model(true, true, false, true, "aII", shader, &g_materials[19]);
	//A
	Shape * Shape1 = new Shape(glm::vec3(1.0f, 0.5f, 0.0f));
	Shape * Shape2 = new Shape(glm::vec3(2.0f, 2.5f, 0.0f));
	Shape * Shape3 = new Shape(glm::vec3(3.0f, 0.5f, 0.0f));
	Shape * Shape4 = new Shape(glm::vec3(2.0f, 4.5f, 0.0f));

	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 5.0f, 1.0f));
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.5, 0.0f));
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Shape1->transform(scale);
	Shape3->transform(scale);
	A->addPolygon(Shape1);
	A->addPolygon(Shape2);
	A->addPolygon(Shape3);
	A->addPolygon(Shape4);
	A->transform(rotate);
	A->transform(translate);

	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	A->transform(rotate);
	// [Create 0 Model]
	Model* zero = new Model(true, true, false, true, "0II", shader, &g_materials[20]);

	//0
	Shape * Shape5 = new Shape(glm::vec3(-1.0f, 0.5f, 8.50f));
	Shape * Shape6 = new Shape(glm::vec3(-2.0f, 0.5f, 8.50f));
	Shape * Shape7 = new Shape(glm::vec3(-3.0f, 0.5f, 8.50f));
	Shape * Shape8 = new Shape(glm::vec3(-2.0f, 4.5f, 8.50f));

	Shape5->transform(scale);
	Shape7->transform(scale);
	zero->addPolygon(Shape5);
	zero->addPolygon(Shape6);
	zero->addPolygon(Shape7);
	zero->addPolygon(Shape8);
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	zero->transform(rotate);
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	zero->transform(rotate);
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	zero->transform(rotate);
	translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -7.50f, 0.0f));
	zero->transform(translate);

	// Add to model container
	modelContainer->addModel(A);
	modelContainer->addModel(zero);
}

static void createZimingsModel(ModelContainer* modelContainer, Shader* shader)
{

	glm::mat4 scale;
	glm::mat4 rotate;
	glm::mat4 translate;
	// [Create M Model]
	Model* M = new Model(true, true, false, true, "m", shader, &g_materials[9]);

	Shape * cb1 = new Shape(glm::vec3(-5.0f, 0.5f, 0.0f));
	Shape * cb2 = new Shape(glm::vec3(-4.0f, 3.5f, 0.0f));
	Shape * cb3 = new Shape(glm::vec3(-3.0f, 0.5f, 0.0f));
	Shape * cb4 = new Shape(glm::vec3(-2.0f, 3.5f, 0.0f));
	Shape * cb5 = new Shape(glm::vec3(-1.0f, 0.5f, 0.0f));

	// Create 1x4 columns
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	cb1->transform(scale);
	cb3->transform(scale);
	cb5->transform(scale);

	M->addPolygon(cb1);
	M->addPolygon(cb2);
	M->addPolygon(cb3);
	M->addPolygon(cb4);
	M->addPolygon(cb5);

	// [Create 4 Model]
	Model* four = new Model(true, true, false, true, "4", shader, &g_materials[4]);

	Shape * cb6 = new Shape(glm::vec3(1.0f, 0.83f, 0.0f)); 
	Shape * cb7 = new Shape(glm::vec3(2.0f, 1.5f, 0.0f));
	Shape * cb8 = new Shape(glm::vec3(3.0f, 0.5f, 0.0f)); 
	Shape * cb9 = new Shape(glm::vec3(4.0f, 1.5f, 0.0f));

	// Create 1x3 columns
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
	cb6->transform(scale);

	// Create 1x4 columns
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	cb8->transform(scale);

	four->addPolygon(cb6);
	four->addPolygon(cb7);
	four->addPolygon(cb8);
	four->addPolygon(cb9);

	// Set initial position
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	M->transform(rotate);

	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	four->transform(rotate);
	rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	M->transform(rotate);

	// Add to model container
	modelContainer->addModel(M);
	modelContainer->addModel(four);
}

static void createLightModel(Model * model) {
	Shape * cb1 = new Shape(glm::vec3(0.0f, 0.0f, 0.0f));

	model->addPolygon(cb1);
}