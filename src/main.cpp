/*
	COMP 371 - Section CC
	Practical Assignment #3
	Written By:
		Benjamin Therien (40034572)
		Sean Heinrichs (40075789)
		Wayne Huras (40074423)
		Isabelle Gourchette (40008121)
		Ziming Wang (40041601)
	Due:  August 21st, 2020

sand: 	https://gallery.yopriceville.com/Backgrounds/Background_Beach_Sand#.XzsmF2hKiUk
*/

#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL

#include "utils/AssimpTranslation.h"
#include "Objects/geometry/Polygon.h"
#include "Objects/Grid.hpp"
#include "Objects/Terrain.hpp"
#include "Objects/Camera.h"
#include "Objects/lighting/Light.h"
#include "modeling/OurModels.cpp"
#include "utils/GL_Error.h"
#include "utils/objloader.cpp"
#include "utils/renderHelpers.h"
#include "Opengl_a/Shader.h"
#include "Opengl_a/Material.h"
#include "Common.h"

#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/parse.h>s
#include <irrKlang.h>
#include <future>
#include "utils/SimplexNoise.h"

// for Skybox
#include "utils/stb_image.h"
#include <vector>
#include <stdio.h>

#define GLFW_REFRESH_RATE 60
#define	GLFW_DOUBLEBUFFER GLFW_TRUE

/* Function Declarations */
void processInput(GLFWwindow *window, ModelContainer** models, bool collision, irrklang::ISound * walkingSound);
void cursorPositionCallback(GLFWwindow * window, double xPos, double yPos);
void setupTextureMapping();
void RenderScene(Shader* shader, std::vector<ModelContainer*> models3d);
void RenderGrid(Shader* shader, unsigned int grid_VAOs[], Grid mainGrid);
void ShadowFirstPass(Shader* shader, std::vector<ModelContainer*> models3d, unsigned int grid_VAOs[], Grid mainGrid);
void ShadowSecondPass(Shader* shader, std::vector<ModelContainer*> models3d, unsigned int grid_VAOs[], Grid mainGrid);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int loadCubemap(std::vector<std::string> faces);
void loadSkybox(Shader &skyboxShader);
void drawSkybox(Shader &skyboxShader);
float distanceFromCamera(glm::vec3 cameraPos, AABB aabb);
bool checkCollision(ModelContainer** models);

/* Config Setup */
YAML::Node config = YAML::LoadFile("comp371/assignment1/src/config.yaml");
bool isDay = config["Variables"][0]["value"].as<std::string>().compare("DAY") == 0;

//Variables for terrain
float TERRAIN_SMOOTHNESS = 0.99;
int TERRAIN_BIG_SIZE = 0;
int TERRAIN_SIZE = 10;
int VERTEX_COUNT_TERRAIN = 100;

/* Global Constants */
unsigned int WINDOW_WIDTH = 1024;
unsigned int WINDOW_HEIGHT = 768;
const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;
float SENSITIVITY = 0.9f;

