// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// A wrapper class to draw colorful Utah/Newell teapots.
//
//

#include <iostream>
#include <glm/vec3.hpp>
#include "glut_teapot.h"
#include "Material.h"

#ifndef _UTAH_TEAPOT_H_
#define _UTAH_TEAPOT_H_

class UtahTeapot{
public:
  glm::vec3 position;
  float scale;
  Material *material;
  bool visible;

  UtahTeapot(glm::vec3 pos, float s, Material* m): position(pos), scale(s){
    material = m;
    visible = true;
  }
  
  UtahTeapot( ):position(glm::vec3(0, 0, 0)), scale(1.0){
    material = new Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(0.5, 0.5, 0.5, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
    visible = true;
  }
  
  ~UtahTeapot( ){
    delete material;
  }

  void draw( ){
    _glutSolidTeapot(scale);
  }

  void debug( ){
    std::cerr << "UtahTeapot" << std::endl;
    std::cerr << "position: " << glm::to_string(position) << std::endl;
    std::cerr << "scale: " << scale << std::endl;
    material->debug( );
  }


};

#endif
