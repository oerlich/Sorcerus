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
    glm::vec3 calcCenter(int index) {
        return glm::vec3((shapes[index]->min.x + shapes[index]->max.x) / 2,
                         (shapes[index]->min.y + shapes[index]->max.y) / 2,
                         (shapes[index]->min.y + shapes[index]->max.y) / 2);
    }

private:

};


#endif