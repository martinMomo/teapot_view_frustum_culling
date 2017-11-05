// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// A camera class.
//
//

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/normalize_dot.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef _CAMERA_H_
#define _CAMERA_H_

class Camera{
private:
  float _rotationDelta;

public:
  glm::vec3 eyePosition;
  glm::vec3 upVector;
  glm::vec3 lookAt;
  float fovy;
  float near;
  float far;


  Camera(glm::vec3 position, glm::vec3 up, glm::vec3 la, float fieldOfViewInY, float n, float f):_rotationDelta(0.05), eyePosition(position), upVector(up), lookAt(la), fovy(fieldOfViewInY), near(n), far(f){
  }

  Camera( ){ };
  ~Camera( ){}

  glm::vec3 gaze( ){
    // this will fail miserably if gaze is a zero length vector.
    glm::vec3 gaze = glm::normalize(lookAt - eyePosition);
    return gaze;
  }

  glm::vec3 right( ){
    glm::vec3 r = glm::cross(gaze( ), upVector);
    return r / glm::length(r);
  }

  void forward( ){
    glm::vec3 g = gaze();
    glm::mat4 m = glm::translate(g * _rotationDelta);
    eyePosition = m * glm::vec4(eyePosition, 1);
  }

  void backward( ){
    glm::vec3 g = gaze();
    glm::mat4 m = glm::translate(g * -_rotationDelta);
    eyePosition = m * glm::vec4(eyePosition, 1);
  }

  void panLeft( ){
    // move everything to the origin
    glm::vec3 __lookAt = lookAt - eyePosition;
    // transform
    glm::mat4 m = glm::rotate(_rotationDelta, upVector);
    __lookAt = m * glm::vec4(__lookAt, 1.0);
    // Move everything back
    lookAt = __lookAt + eyePosition;
  }

  void panRight( ){
    // move everything to the origin
    glm::vec3 __lookAt = lookAt - eyePosition;
    // transform
    glm::mat4 m = glm::rotate(-_rotationDelta, upVector);
    __lookAt = m * glm::vec4(__lookAt, 1.0);
    // Move everything back
    lookAt = __lookAt + eyePosition;
  }

  void rotateCameraLeft( ){
    glm::vec3 f = glm::normalize(-eyePosition);
    glm::vec3 _up = glm::normalize(upVector);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, u);
    eyePosition = m * eyePosition;
  }

  void rotateCameraRight( ){
    glm::vec3 f = glm::normalize(-eyePosition);
    glm::vec3 _up = glm::normalize(upVector);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(-_rotationDelta, u);
    eyePosition = m * eyePosition;
  }

  void rotateCameraUp( ){
    glm::vec3 r = right( );
    glm::mat3 m = glm::rotate(-_rotationDelta, r);
    /*
    glm::vec3 f = glm::normalize(-eyePosition);
    glm::vec3 _up = glm::normalize(upVector);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(-_rotationDelta, s);
    */
    upVector = m * upVector;
    eyePosition = m * eyePosition;
  }

  void rotateCameraDown( ){
    glm::vec3 f = glm::normalize(-eyePosition);
    glm::vec3 _up = glm::normalize(upVector);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, s);
    upVector = m * u;
    eyePosition = m * eyePosition;
  }

  void perspectiveMatrix(glm::mat4& m, float windowAspectRatio){
    m = glm::perspective(fovy, windowAspectRatio, near, far);
  }

  void lookAtMatrix(glm::mat4& m){
    m = glm::lookAt(eyePosition, lookAt, upVector);
  }

  void debug( ){
    glm::vec3 gaze = lookAt - eyePosition;
    std::cerr << "position " << glm::to_string(eyePosition) << "(" << glm::length(eyePosition) << ")" << "\nlook at " << glm::to_string(lookAt) << "\ngaze " << glm::to_string(gaze) << "(" << glm::length(gaze) << ")" << "\nup " << glm::to_string(upVector) << "(" << glm::length(upVector) << ")" << std::endl << std::endl;
  }

};

#endif
