////////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// Copyright (c) 2013-24 by Carlos Martinho
// 
// Catarina Gonçalves Costa ist1112377
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
#include <time.h>

int nRand;

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
  GLint GroupID;

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
  Shaders->addUniform("GroupID");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
  GroupID = Shaders->Uniforms["GroupID"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] = {
    //right triangle
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.25f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.0f, 0.25f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},

    //square
    {{-0.125f, -0.125f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.125f, -0.125f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{-0.125f, 0.125f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.125f, 0.125f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},

    //parallelogram
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, //0
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.25f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.75f, 0.25f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    {{0.25f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}} //1
};

const GLubyte Indices[] = {0,1,2,
                           3,4,5,4,6,5,
                           7,12,8,12,9,8,12,11,9,11,10,9};

void MyApp::createBufferObjects() {
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(2, VboId);

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

// Common constants
const float sqrt125_half = sqrt(0.125f) / 2.0f;
const glm::mat4 I = glm::mat4(1.0f);

//Scale matrix used for every object
const glm::mat4 GS = glm::scale(I, glm::vec3(0.5f, 0.5f, 1.0f));

// Rotation matrices
const glm::mat4 R90_pos = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
const glm::mat4 R90_neg = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
const glm::mat4 R45_pos = glm::rotate(I, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
const glm::mat4 R45_neg = glm::rotate(I, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -1.0f));

// Translation matrices
const glm::mat4 T0 = glm::translate(glm::vec3(0.0f, 0.25f, 0.0f));
const glm::mat4 T1 = glm::translate(glm::vec3(sqrt125_half, 0.0f, 0.0f));
const glm::mat4 T2 = glm::translate(glm::vec3(-sqrt125_half, 0.0f, 0.0f));
const glm::mat4 T3 = glm::translate(glm::vec3(0.125f, 0.125f, 0.0f));
const glm::mat4 T4 = glm::translate(glm::vec3(0.25f, 0.0f, 0.0f));
const glm::mat4 T5 = glm::translate(glm::vec3(0.0f, 0.5f, 0.0f));
const glm::mat4 T6 = glm::translate(glm::vec3(0.5f,-0.5f - sqrt125_half,0.0f));
const glm::mat4 T7 = glm::translate(glm::vec3(-0.11f,-1.39f - sqrt125_half, 0.0f));
const glm::mat4 T8 = glm::translate(glm::vec3(-sqrt125_half -0.075, -1.25f - sqrt125_half, 0.0f));
const glm::mat4 T9 = glm::translate(glm::vec3(0.4f, -0.4f - sqrt125_half, 0.0f));

static void drawSmallTriangle(GLint MatrixId, glm::mat4 Mt, glm::mat4 Mx, GLint GroupID, int group) {
    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(Mx*Mt));
    glUniform1i(GroupID, nRand*group);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
        reinterpret_cast<GLvoid*>(0));
}

static void drawSquare(GLint MatrixId, glm::mat4 Mt, glm::mat4 Mx, GLint GroupID, int group) {
    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(Mx*Mt));
    glUniform1i(GroupID, nRand*group);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(3 * sizeof(Indices[0])));
}

static void drawMediumTriangle(GLint MatrixId, glm::mat4 Mx, GLint GroupID, int group) {
    drawSmallTriangle(MatrixId, T0, Mx, GroupID, group);
    drawSquare(MatrixId, T3, Mx, GroupID, group);
    drawSmallTriangle(MatrixId, T4, Mx, GroupID, group);
}

static void drawBigTriangle(GLint MatrixId, glm::mat4 Mx, GLint GroupID, int group) {
    drawMediumTriangle(MatrixId, Mx * T5 * R90_neg * R90_neg, GroupID, group);
    drawMediumTriangle(MatrixId, Mx * T5 * R90_pos * R90_neg * R90_neg, GroupID, group);
}

static void drawParallelogram(GLint MatrixId, glm::mat4 Mt, GLint GroupID, int group) {
    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(Mt));
    glUniform1i(GroupID, nRand*group);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(9 * sizeof(Indices[0])));
}

void MyApp::drawScene() {
  // Drawing directly in clip space

  glBindVertexArray(VaoId);
  Shaders->bind();

  drawSquare(MatrixId,I, GS* R45_pos, GroupID,1);
  drawSmallTriangle(MatrixId, I, GS * T1 * R45_neg, GroupID,2);
  drawSmallTriangle(MatrixId, I, GS * T2 * R45_pos * R90_pos, GroupID, 3);
  drawBigTriangle(MatrixId, GS * T6 * R90_pos, GroupID, 4);
  drawBigTriangle(MatrixId, GS * T8 * R90_neg, GroupID, 5);
  drawMediumTriangle(MatrixId, GS * T7 * R45_pos * R90_pos * R90_pos, GroupID, 6);
  drawParallelogram(MatrixId, GS * T9, GroupID, 7);

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
    srand(time(NULL));
    nRand = rand();
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp);
  engine.setOpenGL(4, 5);
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
