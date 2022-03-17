#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>

#include "Camera.h"

glm::mat4 Camera::getV() const
{
    return glm::lookAt(position, position + getDirection(), getUp());
}

glm::mat4 Camera::getP() const
{
    return glm::perspective(fov, aspectRatio, near, far);
}

glm::mat4 Camera::getVP() const
{
    return getP() * getV();
}

glm::vec3 Camera::normalizeViewAngle(glm::vec3 angle)
{
    if (angle.x > glm::half_pi<float>())
        angle.x = glm::half_pi<float>();
    if (angle.x < -glm::half_pi<float>())
        angle.x = -glm::half_pi<float>();

    while (angle.y > glm::pi<float>())
        angle.y -= glm::two_pi<float>();
    while (angle.y < -glm::pi<float>())
        angle.y += glm::two_pi<float>();

    while (angle.z > glm::pi<float>())
        angle.z -= glm::two_pi<float>();
    while (angle.z < -glm::pi<float>())
        angle.z += glm::two_pi<float>();

    return angle;
}

glm::vec3 Camera::angleToDirection(glm::vec3 angle)
{
    float sinTheta = sinf(-angle.x);
    float cosTheta = cosf(-angle.x);
    float sinPhi = sinf(angle.y);
    float cosPhi = cosf(angle.y);
    return { cosPhi * cosTheta, sinTheta, sinPhi * cosTheta };
}

void Camera::setPosition(const glm::vec3 &value)
{
    position = value;
}

void Camera::setViewAngle(const glm::vec3 &value)
{
    viewAngle = normalizeViewAngle(value);
}

void Camera::setFov(const float &value)
{
    fov = value;
}

void Camera::setAspectRatio(const float &value)
{
    aspectRatio = value;
}

void Camera::setNear(const float &value)
{
    near = value;
}

void Camera::setFar(const float &value)
{
    far = value;
}

glm::vec3 Camera::getPosition() const
{
    return position;
}

glm::vec3 Camera::getViewAngle() const
{
    return viewAngle;
}

glm::vec3 Camera::getDirection() const
{
    return angleToDirection(viewAngle);
}

glm::vec3 Camera::getUp() const
{
    auto angle = viewAngle;
    angle.x -= glm::half_pi<float>();
    auto upWithoutRoll = angleToDirection(angle);

    if (fabs(viewAngle.z) < 0.01)
        return upWithoutRoll;
    glm::mat3 rotation = glm::rotate(glm::mat4(1.0f), viewAngle.z, getDirection());
    return angleToDirection(rotation * upWithoutRoll);
}

float Camera::getFov() const
{
    return fov;
}
