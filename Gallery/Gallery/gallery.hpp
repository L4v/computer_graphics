#pragma once
#include "camera.hpp"

struct EngineState {
    bool mMoveForward;
    bool mMoveBackward;
    bool mMoveLeft;
    bool mMoveRight;
    bool mFirstMouse;
    glm::vec2 mCursorPos;
    float mDT;
    Camera* mCamera;

    EngineState(Camera* camera) : mDT(0.0f) {
        mCamera = camera;
        mFirstMouse = true;
        mMoveForward = false;
        mMoveBackward = false;
        mMoveLeft = false;
        mMoveRight = false;
    }
};