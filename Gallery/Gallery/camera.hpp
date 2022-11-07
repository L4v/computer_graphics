#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define G 9.81f

class Camera {
public:
    enum MoveDirection {
        MOVE_LEFT = 0,
        MOVE_RIGHT,
        MOVE_BWD,
        MOVE_FWD
    };

    glm::mat4 mProjection;
    glm::mat4 mView;

    glm::vec3 mWorldUp;
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;

    glm::vec3 mVelocity;

    float mMoveSpeed;
    float mLookSpeed;
    float mPitch;
    float mYaw;
    float mPlayerHeight; // Should be moved out

    Camera();

    void Gravity(float dt);
    void Move(MoveDirection dir, float dt);
    void Rotate(float dx, float dy, float dt);
    void UpdateProjection(int framebufferWidth, int framebufferHeight);
    void UpdateView();

private:
    void updateVectors();
};