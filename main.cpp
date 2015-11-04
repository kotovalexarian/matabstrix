#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data);

static GLuint load_shader(GLenum type, const char *source);

static GLuint vertex_shader;
static GLuint fragment_shader;

static GLuint program;

static void iterate();

static GLuint view_matrix;

static float delta_z = 0, delta_x = 0;

static const char vertex_shader_source[] =  \
"attribute vec4 position;                \n"\
"attribute vec3 color;                   \n"\

"varying vec4 f_color;                   \n"\

"uniform mat4 view_matrix;               \n"\

"void main(void) {                       \n"\
"  gl_Position = view_matrix * position; \n"\
"  f_color = vec4(color, 1.0);           \n"\
"}                                       \n";

static const char fragment_shader_source[] = \
"precision lowp float;                    \n"\

"varying vec4 f_color;                    \n"\

"void main(void) {                        \n"\
"  gl_FragColor = f_color;                \n"\
"}                                        \n";

static const GLfloat triangle_vertices[] = {
   0.0,  0.0,  0.8,  1.0, 0.0, 0.0,
  -0.3,  0.0,  0.0,  0.0, 1.0, 0.0,
   0.3,  0.0,  0.0,  0.0, 0.0, 1.0,
};

static GLuint triangle;

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

  emscripten_set_mousemove_callback(nullptr, nullptr, false, on_em_mousemove);

  vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_shader_source);
  fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glBindAttribLocation(program, 0, "position");
  glBindAttribLocation(program, 1, "color");
  glLinkProgram(program);
  glUseProgram(program);

  view_matrix = glGetUniformLocation(program, "view_matrix");

  glGenBuffers(1, &triangle);
  glBindBuffer(GL_ARRAY_BUFFER, triangle);
  glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(GLfloat), triangle_vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(0));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glViewport(0, 0, 640, 480);

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

void iterate()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glm::mat4 view = glm::mat4(1.0f)
    * glm::rotate(glm::mat4(1.0f), glm::radians(delta_x - 90), glm::vec3(1.0f, 0.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(delta_z), glm::vec3(0.0f, 0.0f, 1.0f));

  glUniformMatrix4fv(view_matrix, 1, GL_FALSE, glm::value_ptr(view));

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glfwPollEvents();
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
