HW5：
The fifth homework need to apply UV mapping.
1. First for single pixels uv coordinate, we can't interpolate it on screen coordninate, instead we need do interpolation in perspective space.
2. replace ks, kd with 
2. Do gouraud shaing and phong shading, for gouraud shading, only interpolate color on every pixel. U need first calculate colors on three vertices.  
3. For phong shading, do normal interpolation, then apply phong shading model on it.
4. The differences between them is phong shading making the specular light part more clearly. 