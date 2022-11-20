#include "camera.hpp"


OrbitalCamera::OrbitalCamera(float fov, float distance, float rotateSpeed, float zoomSpeed, const glm::vec3 &worldUp, const glm::vec3 &target)
    : mWorldUp(worldUp),
      mTarget(target)  {
    mFOV = fov;
    mRadius = distance;
    mRotateSpeed = rotateSpeed;
    mZoomSpeed = zoomSpeed;
    mWorldUp = worldUp;
    mTarget = target;
    mYaw = M_PI / 2.0f;
    mPitch = 0.0f;
    updateVectors();
}

void
OrbitalCamera::Rotate(float dYaw, float dPitch, float dt) {
    dYaw *= mRotateSpeed * dt;
    dPitch *= mRotateSpeed * dt;

    mYaw -= dYaw;
    mPitch -= dPitch;
    if (mYaw > 2.0f * M_PI) {
        mYaw -= 2.0f * M_PI;
    }

    if (mYaw < 0.0f) {
        mYaw += 2.0f * M_PI;
    }

    if(mPitch > M_PI / 2.0f - 1e-4f) {
        mPitch = M_PI / 2.0f - 1e-4f;
    }
    if(mPitch < -M_PI / 2.0f + 1e-4f) {
        mPitch = -M_PI / 2.0f + 1e-4f;
    }
    updateVectors();
}

void
OrbitalCamera::Zoom(float dy, float dt)  {
    dy *= mZoomSpeed * dt;
    mRadius -= dy;
    if (mRadius <= 0.5f) {
        mRadius = 0.5f;
    }

    updateVectors();
}

void
OrbitalCamera::updateVectors() {
    // mPosition.X = mDistance * COS(mYaw) * COS(mPitch);
    // mPosition.Y = -mDistance * SIN(mPitch);
    // mPosition.Z = mDistance * SIN(mYaw) * COS(mPitch);
    mPosition = glm::vec3(mTarget.x + mRadius * cos(mYaw) * cos(mPitch),
        mTarget.y + mRadius * sin(mPitch),
        mTarget.z + mRadius * cos(mPitch) * sin(mYaw));
    
    mFront = glm::normalize(mTarget - mPosition);
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}