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

    float verticalVelocity;
    float speed;

public:
    Player(glm::vec3 startPos, glm::vec3 scaleFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID, float h, Mesh* phbox);
    void setUp(std::shared_ptr<MatrixStack> Model);

    void calcPlayerPos(float g);

    void setPosition(glm::vec3 position);
    void setSpeed(float s) { speed = s; }

    float getVertV() { return verticalVelocity; };
    
    Camera cam;
    Camera* getCam() { return &cam; }

    bool moveF;
    bool moveB;
    bool moveL;
    bool moveR;
    bool jump;

    bool aboveBlocked;
    bool belowBlocked;
    bool frontBlocked;
    bool backBlocked;
    bool leftBlocked;
    bool rightBlocked;

    Mesh * hitbox;
};

#endif
