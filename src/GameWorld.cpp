#include "GameWorld.h"

GameWorld::GameWorld()
{
    g = -0.0098;
    //g = 0;
}

GameWorld::~GameWorld()
{
}

void GameWorld::checkPlayerBlocked()
{
    bool texColl;
    bool noTexColl;

    glm::vec3 gaze = normalize(player->cam.lookAt - player->cam.eye);
    glm::vec3 playerForward = normalize(glm::vec3(gaze.x, 0, gaze.z));
    glm::vec3 playerStrafe = normalize(cross(playerForward, player->cam.upVector));
    glm::vec3 playerUp = player->cam.upVector;

    auto DummyModel = std::make_shared<MatrixStack>();


    DummyModel->pushMatrix();
        DummyModel->loadIdentity();
        //forward
        DummyModel->pushMatrix();
            DummyModel->translate(0.1f*playerForward);
            player->setUp(DummyModel);

            checkAllColl(texturedEntities, DummyModel->topMatrix(), player, texColl);
            checkAllColl(noTexEntities, DummyModel->topMatrix(), player, noTexColl);

            player->frontBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //backward
        DummyModel->pushMatrix();
            DummyModel->translate(-0.1f*playerForward);
            player->setUp(DummyModel);

            checkAllColl(texturedEntities, DummyModel->topMatrix(), player, texColl);
            checkAllColl(noTexEntities, DummyModel->topMatrix(), player, noTexColl);

            player->backBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //left
        DummyModel->pushMatrix();
            DummyModel->translate(-0.1f*playerStrafe);
            player->setUp(DummyModel);

            checkAllColl(texturedEntities, DummyModel->topMatrix(), player, texColl);
            checkAllColl(noTexEntities, DummyModel->topMatrix(), player, noTexColl);

            player->leftBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //right
        DummyModel->pushMatrix();
            DummyModel->translate(0.1f*playerStrafe);
            player->setUp(DummyModel);

            checkAllColl(texturedEntities, DummyModel->topMatrix(), player, texColl);
            checkAllColl(noTexEntities, DummyModel->topMatrix(), player, noTexColl);

            player->rightBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //up
        DummyModel->pushMatrix();
            DummyModel->translate(player->getVertV()*playerUp);
            player->setUp(DummyModel);

            checkAllColl(texturedEntities, DummyModel->topMatrix(), player, texColl);
            checkAllColl(noTexEntities, DummyModel->topMatrix(), player, noTexColl);

            player->aboveBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
        //down
        DummyModel->pushMatrix();
            DummyModel->translate((player->getVertV() + g)*playerUp);
            player->setUp(DummyModel);

            checkAllColl(texturedEntities, DummyModel->topMatrix(), player, texColl);
            checkAllColl(noTexEntities, DummyModel->topMatrix(), player, noTexColl);

            player->belowBlocked = (texColl || noTexColl);
        DummyModel->popMatrix();
    DummyModel->popMatrix();
}

void GameWorld::checkProjsBlocked()
{
    bool texColl;
    bool noTexColl;

    auto DummyModel = std::make_shared<MatrixStack>();

    DummyModel->pushMatrix();
        DummyModel->loadIdentity();
        for (int i = 0; i < projectiles.size(); i++)
        {
            projectiles[i]->calcPos();
            DummyModel->pushMatrix();
                projectiles[i]->setUp(DummyModel);

                checkAllColl(texturedEntities, DummyModel->topMatrix(), projectiles[i].get(), texColl);
                checkAllColl(noTexEntities, DummyModel->topMatrix(), projectiles[i].get(), noTexColl);

                if (texColl || noTexColl)
                    projectiles.erase(projectiles.begin()+i);

            DummyModel->popMatrix();
        }
}

void GameWorld::checkAllColl(const std::vector<Entity*> &entities, glm::mat4 objectTrans, Entity * object, bool &ret)
{
    auto DummyModel = std::make_shared<MatrixStack>();
    std::vector<glm::vec3> newBBoxO;
    bool collRes = false;

    std::vector<glm::vec3> newBBoxP = object->hitbox->recalcBBox(objectTrans);

    DummyModel->pushMatrix();
    DummyModel->loadIdentity();
    for (Entity * e : entities)
    {
        DummyModel->pushMatrix();
        e->setUp(DummyModel);
        newBBoxO = e->mesh->recalcBBox(DummyModel->topMatrix());
        collRes = (
            (newBBoxP[0].x <= newBBoxO[1].x && newBBoxP[1].x >= newBBoxO[0].x) &&
            (newBBoxP[0].y <= newBBoxO[1].y && newBBoxP[1].y >= newBBoxO[0].y) &&
            (newBBoxP[0].z <= newBBoxO[1].z && newBBoxP[1].z >= newBBoxO[0].z));
        if (collRes)
        {
            ret = collRes;
            return;
        }
        DummyModel->popMatrix();
    }
    DummyModel->popMatrix();

    ret = false;
    return;
}

void GameWorld::addEntity(Entity * ent)
{
    if (ent->tex)
        texturedEntities.push_back(ent);
    else
        noTexEntities.push_back(ent);
}

void GameWorld::addProjectile(std::shared_ptr<Projectile> proj)
{
    projectiles.push_back(proj);
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
    checkProjsBlocked();
    player->calcPlayerPos(g);
}
