#include "OpenGLShader.h"
#include "Turtle/Core/Log.h"
#include "turtpch.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>

namespace Turtle {

static GLenum ShaderTypeFromString(const std::string &type) {
  TURT_PROFILE_FUNCTION();

  if (type == "vertex")
    return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel")
    return GL_FRAGMENT_SHADER;

  TURT_CORE_ASSERT(false, "Unsupported shader type");
  return 0;
}

OpenGLShader::OpenGLShader(const std::string &filepath) {
  TURT_PROFILE_FUNCTION();

  auto lastSlash = filepath.find_last_of("/\\");
  lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
  auto lastDot = filepath.rfind('.');
  auto count = lastDot == std::string::npos ? filepath.size() - lastSlash
                                            : lastDot - lastSlash;
  m_Name = filepath.substr(lastSlash, count);

  char buffer[10000] = {0};
  getcwd(buffer, 10000);
  TURT_CORE_INFO("{0}", buffer);
  std::string source = ReadFile(filepath);
  auto shaderSources = PreProcess(source);
  Compile(shaderSources);
}

OpenGLShader::OpenGLShader(const std::string &name,
                           const std::string &vertexSrc,
                           const std::string &fragmentSrc)
    : m_Name(name) {
  TURT_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> sources;
  sources[GL_VERTEX_SHADER] = vertexSrc;
  sources[GL_FRAGMENT_SHADER] = fragmentSrc;
  Compile(sources);
}

OpenGLShader::~OpenGLShader() {
  TURT_PROFILE_FUNCTION();

  glDeleteProgram(m_RendererID);
}

std::string OpenGLShader::ReadFile(const std::string filepath) {
  TURT_PROFILE_FUNCTION();

  std::string result;

  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&result[0], result.size());
    in.close();
  } else {
    TURT_CORE_ERROR("Could not load shader file '{0}'", filepath);
  }
  return result;
}

std::unordered_map<GLenum, std::string>
OpenGLShader::PreProcess(const std::string &source) {
  TURT_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> shaderSources;

  const char *typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);
    TURT_CORE_ASSERT(eol != std::string::npos, "Syntax error");
    size_t begin = pos + typeTokenLength + 1;
    std::string type = source.substr(begin, eol - begin);
    TURT_CORE_ASSERT(ShaderTypeFromString(type),
                     "Unsupported shader type specifier.");

    size_t nextLinePos = source.find_first_not_of("\r\n", eol);
    pos = source.find(typeToken, nextLinePos);
    shaderSources[ShaderTypeFromString(type)] = source.substr(
        nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1
                                                             : nextLinePos));
  }

  return shaderSources;
}

void OpenGLShader::Compile(
    const std::unordered_map<GLenum, std::string> &shaderSources) {
  TURT_PROFILE_FUNCTION();

  GLuint program = glCreateProgram();
  TURT_CORE_ASSERT(shaderSources.size() <= 3,
                   "Shader limit exceeded, currently only support 3");
  std::array<GLenum, 3> glShaderIDs;
  int glShaderIDIndex = 0;

  for (auto &kv : shaderSources) {
    GLenum shaderType = kv.first;
    const std::string &source = kv.second;

    GLuint shader = glCreateShader(shaderType);

    const GLchar *GLsource = source.c_str();
    glShaderSource(shader, 1, &GLsource, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

      glDeleteShader(shader);

      TURT_CORE_ERROR("{0}", infoLog.data());
      TURT_CORE_ASSERT(false, "Shader compilation failure.");
      break;
    }
    glAttachShader(program, shader);
    glShaderIDs[glShaderIDIndex++] = shader;
  }

  glLinkProgram(program);

  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    glDeleteProgram(program);

    for (auto id : glShaderIDs)
      glDeleteShader(id);

    TURT_CORE_ERROR("{0}", infoLog.data());
    TURT_CORE_ASSERT(false, "Shader linking failure.");
    return;
  }

  for (auto id : glShaderIDs)
    glDetachShader(program, id);

  m_RendererID = program;
}

void OpenGLShader::Bind() const {
  TURT_PROFILE_FUNCTION();

  glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
  TURT_PROFILE_FUNCTION();

  glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string &name, int value) {
  UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string &name, int *data,
                               uint32_t count) {
  UploadUniformIntArray(name, data, count);
}

void OpenGLShader::SetFloat(const std::string &name, float value) {
  UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat2(const std::string &name, const glm::vec2 &value) {
  UploadUniformFloat2(name, value);
}

void OpenGLShader::SetFloat3(const std::string &name, const glm::vec3 &value) {
  UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string &name, const glm::vec4 &value) {
  UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value) {
  UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string &name, int value) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string &name, int *data,
                                         uint32_t count) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform1iv(location, count, data);
}

void OpenGLShader::UploadUniformFloat(const std::string &name, float value) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string &name,
                                       const glm::vec2 &values) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform2f(location, values.x, values.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string &name,
                                       const glm::vec3 &values) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string &name,
                                       const glm::vec4 &values) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(const std::string &name,
                                     const glm::mat3 &matrix) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string &name,
                                     const glm::mat4 &matrix) {
  GLint location = glGetUniformLocation(m_RendererID, name.c_str());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
} // namespace Turtle
