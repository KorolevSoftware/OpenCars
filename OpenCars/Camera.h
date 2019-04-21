#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
protected:
	glm::vec3 up;
	glm::vec3 target;
	glm::vec3 location;

public:
	Camera(const glm::vec3 &location, const glm::vec3 &target, const glm::vec3 &up)
		:up(up), location(location), target(target) {};

	void setUp(const glm::vec3 &up);
	void setTarget(const glm::vec3 &target);
	void setLocation(const glm::vec3 &location);

	glm::vec3 getUp();
	glm::vec3 getTarget();
	glm::vec3 getLocation();

	glm::mat4 getViewMatrix();
	~Camera() {};
};

