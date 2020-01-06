add_executable(
    main
    main.cpp
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
