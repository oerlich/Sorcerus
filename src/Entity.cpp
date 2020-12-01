#include "Entity.h"

Entity::Entity(glm::vec3 startPos, glm::vec3 sFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID)
{
    worldPos = startPos;
    scaleFactors = sFactors;
    xRot = xAngle;
    yRot = yAngle;
    zRot = zAngle;
    mesh = m;
    hitbox = m;
    tex = texture;
    materialID = matID;
}

void Entity::setPosition(glm::vec3 position)
{
    worldPos = position;
}

glm::vec3 Entity::getPosition()
{
    return worldPos;
}
