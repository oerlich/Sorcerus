#include "Animation.h"

Animation::Animation(std::string animDir)
{
    int numFrames = 0;
    std::string line;
    std::ifstream info(animDir + "/info.txt");

    if (info.is_open())
    {
        std::getline(info, line);
        numFrames = std::stoi(line);
        info.close();
    }

    for (int i = 1; i <= numFrames; i++)
    {
        std::ifstream fin(animDir + "/frame_" + std::to_string(i) + ".txt");
        std::string str;

        fin.seekg(0, std::ios::end);
        str.reserve(fin.tellg());
        fin.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(fin)),
            std::istreambuf_iterator<char>());

        keyframes.push_back(new Keyframe(str));
    }
    
}

Animation::~Animation()
{
}

glm::vec3 Animation::nextFrame(int meshIndex, float t)
{
    Keyframe * base, * next;
    glm::vec3 baseRots;
    glm::vec3 nextRots;

    float retX, retY, retZ;

    for (int i = 0; i < keyframes.size(); i++)
    {
        if (t >= keyframes[i]->time)
        {
            base = keyframes[i];
            next = keyframes[i + 1];
        }
        else
            break;
    }

    baseRots = base->jointRotations[meshIndex];
    nextRots = next->jointRotations[meshIndex];

    retX = linearInterp(baseRots.x, nextRots.x, next->time - base->time, t - base->time);
    retY = linearInterp(baseRots.y, nextRots.y, next->time - base->time, t - base->time);
    retZ = linearInterp(baseRots.z, nextRots.z, next->time - base->time, t - base->time);

    return glm::vec3(glm::radians(retX), glm::radians(retY), glm::radians(retZ));
}

float Animation::linearInterp(float begin, float end, float dur, float t)
{
    float res = begin * (1 - (t / dur)) + end * (t / dur);
    return (t > dur) ? end : res;
}
