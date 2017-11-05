// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// A wrapper class to draw colorful Utah/Newell teapots.
//
//

#include <iostream>
#include <glm/vec3.hpp>
#include "Material.h"

class UtahTeapot{
public:
  glm::vec3 position;
  float scale;
  Material *material;

  UtahTeapot(glm::vec3 pos, float s, Material* m): position(pos), scale(s){
    material = m;
  }

  UtahTeapot( ):position(glm::vec3(0, 0, 0)), scale(1.0){
    material = new Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(0.5, 0.5, 0.5, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
  }
  void draw( ){
    _glutSolidTeapot(scale);
  }
}
