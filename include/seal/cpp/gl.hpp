#pragma once

#ifdef __cplusplus

#include <string>

#include <seal/gl.h>

namespace seal::gl {

class shader final {
public:
	shader(const std::string& , Seal_GL_ShaderType type);

	shader(const shader& shader) = delete;
	shader& operator=(const shader& shader) = delete;

	shader(shader&&) noexcept;
	shader& operator=(shader&&) noexcept;

	~shader();

	Seal_ID id() const;
	Seal_GL_ShaderType type() const;

private:
	Seal_GL_Shader m_shader;
};

class program final {
public:
	program() = default;

	program(shader vertex, shader fragment);

	program(const program& shader) = delete;
	program& operator=(const program& shader) = delete;

	program(program&&) noexcept;
	program& operator=(program&&) noexcept;

	~program();

	Seal_Int attribute(const std::string& name) const;
	Seal_Int uniform(const std::string& name) const;

	void use() const;

private:

	Seal_GL_Program m_program = NO_PROGRAM;
};

}

#endif