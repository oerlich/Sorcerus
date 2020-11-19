#include "Mesh.h"


Mesh::Mesh(std::vector<tinyobj::shape_t> TOshapes)
{
    for (int i = 0; i < TOshapes.size(); i++)
    {
        auto shape = std::make_shared<Shape>();
        shape->createShape(TOshapes[i]);
        shape->measure();
        shape->init();

        if (i == 0)
        {
            gMin.x = shape->min.x;
            gMin.y = shape->min.y;
            gMin.z = shape->min.z;
            gMax.x = shape->max.x;
            gMax.y = shape->max.y;
            gMax.z = shape->max.z;
        }

        if (shape->min.x < gMin.x)
            gMin.x = shape->min.x;
        if (shape->min.y < gMin.y)
            gMin.y = shape->min.y;
        if (shape->min.z < gMin.z)
            gMin.z = shape->min.z;

        if (shape->max.x > gMax.x)
            gMax.x = shape->max.x;
        if (shape->max.y > gMax.y)
            gMax.y = shape->max.y;
        if (shape->max.y > gMax.z)
            gMax.y = shape->max.z;

        shapes.push_back(shape);
    }

    center = glm::vec3((gMin.x + gMax.x)/2, (gMin.y + gMax.y)/2, (gMin.z + gMax.z)/2);
    
}

Mesh::~Mesh()
{
}
