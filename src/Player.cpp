#include "Player.h"


Player::Player(glm::vec3 startPos, glm::vec3 sFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID, float h) :
    Entity(startPos, sFactors, xAngle, yAngle, zAngle, m, texture, matID),
    height(h),
    cam(0.01, 0.2, glm::vec3(startPos.x, startPos.y + height, startPos.z))
{
    moveF = moveB = moveL = moveR = grounded = jump = false;

    aboveBlocked = false;
    belowBlocked = true;

    verticalVelocity = 0.0f;
    speed = 0.01;
}

void Player::setUp(std::shared_ptr<MatrixStack> Model)
{
    Model->translate(worldPos);
    Model->scale(scaleFactors);
    Model->rotate(zRot, glm::vec3(0, 0, 1));
    Model->rotate(yRot, glm::vec3(0, 1, 0));
    Model->rotate(xRot, glm::vec3(1, 0, 0));
}

void Player::setPosition(glm::vec3 position)
{
    worldPos = position;
    glm::vec3 eyePos = glm::vec3(worldPos.x, worldPos.y + height, worldPos.z) + (0.14f * normalize(glm::vec3(cam.gaze.x, 0, cam.gaze.z)));
    cam.setEyePos(eyePos);
}

