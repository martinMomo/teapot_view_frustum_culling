Teapot Vision Master

Earl Martin Momongan
techwiz@csu.fullerton.edu

This assignment's goal was to practice and learn about view frustrum culling. The program would randomly generate 20 teapots throughout the screen. If coded correctly, whenever a teapot's center would leave the view frustrum of the camera, it would be culled and removed from rendering. It would be rendered again once it re-enters the view frustrum. There is also a bird's-eye-view camera setting that allows you to see which teapots are being rendered in to view by changing the shader from white to red.

The program works by running through a for-loop to iterate through all 20 teapots to check their current positions. The position of an indiviudal teapot is stored in vec4 position. The position is then multiplied against the lookAtMatrix of the main camera and the result stored again in position. The program proceeds to mutiply position against the clipPlaneMatrix and the resulting value is stored one last time in position. Using this data, the program iterates through each teapot comparing it's x, y, and z values, making sure it is in between -w and w. If these comparisons are met, it is within the view frustrum and is tagged true so it can be rendered. If not, then it is outside of the view frustrum  and is instead tagged false.

The function only checks to see if the center point of the teapot is within the view frustrum. When the teapot's center leaves the view frustrum, it will instantaneously disappear from the camera view. The function is called in the bool render() function and uses it to determine whether it needs to display the teapot or not. As of last testing, there is not listed bugs with the program and works as intended.
