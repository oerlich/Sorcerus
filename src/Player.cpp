#include "Player.h"


Player::Player(glm::vec3 startPos, glm::vec3 sFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID, float h) :
    Entity(startPos, sFactors, xAngle, yAngle, zAngle, m, texture, matID),
    height(h),
    cam(0.01, 0.2, glm::vec3(startPos.x, startPos.y + height, startPos.z))
{
    moveF = moveB = moveL = moveR = jump = false;

    aboveBlocked = frontBlocked = backBlocked = leftBlocked = rightBlocked = false;
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

void Player::calcPlayerPos(float g)
{
    glm::vec3 nextPos = worldPos;

    glm::vec3 gaze = normalize(cam.lookAt - cam.eye);
    glm::vec3 strafeAxis = normalize(cross(gaze, cam.upVector));
    glm::vec3 prevPos = nextPos;


    if (moveF)
    {
        if(!frontBlocked)
            nextPos += speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (moveB)
    {
        if(!backBlocked)
            nextPos -= speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (moveR)
    {
        if(!rightBlocked)
            nextPos += speed * strafeAxis;
    }
    if (moveL)
    {
        if(!leftBlocked)
            nextPos -= speed * strafeAxis;
    }
    if (jump && belowBlocked)
    {
        if (!(aboveBlocked))
        {
            belowBlocked = false;
            verticalVelocity += 0.045;
        }
    }


    if (belowBlocked)
        verticalVelocity = 0;
    else
        verticalVelocity = glm::max(verticalVelocity + g, -0.1f);

    nextPos.y += verticalVelocity;

    setPosition(nextPos);

    yRot = -cam.getTheta();
}

void Player::setPosition(glm::vec3 position)
{
    worldPos = position;
    glm::vec3 eyePos = glm::vec3(worldPos.x, worldPos.y + height, worldPos.z) + (0.14f * normalize(glm::vec3(cam.gaze.x, 0, cam.gaze.z)));
    cam.setEyePos(eyePos);
}

