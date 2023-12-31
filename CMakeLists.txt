cmake_minimum_required(VERSION 3.0)

project(CPURayTracingGUI)

file(GLOB EDITOR_FILES
    src/editor/*.cpp
    src/editor/*.hpp
)


add_executable(
    ${CMAKE_PROJECT_NAME}
    main.cpp
    src/MainLayer.hpp
    src/MainLayer.cpp
    src/ImGuiRT.hpp
    src/ImGuiUtils.hpp
    src/ImGuiUtils.cpp
    src/CameraController.hpp
    src/CameraController.cpp
    src/GeometricObjectEditors.hpp
    src/GeometricObjectEditors.cpp
    ${EDITOR_FILES}
)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set(WOLF_ENGINE_RELATIVE_TO_ROOT_PATH "dependencies/WolfEngine/")

add_subdirectory(dependencies/WolfEngine)
add_subdirectory(dependencies/CPU-Ray-Tracing)

target_link_libraries(${PROJECT_NAME} wolf_engine)
target_link_libraries(${PROJECT_NAME} cpu_raytracer)
target_include_directories(${PROJECT_NAME} PRIVATE "./dependencies/")

# Outputs the test executable in the build folder
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)