/* Camera Setup */
Camera camera = Camera(glm::vec3(0.0f, 2.0f, -1.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool collision = false;
glm::mat4 model, projection, view;
char prevKey;

// Initialize variables used for rotations
float previousXPos = WINDOW_WIDTH / 2.0f;
float previousYPos = WINDOW_HEIGHT / 2.0f;
float xOffset = 0.0f;
float yOffset = 0.0f;

glm::vec3 cameraJump = glm::vec3(0.0f, 2.0f, 0.0f);
int jumpCounter = 0;

bool firstMouse = true;

// Variables used for Sound
bool isWalking = false;

// Variables used for light and shadows
unsigned int depthMapFBO;
unsigned int depthMap;
float near_plane = 1.0f, far_plane = 10.0f;
glm::mat4 lightSpaceMatrix(1.0f);
glm::mat4 lightProjection(1.0f);
glm::mat4 lightView(1.0f);

// Variables used for Skybox
unsigned int skyboxVAO, skyboxVBO, cubemapTexture_day, cubemapTexture_night;

// Variables used for Fog / Sky Color (Fog/ClearColor)
const float RED = 0.84;
const float BLUE = 0.80;
const float GREEN = 0.7;

//globals used for selecting render mode and models
GLenum MODE = GL_TRIANGLES;
int selected = 0;
int useTextures = 1;
bool useShadows = true;
bool useFlashlight = true;
glm::vec3 activeLightSource = glm::vec3(1.0f, 1.0f, -1.5f);


/* External linkage for global varibles */
GLenum* g_texLocations = new GLenum[32];
Material* g_materials = new Material[32];
Texture* g_textures = new Texture[32];
float *g_shininess = new float[32];
glm::vec3 *g_specularStrength = new glm::vec3[32];

int main(void)
{
	//Reading config file for terrain configurations
	YAML::Node config = YAML::LoadFile("comp371/assignment1/src/config.yaml");

	for (std::size_t i = 0; i < config["Variables"].size(); i++) {

		if (config["Variables"][i]["name"].as<std::string>().compare("terrainSize") == 0) {
			if (config["Variables"][i]["value"].as<std::string>().compare("BIG") == 0) {
				TERRAIN_BIG_SIZE = 0;
			}
			else if (config["Variables"][i]["value"].as<std::string>().compare("NORMAL") == 0) {
				TERRAIN_BIG_SIZE = 1;
			}
			else if (config["Variables"][i]["value"].as<std::string>().compare("SMALL") == 0) {
				TERRAIN_BIG_SIZE = 2;
			}
		}
		else if (config["Variables"][i]["name"].as<std::string>().compare("terrainTopography") == 0) {
			if (config["Variables"][i]["value"].as<std::string>().compare("SMOOTH") == 0) {
				TERRAIN_SMOOTHNESS = 0.99;
			}
			else if (config["Variables"][i]["value"].as<std::string>().compare("HILLS") == 0) {
				TERRAIN_SMOOTHNESS = 0.95;
			}
			else if (config["Variables"][i]["value"].as<std::string>().compare("REALSMOOTH") == 0) {
				TERRAIN_SMOOTHNESS = 0.999999;
			}
		}
	}

	time_t startTime = time(new time_t());
	std::cout << "cpp version: " << __cplusplus << std::endl;
	

	/* Initialize GLFW */
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	GLFWwindow* window;

	// Create a window and its OpenGL context 
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "COMP 371 - Assignment 1", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLEW 
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//enable blending for correct texture rendering effects
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Enable depth test for 3D rendering 
	GLCall(glEnable(GL_DEPTH_TEST));

	//Enable Culling
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glCullFace(GL_BACK));
	GLCall(glFrontFace(GL_CCW));

	// Build and Compile Shader Program 
	Shader modelShader("comp371/assignment1/src/Shaders/assimp.modelShader.vertex", "comp371/assignment1/src/Shaders/assimp.modelShader.fragment");
	Shader lightShader("comp371/assignment1/src/Shaders/lightShader.vertex", "comp371/assignment1/src/Shaders/lightShader.fragment");
	Shader depthShader("comp371/assignment1/src/Shaders/shadow_mapping_depth.vertex", "comp371/assignment1/src/Shaders/shadow_mapping_depth.fragment");
	Shader skyboxShader("comp371/assignment1/src/Shaders/skyboxShader.vertex", "comp371/assignment1/src/Shaders/skyboxShader.fragment");
	setupTextureMapping();

	// [Models]


	std::vector<ModelContainer*> models3d;

	// [Terrain]
	time_t terrainBefore = time(new time_t());
	ModelContainer* terrainC = new ModelContainer();
	Terrain * t = new Terrain(VERTEX_COUNT_TERRAIN, TERRAIN_SIZE, TERRAIN_SMOOTHNESS);
	Shape * loadedShape = new Shape(glm::vec3(0.0f, 0.0f, 0.0f), t->vertices, t->textureCoords, t->normals);
	Model* terrain = new Model(true, true, false, true, "terrain",&modelShader, &g_materials[0]);
	terrain->addPolygon(loadedShape);
	terrain->bindArrayBuffer(true, terrain);
	terrainC->addModel(terrain);
	//terrainC->addScale(glm::vec3(3.0f, 3.0f, 3.0f));
	//terrainC->addTranslation(glm::vec3(0.0f - 5, 0.1f, 0.0f - 5));
	models3d.push_back(terrainC);

	
	std::cout << "it took : " << difftime(time(new time_t), terrainBefore) << " seconds to generate terrain" << std::endl;
	
	ModelContainer* rock = loadModel("../Assets/Models/rock0/rock0.obj", false);
	rock->optimizeModels();
	rock->setVertexController(true, true, false, true);
	rock->bindArrayBuffer();
	rock->addRotationZ(90);
	rock->addRotationX(90);

	ModelContainer* bush = loadModel("../Assets/Models/bush/bush1.obj", false);
	bush->optimizeModels();
	bush->setVertexController(true, true, false, true);
	bush->bindArrayBuffer();
	
	ModelContainer* palmtree = loadModel("../Assets/Models/palmtree/palmtree.obj", false);
	palmtree->optimizeModels();
	palmtree->setVertexController(true, true, false, true);
	palmtree->bindArrayBuffer();
	

	
	ModelContainer* accarrier = loadModel("../Assets/Models/accarrier/accarrier.obj", true);
	accarrier->optimizeModels();
	accarrier->setVertexController(true, true, false, true);
	accarrier->bindArrayBuffer();
	accarrier->addTranslation(glm::vec3(0.0f, -0.5f, -25.0f));
	accarrier->addScale(glm::vec3(0.01f, 0.01f, 0.01f));
	accarrier->addRotationY(90);
	models3d.push_back(accarrier);
	
	float sizeModel = 0.55f;
	if (TERRAIN_BIG_SIZE == 0) {
		sizeModel = 0.55f;
	}
	else if (TERRAIN_BIG_SIZE == 1) {
		sizeModel = 0.25f;
	}
	else if (TERRAIN_BIG_SIZE == 2) {
		sizeModel = 0.15f;
	}

	//ModelContainer* ben = new ModelContainer();
	ModelContainer* ben = loadModel("../Assets/Models/stoneN3/stoneN3.obj", false);
	//createBensModel(ben, &modelShader);
	ben->bindArrayBuffer();
	models3d.push_back(ben);

	ModelContainer* sean = new ModelContainer();
	createSeansModel(sean, &modelShader);
	sean->bindArrayBuffer();
	float terrainHeightSean = t->getHeightOfTerrain(sean->translate_vec.x, sean->translate_vec.z, terrain);
	sean->addScale(glm::vec3(sizeModel, sizeModel, sizeModel));
	sean->addTranslation(glm::vec3(-5.0f, (terrainHeightSean + 0.25f), 0.0f));
	models3d.push_back(sean);

	ModelContainer* isa = new ModelContainer();
	createIsabellesModel(isa, &modelShader);
	isa->bindArrayBuffer();
	float terrainHeightIsa = t->getHeightOfTerrain(isa->translate_vec.x, isa->translate_vec.z, terrain);
	isa->addScale(glm::vec3(sizeModel, sizeModel, sizeModel));
	isa->addTranslation(glm::vec3(3.5f, (terrainHeightIsa + 0.75f), 4.0f));
	models3d.push_back(isa);

	ModelContainer* ziming = new ModelContainer();
	createZimingsModel(ziming, &modelShader);
	ziming->bindArrayBuffer();
	float terrainHeightZiming = t->getHeightOfTerrain(ziming->translate_vec.x, ziming->translate_vec.z, terrain);
	ziming->addScale(glm::vec3(sizeModel, sizeModel, sizeModel));
	ziming->addTranslation(glm::vec3(-4.0f, (terrainHeightZiming + 0.75f), 4.0f));
	models3d.push_back(ziming);

	ModelContainer* wayne = new ModelContainer();
	createWaynesModel(wayne, &modelShader);
	wayne->bindArrayBuffer();
	//float terrainHeightWayne = t->getHeightOfTerrain(wayne->translate_vec.x, wayne->translate_vec.z, terrain);
	wayne->addScale(glm::vec3(sizeModel, sizeModel, sizeModel));
	
	models3d.push_back(wayne);

	float factor = 0;
	int offset = 3;
	if (TERRAIN_BIG_SIZE == 0) {
		terrainC->addScale(glm::vec3(6.0f, 6.0f, 6.0f));
		terrainC->addTranslation(glm::vec3(-10.0f, 0.0f, -10.0f));

		palmtree->addTranslation(glm::vec3(-10.0f, 0.0f, -10.0f));
		palmtree->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		bush->addTranslation(glm::vec3(-10.0f, 0.2f, -10.0f));
		bush->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		rock->addTranslation(glm::vec3(-10.0f, 0.3f, -10.0f));
		rock->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		ben->addScale(glm::vec3(4.0, 4.0, 4.0));
		ben->addTranslation(glm::vec3(12.0f, t->getHeightOfTerrain(12, 12, terrain), 12.0f));

		wayne->addTranslation(glm::vec3(20.0f, 0.5, 10.0f));
		wayne->addRotation(90, glm::vec3(1.0f, 0.0f, 0.0f));

		offset = 4;
		factor = 2;
		t->scale = 6.0f;
		sizeModel = 0.55f;
	}
	else if (TERRAIN_BIG_SIZE == 1) {
		terrainC->addScale(glm::vec3(3.0, 3.0, 3.0));
		terrainC->addTranslation(glm::vec3(-5.0f, 0.1f, -5.0f));

		palmtree->addTranslation(glm::vec3(-6.0f, 0.0f, -6.0f));
		palmtree->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		bush->addTranslation(glm::vec3(-5.0f, 0.2f, -5.0f));
		bush->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		rock->addTranslation(glm::vec3(-5.0f, 0.2f, -5.0f));
		rock->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		ben->addScale(glm::vec3(4.0, 4.0, 4.0));
		ben->addTranslation(glm::vec3(12.0f, t->getHeightOfTerrain(12, 12, terrain) , 12.0f));

		offset = 3;
		factor = 1;
		t->scale = 3.0f;
		sizeModel = 0.25f;
	}
	else if (TERRAIN_BIG_SIZE == 2) {
		terrainC->addScale(glm::vec3(1.0f, 1.0f, 1.0f));
		terrainC->addTranslation(glm::vec3(-5.0 / 3, 0.1f, -5.0 / 3));

		terrainC->addScale(glm::vec3(3.0, 3.0, 3.0));
		terrainC->addTranslation(glm::vec3(-5.0f, 0.1f, -5.0f));

		palmtree->addTranslation(glm::vec3(-10.0f, 0.0f, -10.0f));
		palmtree->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		bush->addTranslation(glm::vec3(-5.0f, 0.2f, -5.0f));
		bush->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		rock->addTranslation(glm::vec3(-5.0f, 0.2f, -5.0f));
		rock->addScale(glm::vec3(0.1f, 0.1f, 0.1f));

		ben->addScale(glm::vec3(10.0, 10.0, 10.0));
		ben->addTranslation(glm::vec3(12.0f, t->getHeightOfTerrain(12, 12, terrain), 12.0f));

		offset = 4;
		factor = 1;
		t->scale = 3.0f;
		sizeModel = 0.15f;
	}


	std::vector<glm::mat4> rocks;
	std::vector<glm::mat4> trees;
	std::vector<glm::mat4> bushes;

	rock->calculateMinMax();
	bush->calculateMinMax();
	palmtree->calculateMinMax();
	terrainC->calculateMinMax();
	std::cout << "terrain : " << terrainC->aabb.max.x << std::endl;
	std::cout << "max x : " << terrainC->aabb.max.x << std::endl;
	std::cout << "max z : " << terrainC->aabb.max.z << std::endl;

	
	srand((unsigned)time(0));

	for (int i = terrainC->aabb.min.x; i < terrainC->aabb.max.x; i += 1 + factor)
		for (int j = terrainC->aabb.min.z; j < terrainC->aabb.max.z; j += 1 + factor) {
			if ((i * 10 + j) % 2 == 0) {
				float x1 = i + (((float)rand() - (float)RAND_MAX / 2.0) / ((float)RAND_MAX / 2.0)) * 5;
				float z1 = j + (((float)rand() - (float)RAND_MAX / 2.0) / ((float)RAND_MAX / 2.0)) * 5;
				if (x1 > terrainC->aabb.min.x + offset && x1 < terrainC->aabb.max.x - offset && z1 > terrainC->aabb.min.z + offset && z1 < terrainC->aabb.max.z - offset)
					rocks.push_back(rock->getTranslatedModelMatrix(glm::vec3(x1, t->getHeightOfTerrain(x1, z1, terrain), z1)));
			}
			float x2 = i + (((float)rand() - (float)RAND_MAX / 2.0) / ((float)RAND_MAX / 2.0)) * 5;
			float z2 = j + (((float)rand() - (float)RAND_MAX / 2.0) / ((float)RAND_MAX / 2.0)) * 5;
			if (x2 > terrainC->aabb.min.x + offset && x2 < terrainC->aabb.max.x - offset && z2 > terrainC->aabb.min.z + offset && z2 < terrainC->aabb.max.z - offset)
				bushes.push_back(bush->getTranslatedModelMatrix(glm::vec3(x2, t->getHeightOfTerrain(x2, z2, terrain), z2)));
			if ((i * 10 + j) % 3 == 0) {
				float x3 = i + (((float)rand() - (float)RAND_MAX / 2.0) / ((float)RAND_MAX / 2.0)) * 5;
				float z3 = j + (((float)rand() - (float)RAND_MAX / 2.0) / ((float)RAND_MAX / 2.0)) * 5;
				if (x3 > terrainC->aabb.min.x + offset && x3 < terrainC->aabb.max.x - offset && z3 > terrainC->aabb.min.z + offset && z3 < terrainC->aabb.max.z - offset)
					trees.push_back(palmtree->getTranslatedModelMatrix(glm::vec3(x3, t->getHeightOfTerrain(x3, z3, terrain), z3)));
			}
		}


	// [Lighting]

	Light* dayTimeDirectionalLight = new Light(glm::vec3(0.0, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.3f, 0.3f, 0.3f)
	);
	Light* dayTimeSpotLight = new Light(glm::vec3(35.0f, 5.0f, -30.0f), false);
	Light* nightTimeDirectionalLight = new Light(glm::vec3(0.0f, 0.0f, 0.0f),     
		glm::vec3(0.07f, 0.07f, 0.07f),			
		glm::vec3(0.4f, 0.4f, 0.4f),				     
		glm::vec3(0.3f, 0.3f, 0.3f)				
	); 
	Light* nightTimeSpotLight = new Light(camera.position,						 
		camera.front,				
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.09f,						 
		0.032,						
		true						
	);


	// [Grid]

	Grid mainGrid = Grid();

	unsigned int grid_VAOs[3], grid_VBOs[3], grid_EBO;
	GLCall(glGenVertexArrays(3, grid_VAOs));
	GLCall(glGenBuffers(3, grid_VBOs));
	GLCall(glGenBuffers(1, &grid_EBO));

	// Grid Floor 
	GLCall(glBindVertexArray(grid_VAOs[1]));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, grid_VBOs[1]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(mainGrid.floorVertices), mainGrid.floorVertices, GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));  // Position
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));  // Texture
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));  // Normals

	ModelContainer** models = new ModelContainer*[6];
	models[0] = ben;
	models[1] = sean;
	models[2] = isa;
	models[3] = ziming;
	models[4] = wayne;


	// Skybox load
	loadSkybox(skyboxShader);

	// [Configure Depth Map FBO]

	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	modelShader.use();
	modelShader.setInt("shadowMap", 11);	// Must be unused texture slot

	// Fog
	modelShader.setVec3("skyColor", RED, BLUE, GREEN);

	bindTextures();

	std::cout << "it took " << difftime(time(new time_t), startTime) << " seconds to reach rendering" << std::endl;

	

	/* Sound */
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
	irrklang::ISound* daySound = SoundEngine->play2D("comp371/assignment1/src/Resources/sound/guitar-oriental.mp3", true, false, true);
	irrklang::ISound* windSound = SoundEngine->play2D("comp371/assignment1/src/Resources/sound/wind.wav", true, false, true);
	irrklang::ISound* walkingSound = SoundEngine->play2D("comp371/assignment1/src/Resources/sound/footsteps.mp3", true, false, true);
	irrklang::ISound* nightSound = SoundEngine->play2D("comp371/assignment1/src/Resources/sound/night_sounds.mp3", true, false, true);
	daySound->setVolume(0.3f);

	// Main Loop 
	while (!glfwWindowShouldClose(window))
	{
		// Sound
		if (isWalking) {
			if (walkingSound->getIsPaused()) {
				walkingSound->setIsPaused(false);
			}
		}
		else if (!isWalking) {
			if (!walkingSound->getIsPaused()) {
				walkingSound->setIsPaused(true);
			}
		}
		if (isDay) {
			if (daySound->getIsPaused()) {
				daySound->setIsPaused(false);
			}
			if (!nightSound->getIsPaused()) {
				nightSound->setIsPaused(true);
			}
		}
		else if (!isDay) {
			if (!daySound->getIsPaused()) {
				daySound->setIsPaused(true);
			}
			if (nightSound->getIsPaused()) {
				nightSound->setIsPaused(false);
			}
		}

		// Set frame for Camera (taken from LearnOpenGL)
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check Collision
		collision = false;
		collision = checkCollision(models);

		// Set camera y value
		float terrainHeight;
		terrainHeight = t->getHeightOfTerrain(camera.position.x, camera.position.z, terrain);
		camera.position.y = terrainHeight + 1.0f;

		// Event Handling
		processInput(window, models, collision, walkingSound);

		// Render
		GLCall(glClearColor(RED, BLUE, GREEN, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Start Using Model Shader
		modelShader.use();
		modelShader.setInt("useTextures", useTextures);
		modelShader.setBool("useShadows", useShadows);
		modelShader.setBool("useFlashlight", useFlashlight);
		modelShader.setVec3("viewPos", camera.position);

		// Set Light Properties
		isDay ? dayTimeDirectionalLight->setShaderValues(&modelShader, false) : nightTimeDirectionalLight->setShaderValues(&modelShader, false);
		isDay ? dayTimeSpotLight->setShaderValues(&modelShader, true) : nightTimeSpotLight->setFlashLightShaderValues(&modelShader, &camera);

		// Recompute Camera Pipeline
		projection = glm::perspective(glm::radians(camera.fieldOfViewAngle), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		view = camera.calculateViewMatrix();

		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		modelShader.setMat4("model", model);

		// Render Scene with shadowmap to calculate shadows with depthShader (1ST PASS)
		ShadowFirstPass(&depthShader, models3d, grid_VAOs, mainGrid);

		// Reset Viewport
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Scene as normal using the generated depth/shadowmap with modelShader(2ND PASS)
		ShadowSecondPass(&modelShader, models3d, grid_VAOs, mainGrid);


		for(std::vector<glm::mat4>::iterator it = rocks.begin(); it < rocks.end(); it++)
		{
			rock->drawMod(MODE, &modelShader,*it);
		}
		for (std::vector<glm::mat4>::iterator it = trees.begin(); it < trees.end(); it++)
		{
			palmtree->drawMod(MODE, &modelShader, *it);
		}
		for (std::vector<glm::mat4>::iterator it = bushes.begin(); it < bushes.end(); it++)
		{
			bush->drawMod(MODE, &modelShader, *it);
		}


		// [Objects Not Affected by Light Source Go Below]

		// Start Using Lighting Shader
		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);

		// Draw Skybox as last item
		drawSkybox(skyboxShader);

		// Swap Buffers and Poll for Events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate resources 
	ben->deallocate();
	sean->deallocate();
	wayne->deallocate();
	isa->deallocate();
	ziming->deallocate();
	terrain->deallocate();
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

	// Terminate Program 
	glfwTerminate();
	return 0;
}

// Event handling functions
void processInput(GLFWwindow *window, ModelContainer** models, bool collision, irrklang::ISound* walkingSound)
{
	float cameraSpeed = 1.0 * deltaTime;

	// [Render Mode]

	// Press "SHIFT + P" to change rendering to POINTS
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		MODE = GL_POINTS;
	}

	// Press "SHIFT + L" to change rendering to LINES
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		MODE = GL_LINES;
	}

	// Press "SHIFT + T" to change rendering to TRIANGLES
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		MODE = GL_TRIANGLES;
	}

	// [Model Selection]

	// Press "W" to move FORWARD
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (!collision || (collision && prevKey != 'W')))
	{
		prevKey = 'W';
		camera.moveForward(cameraSpeed);
		walkingSound->setPlaybackSpeed(1.0f);
	}

	// Press "S" to move BACKWARD
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (!collision || (collision && prevKey != 'S')))
	{
		prevKey == 'S';
		camera.moveBackward(cameraSpeed);
		walkingSound->setPlaybackSpeed(1.0f);
	}

	// Press "A" to move LEFT
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (!collision || (collision && prevKey != 'A')))
	{
		prevKey = 'A';
		camera.moveLeft(cameraSpeed);
		walkingSound->setPlaybackSpeed(1.0f);
	}

	// Press "D" to move RIGHT
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (!collision || (collision && prevKey != 'D')))
	{
		prevKey = 'D';
		camera.moveRight(cameraSpeed);
		walkingSound->setPlaybackSpeed(1.0f);
	}

	//Press "SPACE" to JUMP
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.position += cameraJump * cameraSpeed;
		jumpCounter++;
	}

	else if (jumpCounter > 0)
	{
		camera.position -= cameraJump * cameraSpeed;
		jumpCounter--;
	}

	// Press "SHIFT + W" to move FORWARD faster
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		camera.moveForward(cameraSpeed * 6 * 1.5);
		walkingSound->setPlaybackSpeed(1.5f);
	}

	// Press "SHIFT + S" to move BACKWARD faster
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
			camera.moveBackward(cameraSpeed * 1.5);
			walkingSound->setPlaybackSpeed(1.5f);
		
	}

	// Press "SHIFT + D" to move RIGHT faster
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		camera.moveRight(cameraSpeed * 1.5);
		walkingSound->setPlaybackSpeed(1.5f);
	}

	// Press "SHIFT + A" to move LEFT faster
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		camera.moveLeft(cameraSpeed * 1.5);
		walkingSound->setPlaybackSpeed(1.5f);
	}

	// [Rotation]

	// Press 'SHIFT + A' to rotate the model to the left 5 degrees about y-axis
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		models[selected]->addRotation(5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// Press 'SHIFT + D' to rotate the model to the right 5 degrees about y-axis
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		models[selected]->addRotation(-5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}


	//TRANSLATE

	// Press "SHIFT + A" to move the selected model to the LEFT
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
			models[selected]->addTranslation(glm::vec3(0.8f, 0.0f, 0.0f));
		else
			models[selected]->addTranslation(glm::vec3(0.1f, 0.0f, 0.0f));
	}

	// Press "SHIFT + D" to move the selected model to the RIGHT
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
			models[selected]->addTranslation(glm::vec3(-0.8f, 0.0f, 0.0f));
		else
			models[selected]->addTranslation(glm::vec3(-0.1f, 0.0f, 0.0f));
	}

	// Press "SHIFT + W" to move the selected model UP
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
			models[selected]->addTranslation(glm::vec3(0.0f, 0.8f, 0.0f));
		else
			models[selected]->addTranslation(glm::vec3(0.0f, 0.1f, 0.0f));
	}

	// Press "SHIFT + S" to move the selected model DOWN
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
			models[selected]->addTranslation(glm::vec3(0.0f, -0.8f, 0.0f));
		else
			models[selected]->addTranslation(glm::vec3(0.0f, -0.1f, 0.0f));
	}

	// Press "SHIFT + Q" to move the selected model DOWN
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
			models[selected]->addTranslation(glm::vec3(0.0f, 0.0f, -0.8f));
		else
			models[selected]->addTranslation(glm::vec3(0.0f, 0.0f, -0.1f));
	}

	// Press "SHIFT + E" to move the selected model DOWN
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
			models[selected]->addTranslation(glm::vec3(0.0f, 0.0f, 0.8f));
		else
			models[selected]->addTranslation(glm::vec3(0.0f, 0.0f, 0.1f));

	}

	// [Scale]

	// Press 'U' to scale UP the model
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		models[selected]->addScale(glm::vec3(0.1f, 0.1f, 0.1f));
	}

	// Press 'J' to scale DOWN the model
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		models[selected]->addScale(glm::vec3(-0.0001f, -0.0001f, -0.0001f));
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		models[selected]->resetShear();
	}

	// [Texture Toggle]

	// Press 'X' to turn textures OFF
	if ((useTextures != 1) && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		useTextures = 1;
	}

	// Press 'SHIFT + X' to turn textures ON
	if ((useTextures != 0) && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		useTextures = 0;
	}

	// [Shadow Toggle]

	// Press 'M' to turn shadows OFF
	if (!useShadows && glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		useShadows = true;
	}

	// Press 'SHIFT + M' to turn shadows ON
	if (useShadows && glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		useShadows = false;
	}

	// [Flashlight Toggle]

	// Press 'F' to turn the flashlight ON
	if (!useFlashlight && !isDay && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		useFlashlight = true;
	}

	// Press 'SHIFT + F' to turn the flashlight OFF
	if (useFlashlight && !isDay && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
	{
		useFlashlight = false;
	}

	// Set isWalking to determine if footstep sounds should play
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		isWalking = true;
	}

	// Set isWalking to determine if footstep sounds should play
	if (!glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&
		!glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
		!glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS &&
		!glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		isWalking = false;
	}

	// Press Z to set the scene to DAY
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)) 
	{
		isDay = true;
		useShadows = true;
		useFlashlight = true;	// Need to activate spot lighting in shader
	}

	// Press SHIFT + Z to set the scene to NIGHT
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)) 
	{
		isDay = false;
		useShadows = false;
	}
}

