#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "WindowManager.h"
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    float phi;
    float theta;
    int lastX;
    int lastY;
    float speed;
    float sensitivity;
    bool firstMouse;

    void moveLookAt(WindowManager* windowManager);
    void moveEye();

public:
    glm::vec3 eye;
    glm::vec3 lookAt;
    glm::vec3 upVector;
    glm::vec3 gaze;

    bool dollyF;
    bool dollyB;
    bool strafeR;
    bool strafeL;
    bool rise;
    bool fall;

    Camera(float sIn, float sens, glm::vec3 initPos);
    virtual ~Camera();
    void setUpCam(WindowManager* windowManager);
    void setEyePos(glm::vec3 pos);
    void setSpeed(float s);
    void enableCursor(WindowManager* windowManager);

    float getTheta() { return theta; }
    float getPhi() { return phi; }
};

#endif
