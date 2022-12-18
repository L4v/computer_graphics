/**
 * @file camera.hpp
 * @author Jovan Ivosevic
 * @brief FPS Camera class
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    /**
     * @brief Moves camera in specified direction
     * 
     * @param dir Direction
     * @param dt Delta time
     */
    void Move(float dx, float dy, float dt);
    /**
     * @brief Rotates camera depending on difference between previous and current cursor position
     *
     * @param dx Delta x
     * @param dy Delta y
     * @param dt Delta time
     */
    void Rotate(float dx, float dy, float dt);

    /**
     * @brief Returns position vector
     *
     * @returns Position vector
     */
    glm::vec3 GetPosition();

    /**
     * @brief Returns target vector
     *
     * @returns Target vector
     */
    glm::vec3 GetTarget();

    /**
     * @brief Returns up vector
     *
     * @returns Up vector
     */
    glm::vec3 GetUp();


private:
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
    void updateVectors();
};