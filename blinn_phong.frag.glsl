# version 120
/*
 * Michael Shafae
 * mshafae at fullerton.edu
 * 
 * A simple Phong shader with two light sources.
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

varying vec3 myNormal;
varying vec4 myVertex;

// These are passed in from the CPU program
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
// Information about the lights
uniform vec4 light0_position;
uniform vec4 light0_color;
uniform vec4 light1_position;
uniform vec4 light1_color;
// Material properties
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

vec4 computeLight(const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 reflection){

  float nDotL = dot(normal, direction);
  vec4 lambert = diffuse * lightcolor * max(nDotL, 0.0);

  float nDotR = dot(normal, reflection);
  vec4 phong = specular * lightcolor * pow(max(nDotR, 0.0), shininess);

  vec4 retval = lambert + phong;
  return retval;
}       

void main (void){

  // They eye is always at (0,0,0) looking down -z axis 
  // Also compute current fragment position and direction to eye 

  const vec3 eyepos = vec3(0,0,0);
  vec4 _mypos = modelViewMatrix * myVertex;
  vec3 mypos = _mypos.xyz / _mypos.w;
  vec3 eyedirn = normalize(eyepos - mypos);

  // Compute normal, needed for shading. 
  vec4 _normal = normalMatrix * vec4(myNormal, 0.0);
  vec3 normal = normalize(_normal.xyz);

  // Light 0, point
  vec3 position0 = light0_position.xyz / light0_position.w;
  vec3 direction0 = normalize(position0 - mypos);
  vec3 half0 = normalize(direction0 + eyedirn);

  vec4 color0 = computeLight(direction0, light0_color, normal, half0) ;

  // Light 1, point 
  vec3 position1 = light1_position.xyz / light1_position.w;
  vec3 direction1 = normalize(position1 - mypos);
  vec3 half1 = normalize(direction1 + eyedirn); 

  vec4 color1 = computeLight(direction1, light1_color, normal, half1) ;
    
  gl_FragColor = ambient + color0 + color1;
}
