HW4：
The fourth homework need to perform phong/Gouraud shading on the utahpot.
1. first load the data and do linear normal, color interpolation (only on screen space, not very accurate, but since color is still continuious, So eyes may can't tell any differences.)
2. Do gouraud shaing and phong shading, for gouraud shading, only interpolate color on every pixel. U need first calculate colors on three vertices.  
3. For phong shading, do normal interpolation, then apply phong shading model on it.
4. The differences between them is phong shading making the specular light part more clearly. 