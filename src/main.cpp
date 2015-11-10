#include "gl.hpp"
#include "program.hpp"
#include "scene.hpp"
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

static Scene scene;

static Camera camera(scene);
static Camera camera1(scene);
static Camera camera2(scene);
static Camera camera3(scene);

static Model *protagonist;
static Model *suzanne;
static Model *teapot;
static Model *bunny;

static Object *protagonist1;
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

  protagonist = new Model("protagonist.obj");
  suzanne = new Model("suzanne.obj");
  teapot = new Model("teapot.obj");
  bunny = new Model("bunny.obj");

  protagonist1 = new Object(*protagonist);
  protagonist1->position.z = 4;

  suzanne1 = new Object(*suzanne);
  suzanne1->position.z = -2;
  suzanne1->position.y = 2;

  teapot1 = new Object(*teapot);
  teapot1->position.x = -2.0;
  teapot1->position.y = 1.0;
  teapot1->angles.y = 45;

  bunny1 = new Object(*bunny);
  bunny1->position.x = 2.0;

  scene << protagonist1
        << suzanne1
        << teapot1
        << bunny1;

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

  glViewport(0, 0, 640 / 2, 480 / 2);
  camera.draw();

  glViewport(640 / 2, 0, 640 / 2, 480 / 2);
  camera1.draw();

  glViewport(640 / 2, 480 / 2, 640 / 2, 480 / 2);
  camera2.draw();

  glViewport(0, 480 / 2, 640 / 2, 480 / 2);
  camera3.draw();
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
