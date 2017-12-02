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


class TeapotVisionApp : public GLFWApp{
private:
  float rotationDelta;

  glm::vec3 centerPosition;
  Camera* currentCamera;
  // main point of view camera
  Camera mainCamera;
  // Bird's eye view camera
  Camera bevCamera;

  glm::mat4 modelViewMatrix;
  glm::mat4 projectionMatrix;
  glm::mat4 normalMatrix;
  
  GLSLProgram shaderProgram;

  SpinningLight light0;
  SpinningLight light1; 

  UtahTeapot* teapots[20];
  const int teapotCount = 20;

  bool debugMaterialFlag;

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
  TeapotVisionApp(int argc, char* argv[]) :
    GLFWApp(argc, argv, std::string("Teapot Vision").c_str( ), 
            600, 600){ }
  
  void initCenterPosition( ){
    centerPosition = glm::vec3(0.0, 0.0, 0.0);
  }
  
  void initTeapots( ){
    std::srand(time(NULL));
    for(int i = 0; i < teapotCount; i++){
      glm::vec3 _diffuseColor = glm::linearRand(glm::vec3(0.2), glm::vec3(1.0));
      //std::cerr << glm::to_string(_diffuseColor) << std::endl;
      glm::vec4 diffuseColor = glm::vec4(_diffuseColor, 1.0);
      Material* m = new Material(glm::vec4(0.2, 0.2, 0.2, 1.0), diffuseColor, glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
      glm::vec2 xy = glm::diskRand(30.0);
      glm::vec3 position = glm::vec3(xy, 0.0);
      teapots[i] = new UtahTeapot(position, 1.0, m);
    }
  }

  void initCamera( ){
    // Main point of view camera
    mainCamera = Camera(glm::vec3(0.0, 0.0, 20.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), 45.0, 0.2, 50.0);
    // Bird's eye view camera
    bevCamera =  Camera(glm::vec3(0.0, 0.0, 70.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), 45.0, 10.0, 150.0);
    currentCamera = &mainCamera;
  }

  void initRotationDelta( ){
    rotationDelta = 0.05;
  }
   
  void initLights( ){
    glm::vec3 color0(1.0, 1.0, 1.0);
    glm::vec3 position0(0.0, 20.0, 20.0);
    glm::vec3 color1(1.0, 1.0, 1.0);
    glm::vec3 position1(0.0, 20.0, -20.0);
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
    debugMaterialFlag = false;

    // Load shader programs
    const char* vertexShaderSource = "blinn_phong.vert.glsl";
    const char* fragmentShaderSource = "blinn_phong.frag.glsl";
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
      printf("Shader program is not active, id: %d\n.", shaderProgram.id( ));
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
  
  void checkVisibility(glm::mat4 clipPlaneMatrix){
    // For the sake of this assignment, a teapot is defined to be
    // inside the view frustum if and only if the position data
    // member is within the volume defined by the view frustum.

    glm::vec4 position;  
    glm::mat4 lookAtMatrix; // multiplied with clipPlaneMatrix

    mainCamera.lookAtMatrix(lookAtMatrix);

    for(int i = 0; i < teapotCount; i++){
      // do something here to check to see if the teapots
      // are in or out of the view frustum. Set the visibility
      // flag as needed.
	  position = glm::vec4(teapots[i]->position, 1.0);
      position = lookAtMatrix * position;
      position = clipPlaneMatrix * position;

      if (position.x > -position.w && position.x < position.w &&
          position.y > -position.w && position.y < position.w &&
		  position.z > -position.w && position.z < position.w) {
        teapots[i]->visible = true;
      }
      else {
        teapots[i]->visible = false;
      }
    }
  }

  bool render( ){
    glm::vec4 _light0;
    glm::vec4 _light1;
    glm::mat4 lookAtMatrix;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::tuple<int, int> w = windowSize( );
    double ratio = double(std::get<0>(w)) / double(std::get<1>(w));

    glm::mat4 clipPlaneMatrix;
    mainCamera.perspectiveMatrix(clipPlaneMatrix, ratio);
    checkVisibility(clipPlaneMatrix);

    currentCamera->perspectiveMatrix(projectionMatrix, ratio);

    currentCamera->lookAtMatrix(lookAtMatrix);

    // Set light & material properties for the teapot;
    // lights are transformed by current modelview matrix
    // such that they are positioned correctly in the scene.
    _light0 = lookAtMatrix * light0.position4( );
    _light1 = lookAtMatrix * light1.position4( );
    
    if(currentCamera == &mainCamera){
      for(int i = 0; i < teapotCount; i++){
        if(teapots[i]->visible){
          // If the teapot is visible and it's in the main camera mode
          // then draw the teapot; otherwise don't
          modelViewMatrix = glm::translate(lookAtMatrix, teapots[i]->position);
          //modelViewMatrix = lookAtMatrix;
          normalMatrix = glm::inverseTranspose(modelViewMatrix);
          shaderProgram.activate( );
          activateUniforms(_light0, _light1, teapots[i]->material);
          //no_lightShaderProgram.activate( );
          teapots[i]->draw( );
        }
      }
    }else{
          // If this is the bird's eye view then draw everything
          // but with different materials
      Material redMaterial = Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
      Material whiteMaterial = Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
      Material yellowMaterial = Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(1.0, 1.0, 0.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
      Material blueMaterial = Material(glm::vec4(0.2, 0.2, 0.2, 1.0), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), 100.0);
      Material* currentMaterial = &redMaterial;
      shaderProgram.activate( );
      for(int i = 0; i < teapotCount; i++){
        // multiply the lookAtMatrix with the teapot's translation
        // to position the teapot in the right spot.
        modelViewMatrix = glm::translate(lookAtMatrix, teapots[i]->position);
        normalMatrix = glm::inverseTranspose(modelViewMatrix);
        if(teapots[i]->visible){
          currentMaterial = &redMaterial;
        }else{
          currentMaterial = &whiteMaterial;
        }
        if(debugMaterialFlag){
          activateUniforms(_light0, _light1, teapots[i]->material);
        }else{
          activateUniforms(_light0, _light1, currentMaterial);
        }
        teapots[i]->draw( );
      }
      modelViewMatrix = glm::translate(lookAtMatrix, mainCamera.eyePosition);
      normalMatrix = glm::inverseTranspose(modelViewMatrix);
      activateUniforms(_light0, _light1, &yellowMaterial);
      mainCamera.draw( );
      mainCamera.drawViewFrustum(ratio);

      modelViewMatrix = glm::translate(lookAtMatrix, light0.position);
      normalMatrix = glm::inverseTranspose(modelViewMatrix);
      activateUniforms(_light0, _light1, &blueMaterial);
      light0.draw( );

      modelViewMatrix = glm::translate(lookAtMatrix, light1.position);
      normalMatrix = glm::inverseTranspose(modelViewMatrix);
      activateUniforms(_light0, _light1, &blueMaterial);
      light1.draw( );
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
    }else if(isKeyPressed('3')){
      debugMaterialFlag = !debugMaterialFlag;
    }else if(isKeyPressed('P')){
      currentCamera = &mainCamera;
    }else if(isKeyPressed('B')){
      currentCamera = &bevCamera;
    }
    return !msglError( );
  }
    
};


int main(int argc, char* argv[]){
  TeapotVisionApp app(argc, argv);
  return app();
}



