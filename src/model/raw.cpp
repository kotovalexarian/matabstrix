#include "raw.hpp"

#include <cstdio>

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

void Raw::draw() const
{
  glEnableVertexAttribArray(INDEX_POSITION);
  glBindBuffer(GL_ARRAY_BUFFER, positions_id);
  glVertexAttribPointer(INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  glDisableVertexAttribArray(INDEX_TEX_COORD);

  glEnableVertexAttribArray(INDEX_NORMAL);
  glBindBuffer(GL_ARRAY_BUFFER, normals_id);
  glVertexAttribPointer(INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
}
