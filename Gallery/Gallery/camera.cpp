#include "camera.hpp"

Camera::Camera() {
    mPosition = glm::vec3(0.0f);
    mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mRight = glm::vec3(1.0f, 0.0f, 0.0f);
    mVelocity = glm::vec3(0.0f);
    mPitch = 0.0f;
    mYaw = -90.0f;
    mMoveSpeed = 8.0f;
    mLookSpeed = 50.0f;
    mProjection = glm::mat4(1.0f);
    mView = glm::mat4(1.0f);
    mPlayerHeight = 2.0f;
    updateVectors();
}

void
Camera::Gravity(float dt) {
    mVelocity.y -= G * dt;
    mPosition += mVelocity * dt;
    updateVectors();
}

void
Camera::Move(MoveDirection dir, float dt) {
    float Velocity = mMoveSpeed * dt;

    switch (dir) {
        case MOVE_LEFT: mPosition -= Velocity * mRight; break;
        case MOVE_RIGHT:  mPosition += Velocity * mRight; break;
        case MOVE_BWD: mPosition -= Velocity * mFront; break;
        case MOVE_FWD: mPosition += Velocity * mFront; break;
    }

    updateVectors();
}

void 
Camera::Rotate(float dx, float dy, float dt) {
    float RotateVelocity = mLookSpeed * dt;
    mYaw += dx * RotateVelocity;
    mPitch += dy * RotateVelocity;

    if (mPitch > 89.0f) {
        mPitch = 89.0f;
    }
    if (mPitch < -89.0f) {
        mPitch = -89.0f;
    }

    updateVectors();
}

void 
Camera::UpdateProjection(int framebufferWidth, int framebufferHeight) {
    mProjection = glm::perspective(45.0f, framebufferWidth / (float)framebufferHeight, 0.1f, 100.0f);
}

void
Camera::UpdateView() {
    mView = glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void 
Camera::updateVectors() {
    mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront.y = sin(glm::radians(mPitch));
    mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(mFront);
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
    mPosition = glm::clamp(mPosition, glm::vec3(-15.0f, mPlayerHeight, -15.0f), glm::vec3(15.0f, mPlayerHeight, 15.0f));
}