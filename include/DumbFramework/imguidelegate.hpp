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
#ifndef _DUMB_FW_IMGUI_DELEGATE_
#define _DUMB_FW_IMGUI_DELEGATE_

#include <DumbFramework/runner.hpp>
#include <external/stb_image.h>
#include <imgui/imgui.h>

#include <DumbFramework/render/program.hpp>
#include <DumbFramework/render/texture2d.hpp>
#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/renderer.hpp>
#include <DumbFramework/runner.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Dumb {

const char* ImGuiDelegateVertexShader();
const char* ImGuiDelegateFragmentShader();

/**
 * ImGui delegate.
 * @param <T> Delegate class.
 * 
 * This is a delegate for the GLFW application wrapper.
 * The delegate must implement the following methods:
 *   - void init()
 *   - void destroy()
 *   - void gui()
 *   - void render()
 */
template <typename T>
class ImGuiDelegate
{
    DECLARE_WRAPPER_METHODS
    
    public:
        /** 
         * Constructor.
         * @param [in] width  Window witdh
         * @param [in] height Window height
         * @param [in] title  Window title.
         * @param [in] delegate Delegate.
         */
        ImGuiDelegate(int width, int height, char const* title, T *delegate);
        /**
         * Destructor.
         */
        ~ImGuiDelegate();

    protected:
        static void renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count);
        static const char* getClipboardText();
        static void setClipboardText(const char* txt);

    protected:
        /** Window width. **/
        int _width;
        /** Window height. **/
        int _height;
        /** Window title. **/
        char const* _title;
        /** Window pointer. **/
        GLFWwindow *_window;
        /** GLSL program **/
        Render::Program _program;
        /** Font texture. **/
        Render::Texture2D _fontTexture;
        /** Vertex buffer. **/
        Render::VertexBuffer _vertexBuffer;
        /** Vertex stream. **/
        Render::VertexStream _vertexStream;
        /** Mouse button state (true: pressed, false: released). **/
        bool _mousePressed[2];
        /** Mouse pointer scaled position. **/
        glm::vec2 _mousePosScale;
        /** Projection matrix uniform id. **/
        int _projectionMatrixId;
        /**  Delegate instance. **/
        T *_delegate;
};

#include "imguidelegate.inl"

} // Dumb

#endif // _DUMB_FW_IMGUI_DELEGATE_
