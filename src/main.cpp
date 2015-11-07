#include "gl.hpp"
#include "program.hpp"
#include "object.hpp"

#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static Program build_program();

static void iterate();

static GLFWCALL void on_key(int key, int action);
static EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data);

GLuint mvp_uniform;
static GLuint texture_uniform;

static bool keys[GLFW_KEY_LAST];

static float pos_x = 0, pos_y = -4;
static float delta_z = 0, delta_x = 0;

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

  Program program = build_program();
  program.use();

  mvp_uniform = program.get_uniform_location("mvp");

  texture_uniform = program.get_uniform_location("texture");
  glUniform1i(texture_uniform, 0);

  suzanne = new Model("/data/models/suzanne.obj");
  teapot = new Model("/data/models/teapot.obj");
  bunny = new Model("/data/models/bunny.obj");

  suzanne1 = new Object(*suzanne);
  suzanne1->position.y = 2;
  suzanne1->position.z = 2;

  teapot1 = new Object(*teapot);
  teapot1->position.x = -2.0;
  teapot1->rotation.z = 45;

  bunny1 = new Object(*bunny);
  bunny1->position.x = 2.0;

  glViewport(0, 0, 640, 480);

  glEnable(GL_DEPTH_TEST);

  glClearColor(1, 1, 1, 0);

  emscripten_set_main_loop(iterate, 0, 1);
}

Program build_program()
{
  const Shader vertex_shader = Shader(GL_VERTEX_SHADER, "/data/shaders/vertex.glsl");
  const Shader fragment_shader = Shader(GL_FRAGMENT_SHADER, "/data/shaders/fragment.glsl");

  Program program;
  program.attach_shader(vertex_shader);
  program.attach_shader(fragment_shader);
  program.bind_attrib_location(0, "position");
  program.bind_attrib_location(1, "tex_coord");
  program.bind_attrib_location(2, "normal");
  program.link();

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  return program;
}

void iterate()
{
  if (keys['W'])
  {
    pos_x += 0.1 * sin(glm::radians(delta_z));
    pos_y += 0.1 * cos(glm::radians(delta_z));
  }

  if (keys['S'])
  {
    pos_x -= 0.1 * sin(glm::radians(delta_z));
    pos_y -= 0.1 * cos(glm::radians(delta_z));
  }

  if (keys['A'])
  {
    pos_x -= 0.1 * cos(glm::radians(delta_z));
    pos_y += 0.1 * sin(glm::radians(delta_z));
  }

  if (keys['D'])
  {
    pos_x += 0.1 * cos(glm::radians(delta_z));
    pos_y -= 0.1 * sin(glm::radians(delta_z));
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-pos_x, -pos_y, -2.0f));

  glm::mat4 view = glm::mat4(1.0f)
    * glm::rotate(glm::mat4(1.0f), glm::radians(delta_x - 90), glm::vec3(1.0f, 0.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(delta_z), glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 projection = glm::perspective(45.0f, (float)640 / (float)480, 0.1f, 10.0f);

  glm::mat4 mvp = projection * view * model;

  suzanne1->draw(mvp);
  teapot1->draw(mvp);
  bunny1->draw(mvp);
}

GLFWCALL void on_key(int key, int action)
{
  keys[key] = action != GLFW_RELEASE;
}

EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data)
{
  delta_z += mouse_event->movementX;
  delta_x += mouse_event->movementY;

  if (delta_z < 0)
    delta_z = 359;
  else
  if (delta_z >= 360)
    delta_z = 0;

  if (delta_x < -90)
    delta_x = -90;
  else
  if (delta_x > 90)
    delta_x = 90;

  return true;
}
