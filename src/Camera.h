#ifndef MAPTOOL_SRC_CAMERA_H
#define MAPTOOL_SRC_CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera {
public:
    Camera()
        : Camera({ 0.0f, 10.0f, 0.0f }, { glm::half_pi<float>(), 0.0f, 0.0f }, 65.0f, 1.0f, 1.0f, 100.0f)
    {
    }

    Camera(glm::vec3 position, glm::vec3 viewAngle, float fov, float aspectRatio, float near, float far)
        : position(position), viewAngle(normalizeViewAngle(viewAngle)), fov(fov), aspectRatio(aspectRatio), near(near), far(far)
    {
    }

    Camera(const Camera &obj)
        : Camera(obj.position, obj.viewAngle, obj.fov, obj.aspectRatio, obj.near, obj.far)
    {
    }

    Camera(const Camera &&obj) = delete;

    glm::mat4 getV() const;
    glm::mat4 getP() const;
    glm::mat4 getVP() const;

    void setPosition(const glm::vec3 &value);
    void setViewAngle(const glm::vec3 &value);
    void setFov(const float &value);
    void setAspectRatio(const float &value);
    void setNear(const float &value);
    void setFar(const float &value);

    glm::vec3 getPosition() const;
    glm::vec3 getViewAngle() const;
    glm::vec3 getDirection() const;
    glm::vec3 getUp() const;
    float getFov() const;

private:
    static glm::vec3 normalizeViewAngle(glm::vec3 angle);
    static glm::vec3 angleToDirection(glm::vec3 angle);

    glm::vec3 position;
    glm::vec3 viewAngle;
    float fov;
    float aspectRatio;
    float near;
    float far;
};

#endif //MAPTOOL_SRC_CAMERA_H
