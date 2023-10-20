#include "application.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "glm/ext/scalar_constants.hpp" // glm::pi
#include <vector>
#include "ray.h"

#include <iostream>
#include <fstream>
#include "camera.h"

    struct HitInfo {
        bool hit;
        float t;
        glm::vec3 hitPoint;
        glm::vec3 normal;
    };

    colorRGB ray_color(const ray& r) {
        glm::vec3 unit_direction = glm::normalize(r.direction());
        auto a = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - a) * colorRGB(1.0, 1.0, 1.0) + a * colorRGB(0.5, 0.7, 1.0);
    }

    // The intersection algorithm using the "slab method"
    bool intersectBox(const ray & ray, const glm::vec3 & min, const glm::vec3 & max, HitInfo & info) {
        // Initialize variables for the minimum and maximum intersection distances along the ray
        float tXMin, tXMax, tYMin, tYMax, tZMin, tZMax;

        tXMin = (min.x - ray.origin().x) / ray.direction().x;
        tXMax = (max.x - ray.origin().x) / ray.direction().x;

        // Swap tXMin and tXMax if necessary to ensure tXMin <= tXMax
        if (tXMin > tXMax) {
            std::swap(tXMin, tXMax);
        }

        // Calculate intersection distances along the Y-axis
        tYMin = (min.y - ray.origin().y) / ray.direction().y;
        tYMax = (max.y - ray.origin().y) / ray.direction().y;

        // Swap tYMin and tYMax if necessary
        if (tYMin > tYMax) {
            std::swap(tYMin, tYMax);
        }

        // Check if the intersection along the Y-axis overlaps with the X-axis intersection
        if ((tXMin > tYMax) || (tYMin > tXMax)) {
            info.hit = false; // No intersection
            return false;
        }

        // Update tXMin and tXMax to consider the Y-axis intersections
        tXMin = std::max(tXMin, tYMin);
        tXMax = std::min(tXMax, tYMax);

        // Calculate intersection distances along the Z-axis
        tZMin = (min.z - ray.origin().z) / ray.direction().z;
        tZMax = (max.z - ray.origin().z) / ray.direction().z;

        // Swap tZMin and tZMax if necessary
        if (tZMin > tZMax) {
            std::swap(tZMin, tZMax);
        }

        // Check if the intersection along the Z-axis overlaps with the updated X and Y-axis intersections
        if ((tXMin > tZMax) || (tZMin > tXMax)) {
            info.hit = false; // No intersection
            return false;
        }

        // Update tXMin and tXMax to consider the Z-axis intersections
        tXMin = std::max(tXMin, tZMin);
        tXMax = std::min(tXMax, tZMax);

        // Check if the intersection is in the positive ray direction
        if (tXMin < 0 && tXMax < 0) {
            info.hit = false; // Intersection behind the ray's origin
            return false;
        }

        // If tXMin is positive, it's the entry point of intersection
        // Calculate the hit point and normal at the intersection
        info.hit = true;
        info.t = (tXMin >= 0) ? tXMin : tXMax; // Use the nearest intersection point
        info.hitPoint = ray.origin() + info.t * ray.direction();

        // Calculate the normal of the hit face (outward-facing normal of the AABB)
        if (info.t == tXMin) {
            info.normal = glm::vec3(-1.0f, 0.0f, 0.0f); // Left face
        }
        else if (info.t == tXMax) {
            info.normal = glm::vec3(1.0f, 0.0f, 0.0f); // Right face
        }
        else if (info.t == tYMin) {
            info.normal = glm::vec3(0.0f, -1.0f, 0.0f); // Bottom face
        }
        else if (info.t == tYMax) {
            info.normal = glm::vec3(0.0f, 1.0f, 0.0f); // Top face
        }
        else if (info.t == tZMin) {
            info.normal = glm::vec3(0.0f, 0.0f, -1.0f); // Front face
        }
        else {
            info.normal = glm::vec3(0.0f, 0.0f, 1.0f); // Back face
        }

        return true;
    }

    colorRGB shade(const HitInfo & info) {
        // Simple shading: return red color for hits, else return background color
        return info.hit ? colorRGB(1.0f, 0.0f, 0.0f) : colorRGB();
    }

    void saveAsPPM(const std::vector<std::vector<colorRGB>>&pixelData, int width, int height, const char* filename) {
        std::cout << "Saving image..."  << std::endl;
        std::ofstream img(filename);
        img << "P3" << std::endl;
        img << width << " " << height << std::endl;
        img << "255" << std::endl;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int r = static_cast<int>(pixelData[i][j].r * 255.0f);
                int g = static_cast<int>(pixelData[i][j].g * 255.0f);
                int b = static_cast<int>(pixelData[i][j].b * 255.0f);

                img << r << " " << g << " " << b << std::endl;
            }
        }

        std::cout << "Image saved as '" << filename << "'" << std::endl;
    }

    int main() {
        // Image dimentions
        int imageWidth = 600;
        float aspectRatio = 1 / 1;
        int imageHeight = static_cast<int>(imageWidth / aspectRatio);

        glm::vec3 backgroundColor(0.0f);
        //std::vector<std::vector<glm::vec3>> pixelData(imageHeight, std::vector<glm::vec3>(imageWidth, backgroundColor));

        glm::vec3 boxMin(0.2f, 0.2f, 0.2f);
        glm::vec3 boxMax(0.8f, 0.8f, 0.8f);

        // Camera setup
        glm::vec3 cameraPosition(0.0f, 0.0f, -2.0f);  // Position of the camera
        glm::vec3 cameraTarget(0.5f, 0.5f, 0.5f);    // Point the camera is looking at
        glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);        // Up direction of the camera
        camera cam = camera(imageWidth, imageHeight, cameraPosition, cameraTarget, cameraUp);

        // Iterate over pixels
        for (int i = 0; i < imageHeight; ++i) {
            // Progress indicator
            std::clog << "\rScanlines remaining: " << (imageHeight - i) << ' ' << std::flush;
            for (int j = 0; j < imageWidth; ++j) {
                // Calculate normalized device coordinates (NDC) for the pixel
                float ndcX = (2.0f * static_cast<float>(j) / static_cast<float>(imageWidth)) - 1.0f;
                float ndcY = 1.0f - (2.0f * static_cast<float>(i) / static_cast<float>(imageHeight));

                // Create a ray direction vector in camera space
                glm::vec4 rayDirectionCameraSpace(ndcX, ndcY, -1.0f, 1.0f); // The -1.0f is the direction along the negative Z-axis
                glm::vec4 rayDirectionWorldSpace = glm::inverse(cam.getMatrix()) * rayDirectionCameraSpace;

                // Normalize the ray direction
                glm::vec3 rayDirection = glm::normalize(glm::vec3(rayDirectionWorldSpace));

                // Set the ray origin to the camera position
                ray ray(cameraPosition, rayDirection);

                // Perform ray-box intersection and shading as described in the previous example
                HitInfo hitInfo;
                if (intersectBox(ray, boxMin, boxMax, hitInfo)) {
                    cam.pixels[j][i] = shade(hitInfo);
                }


                //// Calculate the pixel center in world space
                //glm::vec3 pixel_center = cam.cameraPosition + glm::vec3(i, j, 0);

                //// Create a ray from the camera position to the pixel center
                //glm::vec3 ray_direction = glm::normalize(pixel_center - cam.cameraPosition);
                //ray r(cam.cameraPosition, ray_direction);

                //// Calculate the color of the pixel by tracing the ray
                //colorRGB pixel_color = ray_color(r);

                ////std::clog << "\r(" << pixel_color.r << ", " << pixel_color.g << ", " << pixel_color.b << ")" << std::flush;

                //// Write the color to the pixel data array
                //cam.pixels[j][i] = pixel_color;
            }
        }

        std::clog << "\rDone.                 \n";
        
        saveAsPPM(cam.pixels, imageWidth, imageHeight, "red_box.ppm");

        return 0;
    }
