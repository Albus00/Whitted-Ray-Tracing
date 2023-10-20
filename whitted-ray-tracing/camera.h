#pragma once
#include <vector>
#include "colorRGB.h"
#include "glm/glm.hpp"

class camera
{
public:
	camera(int imageWidth, int imageHeight, glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
	glm::mat4 getMatrix() { return cameraMatrix; }

	glm::vec3 cameraPosition;
	std::vector<std::vector<colorRGB>> pixels;
private:
	glm::mat4 cameraMatrix;
};
