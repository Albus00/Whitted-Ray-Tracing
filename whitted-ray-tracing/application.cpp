#include "application.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "glm/ext/scalar_constants.hpp" // glm::pi
#include <vector>

#include <iostream>
#include <fstream>

void saveAsPPM(const std::vector<std::vector<glm::vec3>>& pixelData, int width, int height, const char* filename) {
	std::ofstream img("picture.ppm");
	img << "P3" << std::endl;
	img << 600 << " " << 600 << std::endl;
	img << "255" << std::endl;

	int r = floor(theCamera.thePixels[i * 600 + j].pixelColor.r * 0.07 * 255.0 / 3200.0);
	int g = floor(theCamera.thePixels[i * 600 + j].pixelColor.g * 0.07 * 255.0 / 3200.0);
	int b = floor(theCamera.thePixels[i * 600 + j].pixelColor.b * 0.07 * 255.0 / 3200.0);

	img << r << " " << g << " " << b << std::endl;


	system("explorer picture.ppm");
}

int main() {
	// Create a 600x600 red plane using GLM
	glm::vec3 redColor(1.0f, 0.0f, 0.0f);
	int planeWidth = 600;
	int planeHeight = 600;

	// Create a 2D vector to represent the pixels of the plane
	std::vector<std::vector<glm::vec3>> pixelData(planeHeight, std::vector<glm::vec3>(planeWidth, redColor));

	// Save the pixel data as a PPM image
	saveAsPPM(pixelData, planeWidth, planeHeight, "red_plane.ppm");

	std::cout << "Image saved as 'red_plane.ppm'" << std::endl;

	return 0;
}
