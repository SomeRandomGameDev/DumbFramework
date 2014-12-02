
namespace Framework {

// ImGui vertex shader
const char* ImGuiDelegateVertexShader()
{
    static const char* vertexShader = R"EOT(
#version 410 core
uniform mat4 projection;

layout (location=0) in vec2 vs_position;
layout (location=1) in vec2 vs_uv;
layout (location=2) in vec4 vs_color;

noperspective out vec2 fs_uv;
noperspective out vec4 fs_col;
void main()
{
    fs_col = vs_color;
    fs_uv  = vs_uv;
    gl_Position = projection * vec4(vs_position, 0.0, 1.0);
}
)EOT";
    return vertexShader;
}

// ImGui fragment shader
const char* ImGuiDelegateFragmentShader()
{
    static const char* fragmentShader = R"EOT(
#version 410 core
uniform sampler2D un_font;

noperspective in vec2 fs_uv;
noperspective in vec4 fs_col;

out vec4 out_color;

void main()
{
    out_color = texture(un_font, fs_uv) * fs_col;
}
)EOT";
    return fragmentShader;
}

} // Framework
