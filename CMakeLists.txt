cmake_minimum_required(VERSION 3.20)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3
find_package(SDL3 REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenImageDenoise REQUIRED)

# All source files
set(SOURCES
    src/main.cpp

    src/core/application.cpp
    src/core/renderer.cpp
    src/core/TileScheduler.cpp

    src/scene/scene.cpp
    src/scene/environment.cpp
    src/scene/TransformNode.cpp
    src/scene/spherelight.cpp

    src/math/vector3.cpp
    src/math/ray.cpp
    src/math/random.cpp
    src/math/Matrix.cpp
    src/math/Transform.cpp

    src/camera/camera.cpp

    src/integrator/integrator.cpp

    src/rendering/framebuffer.cpp
    src/rendering/presenter.cpp
    src/rendering/AccumulationBuffer.cpp
    src/rendering/denoiser.cpp

    src/primitives/sphere.cpp
    src/primitives/triangle.cpp
    src/primitives/mesh.cpp

    src/material/material.cpp
    src/material/lambertian.cpp
    src/material/metal.cpp
    src/material/glass.cpp
    src/material/emissive.cpp
    src/material/solidcolor.cpp
    src/material/checkerboard.cpp
    src/material/imagetexture.cpp

    src/acceleration/aabb.cpp
    src/acceleration/BVHNode.cpp

    src/loaders/OBJLoader.cpp
)

add_executable(RayTracer ${SOURCES})

target_include_directories(RayTracer PRIVATE
    src
    third_party/stb
)

target_link_libraries(RayTracer PRIVATE SDL3::SDL3 Threads::Threads OpenImageDenoise)
