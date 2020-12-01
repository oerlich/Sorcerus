#include "Obstacle.h"


Obstacle::Obstacle(glm::vec3 startPos, glm::vec3 sFactors, float xAngle, float yAngle, float zAngle, Mesh * m, std::shared_ptr<Texture> texture, int matID) :
    Entity(startPos, sFactors, xAngle, yAngle, zAngle, m, texture, matID)
{}

void Obstacle::setUpAndDraw(std::shared_ptr<MatrixStack> Model, std::shared_ptr<Program> shader)
{
    Model->translate(worldPos);
    Model->scale(scaleFactors);
    Model->rotate(zRot, glm::vec3(0, 0, 1));
    Model->rotate(yRot, glm::vec3(0, 1, 0));
    Model->rotate(xRot, glm::vec3(1, 0, 0));
    glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, glm::value_ptr(Model->topMatrix()));
    for (std::shared_ptr<Shape> s : mesh->shapes)
        s->draw(shader);
}

void Obstacle::setUp(std::shared_ptr<MatrixStack> Model)
{
    Model->translate(worldPos);
    Model->scale(scaleFactors);
    Model->rotate(zRot, glm::vec3(0, 0, 1));
    Model->rotate(yRot, glm::vec3(0, 1, 0));
    Model->rotate(xRot, glm::vec3(1, 0, 0));
}
