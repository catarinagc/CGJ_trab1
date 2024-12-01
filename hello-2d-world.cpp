////////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// Copyright (c) 2013-24 by Carlos Martinho
//
// INTRODUCES:
// GL PIPELINE, mglShader.hpp, mglConventions.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <sstream>
#include "../mgl/mgl.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
 public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

 private:
  const GLuint POSITION = 0, COLOR = 1;
  GLuint VaoId, VboId[2];
  std::unique_ptr<mgl::ShaderProgram> Shaders;
  GLint MatrixId;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
  Shaders = std::make_unique<mgl::ShaderProgram>();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] = {
/*
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{2.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{2.0f, 2.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 2.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
*/
/*    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.50f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
*/
    //right triangle
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.0f, 0.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},

    //square
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},

    //parallelogram
    {{0.0f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.25f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.75f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
};

const GLubyte Indices[] = {0,1,2,
                           3,5,6,3,4,5,
                           10,8,7,10,9,8,10,11,9,11,12,9}; //indice dos vertices no array Vertices[]

/*const Vertex RightTriangleVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
};

const Vertex SquareVertices[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
}; */

void MyApp::createBufferObjects() {
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId); //vertex array object id
  {
    glGenBuffers(2, VboId); //vertex buffer object id

    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(POSITION);
      glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<GLvoid *>(0));
      glEnableVertexAttribArray(COLOR);
      glVertexAttribPointer(
          COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
          reinterpret_cast<GLvoid *>(sizeof(Vertices[0].XYZW)));
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                   GL_STATIC_DRAW);
    }
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(2, VboId);
}

void MyApp::destroyBufferObjects() {
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(COLOR);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I(1.0f);
const glm::mat4 M = glm::translate(glm::vec3(-1.0f, -1.0f, 0.0f));
const glm::mat4 L = glm::translate(glm::vec3(-1.0f, 0.0f, 0.0f));
const glm::mat4 M1 = glm::translate(glm::vec3(-0.25f, 0.0f, 0.0f));
glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//M1 = glm::rotate(M1, glm::vec3(0.0f,-1.0f,0.0f));
glm::mat4 Transform = R * M1;
const glm::mat4 C = glm::translate(glm::vec3(0.0f, 0.25f, 0.0f));
const glm::mat4 C2 = glm::translate(glm::vec3(0.25f, 0.0f, 0.0f));
const glm::mat4 S = glm::scale(I, glm::vec3(2.0f, 2.0f, 1.0f));
const glm::mat4 C3 = glm::translate(glm::vec3(-0.25f, 0.0f, 0.0f));
void MyApp::drawScene() { //onde defino a matriz de transformação e o que vou desenhar
  // Drawing directly in clip space

  glBindVertexArray(VaoId);
  Shaders->bind();
  

  /*
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,   //3 = n de vertices
                 reinterpret_cast<GLvoid *>(0));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(3*sizeof(Indices[0]))); //6 pq sao "2 triangulos"
  
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(L));
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(9 * sizeof(Indices[0])));
  */

  //small triangle 1
  /*
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,  
      reinterpret_cast<GLvoid*>(0)); 
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(R));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 
      reinterpret_cast<GLvoid*>(0));
*/
  //small triangle 2
  /*
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(R));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
 */

  //medium triangle
  /*
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C2));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(3 * sizeof(Indices[0])));
  */
  //big triangle 1 (2 medium triangles)
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C2));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(3 * sizeof(Indices[0])));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C*R));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(R));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(3 * sizeof(Indices[0])));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C3*R));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  //big triangle 2 (2 medium triangles)
  /*
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C2));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(3 * sizeof(Indices[0])));

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C * R));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(R));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(3 * sizeof(Indices[0])));
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(C3 * R));
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));
*/
  /*
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE,   //3 = n de vertices
      reinterpret_cast<GLvoid*>(0));
*/
  Shaders->unbind();
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createBufferObjects();
  createShaderProgram();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 5);
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
