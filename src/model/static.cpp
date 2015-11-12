#include "static.hpp"

#include "../mtllib.hpp"

#include <vector>
#include <fstream>
#include <sstream>

using namespace Models;

const std::string Static::filename(const std::string &name)
{
  return "/data/models/" + name;
}

Static::Static(Store &store, const std::string &name)
{
  std::ifstream file(filename(name), std::ios::in);

  const Mtllib *mtllib = nullptr;

  std::vector<glm::vec3> tmp_positions;
  std::vector<glm::vec2> tmp_tex_coords;
  std::vector<glm::vec3> tmp_normals;

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
        if (attr_count == 3)
          add(tmp_positions[v[i] - 1],
              tmp_normals[vn[i] - 1],
              tmp_tex_coords[vt[i] - 1]);
        else
          Mesh::add(tmp_positions[v[i] - 1],
                    tmp_normals[vn[i] - 1]);
    }
    else
    if (line.substr(0, 7) == "mtllib ")
    {
      mtllib = (Mtllib*)store.load<Mtllib>(line.substr(7));
    }
    else
    if (line.substr(0, 7) == "usemtl ")
    {
      _material = (*mtllib)[line.substr(7)];
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

void Static::draw() const
{
  _material->use();

  glBindBuffer(GL_ARRAY_BUFFER, positions_id);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(0));
  glEnableVertexAttribArray(0);

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
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

  glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
}
