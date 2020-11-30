#pragma once
#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader/tiny_obj_loader.h>

#include "Shape.h"

class Mesh {
public:
    Mesh(std::vector<tinyobj::shape_t> TOshapes);
    virtual ~Mesh();
    glm::vec3 gMin;
    glm::vec3 gMax;
    glm::vec3 center;
    std::vector<std::shared_ptr<Shape>> shapes;

    std::vector<glm::vec3> recalcBBox(glm::mat4 transform);

private:

};


#endif