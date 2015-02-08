/** 
 * Constructor.
 * @param [in] width    Window witdh
 * @param [in] height   Window height
 * @param [in] title    Window title.
 * @param [in] delegate Delegate.
 */
template<class T>
ImGuiDelegate<T>::ImGuiDelegate(int width, int height, char const* title,
                                T *delegate)
    : _width(width)
    , _height(height)
    , _title(title)
    , _window(nullptr)
    , _program()
    , _fontTexture()
    , _vertexBuffer()
    , _vertexStream()
    , _delegate(delegate)
{}

/**
 * Destructor.
 */
template<class T>
ImGuiDelegate<T>::~ImGuiDelegate()
{
    _program.destroyShaders();
    _program.destroy();
    _fontTexture.destroy();
    _vertexBuffer.destroy();
    _vertexStream.destroy();
}
/**
 * Create window.
 */
template<class T>
GLFWwindow* ImGuiDelegate<T>::createWindow()
{
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    _window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
    return _window;
}
/**
 * Destroy window.
 * @param [in] window Window pointer.
 */
template<class T>
void ImGuiDelegate<T>::destroyWindow(GLFWwindow *window)
{
    _delegate->destroy();
    if(window != _window)
    {
        return;
    }
    glfwDestroyWindow(_window);
    _window = nullptr;
    
    _program.destroy();
    _fontTexture.destroy();
    _vertexBuffer.destroy();
    _vertexStream.destroy();
    ImGui::Shutdown();
}
/**
 * Initialize delegate.
 */
template<class T>
void ImGuiDelegate<T>::init()
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
    io.RenderDrawListsFn  = ImGuiDelegate<T>::renderDrawLists;
    io.SetClipboardTextFn = ImGuiDelegate<T>::setClipboardText;
    io.GetClipboardTextFn = ImGuiDelegate<T>::getClipboardText;

    // Default font (embedded in code)
    int width, height;
    unsigned char* pixels;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    bool ret = _fontTexture.create(glm::ivec2(width, height), Render::Texture::PixelFormat::RGBA_8);
    if(false == ret)
    {
        return;
    }
    
    _fontTexture.setData(pixels);
    _fontTexture.bind();
        _fontTexture.setMagFilter(Render::Texture::MagFilter::NEAREST_TEXEL);
        _fontTexture.setMinFilter(Render::Texture::MinFilter::NEAREST_TEXEL);
    _fontTexture.unbind();
    io.Fonts->TexID = (void *)(intptr_t)_fontTexture.id();
    
    // Create vertex buffer
    ret = _vertexBuffer.create(sizeof(ImDrawVert) * 10000, nullptr, Render::BufferObject::Access::Frequency::STREAM, Render::BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        return;
    }
    
    // Create vertex stream
    ret = _vertexStream.create();
    if(false == ret)
    {
        return;
    }
    ret = _vertexStream.add(&_vertexBuffer, { { 0, { Render::Geometry::ComponentType::FLOAT,          2, false, sizeof(ImDrawVert),  0, 0 }},
                                              { 1, { Render::Geometry::ComponentType::FLOAT,          2, false, sizeof(ImDrawVert),  8, 0 }},
                                              { 2, { Render::Geometry::ComponentType::UNSIGNED_BYTE,  4, true,  sizeof(ImDrawVert), 16, 0 }} } );
    if(false == ret)
    {
        return;
    }
    
    ret = _vertexStream.compile();
    if(false == ret)
    {
        return;
    }
    
    // Create program.
    ret = _program.create( {{Render::Shader::Type::VERTEX_SHADER,   ImGuiDelegateVertexShader()},
                            {Render::Shader::Type::FRAGMENT_SHADER, ImGuiDelegateFragmentShader()}} );
    if(false == ret)
    {
        return;
    }
    
    ret = _program.link();
    if(false == ret)
    {
        return;
    }
    
    _program.begin();
        GLint fontTexId = _program.getUniformLocation("un_font");
        _projectionMatrixId = _program.getUniformLocation("projection");

        _program.uniform(fontTexId, 0);
    _program.end();
    
    // Reset mouse
    _mousePressed[0] = _mousePressed[1] = false;
    io.MouseWheel = 0;
    
    _delegate->init();
}
/**
 * Render everything.
 */
template<class T>
void ImGuiDelegate<T>::render()
{
    ImGuiIO& io = ImGui::GetIO();

    // Setup timestep
    static double time = 0.0f;
    const double current_time =  glfwGetTime();
    io.DeltaTime = (float)(current_time - time);
    time = current_time;

    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    double mouse_x, mouse_y;
    glfwGetCursorPos(_window, &mouse_x, &mouse_y);
    io.MousePos = ImVec2((float)mouse_x * _mousePosScale.x, (float)mouse_y * _mousePosScale.y);      // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
    io.MouseDown[0] = _mousePressed[0] || glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    io.MouseDown[1] = _mousePressed[1] || glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) != 0;

    // Start the frame
    ImGui::NewFrame();
    _delegate->gui();

    _delegate->render();

    // Render interface.
    ImGui::Render();

    _mousePressed[0] = _mousePressed[1] = false;
    io.MouseWheel = 0;
}

