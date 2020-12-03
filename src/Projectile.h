#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "Entity.h"
#include <glm/gtc/type_ptr.hpp>

class Projectile : public Entity {
private:
    glm::vec3 direction;
    float speed;
public:
    Projectile(glm::vec3 startPos, glm::vec3 scaleFactors, float xAngle, float yAngle, float zAngle,
        Mesh* m, std::shared_ptr<Texture> texture, int matID, glm::vec3 direction, float s, bool lEmit, glm::vec3 lColor, glm::vec3 lABC);
    virtual ~Projectile();

    void setUpAndDraw(std::shared_ptr<MatrixStack> Model, std::shared_ptr<Program> shader);
    void setUp(std::shared_ptr<MatrixStack> Model);

    void calcPos();


    bool lightEmit;

    glm::vec3 lightColor;
    glm::vec3 lightABC;
};

#endif