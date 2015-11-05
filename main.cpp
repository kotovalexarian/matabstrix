#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static GLuint load_shader(GLenum type, const char *source);
static GLuint build_program();

static GLuint create_array_buffer(GLuint index, GLsizeiptr size, const GLvoid *data, GLint count, GLenum type);
static GLuint create_element_array_buffer(GLsizeiptr size, const GLvoid *data);

static void iterate();

static GLFWCALL void on_key(int key, int action);
static EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data);

static GLuint mvp_id;

static bool keys[GLFW_KEY_LAST];

static float pos_x = 0, pos_y = -4;
static float delta_z = 0, delta_x = 0;

static GLuint cube_id;
static GLuint pyramid_id;

static const char vertex_shader_source[] =  \
"attribute vec4 position;                \n"\
"attribute vec3 color;                   \n"\

"varying vec4 f_color;                   \n"\

"uniform mat4 mvp;                       \n"\

"void main(void) {                       \n"\
"  gl_Position = mvp * position;         \n"\
"  f_color = vec4(color, 1.0);           \n"\
"}                                       \n";

static const char fragment_shader_source[] = \
"precision lowp float;                    \n"\

"varying vec4 f_color;                    \n"\

"void main(void) {                        \n"\
"  gl_FragColor = f_color;                \n"\
"}                                        \n";

static GLfloat vertices[] = {
  // front
  -1.0, -1.0,  1.0,
  1.0, -1.0,  1.0,
  1.0,  1.0,  1.0,
  -1.0,  1.0,  1.0,
  // back
  -1.0, -1.0, -1.0,
  1.0, -1.0, -1.0,
  1.0,  1.0, -1.0,
  -1.0,  1.0, -1.0,
  // pyramid
  2 + 0.0, 0.0, 1.0,
  2 + -0.5, -0.5, 0.0,
  2 + 0.5, 0.5, 0.0,
  2 + -0.5, 0.5, 0.0,
  2 + 0.5, -0.5, 0.0,
};

const GLfloat colors[] = {
  // front colors
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
  1.0, 1.0, 1.0,
  // back colors
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
  1.0, 1.0, 1.0,
  // pyramid
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
  1.0, 1.0, 0.0,
  1.0, 0.0, 1.0,
};

const GLushort cube[] = {
  // front
  0, 1, 2,
  2, 3, 0,
  // top
  3, 2, 6,
  6, 7, 3,
  // back
  7, 6, 5,
  5, 4, 7,
  // bottom
  4, 5, 1,
  1, 0, 4,
  // left
  4, 0, 3,
  3, 7, 4,
  // right
  1, 5, 6,
  6, 2, 1,
};

const GLushort pyramid[] = {
  8, 9, 11,
  8, 9, 12,
  8, 10, 11,
  8, 10, 12,
  9, 10, 11,
  9, 10, 12,
};

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

  const GLuint program = build_program();
  glUseProgram(program);

  mvp_id = glGetUniformLocation(program, "mvp");

  create_array_buffer(0, sizeof(vertices), vertices, 3, GL_FLOAT);
  glEnableVertexAttribArray(0);

  create_array_buffer(1, sizeof(colors), colors, 3, GL_FLOAT);
  glEnableVertexAttribArray(1);

  cube_id = create_element_array_buffer(sizeof(cube), cube);
  pyramid_id = create_element_array_buffer(sizeof(pyramid), pyramid);

  glViewport(0, 0, 640, 480);

  glEnable(GL_DEPTH_TEST);

  glClearColor(0, 0, 0, 0);

  emscripten_set_main_loop(iterate, 0, 1);
}

GLuint load_shader(const GLenum type, const char *source)
{
  const GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  return shader;
}

GLuint build_program()
{
  const GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_shader_source);
  const GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glBindAttribLocation(program, 0, "position");
  glBindAttribLocation(program, 1, "color");
  glLinkProgram(program);
  return program;
}

GLuint create_array_buffer(GLuint index, GLsizeiptr size, const GLvoid *data, GLint count, GLenum type)
{
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(index, count, type, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
  return id;
}

GLuint create_element_array_buffer(GLsizeiptr size, const GLvoid *data)
{
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  return id;
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

  glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_id);
  glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid_id);
  glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_SHORT, 0);
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
