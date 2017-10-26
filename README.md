# OpenGL_Practice
The homeworks of Computer Graphics.
## [Homework3](https://github.com/wy54224/OpenGL_Practice/tree/47abb6633b2af1c4d1f2697f5d231b10dd57e2fc)
- Use opengl to draw a rectangle  
- Refer to [learnopengl.com](https://learnopengl.com/#!Getting-started/Shaders) and create a shader class  
- Define a uniform in fragment shader to change the color of rectangle arbitrarily  
- Use dear ImGUI to create a simple main menu bar  
  
<img width="500" src="https://raw.githubusercontent.com/wiki/wy54224/OpenGL_Practice/homework3_1.png"/>  

## [Homework4](https://github.com/wy54224/OpenGL_Practice/tree/1276e3e9fd454e7018535a8801ddca206b401175)
- Learn the rasterization algorithm  
- Line rasterization: [DDA](https://github.com/wy54224/OpenGL_Practice/wiki/Rasterization-Algorithm#DititalDifferentialAnalyzer), [Bresenham Algorithm](https://github.com/wy54224/OpenGL_Practice/wiki/Rasterization-Algorithm#BresenhamAlgorithm)  
- Triangle rasterization: [Edge Walking](https://github.com/wy54224/OpenGL_Practice/wiki/Rasterization-Algorithm#EdgeWalking), [Edge Equations](https://github.com/wy54224/OpenGL_Practice/wiki/Rasterization-Algorithm#EdgeEquations)  
- Circle rasterization: [Circle Bresenham Algorithm](https://github.com/wy54224/OpenGL_Practice/wiki/Rasterization-Algorithm#CircleBresenhamAlgorithm)  
- All code has impletemented in file [rasterization.h](https://github.com/wy54224/OpenGL_Practice/blob/1276e3e9fd454e7018535a8801ddca206b401175/rasterization.h) 
 
## [Homework5](https://github.com/wy54224/OpenGL_Practice/tree/0a7c5686bca611f7c5cd7afb983d5ec5dffe4515)
- Learn matrix transformations
- Implement rotate, translate and scale transformation
- Use this transformation to make a significant animation(The usage of texture)
  
<img width="500" src="https://raw.githubusercontent.com/wiki/wy54224/OpenGL_Practice/MoonAroundEarth.png"/>  

- Complete the rotate, translate and scale matrix in vertex shader(Though it may seem foolish), just for practice GLSL

## [Homework6](https://github.com/wy54224/OpenGL_Practice/tree/1c1f8abc69c751c35b0bd6af9b1c48ec79838d4b)
- Learn orthogonal projection and perspective projection
- Use quaternion to create a camera class
- What is [quaternion](https://github.com/wy54224/OpenGL_Practice/wiki/quaternion)?