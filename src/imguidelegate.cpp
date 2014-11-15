#include <imgui/imgui.h>
#include <DumbFramework/renderer.hpp>
#include <DumbFramework/imguidelegate.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const char* g_vertexShader = R"EOT(


)EOT";

static const char* g_fragmentShader = R"EOT(


)EOT";

namespace Framework {

ImGuiDelegate::ImGuiDelegate()
    : _window(nullptr)
    , _program()
    , _vertexShader()
    , _fragmentShader()
    , _fontTexture()
    , _vertexBuffer()
    , _vertexStream()
{}

ImGuiDelegate::~ImGuiDelegate()
{
    _program.destroy();
    _vertexShader.destroy();
    _fragmentShader.destroy();
    _fontTexture.destroy();
    _vertexBuffer.destroy();
    _vertexStream.destroy();
}

GLFWwindow* ImGuiDelegate::createWindow()
{
    // [todo]
    return nullptr;
}

void ImGuiDelegate::destroyWindow(GLFWwindow *window)
{
    // [todo]
}

void ImGuiDelegate::init()
{
    // Initialize ImGui.
    int windowWidth, windowHeight;
    int fbWidth, fbHeight;
        
    // Some screens e.g. Retina display have framebuffer size != from window size, and mouse inputs are given in window/screen coordinates.
    glfwGetWindowSize(_window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(_window, &fbWidth, &fbHeight);
    _mousePosScale.x = (float)fbWidth / windowWidth;
    _mousePosScale.y = (float)fbHeight / windowHeight;

    ImGuiIO& io = ImGui::GetIO();
    io.UserData = this;
    // Display size, in pixels. For clamping windows positions.
    io.DisplaySize = ImVec2((float)fbWidth, (float)fbHeight);
    // Time elapsed since last frame, in seconds (in this sample app we'll override this every frame because our timestep is variable)
    io.DeltaTime = 1.0f/60.0f;
    // Align OpenGL texels
    io.PixelCenterOffset = 0.0f;
    // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_Tab]        = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]  = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]    = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow]  = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_Home]       = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End]        = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Delete]     = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace]  = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter]      = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape]     = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A]          = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C]          = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V]          = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X]          = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y]          = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z]          = GLFW_KEY_Z;
    // Setup ImGui callbacks.
    io.RenderDrawListsFn  = ImGuiDelegate::renderDrawLists;
    io.SetClipboardTextFn = ImGuiDelegate::setClipboardText;
    io.GetClipboardTextFn = ImGuiDelegate::getClipboardText;

    // Default font (embedded in code)
    // [todo] texture loader
    const void* png_data;
    unsigned int png_size;
    ImGui::GetDefaultFontData(NULL, NULL, &png_data, &png_size);
    int tex_x, tex_y, tex_comp;
    void* tex_data = stbi_load_from_memory((const unsigned char*)png_data, (int)png_size, &tex_x, &tex_y, &tex_comp, 0);
    IM_ASSERT(tex_data != NULL);

    bool ret = _fontTexture.create(glm::ivec2(tex_x, tex_y), Texture::PixelFormat::RGBA_8);
    if(false == ret)
    {
        // [todo]
    }
    _fontTexture.setMagFilter(Texture::MagFilter::NEAREST_TEXEL);
    _fontTexture.setMinFilter(Texture::MinFilter::NEAREST_TEXEL);
    _fontTexture.setData(tex_data);

    stbi_image_free(tex_data);
    
    // Create vertex buffer
    ret = _vertexBuffer.create(sizeof(ImDrawVert) * 16384, nullptr);
    if(false == ret)
    {
        // [todo]
    }
    
    // Create vertex stream
    ret = _vertexStream.build(&_vertexBuffer, { { 0, Geometry::ComponentType::FLOAT, 2, sizeof(ImDrawVert),  0, 0 },
                                                { 1, Geometry::ComponentType::FLOAT, 2, sizeof(ImDrawVert),  8, 0 },
                                                { 2, Geometry::ComponentType::BYTE,  4, sizeof(ImDrawVert), 16, 0 } } );
    if(false == ret)
    {
        // [todo]
    }
    
    // Create shaders
    ret = _vertexShader.create(Shader::Type::VERTEX_SHADER, g_vertexShader);
    if(false == ret)
    {
        // [todo]
    }
    ret = _fragmentShader.create(Shader::Type::FRAGMENT_SHADER, g_fragmentShader);
    if(false == ret)
    {
        // [todo]
    }
    // Create program
    ret = _program.create();
    if(false == ret)
    {
        // [todo]
    }
    ret = _program.attach(_vertexShader);
    if(false == ret)
    {
        // [todo]
    }
    ret = _program.attach(_fragmentShader);
    if(false == ret)
    {
        // [todo]
    }
    ret = _program.link();
    if(false == ret)
    {
    }
    
    _projectionMatrixId = _program.getUniformLocation("projection");
}

void ImGuiDelegate::render()
{
}

void ImGuiDelegate::handleKeyAction(int key, int scancode, int action, int mods)
{
    (void)scancode;
    ImGuiIO& io = ImGui::GetIO();
    if(GLFW_PRESS == action)
    {
        io.KeysDown[key] = true;
    }
    if(GLFW_RELEASE == action)
    {
        io.KeysDown[key] = false;
    }
    io.KeyCtrl  = (mods & GLFW_MOD_CONTROL) != 0;
    io.KeyShift = (mods & GLFW_MOD_SHIFT  ) != 0;
}

void ImGuiDelegate::handleMouseButtonAction(int button, int action, int mods)
{
    (void)mods;
    if((GLFW_PRESS == action) && ((button >= 0) && (button < 2)))
    {
        _mousePressed[button] = true;
    }
}

void ImGuiDelegate::handleMousePositionAction(double x, double y)
{
    (void)x;
    (void)y;
    // Nothing atm
}

void ImGuiDelegate::handleMouseWheelAction(double x, double y)
{
    (void)x;
    ImGui::GetIO().MouseWheel = (y != 0.0f) ? ((y > 0.0f) ? 1 : - 1) : 0;
}

void ImGuiDelegate::handleWindowSize(int w, int h)
{
    (void)w;
    (void)h;
    // Nothing as the window is not resizable.
}

void ImGuiDelegate::handleChar(unsigned int unicodeChar)
{
    if((unicodeChar > 0) && (unicodeChar < 0x10000))
    {
        ImGui::GetIO().AddInputCharacter((unsigned short)unicodeChar);
    }
}

void ImGuiDelegate::handleCursorEnter(int flag)
{
    (void)flag;
    // [todo] Nothing atm.
}

void ImGuiDelegate::renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
{
    if (cmd_lists_count == 0)
        return;

    Renderer& renderer = Renderer::instance();

    ImGuiIO& io = ImGui::GetIO();
    ImGuiDelegate *delegate = static_cast<ImGuiDelegate*>(io.UserData);
    if(nullptr == delegate) { return; }
    
    
    renderer.blend(true);
    renderer.blendFunc(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_SRC_ALPHA);
    renderer.culling(false);
    renderer.depthTest(false);
    renderer.scissorTest(true);
    renderer.texture2D(true);
    
    // Setup texture
    delegate->_fontTexture.bind();
    
    // Setup orthographic projection matrix
    const float width  = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;
    glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
    delegate->_program.uniform(delegate->_projectionMatrixId, true, projection);

    // Compute vertex count
    size_t vCount = 0;
    for(int n=0; n<cmd_lists_count; n++)
    {
        vCount += cmd_lists[n]->vtx_buffer.size();
    }

    // Render command lists
    uint8_t *data = (uint8_t*)delegate->_vertexBuffer.map(BufferObject::Access::WRITE_ONLY, 0, cmd_lists_count*sizeof(ImDrawVert));
    for (int n = 0; n < cmd_lists_count; n++)
    {
        const ImDrawList* cmd_list = cmd_lists[n];
        const uint8_t* vtx_buffer = (const uint8_t*)cmd_list->vtx_buffer.begin();
        vCount += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
        memcpy(data, vtx_buffer, vCount);
        data += vCount;
    }
    delegate->_vertexBuffer.unmap();
    
    delegate->_vertexStream.bind();
    for (int n = 0; n < cmd_lists_count; n++)
    {
        int vtx_offset = 0;
        const ImDrawList* cmd_list = cmd_lists[n];
        const ImDrawCmd* pcmd_end = cmd_list->commands.end();
        for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
        {
            renderer.scissor(glm::ivec2(pcmd->clip_rect.x, height - pcmd->clip_rect.w), glm::ivec2(pcmd->clip_rect.z - pcmd->clip_rect.x, pcmd->clip_rect.w - pcmd->clip_rect.y));
            delegate->_vertexStream.draw(Geometry::Primitive::TRIANGLES, vtx_offset, pcmd->vtx_count);
            vtx_offset += pcmd->vtx_count;
        }
    }    
    renderer.scissorTest(false);
    delegate->_vertexStream.unbind();
}

const char* ImGuiDelegate::getClipboardText()
{
    ImGuiDelegate *delegate = static_cast<ImGuiDelegate*>(ImGui::GetIO().UserData);
    if(nullptr == delegate) { return ""; }
    return glfwGetClipboardString(delegate->_window);
}

void ImGuiDelegate::setClipboardText(const char* txt)
{
    ImGuiDelegate *delegate = static_cast<ImGuiDelegate*>(ImGui::GetIO().UserData);
    if(nullptr == delegate) { return; }
    glfwSetClipboardString(delegate->_window, txt);
}

} // Framework
