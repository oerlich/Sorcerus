#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Mesh.h"
#include "Texture.h"
#include "MatrixStack.h"
#include "Program.h"
#include <glm/gtc/type_ptr.hpp>

class Entity
{
private:
    
protected:
    
    Entity(glm::vec3 startPos, glm::vec3 scaleFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID);

public:
    glm::vec3 worldPos;
    glm::vec3 scaleFactors;
    float xRot;
    float yRot;
    float zRot;

    Mesh* mesh;
    Mesh * hitbox;
    std::shared_ptr<Texture> tex;
    int materialID;

    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();
    virtual void setUpAndDraw(std::shared_ptr<MatrixStack> Model, std::shared_ptr<Program> shader) = 0;
    virtual void setUp(std::shared_ptr<MatrixStack> Model) = 0;
};

#endif
