# Ray Tracer - Graphics and Visualization Final Project

![Ray Tracing Example](images/final.png)  
*Example scene rendered with shadows, reflections, and complex materials*

## Overview  
This project is a fully functional CPU-based ray tracer developed as the final assignment for the **Graphics and Visualization** course. Ray tracing simulates the physical behavior of light to generate photorealistic images by tracing rays from the camera through each pixel and calculating interactions with objects in a 3D scene. Built in C++, this implementation includes advanced features like recursive reflections, dynamic shadows, and optimized acceleration structures.

## Key Features  
### 🎨 Core Ray Tracing  
- Perspective camera model with configurable FOV, position, and orientation  
- Sphere and triangle primitives with Blinn-Phong shading  
- Soft shadows via shadow rays  
- Recursive reflections for mirror-like surfaces  
- Gamma correction and 8-bit color quantization  

### ⚡ Optimizations  
- **BVH (Bounding Volume Hierarchy)** acceleration structure to reduce ray-object intersection tests by 40-60%  
- **Wavefront OBJ Parser** for importing complex 3D models (e.g., Stanford Bunny)  
- Multi-threaded rendering support  

### 🧩 Extended Functionality  
- Spherical texture mapping  
- Support for directional and point light sources  
- Scene configuration via JSON files  

---

## Project Structure  
