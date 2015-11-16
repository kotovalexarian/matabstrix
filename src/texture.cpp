#include "texture.hpp"

#include <string>

#include <stb_image.h>

const std::string Texture::filename(const std::string &name)
{
  return "/textures/" + name;
}

Texture::Texture(const Adapter &adapter, const std::string &name)
{
  GLsizei width, height, comp;
  GLvoid *data = stbi_load(adapter.filename<Texture>(name).c_str(), &width, &height, &comp, 0);

  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

void Texture::use() const
{
  glBindTexture(GL_TEXTURE_2D, _id);
}
