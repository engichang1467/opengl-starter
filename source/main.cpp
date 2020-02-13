#include <iostream>
#define OUT(X) std::cout << X << std::endl;

// glad before glfw
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "math.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "types.hpp"

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stdio.h>

// settings
const U32 SCR_WIDTH = 1366;
const U32 SCR_HEIGHT = 768;
const F32 DRAW_DISTANCE = 200.0f;
V3        color = V3(0.7f, 0.3f, 0.4f);

struct State
{
    F32  zoom = 5;
    F32  rotationSpeed = 0.1;
    F32  rotation = 0;
    V3   translation = V3(0, 0, 0);
    bool showWireframe = true;
    bool isSmooth = false;
    bool isFirstFrame = true;
    I32  selectedModelIndex = 4;
};

static State state = State();

struct Time
{
    F32 current;
    F32 delta;

    Time() : current(0.0f), delta(0.0f) {}
    Time(F32 time) : current(time), delta(0.0f) {}

    void update(F32 time)
    {
        delta = time - current;
        current = time;
    }
};

static Time t = Time();
V3          cameraPosition = normalize(V3(0.0f, 1.0f, 2.0f));
Camera      camera(cameraPosition, V3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, t.delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, t.delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, t.delta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, t.delta);
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
    // glfwSetCursorPosCallback(window, mouseCallback);
    // glfwSetScrollCallback(window, glfw_scrollCallback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // build and compile our shader zprog ram
    Shader shader = Shader("shaders/shader.vert", "shaders/shader.frag");
    shader.use();
    shader.setV3("color", color);

    // load mesh
    WingedEdgeMesh mesh;

    // imgui: state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    t.update(glfwGetTime());

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // time
        t.update(glfwGetTime());

        // input
        processInput(window);

        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // imgui: create frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Assignment 1");

        ImGui::SliderFloat("zoom", &state.zoom, 0.0f, 10.0f);

        ImGui::SliderFloat("rotation speed", &state.rotationSpeed, 0.0f, 1.0f);
        ImGui::SliderFloat("rotate", &state.rotation, 0.0f, 1.0f);

        ImGui::SliderFloat("translate x", &state.translation.x, -5.0f, 5.0f);
        ImGui::SliderFloat("translate y", &state.translation.y, -5.0f, 5.0f);
        ImGui::SliderFloat("translate z", &state.translation.z, -5.0f, 5.0f);

        ImGui::Checkbox("Show Wireframe", &state.showWireframe);
        if (ImGui::Checkbox("Smooth", &state.isSmooth))
        {
            mesh.load(state.isSmooth);
        }

        const char *models[] = {
            "horse.obj",
            "horse_s.obj",
            "venus.obj",
            "wheel.obj",
            "chess_piece.obj",
            "tet.obj"};

        if (ImGui::Combo("model", &state.selectedModelIndex, models, IM_ARRAYSIZE(models)) || state.isFirstFrame)
        {
            mesh = WingedEdgeMesh("assets/" + std::string(models[state.selectedModelIndex]));
            mesh.load(state.isSmooth);
        }

        if (ImGui::Button("subdivide"))
        {
            mesh.subdivide();
            mesh.load(state.isSmooth);
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // scene
        state.rotation = state.rotation + state.rotationSpeed * t.delta;
        if (1 < state.rotation) state.rotation = 0;
        mesh.m = rotationY(state.rotation * 2 * PI);
        mesh.m.translate(state.translation);

        shader.setM4("model", mesh.m);

        camera.position = cameraPosition * state.zoom;
        M4 view = camera.getViewMatrix();
        shader.setM4("view", view);

        M4 projection = perspective(radians(camera.zoom), (F32)SCR_WIDTH / (F32)SCR_HEIGHT, 0.1f, DRAW_DISTANCE);
        shader.setM4("projection", projection);

        V3 light = normalize(V3(0.2f, -1.0f, -0.4f));
        shader.setV3("light", light);

        shader.setBool("showWireframe", state.showWireframe);

        mesh.draw();

        // imgui: render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();

        state.isFirstFrame = false;
    }

    glfwTerminate();
    return 0;
}