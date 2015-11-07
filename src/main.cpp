#include "program.hpp"
#include "object.hpp"

#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL_image.h>

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static Program build_program();

static void iterate();

static GLFWCALL void on_key(int key, int action);
static EM_BOOL on_em_mousemove(int event_type, const EmscriptenMouseEvent *mouse_event, void *user_data);

static GLuint mvp_uniform;
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

static Texture *suzanne_tex;
static Texture *teapot_tex;
static Texture *bunny_tex;

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

void Program::use() const
{
  glUseProgram(_id);
}

GLuint Program::get_uniform_location(const GLchar *name) const
{
  return glGetUniformLocation(_id, name);
}

Model::Model(const char *const filename)
{
  std::ifstream file(filename, std::ios::in);

  std::vector<glm::vec3> tmp_positions;
  std::vector<glm::vec2> tmp_tex_coords;
  std::vector<glm::vec3> tmp_normals;

  size_t index = 0;

  int attr_count = 0;

  std::string line;
  while (std::getline(file, line))
  {
    if (line.substr(0, 2) == "v ")
    {
      glm::vec3 v;
      sscanf(line.data(), "v %f %f %f", &v.x, &v.y, &v.z);
      tmp_positions.push_back(v);
    }
    else
    if (line.substr(0, 3) == "vt ")
    {
      glm::vec2 vt;
      sscanf(line.data(), "vt %f %f", &vt.x, &vt.y);
      tmp_tex_coords.push_back(vt);
    }
    else
    if (line.substr(0, 3) == "vn ")
    {
      glm::vec3 vn;
      sscanf(line.data(), "vn %f %f %f", &vn.x, &vn.y, &vn.z);
      tmp_normals.push_back(vn);
    }
    else
    if (line.substr(0, 2) == "f ")
    {
      if (attr_count == 0)
      {
        if (line.find("//") == std::string::npos)
          attr_count = 3;
        else
          attr_count = 2;
      }

      GLushort v[3];
      GLushort vt[3];
      GLushort vn[3];

      if (attr_count == 2)
        sscanf(line.data(), "f %hu//%hu %hu//%hu %hu//%hu", &v[0], &vn[0], &v[1], &vn[1], &v[2], &vn[2]);
      else
        sscanf(line.data(), "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu", &v[0], &vt[0], &vn[0],
                                                                     &v[1], &vt[1], &vn[1],
                                                                     &v[2], &vt[2], &vn[2]);

      for (int i = 0; i < 3; ++i)
      {
        positions.push_back(tmp_positions[v[i] - 1]);
        if (attr_count == 3)
          tex_coords.push_back(tmp_tex_coords[vt[i] - 1]);
        normals.push_back(tmp_normals[vn[i] - 1]);
        elements.push_back(index++);
      }
    }
    else
    if (line.substr(0, 7) == "mtllib ")
    {
      _material = new Material(line.substr(7).c_str());
    }
  }

  positions_id = create_array_buffer(GL_ARRAY_BUFFER, 3 * positions.size() * sizeof(GLfloat), positions.data());

  if (attr_count == 3)
    tex_coords_id = create_array_buffer(GL_ARRAY_BUFFER, 2 * tex_coords.size() * sizeof(GLfloat), tex_coords.data());
  else
    tex_coords_id = 0;

  normals_id = create_array_buffer(GL_ARRAY_BUFFER, 3 * normals.size() * sizeof(GLfloat), normals.data());

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
  _material->use();

  glBindBuffer(GL_ARRAY_BUFFER, positions_id);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  if (tex_coords_id == 0)
    glDisableVertexAttribArray(1);
  else
  {
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coords_id);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
  }

  glBindBuffer(GL_ARRAY_BUFFER, normals_id);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
}

void Object::draw(const glm::mat4 &mvp) const
{
  const glm::mat4 transform = glm::translate(mvp, position)
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(transform));

  _model.draw();
}

Texture::Texture(const char *const filename)
{
  SDL_Surface *surface = IMG_Load((std::string("/data/textures/") + filename).c_str());

  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

  SDL_FreeSurface(surface);
}

void Texture::use() const
{
  glBindTexture(GL_TEXTURE_2D, _id);
}

Material::Material(const char *const filename)
{
  std::ifstream file(std::string("/data/materials/") + filename, std::ios::in);

  std::string line;
  while (std::getline(file, line))
  {
    if (line.substr(0, 7) == "map_Kd ")
    {
      _texture = new Texture(line.substr(7).c_str());
    }
  }
}

void Material::use() const
{
  _texture->use();
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

  suzanne_tex->use();
  suzanne1->draw(mvp);

  teapot_tex->use();
  teapot1->draw(mvp);

  bunny_tex->use();
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
