#include "Player.h"


Player::Player(glm::vec3 startPos, glm::vec3 sFactors, float xAngle, float yAngle, float zAngle, Mesh* m, std::shared_ptr<Texture> texture, int matID, float h, Mesh * playerhitbox) :
    Entity(startPos, sFactors, xAngle, yAngle, zAngle, m, texture, matID),
    height(h),
    cam(0.01, 0.2, glm::vec3(startPos.x, startPos.y + height, startPos.z))
{
    moveF = moveB = moveL = moveR = jump = false;

    basePosition = true;

    animID = -1;

    aboveBlocked = frontBlocked = backBlocked = leftBlocked = rightBlocked = false;
    belowBlocked = true;

    verticalVelocity = 0.0f;
    speed = 0.03;
    hitbox = playerhitbox;
}

void Player::setUpAndDraw(std::shared_ptr<MatrixStack> Model, std::shared_ptr<Program> shader)
{
    float currTime = glfwGetTime();

    if (animID >= 0 && (currTime - animStartTime) > mesh->meshAnimations[animID]->getDuration())
    {
        basePosition = true;
        animID = -1;
    }

    Model->translate(worldPos);
    //Model->translate(glm::vec3(0, -1.5, 0));
    Model->scale(scaleFactors);
    Model->rotate(zRot, glm::vec3(0, 0, 1));
    Model->rotate(yRot, glm::vec3(0, 1, 0));
    //Model->rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
    Model->rotate(xRot, glm::vec3(1, 0, 0));

    //right arm
    {
        //right shoulder
        Model->pushMatrix();
            Model->translate(glm::vec3(0, -17, 140));
            if (basePosition)
            {
            
                Model->rotate(glm::radians(100.0f), glm::vec3(1, 0, 0));
                Model->rotate(glm::radians(-5.0f), glm::vec3(0, 1, 0));
                Model->rotate(glm::radians(80.0f), glm::vec3(0, 0, 1));
            }
            else
            {
                glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(15, currTime - animStartTime);
                Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                Model->rotate(rotations.z, glm::vec3(0, 0, 1));
            }
            Model->translate(glm::vec3(0, 17, -140));

            // right elbow
            Model->pushMatrix();
                Model->translate(glm::vec3(2, -45, 138));
                if (basePosition)
                {
                    Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                    Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                    Model->rotate(glm::radians(45.0f), glm::vec3(0, 0, 1));
                }
                else
                {
                    glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(17, currTime - animStartTime);
                    Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                    Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                    Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                }
                Model->translate(glm::vec3(-2, 45, -138));

                // right wrist
                Model->pushMatrix();
                    Model->translate(glm::vec3(2.2, -70, 138));
                    if (basePosition)
                    {
                        Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
                    }
                    else
                    {
                        glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(19, currTime - animStartTime);
                        Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                        Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                        Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                    }
                    Model->translate(glm::vec3(-2.2, 70, -138));

                    glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                    for (int i = 19; i <= 20; i++)
                        mesh->shapes[i]->draw(shader);

                Model->popMatrix();

                glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                for (int i = 17; i <= 18; i++)
                    mesh->shapes[i]->draw(shader);

            Model->popMatrix();

            glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
            for (int i = 15; i <= 16; i++)
                mesh->shapes[i]->draw(shader);

        Model->popMatrix();
    }

    //left arm
    {
        // left shoulder
        Model->pushMatrix();
            Model->translate(glm::vec3(0, 17, 140));
            if(basePosition)
            {
                Model->rotate(glm::radians(-80.0f), glm::vec3(1, 0, 0));
                Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
            }
            else
            {
                glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(21, currTime - animStartTime);
                Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                Model->rotate(rotations.z, glm::vec3(0, 0, 1));
            }
            Model->translate(glm::vec3(0, -17, -140));

            //left elbow
            Model->pushMatrix();
                Model->translate(glm::vec3(2, 45, 138));
                if (basePosition)
                {
                    Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                    Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                    Model->rotate(glm::radians(-25.0f), glm::vec3(0, 0, 1));
                }
                else
                {
                    glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(23, currTime - animStartTime);
                    Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                    Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                    Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                }
                Model->translate(glm::vec3(-2, -45, -138));

                // right wrist
                Model->pushMatrix();
                    Model->translate(glm::vec3(2.1, 70, 138));
                    if (basePosition)
                    {
                        Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
                    }
                    else
                    {
                        glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(25, currTime - animStartTime);
                        Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                        Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                        Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                    }
                    Model->translate(glm::vec3(-2.1, -70, -138));

                    glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                    for (int i = 25; i <= 26; i++)
                        mesh->shapes[i]->draw(shader);

                Model->popMatrix();

                glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                for (int i = 23; i <= 24; i++)
                    mesh->shapes[i]->draw(shader);
            Model->popMatrix();


            glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
            for (int i = 21; i <= 22; i++)
                mesh->shapes[i]->draw(shader);
        Model->popMatrix();
    }

    //right leg
    {
        //pelvis
        Model->pushMatrix();
            Model->translate(glm::vec3(3, -8, 96));
            if (basePosition)
            {
                Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
            }
            else
            {
                glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(5, currTime - animStartTime);
                Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                Model->rotate(rotations.z, glm::vec3(0, 0, 1));
            }
            Model->translate(glm::vec3(-3, 8, -96));

            //knee
            Model->pushMatrix();
                Model->translate(glm::vec3(3, -8, 50));
                if (basePosition)
                {
                    Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                    Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                    Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
                }
                else
                {
                    glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(3, currTime - animStartTime);
                    Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                    Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                    Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                }
                Model->translate(glm::vec3(-3, 8, -50));

                //ankle
                Model->pushMatrix();
                    Model->translate(glm::vec3(3, -8, 8));
                    if (basePosition)
                    {
                        Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
                    }
                    else
                    {
                        glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(1, currTime - animStartTime);
                        Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                        Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                        Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                    }
                    Model->translate(glm::vec3(-3, 8, -8));

                    glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                    for (int i = 0; i <= 1; i++)
                        mesh->shapes[i]->draw(shader);

                Model->popMatrix();

                glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                for (int i = 2; i <= 3; i++)
                    mesh->shapes[i]->draw(shader);

            Model->popMatrix();

            glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
            for (int i = 4; i <= 5; i++)
                mesh->shapes[i]->draw(shader);

        Model->popMatrix();
    }

    //left leg
    {
        //pelvis
        Model->pushMatrix();
            Model->translate(glm::vec3(3, 8, 96));
            if (basePosition)
            {
                Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
            }
            else
            {
                glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(11, currTime - animStartTime);
                Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                Model->rotate(rotations.z, glm::vec3(0, 0, 1));
            }
            Model->translate(glm::vec3(-3, -8, -96));

            //knee
            Model->pushMatrix();
                Model->translate(glm::vec3(3, 8, 50));
                if (basePosition)
                {
                    Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                    Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                    Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
                }
                else
                {
                    glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(9, currTime - animStartTime);
                    Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                    Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                    Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                }
                Model->translate(glm::vec3(-3, -8, -50));

                //ankle
                Model->pushMatrix();
                    Model->translate(glm::vec3(3, 8, 8));
                    if (basePosition)
                    {
                        Model->rotate(glm::radians(0.0f), glm::vec3(1, 0, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 1, 0));
                        Model->rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
                    }
                    else
                    {
                        glm::vec3 rotations = mesh->meshAnimations[animID]->nextFrame(7, currTime - animStartTime);
                        Model->rotate(rotations.x, glm::vec3(1, 0, 0));
                        Model->rotate(rotations.y, glm::vec3(0, 1, 0));
                        Model->rotate(rotations.z, glm::vec3(0, 0, 1));
                    }
                    Model->translate(glm::vec3(-3, -8, -8));

                    glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                    for (int i = 6; i <= 7; i++)
                        mesh->shapes[i]->draw(shader);

                Model->popMatrix();

                glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
                for (int i = 8; i <= 9; i++)
                    mesh->shapes[i]->draw(shader);

            Model->popMatrix();

            glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
            for (int i = 10; i <= 11; i++)
                mesh->shapes[i]->draw(shader);

        Model->popMatrix();
    }

    //belly, hips, torso
    glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
    for (int i = 12; i <= 14; i++)
        mesh->shapes[i]->draw(shader);
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

    gaze = normalize(cam.lookAt - cam.eye);
    glm::vec3 strafeAxis = normalize(cross(gaze, cam.upVector));
    glm::vec3 prevPos = nextPos;


    if (moveF && !frontBlocked)
    {
        nextPos += speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (moveB && !backBlocked)
    {
        nextPos -= speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (moveR && !rightBlocked)
    {
        nextPos += speed * strafeAxis;
    }
    if (moveL && !leftBlocked)
    {
        nextPos -= speed * strafeAxis;
    }
    if (jump && belowBlocked)
    {
        belowBlocked = false;
        verticalVelocity += 0.11;
    }

    
    if (belowBlocked || aboveBlocked)
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

void Player::playAnimation(int animationID)
{
    basePosition = false;
    animStartTime = glfwGetTime();
    animID = animationID;
}

void Player::stopAnimation()
{
    basePosition = true;
    animStartTime = glfwGetTime();
    animID = -1;
}

