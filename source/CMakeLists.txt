add_executable(
    main
    main.cpp
    imgui.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
)

target_link_libraries(
    main PRIVATE
    glad
    glfw
)

# target_include_directories(
#     main PUBLIC
#     ../extern/glad
# )
