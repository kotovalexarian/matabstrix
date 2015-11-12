#include "gl.hpp"
#include "program.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "store.hpp"

#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static Store store;

static void iterate();

static GLFWCALL void on_key(int key, int action);
static EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data);

GLuint mvp_uniform;
GLuint local_modelview_uniform;

static GLuint texture_uniform;

static bool keys[GLFW_KEY_LAST];

static Scene scene;

static Camera camera(scene);
static float camera_angles_x = 0;

static const Model *protagonist;
static const Model *car;
static const Model *suzanne;
static const Model *teapot;
static const Model *bunny;

static Object *protagonist1;
static Object *car1;
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

  const Program *program = store.load<Program>("textured");
  program->use();

  mvp_uniform = program->get_uniform_location("mvp");
  local_modelview_uniform = program->get_uniform_location("local_modelview");

  texture_uniform = program->get_uniform_location("texture");
  glUniform1i(texture_uniform, 0);

  camera.projection = glm::perspective(45.0f, (float)640 / (float)480, 0.1f, 100.0f);
  camera.position.z = 2;

  protagonist = store.load<Model>("protagonist.obj");
  car = store.load<Model>("car.obj");
  suzanne = store.load<Model>("suzanne.obj");
  teapot = store.load<Model>("teapot.obj");
  bunny = store.load<Model>("bunny.obj");

  protagonist1 = new Object(*protagonist);
  protagonist1->position.z = 4;

  car1 = new Object(*car);
  car1->position.z = -10;

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
        << car1
        << suzanne1
        << teapot1
        << bunny1;

  glViewport(0, 0, 640, 480);

  glEnable(GL_DEPTH_TEST);

  glClearColor(1, 1, 1, 0);

  emscripten_set_main_loop(iterate, 0, 1);
}

void iterate()
{
  if (keys[(unsigned char)'W'])
  {
    protagonist1->position.x -= 0.1 * sin(glm::radians(protagonist1->angles.y));
    protagonist1->position.z -= 0.1 * cos(glm::radians(protagonist1->angles.y));
  }

  if (keys[(unsigned char)'S'])
  {
    protagonist1->position.x += 0.1 * sin(glm::radians(protagonist1->angles.y));
    protagonist1->position.z += 0.1 * cos(glm::radians(protagonist1->angles.y));
  }

  if (keys[(unsigned char)'D'])
  {
    protagonist1->position.x += 0.1 * cos(glm::radians(protagonist1->angles.y));
    protagonist1->position.z -= 0.1 * sin(glm::radians(protagonist1->angles.y));
  }

  if (keys[(unsigned char)'A'])
  {
    protagonist1->position.x -= 0.1 * cos(glm::radians(protagonist1->angles.y));
    protagonist1->position.z += 0.1 * sin(glm::radians(protagonist1->angles.y));
  }

  camera.base = protagonist1->transformation()
    * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(camera_angles_x), glm::vec3(1.0f, 0.0f, 0.0f));

  suzanne1->angles.z = glfwGetTime() * 360 / 4;
  teapot1->angles.x = glfwGetTime() * 360 / 6;
  bunny1->angles.y = glfwGetTime() * 360 / 2;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera.draw();
}

GLFWCALL void on_key(int key, int action)
{
  keys[key] = action != GLFW_RELEASE;
}

EM_BOOL on_em_mousemove(__attribute__((unused)) int event_type,
                        const EmscriptenMouseEvent *mouse_event,
                        __attribute__((unused)) void *user_data)
{
  protagonist1->angles.y -= mouse_event->movementX;
  camera_angles_x -= mouse_event->movementY;

  if (protagonist1->angles.y < 0)
    protagonist1->angles.y = 359;
  else
  if (protagonist1->angles.y >= 360)
    protagonist1->angles.y = 0;

  if (camera_angles_x < -90)
    camera_angles_x = -90;
  else
  if (camera_angles_x > 90)
    camera_angles_x = 90;

  return true;
}
