#include "OpenGLTexture.h"
#include "Turtle/Core/Log.h"
#include "turtpch.h"

#include "Turtle/Core/Hash.h"

#include "stb_image.h"
#include <GL/gl.h>

namespace Turtle {

OpenGLTexture2D::OpenGLTexture2D(const std::string &path) : m_Path(path) {
  TURT_PROFILE_FUNCTION();

  m_AssetID = MurmurHash(path.c_str(), path.size());

  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc *data = nullptr;
  {
    TURT_PROFILE_SCOPE("2D texture load - "
                       "OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  }
  TURT_CORE_ASSERT(data, "failed to load image.");
  m_Width = width;
  m_Height = height;

  GLenum internalFormat = 0, dataFormat = 0;
  if (channels == 4) {
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;
  } else if (channels == 3) {
    internalFormat = GL_RGB8;
    dataFormat = GL_RGB;
  }

  TURT_CORE_ASSERT(internalFormat & dataFormat, "Format not supported.");

  m_InternalFormat = internalFormat;
  m_DataFormat = dataFormat;

  glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
  glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

  glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat,
                      GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height) {
  TURT_PROFILE_FUNCTION();

  m_InternalFormat = GL_RGBA8;
  m_DataFormat = GL_RGBA;

  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);
  glTexImage2D(m_RendererID, 0, m_InternalFormat, m_Width, m_Height, 0,
               GL_TEXTURE_2D, GL_UNSIGNED_INT_8_8_8_8, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void OpenGLTexture2D::SetData(void *data, uint32_t size) {
  TURT_PROFILE_FUNCTION();

  uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
  TURT_CORE_ASSERT(size == m_Width * m_Height * bpp,
                   "Data must be entire texture");

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat,
                  GL_UNSIGNED_BYTE, data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
  TURT_PROFILE_FUNCTION();

  glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
  TURT_PROFILE_FUNCTION();

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
} // namespace Turtle