void cursorPositionCallback(GLFWwindow * window, double xPos, double yPos)
{
	if (firstMouse)
	{
		previousXPos = xPos;
		previousYPos = yPos;
		firstMouse = false;
	}

	float xoffset = (xPos - previousXPos) * SENSITIVITY;
	float yoffset = (previousYPos - yPos) * SENSITIVITY;

	previousXPos = xPos;
	previousYPos = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
//map textures to a global data structure
void setupTextureMapping()
{
	g_texLocations[0] = GL_TEXTURE0;
	g_texLocations[1] = GL_TEXTURE1;
	g_texLocations[2] = GL_TEXTURE2;
	g_texLocations[3] = GL_TEXTURE3;
	g_texLocations[4] = GL_TEXTURE4;
	g_texLocations[5] = GL_TEXTURE5;
	g_texLocations[6] = GL_TEXTURE6;
	g_texLocations[7] = GL_TEXTURE7;
	g_texLocations[8] = GL_TEXTURE8;
	g_texLocations[9] = GL_TEXTURE9;
	g_texLocations[10] = GL_TEXTURE10;
	g_texLocations[11] = GL_TEXTURE11; //used by shadow map
	g_texLocations[12] = GL_TEXTURE12; // used by skybox
	g_texLocations[13] = GL_TEXTURE13;
	g_texLocations[14] = GL_TEXTURE14;
	g_texLocations[15] = GL_TEXTURE15;
	g_texLocations[16] = GL_TEXTURE16; // used by skybox
	g_texLocations[17] = GL_TEXTURE17;
	g_texLocations[18] = GL_TEXTURE18;
	g_texLocations[19] = GL_TEXTURE19;
	g_texLocations[20] = GL_TEXTURE20;
	g_texLocations[21] = GL_TEXTURE21;
	g_texLocations[22] = GL_TEXTURE22;
	g_texLocations[23] = GL_TEXTURE23;
	g_texLocations[24] = GL_TEXTURE24;
	g_texLocations[25] = GL_TEXTURE25;
	g_texLocations[26] = GL_TEXTURE26;
	g_texLocations[27] = GL_TEXTURE27;
	g_texLocations[28] = GL_TEXTURE28;
	g_texLocations[29] = GL_TEXTURE29;
	g_texLocations[30] = GL_TEXTURE30;
	g_texLocations[31] = GL_TEXTURE31;


	g_textures[0] = Texture("comp371/assignment1/src/Resources/sand.jpg");
	g_textures[1] = Texture("comp371/assignment1/src/Resources/cast_iron.png");
	g_textures[2] = Texture("comp371/assignment1/src/Resources/chrome.png");
	g_textures[3] = Texture("comp371/assignment1/src/Resources/speaker_holes.png");
	g_textures[4] = Texture("comp371/assignment1/src/Resources/shiny_metal.png");
	g_textures[5] = Texture("comp371/assignment1/src/Resources/box1.png");
	g_textures[6] = Texture("comp371/assignment1/src/Resources/box2.png");
	g_textures[7] = Texture("comp371/assignment1/src/Resources/box3.png");
	g_textures[8] = Texture("comp371/assignment1/src/Resources/box4.png");
	g_textures[9] = Texture("comp371/assignment1/src/Resources/box5.png");
	g_textures[10] = Texture("comp371/assignment1/src/Resources/water.jpg");
	//g_textures[11] // used by shadow map
	//g_textures[12] // used by skybox
	//g_textures[13] = Texture("C:\\Users\\Benjamin Therien\\Documents\\comp371\\models\\backpack\\diffuse.jpg");
	g_textures[14] = Texture("comp371/assignment1/src/Resources/rustedmetal.jpg");
	g_textures[15] = Texture("comp371/assignment1/src/Resources/oldwood.jpg");
	//g_textures[16] // used by skybox
	g_textures[17] = Texture("comp371/assignment1/src/Resources/63_rusty_dirty_metal.jpg");
	g_textures[18] = Texture("comp371/assignment1/src/Resources/94_rock_stone.jpg"); 
	g_textures[19] = Texture("comp371/assignment1/src/Resources/blocs.jpg"); 
	g_textures[20] = Texture("comp371/assignment1/src/Resources/blocs2.jpg"); 

	g_shininess[0] = 32.0f;
	g_shininess[1] = 2.0f;
	g_shininess[2] = 2.0f;
	g_shininess[3] = 2.0f;
	g_shininess[4] = 2.0f;
	g_shininess[5] = 256.0f;
	g_shininess[6] = 256.0f;
	g_shininess[7] = 256.0f;
	g_shininess[8] = 256.0f;
	g_shininess[9] = 256.0f;
	g_shininess[10] = 64.0f;
	g_shininess[13] = 64.0f;
	//g_shininess[11] = 64.0f;// used by shadow map
	g_shininess[12] = 64.0f; // used by skybox
	g_shininess[14] = 128.0f;
	g_shininess[15] = 256.0f;
	g_shininess[16] = 64.0f; // used by skybox
	g_shininess[17] = 2.0f;
	g_shininess[18] = 2.0f;
	g_shininess[19] = 2.0f;
	g_shininess[20] = 2.0f;

	g_specularStrength[0] = glm::vec3(0.5f, 0.5f, 0.5f);
	g_specularStrength[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	g_specularStrength[2] = glm::vec3(1.0f, 1.0f, 1.0f);
	g_specularStrength[3] = glm::vec3(1.0f, 1.0f, 1.0f);
	g_specularStrength[4] = glm::vec3(1.0f, 1.0f, 1.0f);
	g_specularStrength[5] = glm::vec3(0.1f, 0.1f, 0.1f);
	g_specularStrength[6] = glm::vec3(0.1f, 0.1f, 0.1f);
	g_specularStrength[7] = glm::vec3(0.1f, 0.1f, 0.1f);
	g_specularStrength[8] = glm::vec3(0.1f, 0.1f, 0.1f);
	g_specularStrength[9] = glm::vec3(0.1f, 0.1f, 0.1f);
	g_specularStrength[10] = glm::vec3(0.5f, 0.5f, 0.5f);
	//g_specularStrength[11] = glm::vec3(0.5f, 0.5f, 0.5f);    used by shadow map
	g_specularStrength[12] = glm::vec3(0.5f, 0.5f, 0.5f);   // used by skybox
	g_specularStrength[13] = glm::vec3(0.5f, 0.5f, 0.5f);
	g_specularStrength[14] = glm::vec3(0.5f, 0.5f, 0.5f);
	g_specularStrength[15] = glm::vec3(0.3f, 0.3f, 0.3f);
	g_specularStrength[16] = glm::vec3(0.5f, 0.5f, 0.5f);   // used by skybox
	g_specularStrength[17] = glm::vec3(1.0f, 1.0f, 1.0f);
	g_specularStrength[18] = glm::vec3(1.0f, 1.0f, 1.0f);
	g_specularStrength[19] = glm::vec3(1.0f, 1.0f, 1.0f);
	g_specularStrength[20] = glm::vec3(1.0f, 1.0f, 1.0f);

	g_materials[0] = Material(g_specularStrength[0], g_textures[0], g_shininess[0]);
	g_materials[1] = Material(g_specularStrength[1], g_textures[1], g_shininess[1]);
	g_materials[2] = Material(g_specularStrength[2], g_textures[2], g_shininess[2]);
	g_materials[3] = Material(g_specularStrength[3], g_textures[3], g_shininess[3]);
	g_materials[4] = Material(g_specularStrength[4], g_textures[4], g_shininess[4]);
	g_materials[5] = Material(g_specularStrength[5], g_textures[5], g_shininess[5]);
	g_materials[6] = Material(g_specularStrength[6], g_textures[6], g_shininess[6]);
	g_materials[7] = Material(g_specularStrength[7], g_textures[7], g_shininess[7]);
	g_materials[8] = Material(g_specularStrength[8], g_textures[8], g_shininess[8]);
	g_materials[9] = Material(g_specularStrength[9], g_textures[9], g_shininess[9]);
	g_materials[10] = Material(g_specularStrength[10], g_textures[10], g_shininess[10]);
	//g_materials[11] = Material(g_specularStrength[11], g_textures[11], g_shininess[11]);
	g_materials[12] = Material(g_specularStrength[12], g_textures[12], g_shininess[12]);
	g_materials[14] = Material(g_specularStrength[14], g_textures[14], g_shininess[14]);
	g_materials[15] = Material(g_specularStrength[15], g_textures[15], g_shininess[15]);
	g_materials[16] = Material(g_specularStrength[16], g_textures[16], g_shininess[16]);
	g_materials[17] = Material(g_specularStrength[17], g_textures[17], g_shininess[17]);
	g_materials[18] = Material(g_specularStrength[18], g_textures[18], g_shininess[18]);
	g_materials[19] = Material(g_specularStrength[19], g_textures[19], g_shininess[19]);
	g_materials[20] = Material(g_specularStrength[20], g_textures[20], g_shininess[20]);
}

void RenderScene(Shader* shader, std::vector<ModelContainer*> models3d)
{
	bindTextures();
	shader->use();
	for (std::vector<ModelContainer*>::iterator it = models3d.begin(); it < models3d.end(); it++)
		(*it)->draw(MODE, shader);

}

void RenderGrid(Shader* shader, unsigned int grid_VAOs[], Grid mainGrid)
{
	// [Grid Floor]

	GLCall(glBindVertexArray(grid_VAOs[1]));
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(335.0f, 335.0f, 35.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0005f));
	g_textures[10].bind(g_texLocations[10]);
	//shader->setFloat("loaded", 1);
	//shader->setFloat("material.shininess", g_shininess[10]);
	//shader->setVec3("material.specular", g_specularStrength[10]);
	//shader->setInt("material.diffuse", 10);
	g_materials[10].setShader(shader);
	shader->setMat4("model", model);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void ShadowFirstPass(Shader* shader, std::vector<ModelContainer*> models3d, unsigned int grid_VAOs[], Grid mainGrid)
{
	// Render Depth of Scene to Texture (from the light's perspective)
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(activeLightSource, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	// Start Using Depth Shader
	shader->use();
	shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Rendering Models and Grid with the DepthShader
	RenderScene(shader, models3d);
	RenderGrid(shader, grid_VAOs, mainGrid);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowSecondPass(Shader* shader, std::vector<ModelContainer*> models3d, unsigned int grid_VAOs[], Grid mainGrid)
{
	// Render Scene as Normal using the Generated Depth/Shadow map  
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the Light Uniforms
	shader->use();
	shader->setVec3("viewPos", camera.position);
	shader->setVec3("lightPos", activeLightSource);
	shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	// Rendering Models and Grid with modelShader
	RenderScene(shader, models3d);
	RenderGrid(shader, grid_VAOs, mainGrid);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, depthMap);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void loadSkybox(Shader &skyboxShader)
{
	// skybox START // Credit - https://learnopengl.com/Advanced-OpenGL/Cubemaps
	float skyboxVertices[] = {
		// positions          
		-35.7f,  35.7f, -35.7f,
		-35.7f, -35.7f, -35.7f,
		 35.7f, -35.7f, -35.7f,
		 35.7f, -35.7f, -35.7f,
		 35.7f,  35.7f, -35.7f,
		-35.7f,  35.7f, -35.7f,

		-35.7f, -35.7f,  35.7f,
		-35.7f, -35.7f, -35.7f,
		-35.7f,  35.7f, -35.7f,
		-35.7f,  35.7f, -35.7f,
		-35.7f,  35.7f,  35.7f,
		-35.7f, -35.7f,  35.7f,

		 35.7f, -35.7f, -35.7f,
		 35.7f, -35.7f,  35.7f,
		 35.7f,  35.7f,  35.7f,
		 35.7f,  35.7f,  35.7f,
		 35.7f,  35.7f, -35.7f,
		 35.7f, -35.7f, -35.7f,

		-35.7f, -35.7f,  35.7f,
		-35.7f,  35.7f,  35.7f,
		 35.7f,  35.7f,  35.7f,
		 35.7f,  35.7f,  35.7f,
		 35.7f, -35.7f,  35.7f,
		-35.7f, -35.7f,  35.7f,

		-35.7f,  35.7f, -35.7f,
		 35.7f,  35.7f, -35.7f,
		 35.7f,  35.7f,  35.7f,
		 35.7f,  35.7f,  35.7f,
		-35.7f,  35.7f,  35.7f,
		-35.7f,  35.7f, -35.7f,

		-35.7f, -35.7f, -35.7f,
		-35.7f, -35.7f,  35.7f,
		 35.7f, -35.7f, -35.7f,
		 35.7f, -35.7f, -35.7f,
		-35.7f, -35.7f,  35.7f,
		 35.7f, -35.7f,  35.7f
	};
	// skybox VAO

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	stbi_set_flip_vertically_on_load(false);

	std::vector<std::string> dayFaces
	{
		"comp371/assignment1/src/Resources/skybox/desert_1/right.png",
		"comp371/assignment1/src/Resources/skybox/desert_1/left.png",
		"comp371/assignment1/src/Resources/skybox/desert_1/top.png",
		"comp371/assignment1/src/Resources/skybox/desert_1/bottom.png",
		"comp371/assignment1/src/Resources/skybox/desert_1/front.png",
		"comp371/assignment1/src/Resources/skybox/desert_1/back.png"
	};

	std::vector<std::string> nightFaces
	{
		"comp371/assignment1/src/Resources/skybox/night_sky/right.png",
		"comp371/assignment1/src/Resources/skybox/night_sky/left.png",
		"comp371/assignment1/src/Resources/skybox/night_sky/top.png",
		"comp371/assignment1/src/Resources/skybox/night_sky/bottom.png",
		"comp371/assignment1/src/Resources/skybox/night_sky/front.png",
		"comp371/assignment1/src/Resources/skybox/night_sky/back.png"
	};


	cubemapTexture_day = loadCubemap(dayFaces);
	cubemapTexture_night = loadCubemap(nightFaces);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// skybox END
}

void drawSkybox(Shader &skyboxShader)
{
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.calculateViewMatrix())); // remove translation from the view matrix
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	
	if (isDay) {
		skyboxShader.use();
		skyboxShader.setInt("skybox", 12);
		glActiveTexture(GL_TEXTURE12);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_day);
	}
	else {
		skyboxShader.use();
		skyboxShader.setInt("skybox", 16);
		glActiveTexture(GL_TEXTURE16);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_night);
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

float distanceFromCamera(glm::vec3 cameraPos, AABB aabb)
{
	float sqDist = 0.0f;
	float cameraPositions[3] = { cameraPos.x, cameraPos.y, cameraPos.z };

	for (int i = 0; i < 3; i++)
	{
		// For each axis, count any excess distance outside box extents
		float cameraVector = cameraPositions[i];

		if (cameraVector < aabb.min[i])
		{
			sqDist += (aabb.min[i] - cameraVector) * (aabb.min[i] - cameraVector);
		}
		else if (cameraVector > aabb.max[i])
		{
			sqDist += (cameraVector - aabb.max[i]) * (cameraVector - aabb.max[i]);
		}
	}

	return glm::sqrt(sqDist);
}

// TODO: If performance is taking a hit after all additional models are added. This method should only calculate collision for models close to the camera
// TODO: Check asimp imports
bool checkCollision(ModelContainer** models)
{
	bool collisionDetected = false;

	// Check our models (letters and numbers)
	for (int i = 0; i < 5; i++)
	{

		
		// Check if Number has collision
		collisionDetected ? true : collisionDetected = distanceFromCamera(camera.position, models[i]->models[0]->getAABB()) < 0.2f ? true : false;

		// Check if Letter has collision
		if(models[i]->models.size() > 1)
			collisionDetected ? true : collisionDetected = distanceFromCamera(camera.position, models[i]->models[1]->getAABB()) < 0.2f ? true : false;
	}

	return collisionDetected;
}