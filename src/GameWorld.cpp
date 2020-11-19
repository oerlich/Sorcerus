#include "GameWorld.h"

GameWorld::GameWorld()
{}

GameWorld::~GameWorld()
{
}

void GameWorld::addEntity(Entity * ent)
{
    if (ent->tex)
        texturedEntities.push_back(ent);
    else
        noTexEntities.push_back(ent);
}

void GameWorld::addLight(glm::vec3 pos, glm::vec3 colorIntensity)
{
    if (pointLights_pos.size() == MAX_POINT_LIGHTS)
    {
        std::cerr << "Invalid attempt to add light beyond maximum for Game world was caught and ignored" << std::endl;
        return;
    }

    pointLights_pos.push_back(pos);
    pointLights_col_int.push_back(colorIntensity);
}

std::vector<glm::vec3> GameWorld::getLightPositions()
{
    return pointLights_pos;
}

std::vector<glm::vec3> GameWorld::getLightColorIntensity()
{
    return pointLights_col_int;
}

std::vector<Entity*> GameWorld::getTexturedEntities()
{
    return texturedEntities;
}

std::vector<Entity*> GameWorld::getNoTexEntities()
{
    return noTexEntities;
}
