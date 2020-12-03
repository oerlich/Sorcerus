#pragma once
#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

class Keyframe {
public:
    Keyframe(std::string keyFrameStr);
    virtual ~Keyframe();

    std::unordered_map<int, glm::vec3> jointRotations;
    float time;


private:


};

#endif