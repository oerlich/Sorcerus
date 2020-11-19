#pragma once
#ifndef _GAMEWORLD_H_
#define _GAMEWORLD_H_

#define MAX_POINT_LIGHTS 30

#include <vector>
#include <iostream>
#include "Entity.h"
#include <glm/gtc/type_ptr.hpp>

class GameWorld
{
private:
    std::vector<Entity*> texturedEntities;
    std::vector<Entity*> noTexEntities;

    std::vector<glm::vec3> pointLights_pos;
    std::vector<glm::vec3> pointLights_col_int;
public:
    GameWorld();
    virtual ~GameWorld();
    void addEntity(Entity* ent);
    void addLight(glm::vec3 pos, glm::vec3 colorIntensity);
    
    std::vector<glm::vec3> getLightPositions();
    std::vector<glm::vec3> getLightColorIntensity();
    std::vector<Entity*> getTexturedEntities();
    std::vector<Entity*> getNoTexEntities();
};

#endif