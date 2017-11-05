// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// A Phong illumination model material class.
//
//

#include <iostream>
#include <glm/vec3.hpp>

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

class Material{
public:
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  float shininess;

  Material(glm::vec4 amb, glm::vec4 diff, glm::vec4 spec, float shine): ambient(amb), diffuse(diff), specular(spec), shininess(shine){};

  void debug( ){
    std::cerr << "Material" << std::endl;
    std::cerr << "ambient: " << glm::to_string(ambient) << std::endl;
    std::cerr << "diffuse: " << glm::to_string(diffuse) << std::endl;
    std::cerr << "specular: " << glm::to_string(specular) << std::endl;
    std::cerr << "shininess: " << shininess << std::endl;
  }

};

#endif
