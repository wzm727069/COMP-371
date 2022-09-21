#ifndef GRID_H
#define GRID_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	// Constructors & Destructors
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);
	~Camera();

	// Functions
	void panCamera(float yaw);
	void tiltCamera(float pitch);
	void zoomCamera(float zoom);

	void moveForward(float speed);
	void moveBackward(float speed);
	void moveLeft(float speed);
	void moveRight(float speed);

	void ProcessMouseMovement(float xoffset, float yoffset);

	glm::mat4 calculateViewMatrix();

	float fieldOfViewAngle;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

private:
	// Transformation Variables
	float yawAngle;
	float pitchAngle;
	
	void updateCamera();
};


#endif
