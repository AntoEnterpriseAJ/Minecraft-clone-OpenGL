# Minecraft-Style OpenGL Voxel Engine

A C++ project that implements a basic Minecraft-style voxel world using Modern OpenGL. This repository includes classes for **chunks**, **blocks**, **camera**, **world generation**, **skybox**, **lighting** (sun), and a simple UI element (crosshair).  
**It also features multi-threaded chunk loading**, allowing chunk generation to occur asynchronously without blocking the main thread.

![image](https://github.com/user-attachments/assets/9b2410a8-1cee-415a-8d28-a5ade58a29a0)

---

## Features

- **Voxel/Block System**  
  - Blocks have different types: Grass, Water, Stone, etc.  
  - Chunk-based system for efficient rendering of large worlds.  
  - Simple tree generation within chunks.

- **Procedural World Generation**  
  - Basic heightmap-based terrain generation for each chunk.  
  - **Multi-threaded chunk loading** using `std::async`, so the chunk generation and mesh creation happen asynchronously while the main game loop continues.

- **Camera and Movement**  
  - Free-fly camera with support for survival, creative, and spectator modes (expandable).  
  - Supports sprinting, collision checks, and vertical movement.

- **Lighting & Skybox**  
  - A skybox rendering system providing a 360° environment background.  
  - A simple sun cube that moves over time, simulating daylight progression.

- **UI Components**  
  - Crosshair rendering at the center of the screen.

- **Chunk Mesh Generation**  
  - Each chunk is converted into a single mesh for efficient batch rendering.  
  - Neighboring chunk faces are accounted for to avoid rendering hidden block faces.

---

## Screenshots

![image](https://github.com/user-attachments/assets/9dee7388-5afd-4b34-88af-10319cbdfaf8)  
![image](https://github.com/user-attachments/assets/e67d4531-de98-442f-9ee9-0468e9b6a363)

---

## Dependencies

This project is **plug-and-play** — all necessary libraries have already been downloaded or included within the repository itself:

1. **GLFW** — for window/context creation and input handling.
2. **GLAD** — for loading OpenGL function pointers.
3. **GLM** — math library for vectors and matrices.
4. **stb_image.h** — for loading textures.
5. A **C++17**-compatible compiler (MSVC, Clang, or GCC).

---

## Contributing

This project was created for study purposes. Contributions are welcome but not actively sought.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
