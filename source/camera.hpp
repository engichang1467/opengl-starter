#pragma once

#include "math.hpp"
#include "types.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const F32 YAW = -90.0f;
const F32 PITCH = 0.0f;
const F32 SPEED = 10.0f;
const F32 SENSITIVITY = 0.1f;
const F32 ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
struct Camera
{
    // Camera Attributes
    V3 position;
    V3 front;
    V3 up;
    V3 right;
    V3 worldUp;
    // Euler Angles
    F32 yaw;
    F32 pitch;
    // Camera options
    F32 movementSpeed;
    F32 mouseSensitivity;
    F32 zoom;

    // Constructor with vectors
    Camera(V3 position = V3(0.0f, 0.0f, 0.0f), V3 worldUp = V3(0.0f, 1.0f, 0.0f), F32 yaw = YAW, F32 pitch = PITCH)
        : position(position),
          yaw(yaw),
          worldUp(worldUp),
          front(V3(0.0f, 0.0f, -1.0f)),
          pitch(pitch),
          movementSpeed(SPEED),
          mouseSensitivity(SENSITIVITY),
          zoom(ZOOM)
    {
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    M4 getViewMatrix()
    {
        return lookAt(position, position + front, up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(CameraMovement direction, F32 deltaTime)
    {
        F32 velocity = movementSpeed * deltaTime;
        if (direction == FORWARD) position = position + front * velocity;
        if (direction == BACKWARD) position = position - front * velocity;
        if (direction == RIGHT) position = position + right * velocity;
        if (direction == LEFT) position = position - right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(F32 xoffset, F32 yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(F32 yoffset)
    {
        if (zoom >= 1.0f && zoom <= 45.0f)
            zoom -= yoffset;
        if (zoom <= 1.0f)
            zoom = 1.0f;
        if (zoom >= 45.0f)
            zoom = 45.0f;
    }

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        V3 newFront;
        newFront.x = cos(radians(yaw)) * cos(radians(pitch));
        newFront.y = sin(radians(pitch));
        newFront.z = sin(radians(yaw)) * cos(radians(pitch));
        front = normalize(newFront);
        // Also re-calculate the Right and Up vector
        right = normalize(cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = normalize(cross(right, front));
    }
};
