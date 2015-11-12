#include "gl.hpp"
#include "program.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "store.hpp"
#include "model/static.hpp"
#include "model/raw.hpp"

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

static float camera_angles_y = 0;
static float camera_angles_x = 0;

static const Model *protagonist;
static const Model *car;
static const Model *suzanne;
static const Model *teapot;
static const Model *bunny;
static const Model *untitled;

static Object *protagonist1;
static Object *car1;
static Object *suzanne1;
static Object *teapot1;
static Object *bunny1;
static Object *untitled1;

static bool in_car = false;

static float car_vel = 0.0;

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

  protagonist = store.load<Models::Static>("protagonist.obj");
  car = store.load<Models::Static>("car.obj");
  suzanne = store.load<Models::Static>("suzanne.obj");
  teapot = store.load<Models::Static>("teapot.obj");
  bunny = store.load<Models::Static>("bunny.obj");
  untitled = store.load<Models::Raw>("untitled.raw");

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

  untitled1 = new Object(*untitled);
  untitled1->position.x = -6;

  scene << protagonist1
        << car1
        << suzanne1
        << teapot1
        << bunny1
        << untitled1;

  glViewport(0, 0, 640, 480);

  glEnable(GL_DEPTH_TEST);

  glClearColor(1, 1, 1, 0);

  emscripten_set_main_loop(iterate, 0, 1);
}

void iterate()
{
  if (in_car)
  {
    protagonist1->position = car1->position;

    if (keys[(unsigned char)'W'])
      car_vel += 0.1;

    if (keys[(unsigned char)'S'])
      car_vel -= 0.1;

    if (car_vel > 2.0)
      car_vel = 2.0;
    else
    if (car_vel < -1.0)
      car_vel = -1.0;

    if (keys[(unsigned char)'A'])
      car1->angles.y += 0.5 * car_vel;

    if (keys[(unsigned char)'D'])
      car1->angles.y -= 0.5 * car_vel;

    if (car1->angles.y < 0)
      car1->angles.y = 359;
    if (car1->angles.y >= 360)
      car1->angles.y = 0;
  }
  else
  {
    car_vel *= 0.99;

    protagonist1->angles.y = camera_angles_y;

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
  }

  car1->position.x += car_vel * 0.1 * sin(glm::radians(car1->angles.y));
  car1->position.z += car_vel * 0.1 * cos(glm::radians(car1->angles.y));

  if (in_car)
  {
    camera.base = car1->transformation()
      * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
      * glm::rotate(glm::mat4(1.0f), glm::radians(camera_angles_y), glm::vec3(0.0f, 1.0f, 0.0f))
      * glm::rotate(glm::mat4(1.0f), glm::radians(camera_angles_x), glm::vec3(1.0f, 0.0f, 0.0f));
    camera.position.z = 8;
  }
  else
  {
    camera.base = protagonist1->transformation()
      * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f))
      * glm::rotate(glm::mat4(1.0f), glm::radians(camera_angles_x), glm::vec3(1.0f, 0.0f, 0.0f));
    camera.position.z = 2;
  }

  suzanne1->angles.z = glfwGetTime() * 360 / 4;
  teapot1->angles.x = glfwGetTime() * 360 / 6;
  bunny1->angles.y = glfwGetTime() * 360 / 2;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera.draw();
}

GLFWCALL void on_key(int key, int action)
{
  keys[key] = action != GLFW_RELEASE;

  if (key == 'E' && action == GLFW_PRESS)
  {
    if (in_car)
    {
      in_car = false;
      protagonist1->visible = true;
    }
    else
    if (glm::distance(protagonist1->position, car1->position) <= 1.0)
    {
      in_car = true;
      protagonist1->visible = false;
    }
  }
}

EM_BOOL on_em_mousemove(__attribute__((unused)) int event_type,
                        const EmscriptenMouseEvent *mouse_event,
                        __attribute__((unused)) void *user_data)
{
  camera_angles_y -= mouse_event->movementX;
  camera_angles_x -= mouse_event->movementY;

  if (camera_angles_y < 0)
    camera_angles_y = 359;
  else
  if (camera_angles_y >= 360)
    camera_angles_y = 0;

  if (camera_angles_x < -90)
    camera_angles_x = -90;
  else
  if (camera_angles_x > 90)
    camera_angles_x = 90;

  return true;
}
