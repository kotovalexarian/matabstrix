#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <emscripten/emscripten.h>

static GLuint load_shader(GLenum type, const char *source);

static GLuint vertex_shader;
static GLuint fragment_shader;

static GLuint program;

static void iterate();

static GLuint view_matrix;

static glm::mat4 view(
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0
);

static const char vertex_shader_source[] =  \
"attribute vec4 position;                \n"\

"uniform mat4 view_matrix;               \n"\

"void main(void) {                       \n"\
"  gl_Position = view_matrix * position; \n"\
"}                                       \n";

static const char fragment_shader_source[] =   \
"void main(void) {                          \n"\
"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); \n"\
"}                                          \n";

static const GLfloat triangle_vertices[] = {
   0.0,  0.8, 0.0,
  -0.8, -0.8, 0.0,
   0.8, -0.8, 0.0,
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

  vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_shader_source);
  fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glBindAttribLocation(program, 0, "position");
  glLinkProgram(program);
  glUseProgram(program);

  view_matrix = glGetUniformLocation(program, "view_matrix");

  glGenBuffers(1, &triangle);
  glBindBuffer(GL_ARRAY_BUFFER, triangle);
  glBufferData(GL_ARRAY_BUFFER, 3 * 7 * sizeof(GLfloat), triangle_vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<const GLvoid*>(0));
  glEnableVertexAttribArray(0);

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

  glUniformMatrix4fv(view_matrix, 1, GL_FALSE, glm::value_ptr(view));

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glfwPollEvents();
}
