

#pragma once

// #include <glad/glad.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// #include <vector>
// #include "math.hpp"

// // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
// enum Camera_Movement
// {
//     FORWARD,
//     BACKWARD,
//     LEFT,
//     RIGHT
// };

// // Default camera values
// const float YAW = -90.0f;
// const float PITCH = 0.0f;
// const float SPEED = 2.5f;
// const float SENSITIVITY = 0.1f;
// const float ZOOM = 45.0f;

// // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
// struct Camera
// {
//     // Euler Angles
//     F32 yaw;
//     F32 pitch;
//     // Camera options
//     F32 movementSpeed;
//     F32 mouseSensitivity;
//     F32 zoom;
//     // Camera Attributes
//     V3_T<F32> position;
//     V3_T<F32> front;
//     V3_T<F32> up;
//     V3_T<F32> right;
//     V3_T<F32> worldUp;

//     // Constructor with vectors
//     Camera(V3_T<F32> position = V3_T(0.0f, 0.0f, 0.0f),
//            V3_T<F32> up = V3_T(0.0f, 1.0f, 0.0f),
//            F32       yaw = YAW,
//            F32       pitch = PITCH)
//         : front(V3_T(0.0f, 0.0f, -1.0f)),
//           position(position),
//           worldUp(up),
//           yaw(yaw),
//           pitch(pitch)
//     {
//         movementSpeed = SPEED;
//         mouseSensitivity = SENSITIVITY;
//         zoom = 45;
//         updateCameraVectors();
//     }

//     // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
//     // M4_T<F32> GetViewMatrix()
//     glm::mat4 GetViewMatrix()
//     {
//         // return lookAt(position, position + front, up);
//         return glm::lookAt(toGLM(position), toGLM(position + front), toGLM(up));
//     }

//     // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
//     void ProcessKeyboard(Camera_Movement direction, float deltaTime)
//     {
//         float velocity = movementSpeed * deltaTime;
//         if (direction == FORWARD)
//             position = position + front * velocity;
//         if (direction == BACKWARD)
//             position = position + front * velocity;
//         if (direction == LEFT)
//             position = position + right * velocity;
//         if (direction == RIGHT)
//             position = position + right * velocity;
//     }

//     // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
//     void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
//     {
//         xoffset *= mouseSensitivity;
//         yoffset *= mouseSensitivity;

//         yaw += xoffset;
//         pitch += yoffset;

//         // Make sure that when pitch is out of bounds, screen doesn't get flipped
//         if (constrainPitch)
//         {
//             if (pitch > 89.0f)
//                 pitch = 89.0f;
//             if (pitch < -89.0f)
//                 pitch = -89.0f;
//         }

//         // Update Front, Right and Up Vectors using the updated Euler angles
//         updateCameraVectors();
//     }

//     // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
//     void ProcessMouseScroll(float yoffset)
//     {
//         if (zoom >= 1.0f && zoom <= 45.0f)
//             zoom -= yoffset;
//         if (zoom <= 1.0f)
//             zoom = 1.0f;
//         if (zoom >= 45.0f)
//             zoom = 45.0f;
//     }

//     // Calculates the front vector from the Camera's (updated) Euler Angles
//     void updateCameraVectors()
//     {
//         // Calculate the new Front vector
//         V3_T<F32> front;
//         front.x = cos(radians(yaw)) * cos(radians(pitch));
//         front.y = sin(radians(pitch));
//         front.z = sin(radians(yaw)) * cos(radians(pitch));
//         front = normalize(front);
//         // Also re-calculate the Right and Up vector
//         right = normalize(cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//         up = normalize(cross(right, front));
//     }
// };

V3 toM(glm::vec3 v)
{
    return V3(v.x, v.y, v.z);
}

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

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

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
struct Camera
{
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // Euler Angles
    F yaw;
    F pitch;
    // Camera options
    F movementSpeed;
    F mouseSensitivity;
    F zoom;

    // Constructor with vectors
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)),
          movementSpeed(SPEED),
          mouseSensitivity(SENSITIVITY),
          zoom(ZOOM),
          position(position),
          worldUp(up),
          yaw(yaw),
          pitch(pitch)
    {
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    M4 GetViewMatrix()
    {
        std::cout << "OUT: lookAt" << std::endl;
        std::cout << "OUT: glm" << std::endl;
        out(glm::lookAt(position, position + front, up));
        std::cout << "OUT: m" << std::endl;
        out(lookAt(toM(position), toM(position + front), toM(up)));
        return lookAt(toM(position), toM(position + front), toM(up));
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
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
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, front));
    }
};
