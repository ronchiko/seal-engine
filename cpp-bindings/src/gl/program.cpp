#include <seal/cpp/gl.hpp>

#include <seal/cpp/error.hpp>

using namespace seal::gl;

Seal_GL_Program _create_program(shader vertex, shader fragment) 
{
	if(vertex.type() != SEAL_SHADER_VERTEX || fragment.type() != SEAL_SHADER_FRAGMENT) {
		throw seal::error("Got invalid shader types for program");
	}

	Seal_GL_Program program = Seal_GL_LinkProgram(
		{vertex.id(), vertex.type()},
		{fragment.id(), fragment.type()}
	);
	if(program == SEAL_GL_NO_PROGRAM) {
		throw seal::error("Failed to create program");
	}

	return program;
}

program::program(shader vertex, shader fragment) 
	: m_program(_create_program(std::move(vertex), std::move(fragment))) 
{
}

program::program(program&& other) noexcept
	: m_program(other.m_program) 
{
	other.m_program = NO_PROGRAM;
}

program& program::operator=(program&& other) noexcept {
	m_program = other.m_program;
	other.m_program = NO_PROGRAM;
	
	return *this;
}

program::~program() {
	if(m_program != NO_PROGRAM) {
		Seal_GL_DestroyProgram(m_program);
	}
}

void program::use() const {
	Seal_GL_UseProgram(m_program);
}

Seal_Int program::attribute(const std::string& name) const {
	auto attribute = Seal_GL_ProgramAttribLocation(m_program, name.c_str());
	if (attribute == SEAL_GL_NO_ATTRIBUTE) {
		throw seal::error("Program attribute not found");
	}

	return attribute;
}

Seal_GL_UniformAddress program::uniform(const std::string& name) const {
	auto uniform = Seal_GL_ProgramUniformLocation(m_program, name.c_str());
	if (uniform == SEAL_GL_NO_UNIFORM) {
		throw seal::error("Program uniform not found");
	}

	return uniform;
}