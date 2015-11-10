#include "gl.hpp"
#include "program.hpp"
#include "object.hpp"
#include "camera.hpp"

#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static void iterate();

static GLFWCALL void on_key(int key, int action);
static EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data);

GLuint mvp_uniform;
GLuint local_modelview_uniform;

static GLuint texture_uniform;

static bool keys[GLFW_KEY_LAST];

static Camera camera;
static Camera camera1;
static Camera camera2;
static Camera camera3;

static Model *suzanne;
static Model *teapot;
static Model *bunny;

static Object *suzanne1;
static Object *teapot1;
static Object *bunny1;

int main()
{
  if (!glfwInit())
    exit(EXIT_FAILURE);

  if (!glfwOpenWindow(640, 480, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  if (glewInit() != GLEW_OK)
  {
    glfwCloseWindow();
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(on_key);
  emscripten_set_mousemove_callback(nullptr, nullptr, false, on_em_mousemove);

  Program program("textured");
  program.use();

  mvp_uniform = program.get_uniform_location("mvp");
  local_modelview_uniform = program.get_uniform_location("local_modelview");

  texture_uniform = program.get_uniform_location("texture");
  glUniform1i(texture_uniform, 0);

  camera.projection = glm::perspective(45.0f, (float)640 / (float)480, 0.1f, 10.0f);
  camera.position.z = 4;
  camera.position.y = 2;

  camera1.projection = camera.projection;
  camera1.position.y = 1.5;
  camera1.position.x = -5;
  camera1.angles.y = -90;

  camera2.projection = camera.projection;
  camera2.position.y = 1.5;
  camera2.position.z = -5;
  camera2.angles.y = -180;

  camera3.projection = camera.projection;
  camera3.position.y = -5;
  camera3.angles.x = 90;

  suzanne = new Model("suzanne.obj");
  teapot = new Model("teapot.obj");
  bunny = new Model("bunny.obj");

  suzanne1 = new Object(*suzanne);
  suzanne1->position.z = -2;
  suzanne1->position.y = 2;

  teapot1 = new Object(*teapot);
  teapot1->position.x = -2.0;
  teapot1->position.y = 1.0;
  teapot1->angles.y = 45;

  bunny1 = new Object(*bunny);
  bunny1->position.x = 2.0;

  glEnable(GL_DEPTH_TEST);

  glClearColor(1, 1, 1, 0);

  emscripten_set_main_loop(iterate, 0, 1);
}

void iterate()
{
  if (keys[(unsigned char)'W'])
  {
    camera.position.x -= 0.1 * sin(glm::radians(camera.angles.y));
    camera.position.z -= 0.1 * cos(glm::radians(camera.angles.y));
  }

  if (keys[(unsigned char)'S'])
  {
    camera.position.x += 0.1 * sin(glm::radians(camera.angles.y));
    camera.position.z += 0.1 * cos(glm::radians(camera.angles.y));
  }

  if (keys[(unsigned char)'D'])
  {
    camera.position.x += 0.1 * cos(glm::radians(camera.angles.y));
    camera.position.z -= 0.1 * sin(glm::radians(camera.angles.y));
  }

  if (keys[(unsigned char)'A'])
  {
    camera.position.x -= 0.1 * cos(glm::radians(camera.angles.y));
    camera.position.z += 0.1 * sin(glm::radians(camera.angles.y));
  }

  suzanne1->angles.z = glfwGetTime() * 360 / 4;
  teapot1->angles.x = glfwGetTime() * 360 / 6;
  bunny1->angles.y = glfwGetTime() * 360 / 2;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const glm::mat4 mvp = camera.transformation();
  const glm::mat4 mvp1 = camera1.transformation();
  const glm::mat4 mvp2 = camera2.transformation();
  const glm::mat4 mvp3 = camera3.transformation();

  glViewport(0, 0, 640 / 2, 480 / 2);
  suzanne1->draw(mvp);
  teapot1->draw(mvp);
  bunny1->draw(mvp);

  glViewport(640 / 2, 0, 640 / 2, 480 / 2);
  suzanne1->draw(mvp1);
  teapot1->draw(mvp1);
  bunny1->draw(mvp1);

  glViewport(640 / 2, 480 / 2, 640 / 2, 480 / 2);
  suzanne1->draw(mvp2);
  teapot1->draw(mvp2);
  bunny1->draw(mvp2);

  glViewport(0, 480 / 2, 640 / 2, 480 / 2);
  suzanne1->draw(mvp3);
  teapot1->draw(mvp3);
  bunny1->draw(mvp3);
}

GLFWCALL void on_key(int key, int action)
{
  keys[key] = action != GLFW_RELEASE;
}

EM_BOOL on_em_mousemove(__attribute__((unused)) int event_type,
                        const EmscriptenMouseEvent *mouse_event,
                        __attribute__((unused)) void *user_data)
{
  camera.angles.y -= mouse_event->movementX;
  camera.angles.x -= mouse_event->movementY;

  if (camera.angles.y < 0)
    camera.angles.y = 359;
  else
  if (camera.angles.y >= 360)
    camera.angles.y = 0;

  if (camera.angles.x < -90)
    camera.angles.x = -90;
  else
  if (camera.angles.x > 90)
    camera.angles.x = 90;

  return true;
}
