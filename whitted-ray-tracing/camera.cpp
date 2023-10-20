#include "camera.h"
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective

camera::camera(int imageWidth, int imageHeight, glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) : cameraPosition(cameraPosition) {
	pixels = std::vector<std::vector<colorRGB>>(imageHeight, std::vector<colorRGB>(imageWidth));

	// Calculate the view matrix
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

	// Calculate the projection matrix
	float constexpr fov = glm::radians(60.0f);  // Field of view in degrees
	float aspectRatio = static_cast<float>(imageWidth) / static_cast<float>(imageHeight);
	glm::mat4 projectionMatrix = glm::perspective(fov, aspectRatio, 0.1f, 10.0f);

	// Combine the view and projection matrices to get the camera matrix
	cameraMatrix = projectionMatrix * viewMatrix;
}
