// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// A toy program which renders a teapot and two light sources. 
//
//

#include <tuple>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "GLFWApp.h"
#include "GLSLShader.h"
#include "glut_teapot.h"

#include "SpinningLight.h"
#include "Camera.h"
#include "UtahTeapot.h"

void msglVersion(void){
  fprintf(stderr, "OpenGL Version Information:\n");
  fprintf(stderr, "\tVendor: %s\n", glGetString(GL_VENDOR));
  fprintf(stderr, "\tRenderer: %s\n", glGetString(GL_RENDERER));
  fprintf(stderr, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
  fprintf(stderr, "\tGLSL Version: %s\n",
          glGetString(GL_SHADING_LANGUAGE_VERSION));
}


class HelloGLSLApp : public GLFWApp{
private:
  float rotationDelta;

  glm::vec3 centerPosition;
  Camera* currentCamera;
  Camera mainCamera;
  Camera bovCamera;

  glm::mat4 modelViewMatrix;
  glm::mat4 projectionMatrix;
  glm::mat4 normalMatrix;
  
  GLSLProgram shaderProgram;

  SpinningLight light0;
  SpinningLight light1; 

  UtahTeapot* teapots[20];
  const int teapotCount = 20;

  // Variables to set uniform params for lighting fragment shader 
  unsigned int uModelViewMatrix;
  unsigned int uProjectionMatrix;
  unsigned int uNormalMatrix;
  unsigned int uLight0_position;
  unsigned int uLight0_color;
  unsigned int uLight1_position;
  unsigned int uLight1_color;
  unsigned int uAmbient;
  unsigned int uDiffuse;
  unsigned int uSpecular;
  unsigned int uShininess;
  
public:
  HelloGLSLApp(int argc, char* argv[]) :
    GLFWApp(argc, argv, std::string("Teapot Vision").c_str( ), 
            600, 600){ }
  
  void initCenterPosition( ){
    centerPosition = glm::vec3(0.0, 0.0, 0.0);
  }
  
  void initTeapots( ){
    for(int i = 0; i < teapotCount; i++){
      glm::vec3 _diffuseColor = glm::linearRand(glm::vec3(0.2), glm::vec3(1.0));
      std::cerr << glm::to_string(_diffuseColor) << std::endl;
      glm::vec4 diffuseColor = glm::vec4(_diffuseColor, 1.0);
      Material* m = new Material(glm::vec4(0.2, 0.2, 0.2, 1.0), diffuseColor, glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
      glm::vec2 xy = glm::diskRand(30.0);
      glm::vec3 position = glm::vec3(xy, 0.0);
      teapots[i] = new UtahTeapot(position, 1.0, m);
    }
  }

  void initCamera( ){
    mainCamera = Camera(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), 90.0, 1.0, 100.0);
    bovCamera =  Camera(glm::vec3(0.0, 0.0, 40.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), 90.0, 1.0, 100.0);
    currentCamera = &mainCamera;
  }

  void initRotationDelta( ){
    rotationDelta = 0.05;
  }
   
  void initLights( ){
    glm::vec3 color0(1.0, 1.0, 1.0);
    glm::vec3 position0(0.0, 50.0, 50.0);
    glm::vec3 color1(1.0, 1.0, 1.0);
    glm::vec3 position1(0.0, 50.0, -50.0);
    light0 = SpinningLight(color0, position0, centerPosition);
    light1 = SpinningLight(color1, position1, centerPosition);
  }

  bool begin( ){
    msglError( );
    initCenterPosition( );
    initTeapots( );
    initCamera( );
    initRotationDelta( );
    initLights( );
    
    // Load shader program
    const char* vertexShaderSource = "phong.vert.glsl";
    const char* fragmentShaderSource = "phong.frag.glsl";
    FragmentShader fragmentShader(fragmentShaderSource);
    VertexShader vertexShader(vertexShaderSource);
    shaderProgram.attach(vertexShader);
    shaderProgram.attach(fragmentShader);
    shaderProgram.link( );
    shaderProgram.activate( );
    
    printf("Shader program built from %s and %s.\n",
           vertexShaderSource, fragmentShaderSource);
    if( shaderProgram.isActive( ) ){
      printf("Shader program is loaded and active with id %d.\n", shaderProgram.id( ) );
    }else{
      printf("Shader program did not load and activate correctly. Exiting.");
      exit(1);
    }
    
    // Set up uniform variables for the shader program
    uModelViewMatrix = glGetUniformLocation(shaderProgram.id( ), "modelViewMatrix");
    uProjectionMatrix = glGetUniformLocation(shaderProgram.id( ), "projectionMatrix");
    uNormalMatrix = glGetUniformLocation(shaderProgram.id( ), "normalMatrix");
    uLight0_position = glGetUniformLocation(shaderProgram.id( ), "light0_position");
    uLight0_color = glGetUniformLocation(shaderProgram.id( ), "light0_color");
    uLight1_position = glGetUniformLocation(shaderProgram.id( ), "light1_position");
    uLight1_color = glGetUniformLocation(shaderProgram.id( ), "light1_color");
    uAmbient = glGetUniformLocation(shaderProgram.id( ), "ambient");
    uDiffuse = glGetUniformLocation(shaderProgram.id( ), "diffuse");
    uSpecular = glGetUniformLocation(shaderProgram.id( ), "specular");
    uShininess = glGetUniformLocation(shaderProgram.id( ), "shininess");

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    msglVersion( );
    
    return !msglError( );
  }
  
  bool end( ){
    windowShouldClose( );
    return true;
  }
  
  void activateUniforms(glm::vec4& _light0, glm::vec4& _light1, Material* m){
    glUniformMatrix4fv(uModelViewMatrix, 1, false, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(uProjectionMatrix, 1, false, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(normalMatrix));

    glUniform4fv(uLight0_position, 1, glm::value_ptr(_light0));
    glUniform4fv(uLight0_color, 1, glm::value_ptr(light0.color( )));
    
    glUniform4fv(uLight1_position, 1, glm::value_ptr(_light1));
    glUniform4fv(uLight1_color, 1, glm::value_ptr(light1.color( )));

    glUniform4fv(uAmbient, 1, glm::value_ptr(m->ambient));
    glUniform4fv(uDiffuse, 1, glm::value_ptr(m->diffuse));
    glUniform4fv(uSpecular, 1, glm::value_ptr(m->specular));
    glUniform1f(uShininess, m->shininess);
  }
  
  bool render( ){
    glm::vec4 _light0;
    glm::vec4 _light1;
    glm::mat4 lookAtMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::tuple<int, int> w = windowSize( );
    double ratio = double(std::get<0>(w)) / double(std::get<1>(w));

    currentCamera->perspectiveMatrix(projectionMatrix, ratio);

    currentCamera->lookAtMatrix(lookAtMatrix);

    /*currentCamera->debug( );
    teapots[0]->debug( );
    std::cerr << "Look At: " << glm::to_string(lookAtMatrix) << std::endl;
    std::cerr << "Perspective: " << glm::to_string(projectionMatrix) << std::endl;*/

    // Set light & material properties for the teapot;
    // lights are transformed by current modelview matrix
    // such that they are positioned correctly in the scene.
    _light0 = lookAtMatrix * light0.position( );
    _light1 = lookAtMatrix * light1.position( );
    
    for(int i = 0; i < teapotCount; i++){
      if(teapots[i]->visible && currentCamera == &mainCamera){
        // If the teapot is visible and it's in the main camera mode
        // then draw the teapot; otherwise don't
        modelViewMatrix = glm::translate(lookAtMatrix, teapots[i]->position);
        //modelViewMatrix = lookAtMatrix;
        normalMatrix = glm::inverseTranspose(modelViewMatrix);
        shaderProgram.activate( );
        activateUniforms(_light0, _light1, teapots[i]->material);
        teapots[i]->draw( );
      }else if(currentCamera == &bovCamera){
        // If this is the bird's eye view then draw everything
        // but with different materials
        Material redMaterial = Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
        Material whiteMaterial = Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
        Material* currentMaterial = &redMaterial;
        modelViewMatrix = glm::translate(lookAtMatrix, teapots[i]->position);
        //modelViewMatrix = lookAtMatrix;
        normalMatrix = glm::inverseTranspose(modelViewMatrix);
        shaderProgram.activate( );
        if(teapots[i]->visible){
          currentMaterial = &redMaterial;
        }else{
          currentMaterial = &whiteMaterial;
        }
        activateUniforms(_light0, _light1, currentMaterial);
        teapots[i]->draw( );

      }
    }

    
    if(isKeyPressed('Q')){
      end( );      
    }else if(isKeyPressed(GLFW_KEY_EQUAL)){

    }else if(isKeyPressed(GLFW_KEY_MINUS)){

    }else if(isKeyPressed('R')){
      /*initEyePosition( );
      initUpVector( );*/
      initCamera( );
      initRotationDelta( );
      initLights( );  
      printf("Eye position, up vector and rotation delta reset.\n");
    }else if(isKeyPressed(GLFW_KEY_LEFT)){
      mainCamera.rotateCameraRight( );
    }else if(isKeyPressed(GLFW_KEY_RIGHT)){
      mainCamera.rotateCameraLeft( );
    }else if(isKeyPressed(GLFW_KEY_UP)){
      mainCamera.rotateCameraDown( );
    }else if(isKeyPressed(GLFW_KEY_DOWN)){
      mainCamera.rotateCameraUp( );
    }else if(isKeyPressed('W')){
      light0.rotateUp( );
    }else if(isKeyPressed('S')){
      light0.rotateUp( );
    }else if(isKeyPressed('A')){
      light0.rotateLeft( );
    }else if(isKeyPressed('D')){
      light0.rotateLeft( );
    }else if(isKeyPressed('X')){
      light0.roll( );
    }else if(isKeyPressed('Y')){
      light1.rotateUp( );
    }else if(isKeyPressed('H')){
      light1.rotateUp( );
    }else if(isKeyPressed('G')){
      light1.rotateLeft( );
    }else if(isKeyPressed('J')){
      light1.rotateLeft( );
    }else if(isKeyPressed('N')){
      light1.roll( );
    }else if(isKeyPressed('O')){
      mainCamera.forward( );
    }else if(isKeyPressed('L')){
      mainCamera.backward( );
    }else if(isKeyPressed('K')){
      mainCamera.panLeft( );
    }else if(isKeyPressed(';')){
      mainCamera.panRight( );
    }else if(isKeyPressed('1')){
      light0.toggle( );
    }else if(isKeyPressed('2')){
      light1.toggle( );
    }else if(isKeyPressed('P')){
      currentCamera = &mainCamera;
    }else if(isKeyPressed('B')){
      currentCamera = &bovCamera;
    }
    return !msglError( );
  }
    
};


int main(int argc, char* argv[]){
  HelloGLSLApp app(argc, argv);
  return app();
}



