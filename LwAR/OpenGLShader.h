#pragma once


static const char shaderUnlitFrag[] =
"#version 330\n"
"\n"
"in vec2 UV;\n"
"out vec3 color;\n"
"uniform sampler2D myTextureSampler;\n"
"uniform mat4 MV;\n"
"\n"
"void main(void) {\n"
"    color = texture(myTextureSampler, UV).rgb;\n"
"}\n"
;

static const char shaderUnlitVert[] =
"#version 330\n"
"\n"
"in vec2 UV;\n"
"out vec3 color;\n"
"uniform sampler2D myTextureSampler;\n"
"uniform mat4 MV;\n"
"\n"
"void main(void) {\n"
"    color = texture(myTextureSampler, UV).rgb;\n"
"}\n"
;