template<class T>
void ImGuiDelegate<T>::handleKeyAction(int key, int scancode, int action, int mods)
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

template<class T>
void ImGuiDelegate<T>::handleMouseButtonAction(int button, int action, int mods)
{
    (void)mods;
    if((GLFW_PRESS == action) && ((button >= 0) && (button < 2)))
    {
        _mousePressed[button] = true;
    }
}

template<class T>
void ImGuiDelegate<T>::handleMousePositionAction(double x, double y)
{
    (void)x;
    (void)y;
    // Nothing atm
}

template<class T>
void ImGuiDelegate<T>::handleMouseWheelAction(double x, double y)
{
    (void)x;
    ImGui::GetIO().MouseWheel = (y != 0.0f) ? ((y > 0.0f) ? 1 : - 1) : 0;
}

template<class T>
void ImGuiDelegate<T>::handleWindowSize(int w, int h)
{
    (void)w;
    (void)h;
    // Nothing as the window is not resizable.
}

template<class T>
void ImGuiDelegate<T>::handleChar(unsigned int unicodeChar)
{
    if((unicodeChar > 0) && (unicodeChar < 0x10000))
    {
        ImGui::GetIO().AddInputCharacter((unsigned short)unicodeChar);
    }
}

template<class T>
void ImGuiDelegate<T>::handleCursorEnter(int flag)
{
    (void)flag;
    // [todo] Nothing atm.
}

template<class T>
void ImGuiDelegate<T>::renderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
{
    if (cmd_lists_count == 0)
        return;

    Render::Renderer& renderer = Render::Renderer::instance();

    ImGuiIO& io = ImGui::GetIO();
    ImGuiDelegate<T> *delegate = static_cast<ImGuiDelegate<T>*>(io.UserData);
    if(nullptr == delegate) { return; }
    
    renderer.blend(true);
    renderer.blendFunc(Render::BlendFunc::SRC_ALPHA, Render::BlendFunc::ONE_MINUS_SRC_ALPHA);
    renderer.culling(false);
    renderer.depthTest(false);
    renderer.depthBufferWrite(false);
    renderer.scissorTest(true);

    // Upload vertex data
    size_t totalVertexCount = 0;
    for(int n=0; n<cmd_lists_count; n++)
    {
        totalVertexCount += cmd_lists[n]->vtx_buffer.size();
    }

    size_t neededBufferSize = totalVertexCount * sizeof(ImDrawVert);
    if(neededBufferSize > delegate->_vertexBuffer.size())
    {
        delegate->_vertexBuffer.bind();
        delegate->_vertexBuffer.resize(neededBufferSize + 5000);
    }

    delegate->_vertexBuffer.bind();
    uint8_t *data = (uint8_t*)delegate->_vertexBuffer.map(Render::BufferObject::Access::Policy::WRITE_ONLY, 0, totalVertexCount*sizeof(ImDrawVert));
    for (int n = 0; n < cmd_lists_count; n++)
    {
        const ImDrawList* cmd_list = cmd_lists[n];
        const uint8_t* vtx_buffer = (const uint8_t*)cmd_list->vtx_buffer.begin();
        size_t vertexCount = cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
        memcpy(data, vtx_buffer, vertexCount);
        data += vertexCount;
    }
    delegate->_vertexBuffer.unmap();
    delegate->_vertexBuffer.bind();

    // Setup texture
    renderer.setActiveTextureUnit(0);
    delegate->_fontTexture.bind();
    renderer.texture2D(true);
    
    // Setup orthographic projection matrix
    const float width  = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;
    glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
    delegate->_program.begin();
    delegate->_program.uniform(delegate->_projectionMatrixId, false, projection);

    // Render command lists
    delegate->_vertexStream.bind();
    int vtx_offset = 0;
    for (int n = 0; n < cmd_lists_count; n++)
    {
        const ImDrawList* cmd_list = cmd_lists[n];
        const ImDrawCmd* pcmd_end = cmd_list->commands.end();
        for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
        {
            renderer.scissor(glm::ivec2(pcmd->clip_rect.x, height - pcmd->clip_rect.w), glm::ivec2(pcmd->clip_rect.z - pcmd->clip_rect.x, pcmd->clip_rect.w - pcmd->clip_rect.y));
            delegate->_vertexStream.draw(Render::Geometry::Primitive::TRIANGLES, vtx_offset, pcmd->vtx_count);
            vtx_offset += pcmd->vtx_count;
        }
    }
    renderer.scissorTest(false);
    delegate->_vertexStream.unbind();
    
    delegate->_program.end();
    renderer.depthBufferWrite(true);
}

template<class T>
const char* ImGuiDelegate<T>::getClipboardText()
{
    ImGuiDelegate<T> *delegate = static_cast<ImGuiDelegate<T>*>(ImGui::GetIO().UserData);
    if(nullptr == delegate) { return ""; }
    return glfwGetClipboardString(delegate->_window);
}

template<class T>
void ImGuiDelegate<T>::setClipboardText(const char* txt)
{
    ImGuiDelegate<T> *delegate = static_cast<ImGuiDelegate<T>*>(ImGui::GetIO().UserData);
    if(nullptr == delegate) { return; }
    glfwSetClipboardString(delegate->_window, txt);
}
