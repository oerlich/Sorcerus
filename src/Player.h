#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Entity.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>

class Player : public Entity
{
private:

    float height;

public:
    Player(glm::vec3 startPos, glm::vec3 scaleFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID, float h);
    void setUp(std::shared_ptr<MatrixStack> Model);
    Camera cam;

};

#endif
