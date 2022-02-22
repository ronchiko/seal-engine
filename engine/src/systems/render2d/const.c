
#include "const.h"

const char *SEAL_SHADER_VERTEX_PARAM = "Seal_Vertex";
const char *SEAL_SHADER_UV_PARAM = "Seal_UV";
const char *SEAL_SHADER_TRANSFORM_PARAM = "Seal_Transform";
const char *SEAL_SHADER_TINT_PARAM = "Seal_Tint";
const char *SEAL_SHADER_TEXTURE_PARAM = "Seal_Texture";
const char *SEAL_SHADER_CAMERA_MATRIX_PARAM = "Seal_Camera";

#pragma region SHADER FALLBACKS

const char *SEAL_VERTEX_SHADER_FALLBACK =
	"#version 450\n\n"\
	"in vec2 Seal_Vertex;\n"\
	"in vec2 Seal_UV;\n"\
	"in mat3 Seal_Transform;"\
	"in vec4 Seal_Tint;\n"\
	"\n"\
	"uniform mat3 Seal_Camera;\n"\
	"\n"\
	"out vec2 _UV;\n"\
	"out vec4 _Tint;\n"\
	"\n"\
	"void main() {\n"\
	"\tvec3 pos = Seal_Camera * Seal_Transform * vec3(Seal_Vertex.xy, 1.0);\n"\
	"\tgl_Position = vec4(pos.xyz, 1.0);\n"\
	"\t_Tint = Seal_Tint;\n"\
	"\t_UV = Seal_UV;\n}";

const char *SEAL_FRAGMENT_SHADER_FALLBACK = \
	"#version 450\nprecision mediump float;\n\n"\
	"in vec2 _UV;\n"\
	"in vec4 _Tint;\n"\
	"\n"\
	"out vec4 _Frag;\n"\
	"\n"\
	"uniform sampler2D Seal_Texture;\n"\
	"\n"\
	"void main() {\n"\
	"\t_Frag = texture2D(Seal_Texture, _UV) * _Tint;\n"\
	"}\n";


#pragma endregion