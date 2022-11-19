#pragma once
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class OrbitalCamera {
public:
    float mFOV;
    float mPitch;
    float mYaw;
    float mRotateSpeed;
    float mRadius;
    float mZoomSpeed;
    glm::vec3 mWorldUp;
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mTarget;

    OrbitalCamera(float fov, float distance, float rotateSpeed = 10.0f, float zoomSpeed = 10.0f, const glm::vec3 &worldUp = glm::vec3(0.0f, 1.0f, 0.0f), const glm::vec3 &target = glm::vec3(0.0f));
    void Rotate(float dYaw, float dPitch, float dt);
    void Zoom(float dy, float dt);

private:
    void updateVectors();
};