#ifndef _DUMB_FW_IMGUI_DELEGATE_
#define _DUMB_FW_IMGUI_DELEGATE_

#include <functional>
#include <external/stb_image.h>
#include <imgui/imgui.h>
#include <DumbFramework/windowhint.hpp>
#include <DumbFramework/program.hpp>
#include <DumbFramework/texture2d.hpp>
#include <DumbFramework/vertexbuffer.hpp>
#include <DumbFramework/vertexstream.hpp>

namespace Framework {

/**
 * ImGui delegate.
 */
class ImGuiDelegate
{
    public:
        /** 
         * Constructor.
         * @param [in] width  Window witdh
         * @param [in] height Window height
         * @param [in] title  Window title.
         * @param [in] renderDelegate    Render delegate.
         */
        ImGuiDelegate(int width, int height, char const* title,
                      std::function<void()> initDelegate,
                      std::function<void()> renderDelegate);
        /**
         * Destructor.
         */
        ~ImGuiDelegate();
        /**
         * Create window.
         */
        GLFWwindow *createWindow();
        /**
         * Destroy window.
         * @param [in] window Window pointer.
         */
        void destroyWindow(GLFWwindow *window);
        /**
         * Initialize delegate.
         */
        void init();
        /**
         * Render everything.
         */
        void render();
        void handleKeyAction(int key, int scancode, int action, int mods);
        void handleMouseButtonAction(int button, int action, int mods);
        void handleMousePositionAction(double x, double y);
        void handleMouseWheelAction(double x, double y);
        void handleWindowSize(int x, int y);
        void handleChar(unsigned int unicodeChar);
        void handleCursorEnter(int flag);
        
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
        Program _program;
        /** Font texture. **/
        Texture2D _fontTexture;
        /** Vertex buffer. **/
        VertexBuffer _vertexBuffer;
        /** Vertex stream. **/
        VertexStream _vertexStream;
        /** Mouse button state (true: pressed, false: released). **/
        bool _mousePressed[2];
        /** Mouse pointer scaled position. **/
        glm::vec2 _mousePosScale;
        /** Projection matrix uniform id. **/
        int _projectionMatrixId;
        /** Init delegate. **/
        std::function<void()> _init;
        /** Render delegate. **/
        std::function<void()> _render;
};

} // Framework

#endif // _DUMB_FW_IMGUI_DELEGATE_
