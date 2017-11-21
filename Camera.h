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
#include <glm/gtc/type_ptr.hpp>

#include "utilities.h"

#ifndef _CAMERA_H_
#define _CAMERA_H_

class Camera{
private:
  float _rotationDelta;

public:
  glm::vec3 eyePosition;
  glm::vec3 upVector;
  glm::vec3 lookAt;
  // fovy in degrees!!
  float fovy;
  float near;
  float far;


  Camera(glm::vec3 position, glm::vec3 up, glm::vec3 la, float fieldOfViewInY, float n, float f):eyePosition(position), upVector(up), lookAt(la), fovy(fieldOfViewInY), near(n), far(f){
    _rotationDelta = deg2rad(1.0);
  }

  Camera( ){ }
  ~Camera( ){ }

  void draw( ){
    // unit cube; centered at origin, axis aligned
    glBegin(GL_QUADS);
    // Top
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    // Bottom
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    // Front
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    // Back
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    // Left
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    // Right
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glEnd( );
  }

  float halfHeightNear( ){
    float fovy_rads = deg2rad(fovy);
    float hh = tan(fovy_rads / 2.0) * near;
    return hh;
  }

  float halfWidthNear(float windowAspectRatio){
    return halfHeightNear( ) * windowAspectRatio;
  }

  float halfHeightFar( ){
    float fovy_rads = fovy * (M_PI / 180.0);
    float hh = tan(fovy_rads / 2.0) * far;
    return hh;
  }

  float halfWidthFar(float windowAspectRatio){
    return halfHeightFar( ) * windowAspectRatio;
  }

  void drawViewFrustumX(float windowAspectRatio){

  }

  void drawViewFrustum(float windowAspectRatio){
    float hNear = halfHeightNear( );
    float hFar = halfHeightFar( );
    float wNear = halfWidthNear(windowAspectRatio);
    float wFar = halfWidthFar(windowAspectRatio);

    glm::vec3 c = glm::vec3(0, 0, 0);

    glm::vec3 nearCenter = c + gaze( ) * near;
    glm::vec3 farCenter = c + gaze( ) * far;
    glm::vec3 right = this->right( );


    glm::vec3 nur = nearCenter + (upVector * hNear) + (right * wNear);
    glm::vec3 nul = nearCenter + (upVector * hNear) - (right * wNear);
    glm::vec3 nll = nearCenter - (upVector * hNear) - (right * wNear);
    glm::vec3 nlr = nearCenter - (upVector * hNear) + (right * wNear);


    glm::vec3 fur = farCenter + (upVector * hFar) + (right * wFar);
    glm::vec3 ful = farCenter + (upVector * hFar) - (right * wFar);
    glm::vec3 fll = farCenter - (upVector * hFar) - (right * wFar);
    glm::vec3 flr = farCenter - (upVector * hFar) + (right * wFar);

    /*
    std::cerr << "eye position: " << glm::to_string(eyePosition) << std::endl;
    std::cerr << "gaze: " << glm::to_string(gaze( )) << std::endl;
    std::cerr << "fovy: " << glm::to_string(upVector) << std::endl;
    std::cerr << "fovy: " << fovy << std::endl;
    std::cerr << "near: " << near << std::endl;
    std::cerr << "far: " << far << std::endl;
    std::cerr << "nur: " << glm::to_string(nur) << std::endl;
    std::cerr << "nul: " << glm::to_string(nul) << std::endl;
    std::cerr << "nll: " << glm::to_string(nll) << std::endl;
    std::cerr << "nlr: " << glm::to_string(nlr) << std::endl;
    std::cerr << "fur: " << glm::to_string(fur) << std::endl;
    std::cerr << "ful: " << glm::to_string(ful) << std::endl;
    std::cerr << "fll: " << glm::to_string(fll) << std::endl;
    std::cerr << "flr: " << glm::to_string(flr) << std::endl;
    */

    glNormal3f(0.0, 1.0, 0.0);

    // Top
    glBegin(GL_LINE_LOOP);
    glVertex3fv(glm::value_ptr(nur));
    glVertex3fv(glm::value_ptr(fur));
    glVertex3fv(glm::value_ptr(ful));    
    glVertex3fv(glm::value_ptr(nul));
    glEnd( );

    // Bottom
    glBegin(GL_LINE_LOOP);
    glVertex3fv(glm::value_ptr(nlr));
    glVertex3fv(glm::value_ptr(flr));
    glVertex3fv(glm::value_ptr(fll));    
    glVertex3fv(glm::value_ptr(nll));
    glEnd( );

    // Front
    glBegin(GL_LINE_LOOP);
    glVertex3fv(glm::value_ptr(nlr));
    glVertex3fv(glm::value_ptr(nur));
    glVertex3fv(glm::value_ptr(nul));    
    glVertex3fv(glm::value_ptr(nll));
    glEnd( );

    // Back
    glBegin(GL_LINE_LOOP);
    glVertex3fv(glm::value_ptr(flr));
    glVertex3fv(glm::value_ptr(fur));
    glVertex3fv(glm::value_ptr(ful));    
    glVertex3fv(glm::value_ptr(fll));
    glEnd( );
  }

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
    m = glm::perspective(deg2rad(fovy), windowAspectRatio, near, far);
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
