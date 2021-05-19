HW2：
The second homework need to render a Utahpot
1. Read the triangle data from file, include color and triangle position
2. For each triangle, first do rasterization, using cross product to check which pixel should be rendered for current triangle.
3. Do depth test, if current pixel z buffer value larger than cuurent triangle pixel depth, render it, update z buffer value.
3. After all triangle being tested,  render the image.