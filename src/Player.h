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

    void setPosition(glm::vec3 position);

    float verticalVelocity;
    float speed;

    void setSpeed(float s) { speed = s; }

    Camera cam;

    bool moveF;
    bool moveB;
    bool moveL;
    bool moveR;
    bool grounded;
    bool jump;

    bool aboveBlocked;
    bool belowBlocked;

};

#endif
