#include "GameWorld.h"

GameWorld::GameWorld()
{
    g = -0.00098;
}

GameWorld::~GameWorld()
{
}

bool GameWorld::checkColl(glm::vec3 min1, glm::vec3 max1, glm::vec3 min2, glm::vec3 max2)
{
    return (min1.x <= max2.x && max1.x >= min2.x) &&
        (min1.y <= max2.y && max1.y >= min2.y) &&
        (min1.z <= max2.z && max1.z >= min2.z);
}

void GameWorld::checkPlayerBlocked()
{
    bool texColl;
    bool noTexColl;

    glm::vec3 playerForward = normalize(player->cam.lookAt - player->cam.eye);
    glm::vec3 playerStrafe = normalize(cross(playerForward, player->cam.upVector));
    glm::vec3 playerUp = player->cam.upVector;

    auto DummyModel = std::make_shared<MatrixStack>();

    DummyModel->pushMatrix();
        DummyModel->loadIdentity();
        player->setUp(DummyModel);
        //forward
        DummyModel->pushMatrix();
            DummyModel->translate(0.6f*playerForward);
            texColl = checkAllColl(texturedEntities, DummyModel->topMatrix());
            noTexColl = checkAllColl(noTexEntities, DummyModel->topMatrix());
            player->frontBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //backward
        DummyModel->pushMatrix();
            DummyModel->translate(-0.6f*playerForward);
            texColl = checkAllColl(texturedEntities, DummyModel->topMatrix());
            noTexColl = checkAllColl(noTexEntities, DummyModel->topMatrix());
            player->backBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //left
        DummyModel->pushMatrix();
            DummyModel->translate(-0.6f*playerStrafe);
            texColl = checkAllColl(texturedEntities, DummyModel->topMatrix());
            noTexColl = checkAllColl(noTexEntities, DummyModel->topMatrix());
            player->leftBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //right
        DummyModel->pushMatrix();
            DummyModel->translate(0.6f*playerStrafe);
            texColl = checkAllColl(texturedEntities, DummyModel->topMatrix());
            noTexColl = checkAllColl(noTexEntities, DummyModel->topMatrix());
            player->rightBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //up
        DummyModel->pushMatrix();
            DummyModel->translate(glm::max(player->getVertV() + g, -0.1f)*playerUp);
            texColl = checkAllColl(texturedEntities, DummyModel->topMatrix());
            noTexColl = checkAllColl(noTexEntities, DummyModel->topMatrix());
            player->aboveBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //down
        DummyModel->pushMatrix();
            DummyModel->translate(glm::max(player->getVertV() + g, -0.1f)*playerUp);
            texColl = checkAllColl(texturedEntities, DummyModel->topMatrix());
            noTexColl = checkAllColl(noTexEntities, DummyModel->topMatrix());
            player->belowBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
    DummyModel->popMatrix();
}

bool GameWorld::checkAllColl(std::vector<Entity*> entities, glm::mat4 playerTrans)
{
    auto DummyModel = std::make_shared<MatrixStack>();
    std::vector<glm::vec3> newBBoxO;
    bool collRes = false;

    std::vector<glm::vec3> newBBoxP = player->hitbox->recalcBBox(playerTrans);

    DummyModel->pushMatrix();
        DummyModel->loadIdentity();
        for (Entity * e : entities)
        {
            DummyModel->pushMatrix();
                e->setUp(DummyModel);
                newBBoxO = e->mesh->recalcBBox(DummyModel->topMatrix());
                collRes = checkColl(newBBoxP[0], newBBoxP[1], newBBoxO[0], newBBoxO[1]);
                if (collRes)
                    return collRes;
            DummyModel->popMatrix();
        }
    DummyModel->popMatrix();

    return false;
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
    checkPlayerBlocked();
    player->calcPlayerPos(g);
}
