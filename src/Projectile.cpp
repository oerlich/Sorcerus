#include "Projectile.h"

Projectile::Projectile(glm::vec3 startPos, glm::vec3 scaleFactors, float xAngle, float yAngle, float zAngle,
    Mesh * m, std::shared_ptr<Texture> texture, int matID, glm::vec3 dir, float s, bool lEmit, glm::vec3 lColor, glm::vec3 lABC) :
    Entity(startPos, scaleFactors, xAngle, yAngle, zAngle, m, texture, matID)
{
    direction = dir;
    speed = s;
    lightEmit = lEmit;
    lightColor = lColor;
    lightABC = lABC;
}

Projectile::~Projectile()
{
}

void Projectile::setUpAndDraw(std::shared_ptr<MatrixStack> Model, std::shared_ptr<Program> shader)
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

void Projectile::setUp(std::shared_ptr<MatrixStack> Model)
{
    Model->translate(worldPos);
    Model->scale(scaleFactors);
    Model->rotate(zRot, glm::vec3(0, 0, 1));
    Model->rotate(yRot, glm::vec3(0, 1, 0));
    Model->rotate(xRot, glm::vec3(1, 0, 0));
}

void Projectile::calcPos()
{
    worldPos += speed * direction;
}
