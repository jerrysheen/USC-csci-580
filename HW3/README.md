HW3：
The third homework need to perform transformation on the utahpot.
1. Read the triangle data from file, include color and triangle position
2. Define Xsp -> Xip -> Xmi, the first one translate object from perspective space to image space, the second transform it from image space to
pespective space, the last on change the model from model space to image space.
3. Estabilish a matirx stack, which contains all the matirx operation, Every time add new transformation, the stack will update.
4. First change current triangles to homogeneous coordinate form, multiply it with our current matrix stack(do transform to sceen space.). Then multiple the first three x y z with the last w.
5. Render the image.