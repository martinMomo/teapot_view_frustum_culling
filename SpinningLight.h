// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// A point light source that can spin around on the
// surface of a sphere. 
//
//

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/normalize_dot.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include "utilities.h"

#ifndef _SPINNING_LIGHT_H_
#define _SPINNING_LIGHT_H_

class SpinningLight{
public:
  glm::vec3 position;
  // center is like lookAt in the camera
  glm::vec3 center;
  // tangent is like the upVector in the camera
  glm::vec3 tangent;

  SpinningLight( ){ }
  SpinningLight(glm::vec3& color, glm::vec3& position, glm::vec3& center) :
    position(position), center(center), _color(color), _savedColor(color), _isOn(true){
      _rotationDelta = deg2rad(1.0);
      glm::vec3 random_vector = glm::sphericalRand(1.0);
      glm::vec3 d = direction( );
      tangent = glm::normalize(glm::cross(d, random_vector));
    }

  glm::vec4 color( ){return glm::vec4(_color, 1.0);}
  glm::vec4 position4( ){return glm::vec4(position, 1.0);}
  
  void rotateUp( ){
    /*
    glm::vec3 f = direction( );
    glm::vec3 _up = glm::normalize(tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, s);
    tangent = m * u;
    position = m * position;
    */

    glm::vec3 r = right( );
    glm::mat3 m = glm::rotate(-_rotationDelta, r);
    tangent = m * tangent;
    position = m * position;

  }

  void rotateDown( ){
    /*
    glm::vec3 f = direction( );
    glm::vec3 _up = glm::normalize(tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, s);
    tangent = m * u;
    position = m * position;
    */
    glm::vec3 r = right( );
    glm::mat3 m = glm::rotate(_rotationDelta, r);
    tangent = m * tangent;
    position = m * position;

  }

  void rotateLeft( ){
    glm::vec3 f = glm::normalize(direction( ));
    glm::vec3 _up = glm::normalize(tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, u);
    position = m * position;
  }

  void rotateRight( ){
    glm::vec3 f = glm::normalize(direction( ));
    glm::vec3 _up = glm::normalize(tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, u);
    position = m * position;
  }

  void roll( ){
    glm::mat3 m = glm::rotate(-_rotationDelta, direction( ));
    tangent = m * tangent;
  }
  
  void toggle( ){
    _isOn = !_isOn;
    if( _isOn ){
      _color = _savedColor;
    }else{
      _color = glm::vec3(0.0, 0.0, 0.0);
    }
  }

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

private:
  float _rotationDelta;
  glm::vec3 _color;
  glm::vec3 _savedColor;
  bool _isOn;
  
  glm::vec3 direction( ){
    glm::vec3 d;
    d = glm::normalize(center - position);
    return d;
  }

  glm::vec3 right( ){
    glm::vec3 r = glm::normalize(glm::cross(direction( ), tangent));
    return r;
  }

  void debug( ){
    std::cerr << "position " << glm::to_string(position) << "(" << glm::length(position) << ")" << "\ncenter " << glm::to_string(center) << "\ntangent " << glm::to_string(tangent) << "(" << glm::length(tangent) << ")" << std::endl << std::endl;
  }
};

#endif
