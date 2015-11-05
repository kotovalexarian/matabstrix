#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

class Shader
{
public:
  Shader(GLenum type, const char *filename);
  inline GLuint id() const { return _id; };

private:
  GLuint _id;
};

class Program
{
public:
  Program();
  void attach_shader(const Shader &shader);
  void bind_attrib_location(GLuint index, const GLchar *name);
  void link();
  void use();
  GLuint get_uniform_location(const GLchar *name);

private:
  GLuint _id;
};

class Model
{
public:
  Model(const char *filename);
  void draw() const;

private:
  std::vector<GLfloat> positions;
  GLuint positions_id;

  std::vector<GLfloat> colors;
  GLuint colors_id;

  std::vector<GLushort> elements;
  GLuint id;

  static GLuint create_array_buffer(GLenum type, GLsizeiptr size, const GLvoid *data);
};

class Object
{
public:
  Object(const Model &model) : _model(model) {};
  void draw(const glm::mat4 &mvp) const;

  glm::vec3 position;
  glm::vec3 rotation;

private:
  const Model &_model;
};

static Program build_program();

static void iterate();

static GLFWCALL void on_key(int key, int action);
static EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data);

static GLuint mvp_id;

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

  mvp_id = program.get_uniform_location("mvp");

  suzanne = new Model("/data/models/suzanne.obj");
  teapot = new Model("/data/models/teapot.obj");
  bunny = new Model("/data/models/bunny.obj");

  suzanne1 = new Object(*suzanne);

  teapot1 = new Object(*teapot);
  teapot1->position.x = -3.0;
  teapot1->rotation.z = -45;

  bunny1 = new Object(*bunny);
  bunny1->position.x = 3.0;

  glViewport(0, 0, 640, 480);

  glEnable(GL_DEPTH_TEST);

  glClearColor(0, 0, 0, 0);

  emscripten_set_main_loop(iterate, 0, 1);
}

Shader::Shader(const GLenum type, const char *const filename)
{
  FILE *file = fopen(filename, "r");

  fseek(file, 0, SEEK_END);
  const long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *source = (char*)malloc(size + 1);
  fread(source, size, 1, file);
  source[size] = 0;

  fclose(file);

  _id = glCreateShader(type);
  glShaderSource(_id, 1, (const GLchar**)&source, nullptr);
  glCompileShader(_id);

  free(source);
}

Program::Program()
{
  _id = glCreateProgram();
}

void Program::attach_shader(const Shader &shader)
{
  glAttachShader(_id, shader.id());
}

void Program::bind_attrib_location(const GLuint index, const GLchar *const name)
{
  glBindAttribLocation(_id, index, name);
}

void Program::link()
{
  glLinkProgram(_id);
}

void Program::use()
{
  glUseProgram(_id);
}

GLuint Program::get_uniform_location(const GLchar *name)
{
  return glGetUniformLocation(_id, name);
}

Model::Model(const char *const filename)
{
  std::ifstream file(filename, std::ios::in);

  std::string line;
  while (std::getline(file, line))
  {
    if (line.substr(0,2) == "v ")
    {
      std::istringstream s(line.substr(2));
      GLfloat x, y, z;
      s >> x; s >> y, s >> z;
      positions.push_back(x);
      positions.push_back(y);
      positions.push_back(z);
      colors.push_back(1.0f);
      colors.push_back(0.0f);
      colors.push_back(0.0f);
    }
    else
    if (line.substr(0,2) == "f ")
    {
      std::istringstream s(line.substr(2));
      GLushort a, b, c;
      s >> a;
      s >> b;
      s >> c;
      elements.push_back(a - 1);
      elements.push_back(b - 1);
      elements.push_back(c - 1);
    }
  }

  positions_id = create_array_buffer(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data());
  colors_id = create_array_buffer(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), colors.data());
  id = create_array_buffer(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLushort), elements.data());
}

GLuint Model::create_array_buffer(const GLenum type, const GLsizeiptr size, const GLvoid *const data)
{
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(type, id);
  glBufferData(type, size, data, GL_STATIC_DRAW);
  return id;
}

void Model::draw() const
{
  glBindBuffer(GL_ARRAY_BUFFER, positions_id);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  glBindBuffer(GL_ARRAY_BUFFER, colors_id);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
}

void Object::draw(const glm::mat4 &mvp) const
{
  const glm::mat4 transform = glm::translate(mvp, -position)
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(transform));

  _model.draw();
}

Program build_program()
{
  const Shader vertex_shader = Shader(GL_VERTEX_SHADER, "/data/shaders/vertex.glsl");
  const Shader fragment_shader = Shader(GL_FRAGMENT_SHADER, "/data/shaders/fragment.glsl");

  Program program;
  program.attach_shader(vertex_shader);
  program.attach_shader(fragment_shader);
  program.bind_attrib_location(0, "position");
  program.bind_attrib_location(1, "color");
  program.link();

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

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
