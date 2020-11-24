#include "GameWorld.h"

GameWorld::GameWorld()
{
    g = -0.00098;
}

GameWorld::~GameWorld()
{
}

bool GameWorld::checkColl(Entity * e1, Entity * e2)
{
    return false;
}

void GameWorld::calcPlayerPos()
{
    Camera cam = player->cam;
    glm::vec3 nextPos = player->worldPos;

    glm::vec3 gaze = normalize(cam.lookAt - cam.eye);
    glm::vec3 strafeAxis = normalize(cross(gaze, cam.upVector));
    glm::vec3 prevPos = nextPos;


    if (player->moveF)
    {
        nextPos += player->speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (player->moveB)
    {
        nextPos -= player->speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (player->moveR)
    {
        nextPos += player->speed * strafeAxis;
    }
    if (player->moveL)
    {
        nextPos -= player->speed * strafeAxis;
    }
    if (player->jump && player->grounded)
    {
        if (!(player->aboveBlocked))
        {
            player->grounded = false;
            player->belowBlocked = false;
            player->verticalVelocity += 0.055;
        }
    }

    if (!(player->grounded))
    {
        if (player->belowBlocked)
        {
            player->grounded = true;
            player->verticalVelocity = 0;
        }
        else
            player->verticalVelocity = glm::max(player->verticalVelocity + g, -0.1f);
    }

    nextPos.y += player->verticalVelocity;

    player->setPosition(nextPos);

    player->yRot = -player->cam.getTheta();
}

void GameWorld::addEntity(Entity * ent)
{
    if (ent->tex)
        texturedEntities.push_back(ent);
    else
        noTexEntities.push_back(ent);
}

void GameWorld::addLight(glm::vec3 pos, glm::vec3 colorIntensity, glm::vec3 ABC)
{
    if (pointLights_pos.size() == MAX_POINT_LIGHTS)
    {
        std::cerr << "Invalid attempt to add light beyond maximum for Game world was caught and ignored" << std::endl;
        return;
    }

    pointLights_pos.push_back(pos);
    pointLights_col_int.push_back(colorIntensity);
    pointLights_abc.push_back(ABC);
}

std::vector<glm::vec3> GameWorld::getLightPositions()
{
    return pointLights_pos;
}

std::vector<glm::vec3> GameWorld::getLightColorIntensity()
{
    return pointLights_col_int;
}

std::vector<glm::vec3> GameWorld::getLightABC()
{
    return pointLights_abc;
}

std::vector<Entity*> GameWorld::getTexturedEntities()
{
    return texturedEntities;
}

std::vector<Entity*> GameWorld::getNoTexEntities()
{
    return noTexEntities;
}

void GameWorld::setUpWorld()
{
    calcPlayerPos();
}
