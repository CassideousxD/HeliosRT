# RayTracer

A physically based CPU path tracer written in modern C++ using SDL3.

This project was built from scratch to understand the mathematics, rendering algorithms, and architecture behind modern offline renderers. Rather than relying on graphics APIs like OpenGL or Vulkan for rasterization, every pixel is computed through recursive Monte Carlo path tracing.

The renderer currently supports physically based materials, HDR environment lighting, OBJ mesh loading, BVH acceleration, progressive rendering, multithreading, Intel Open Image Denoise integration, and more.

---

## Features

### Core Rendering

- Physically Based Path Tracer
- Recursive Monte Carlo Integration
- Progressive Rendering
- Multi-threaded Tile Renderer
- Configurable Samples Per Pixel
- Configurable Maximum Ray Depth

### Acceleration

- Bounding Volume Hierarchy (BVH)
- Axis-Aligned Bounding Boxes (AABB)
- Automatic BVH Construction

### Camera

- Perspective Camera
- Anti-Aliasing
- Depth of Field (Thin Lens Camera)

### Geometry

- Sphere Primitive
- Triangle Primitive
- OBJ Mesh Loader

### Materials

- Lambertian Diffuse
- Metal
- Dielectric (Glass)
- Emissive Materials

### Textures

- Solid Color
- Checkerboard Texture
- Image Textures

### Lighting

- HDR Environment Lighting
- Sky Gradient Environment

### Rendering

- Progressive Accumulation Buffer
- Frame Buffer
- Tile Scheduler
- Intel Open Image Denoise Integration

### Utilities

- Render Statistics
- SDL3 Display
- Modular Renderer Architecture

---

## Project Structure

```text
src/
├── acceleration/
├── camera/
├── core/
├── integrator/
├── material/
├── math/
├── primitives/
├── rendering/
├── scene/
└── main.cpp
```

Each subsystem has been designed to remain independent and extensible.

---

# Dependencies

- C++17 or newer
- CMake 3.16+
- SDL3
- Intel Open Image Denoise (OIDN)

---

# Building

Clone the repository

```bash
git clone <repository-url>
cd RayTracer
```

Create a build directory

```bash
mkdir build
cd build
```

Generate build files

```bash
cmake ..
```

Compile

```bash
cmake --build .
```

Run

```bash
./RayTracer
```

---

# SDL3

Install SDL3 before building.

On macOS (Homebrew)

```bash
brew install sdl3
```

If SDL3 is installed in a custom location, update the paths inside the project's `CMakeLists.txt`.

---

# Intel Open Image Denoise

Install Intel Open Image Denoise and update the include/library paths inside `CMakeLists.txt` if necessary.

Example locations:

```text
OIDN_INCLUDE_DIR
OIDN_LIBRARY
```

---

# Loading OBJ Models

OBJ meshes can be loaded through the OBJ loader.

If your project stores assets in a different location, update the OBJ path inside the scene creation code.

Example

```cpp
scene.loadOBJ("assets/models/dragon.obj", material);
```

---

# Loading Textures

Image textures are loaded from disk.

Example

```cpp
auto texture = std::make_shared<ImageTexture>(
    "assets/textures/wood.jpg"
);
```

If your texture directory changes, simply update the file path.

---

# Loading HDR Environments

HDR environment maps are loaded similarly.

Example

```cpp
scene.setEnvironment(
    std::make_shared<HDREnvironment>(
        "assets/hdr/studio.hdr"
    )
);
```

Replace the HDR file path with your own environment map.

---

# Renderer Configuration

Rendering parameters are stored inside

```text
src/core/RenderSettings.h
```

Typical parameters include

- Image Width
- Image Height
- Samples Per Pixel
- Maximum Ray Depth
- Thread Count
- Window Title

---

# Adding New Objects

Objects are added to the scene before rendering.

Example

```cpp
scene.addObject(
    std::make_shared<Sphere>(
        center,
        radius,
        material
    )
);
```

After all objects are added, build the BVH.

```cpp
scene.buildBVH();
```

---

# Performance Notes

The renderer uses

- Progressive Rendering
- Multi-threaded Tile Scheduling
- BVH Acceleration
- Intel Open Image Denoise

Large OBJ meshes may take additional time to build the BVH before rendering begins.

---

# Current Capabilities

- Recursive Path Tracing
- HDR Lighting
- OBJ Mesh Rendering
- BVH Acceleration
- Depth of Field
- Glass Refraction
- Metallic Reflection
- Emissive Materials
- Texture Mapping
- Progressive Rendering
- Intel Open Image Denoise

---

# Planned Features

Renderer v2

- Motion Blur
- Area Lights
- Next Event Estimation
- Multiple Importance Sampling
- GGX Microfacet BRDF
- Adaptive Sampling
- Volumetric Rendering
- Mesh-local BVH
- Transform System
- Instancing

---

# Learning Goals

This project was developed as a learning exercise to explore

- Computer Graphics
- Physically Based Rendering
- Monte Carlo Integration
- Ray Tracing
- Path Tracing
- Rendering Acceleration Structures
- Material Systems
- Texture Mapping
- Offline Rendering Architecture

The implementation focuses on clarity, modularity, and extensibility while remaining faithful to the concepts used in production renderers.

---

# License

This project is released under the MIT License.