

#pragma once

// #include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "math.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

template <typename T>
glm::vec3 toGLM(V3<T> v)
{
    return glm::vec3(v.x, v.y, v.z);
}

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
struct Camera
{
    // Euler Angles
    f32 yaw;
    f32 pitch;
    // Camera options
    f32 movementSpeed;
    f32 mouseSensitivity;
    f32 zoom;
    // Camera Attributes
    V3<f32> position;
    V3<f32> front;
    V3<f32> up;
    V3<f32> right;
    V3<f32> worldUp;

    // Constructor with vectors
    Camera(V3<f32> position = V3(0.0f, 0.0f, 0.0f),
           V3<f32> up = V3(0.0f, 1.0f, 0.0f),
           f32     yaw = YAW,
           f32     pitch = PITCH)
        : front(V3(0.0f, 0.0f, -1.0f)),
          position(position),
          worldUp(up),
          yaw(yaw),
          pitch(pitch)
    {
        movementSpeed = SPEED;
        mouseSensitivity = SENSITIVITY;
        zoom = 45;
        updateCameraVectors();
    }
    // Constructor with scalar values
    // Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    //     : front(V3(0.0f, 0.0f, -1.0f)),
    //       movementSpeed(SPEED),
    //       mouseSensitivity(SENSITIVITY),
    //       zoom(ZOOM),
    //       position(V3(posX, posY, posZ)),
    //       worldUp(V3(upX, upY, upZ)),
    //       yaw(yaw),
    //       pitch(pitch)
    // {
    //     updateCameraVectors();
    // }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    // M4<f32> GetViewMatrix()
    glm::mat4 GetViewMatrix()
    {
        // return lookAt(position, position + front, up);
        return glm::lookAt(toGLM(position), toGLM(position + front), toGLM(up));
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            position = position + front * velocity;
        if (direction == BACKWARD)
            position = position + front * velocity;
        if (direction == LEFT)
            position = position + right * velocity;
        if (direction == RIGHT)
            position = position + right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
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
    void ProcessMouseScroll(float yoffset)
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
        V3<f32> front;
        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        front = normalize(front);
        // Also re-calculate the Right and Up vector
        right = normalize(cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = normalize(cross(right, front));
    }
};
