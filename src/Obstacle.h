#pragma once
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "Entity.h"
#include <glm/gtc/type_ptr.hpp>

class Obstacle : public Entity
{
private:


public:
    Obstacle(glm::vec3 startPos, glm::vec3 scaleFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID);
    void setUp(std::shared_ptr<MatrixStack> Model);

};

#endif
