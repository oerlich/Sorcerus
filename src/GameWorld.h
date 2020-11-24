#pragma once
#ifndef _GAMEWORLD_H_
#define _GAMEWORLD_H_

#define MAX_POINT_LIGHTS 30

#include <vector>
#include <iostream>
#include "Entity.h"
#include "Player.h"
#include <glm/gtc/type_ptr.hpp>

class GameWorld
{
private:
    std::vector<Entity*> texturedEntities;
    std::vector<Entity*> noTexEntities;

    std::vector<glm::vec3> pointLights_pos;
    std::vector<glm::vec3> pointLights_col_int;
    std::vector<glm::vec3> pointLights_abc;

    float g;

    bool checkColl(Entity * e1, Entity * e2);
    void checkPlayerBlocked();

public:
    GameWorld();
    virtual ~GameWorld();
    void addEntity(Entity* ent);
    void addLight(glm::vec3 pos, glm::vec3 colorIntensity, glm::vec3 ABC);
    Player* player;
    
    std::vector<glm::vec3> getLightPositions();
    std::vector<glm::vec3> getLightColorIntensity();
    std::vector<glm::vec3> getLightABC();

    std::vector<Entity*> getTexturedEntities();
    std::vector<Entity*> getNoTexEntities();

    void setUpWorld();
};

#endif