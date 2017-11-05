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

#ifndef _SPINNING_LIGHT_H_
#define _SPINNING_LIGHT_H_

class SpinningLight{
public:
  SpinningLight( ){ }
  SpinningLight(glm::vec3& color, glm::vec3& position, glm::vec3& center) :
    _rotationDelta(0.05), _color(color), _position(position), _center(center), _savedColor(color), _isOn(true){
      glm::vec3 random_vector = glm::sphericalRand(1.0);
      glm::vec3 d = direction( );
      _tangent = glm::cross(d, random_vector);
    }

  glm::vec4 color( ){return glm::vec4(_color, 1.0);}
  glm::vec4 position( ){return glm::vec4(_position, 1.0);}
  
  void rotateUp( ){
    glm::vec3 f = direction( );
    glm::vec3 _up = glm::normalize(_tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, s);
    _tangent = m * u;
    _position = m * _position;
  }

  void rotateDown( ){
    glm::vec3 f = direction( );
    glm::vec3 _up = glm::normalize(_tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, s);
    _tangent = m * u;
    _position = m * _position;
  }

  void rotateLeft( ){
    glm::vec3 f = glm::normalize(direction( ));
    glm::vec3 _up = glm::normalize(_tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, u);
    _position = m * _position;
  }

  void rotateRight( ){
    glm::vec3 f = glm::normalize(direction( ));
    glm::vec3 _up = glm::normalize(_tangent);
    glm::vec3 s = glm::normalize(glm::cross(f, _up));
    glm::vec3 u = glm::cross(s, f);
    glm::mat3 m = glm::rotate(_rotationDelta, u);
    _position = m * _position;
  }

  void roll( ){
    glm::mat3 m = glm::rotate(-_rotationDelta, direction( ));
    _tangent = m * _tangent;
  }
  
  void toggle( ){
    _isOn = !_isOn;
    if( _isOn ){
      _color = _savedColor;
    }else{
      _color = glm::vec3(0.0, 0.0, 0.0);
    }
  }

private:
  float _rotationDelta;
  glm::vec3 _color;
  glm::vec3 _position;
  glm::vec3 _center;
  glm::vec3 _tangent;
  glm::vec3 _savedColor;
  bool _isOn;
  
  glm::vec3 direction( ){
    glm::vec3 d;
    d = glm::normalize(_center - _position);
    return d;
  }

  void debug( ){
    std::cerr << "position " << glm::to_string(_position) << "(" << glm::length(_position) << ")" << "\ncenter " << glm::to_string(_center) << "\ntangent " << glm::to_string(_tangent) << "(" << glm::length(_tangent) << ")" << std::endl << std::endl;
  }
};

#endif
