#include <iostream>
#define OUT(X) std::cout << X << std::endl;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// glad before glfw
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "math.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "types.hpp"

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>

// settings
const U32 SCR_WIDTH = 800;
const U32 SCR_HEIGHT = 600;
const F32 DRAW_DISTANCE = 200.0f;

// camera
Camera camera(V3(0.0f, 0.0f, 5.0f));

// timing
F32  deltaTime = 0.0f;
F32  lastFrame = 0.0f;
bool firstMouse = true;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

// glfw: callbacks
F32  lastX = SCR_WIDTH / 2.0f;
F32  lastY = SCR_HEIGHT / 2.0f;
void mouseCallback(GLFWwindow *window, F64 xpos, F64 ypos)
{
    F32 xoffset = xpos - lastX;
    F32 yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void glfw_framebufferSizeCallback(GLFWwindow *window, I32 width, I32 height)
{
    glViewport(0, 0, width, height);
}

static void glfw_scrollCallback(GLFWwindow *window, F64 xoffset, F64 yoffset)
{
    camera.processMouseScroll(yoffset);
}

static void glfw_errorCallback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

I32 main()
{
    std::cout << "start" << std::endl;

    // glfw: initialize and configure
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // enable vsync
    glfwSetFramebufferSizeCallback(window, glfw_framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, glfw_scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // imgui: setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Our state
    bool   show_demo_window = true;
    bool   show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // build and compile our shader zprog ram
    Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");
    shader.use();

    // load mesh
    Mesh mesh = Mesh("./assets/ball.obj");
    mesh.load();

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // per-frame time logic
        F32 currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        shader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        M4 projection = perspective(radians(camera.zoom), (F32)SCR_WIDTH / (F32)SCR_HEIGHT, 0.1f, DRAW_DISTANCE);
        shader.setM4("projection", projection);

        // camera/view transformation
        M4 view = camera.getViewMatrix();
        shader.setM4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        M4 model = M4(1.0f);  // make sure to initialize matrix to identity matrix first
        shader.setM4("model", model);

        // set light direction
        V3 light = normalize(V3(0.2, -1.0, 0.4));
        shader.setV3("light", light);

        mesh.draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}