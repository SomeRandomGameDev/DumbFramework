#ifndef _DUMB_FW_IMGUI_DELEGATE_
#define _DUMB_FW_IMGUI_DELEGATE_

#include <external/stb_image.h>
#include <imgui/imgui.h>
#include <DumbFramework/windowhint.hpp>
#include <DumbFramework/program.hpp>
#include <DumbFramework/texture2d.hpp>
#include <DumbFramework/vertexbuffer.hpp>
#include <DumbFramework/vertexstream.hpp>

namespace Framework {

class ImGuiDelegate
{
    public:
        ImGuiDelegate();
        ~ImGuiDelegate();
        
        GLFWwindow *createWindow();
        void destroyWindow(GLFWwindow *window);
        void init();
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
        GLFWwindow *_window;
        Program _program;
        Texture2D _fontTexture;
        VertexBuffer _vertexBuffer;
        VertexStream _vertexStream;
        bool _mousePressed[2];
        glm::vec2 _mousePosScale;
        int _projectionMatrixId;
};

} // Framework

#endif // _DUMB_FW_IMGUI_DELEGATE_
