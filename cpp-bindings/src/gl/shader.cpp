#include <seal/cpp/gl.hpp>

#include <seal/cpp/error.hpp>

using namespace seal::gl;

Seal_GL_Shader _create_shader(const std::string& source, Seal_GL_ShaderType type) {
	auto shader = Seal_GL_CompileShader(source.c_str(), type);
	if(shader.id == SEAL_GL_NO_SHADER) {
		throw seal::error("Failed to create shader");
	}

	return shader;
}

shader::shader(const std::string& source, Seal_GL_ShaderType type) 
	: m_shader(_create_shader(source, type))
{
}

shader::shader(shader&& other) noexcept 
	: m_shader(other.m_shader)
{
	other.m_shader.id = SEAL_GL_NO_SHADER;
}

shader& shader::operator=(shader&& other) noexcept 
{
	 m_shader = other.m_shader;
	other.m_shader.id = SEAL_GL_NO_SHADER;

	return *this;
}

shader::~shader() {
	if(m_shader.id != SEAL_GL_NO_SHADER) {
		Seal_GL_DestroyShader(m_shader);
	}
}

Seal_ID shader::id() const {
	return m_shader.id;
}

Seal_GL_ShaderType shader::type() const {
	return m_shader.type;
}
