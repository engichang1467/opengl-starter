add_executable(
    main
    main.cpp
    imgui/imgui.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
)

target_link_libraries(
    main PRIVATE
    glad
    glfw
)

