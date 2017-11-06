# version 120 
/*
 * Michael Shafae
 * mshafae at fullerton.edu
 * 
 * A simple shader with no sources.
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

// These are passed in from the CPU program
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;


// These are variables that we wish to send to our fragment shader
// In later versions of GLSL, these are 'out' variables.
//varying vec3 myNormal;
//varying vec4 myVertex;

void main() {
  gl_FrontColor = gl_Color;
  gl_Position = projectionMatrix * modelViewMatrix * gl_Vertex;
  //myNormal = gl_Normal;
  //myVertex = gl_Vertex;
}
