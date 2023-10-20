#pragma once
#include "glm/glm.hpp"
#include "colorRGB.h"

class ray
{
public:
    ray() { color = colorRGB(); }

    ray(const glm::vec3& origin, const glm::vec3& direction) : orig(origin), dir(direction) {}

    glm::vec3 origin() const { return orig; }
    glm::vec3 direction() const { return dir; }
    colorRGB getColor() const { return color; }


private:
    glm::vec3 orig;
    glm::vec3 dir;
    ray* prev;
    ray* next;
    colorRGB color;
};

