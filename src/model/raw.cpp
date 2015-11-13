#include "raw.hpp"

#include "../main.hpp"

#include <cstdio>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Models;

const std::string Raw::filename(const std::string &name)
{
  return "/data/models/" + name;
}

Raw::Raw(__attribute__((unused)) Store &store, const std::string &name)
{
  FILE *file = fopen(filename(name).c_str(), "r");

  while (!feof(file))
  {
    glm::vec3 v[3];

    for (int i = 0; i < 3; ++i)
      fscanf(file, "%f %f %f", &v[i].x, &v[i].y, &v[i].z);

    glm::vec3 normal = (v[0] - v[1]) * (v[2] - v[1]);

    for (int i = 0; i < 3; ++i)
      add(v[i], normal);
  }

  fclose(file);

  positions_id = create_array_buffer(GL_ARRAY_BUFFER, 3 * positions.size() * sizeof(GLfloat), positions.data());
  normals_id = create_array_buffer(GL_ARRAY_BUFFER, 3 * normals.size() * sizeof(GLfloat), normals.data());
  id = create_array_buffer(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLushort), elements.data());
}

void Raw::draw(const glm::mat4 &mvp, const glm::mat4 &transformation) const
{
  const glm::mat4 transform = mvp * transformation;
  glUniformMatrix4fv(exe->uniform(Uniform::mvp), 1, GL_FALSE, glm::value_ptr(transform));

  const glm::mat3 local_modelview = glm::transpose(glm::inverse(glm::mat3(transformation)));
  glUniformMatrix3fv(exe->uniform(Uniform::local_modelview), 1, GL_FALSE, glm::value_ptr(local_modelview));

  glEnableVertexAttribArray(position);
  glBindBuffer(GL_ARRAY_BUFFER, positions_id);
  glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  glEnableVertexAttribArray(normal);
  glBindBuffer(GL_ARRAY_BUFFER, normals_id);
  glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  glDisableVertexAttribArray(tex_coord);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
}
