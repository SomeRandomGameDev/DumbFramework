/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <DumbFramework/imguidelegate.hpp>
namespace Dumb {

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

} // Dumb
