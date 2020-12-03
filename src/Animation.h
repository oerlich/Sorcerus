#pragma once
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "Keyframe.h"

class Animation {
public:
    Animation(std::string animDir);
    virtual ~Animation();

    glm::vec3 nextFrame(int meshIndex, float t);
    float getDuration() { return keyframes[keyframes.size() - 1]->time; }

private:
    std::vector<Keyframe*> keyframes;
    float linearInterp(float begin, float end, float dur, float t);
};

#endif