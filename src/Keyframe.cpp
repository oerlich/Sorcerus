#include "Keyframe.h"

Keyframe::Keyframe(std::string keyFrameStr)
{
    std::vector<std::string> lines;

    std::istringstream iss(keyFrameStr);
    std::string token;
    while (std::getline(iss, token, '\n'))
        lines.push_back(token);

    time = std::stof(lines[0]);

    for (int i = 1; i < lines.size(); i++)
    {
        std::vector<std::string> temp;
        std::istringstream iss(lines[i]);
        std::string token;
        while (std::getline(iss, token, ' '))
            temp.push_back(token);

        jointRotations[std::stoi(temp[0])] = glm::vec3(std::stof(temp[1]), std::stof(temp[2]), std::stof(temp[3]));
    }
}

Keyframe::~Keyframe()
{
}
