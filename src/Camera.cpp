#include "Camera.h"

Camera::Camera(float sIn, float sens, glm::vec3 initPos)
{
    phi = 0.0;
    theta = 3*3.14159 / 2;
    eye = initPos;
    lookAt = glm::vec3(0, 0, 0);
    upVector = glm::vec3(0, 1, 0);
    lastX = 0;
    lastY = 0;
    firstMouse = true;
    speed = sIn;
    sensitivity = sens;

    dollyB = false;
    dollyF = false;
    strafeR = false;
    strafeL = false;
    rise = false;
    fall = false;
}

void Camera::setUpCam(WindowManager * windowManager)
{
    moveLookAt(windowManager);
    moveEye();
}

void Camera::setSpeed(float s)
{
    speed = s;
}

void Camera::enableCursor(WindowManager* windowManager)
{
    glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(windowManager->getHandle(), lastX, lastY);
}

void Camera::moveLookAt(WindowManager* windowManager)
{
    double xpos, ypos;
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    glfwGetCursorPos(windowManager->getHandle(), &xpos, &ypos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double dx = xpos - lastX;
    double dy = ypos - lastY;

    phi -= ((dy / height) * 2 * 3.14159) * sensitivity;
    if (phi > glm::radians(80.0))
        phi = glm::radians(80.0);
    if (phi < -glm::radians(80.0))
        phi = -glm::radians(80.0);

    theta += ((dx / width) * (2 * 3.14159)) * sensitivity;

    lastX = xpos;
    lastY = ypos;

    lookAt = glm::vec3(
        eye.x + 10 * glm::cos(phi)*glm::cos(theta),
        eye.y + 10 * glm::sin(phi),
        eye.z + 10 * glm::cos(phi)*glm::cos((3.14159 / 2.0) - theta
        ));
}

void Camera::moveEye()
{
    glm::vec3 gaze = normalize(lookAt - eye);
    glm::vec3 strafeAxis = normalize(cross(gaze, upVector));

    if (dollyF)
    {
        eye += speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (dollyB)
    {
        eye -= speed * normalize(glm::vec3(gaze.x, 0, gaze.z));
    }
    if (strafeR)
    {
        eye += speed * strafeAxis;
    }
    if (strafeL)
    {
        eye -= speed * strafeAxis;
    }
    if (rise)
    {
        eye += speed * upVector;
    }
    if (fall)
    {
        eye -= speed * upVector;
    }

}

Camera::~Camera()
{
}
