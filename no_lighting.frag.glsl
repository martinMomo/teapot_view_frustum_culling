# version 120
/*
 * Michael Shafae
 * mshafae at fullerton.edu
 * 
 * A simple shader with no light sources.
 *
 * Be aware that for this course, we are limiting ourselves to
 * GLSL v.1.2. This is not at all the contemporary shading
 * programming environment, but it offers the greatest degree
 * of compatability.
 *
 * Please do not use syntax from GLSL > 1.2 for any homework
 * submission.
 *
 */

//varying vec3 myNormal;
//varying vec4 myVertex;

// These are passed in from the CPU program
//uniform mat4 modelViewMatrix;
//uniform mat4 projectionMatrix;
//uniform mat4 normalMatrix;

void main (void){    
  gl_FragColor = gl_Color;
}
