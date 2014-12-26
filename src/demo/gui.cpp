#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/wrapper.hpp>
#include <DumbFramework/renderer.hpp>
#include <DumbFramework/imguidelegate.hpp>
#include <DumbFramework/camera.hpp>
#include <DumbFramework/texture.hpp>
#include <DumbFramework/textureloader.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Framework;

static const float g_cube[] =
{
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f,-1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 1.0f,
     1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
     1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
     1.0f, 1.0f,-1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f, 1.0f, 0.0f,
     1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
     1.0f,-1.0f, 1.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
     1.0f, 1.0f,-1.0f, 1.0f, 0.0f,
     1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f, 0.0f, 1.0f,
     1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     1.0f,-1.0f, 1.0f, 1.0f, 0.0f
};

static const char* g_vertexShader = R"EOT(
#version 410 core

layout (location=0) in vec3 vs_position;
layout (location=1) in vec2 vs_uv;
layout (location=2) in mat4 vs_modelviewproj;

flat out int instanceID; 
out vec2 texCoord;

void main()
{
    texCoord = vs_uv;
    gl_Position = vs_modelviewproj * vec4(vs_position, 1.0);
    instanceID = gl_InstanceID; 
}
)EOT";

static const char* g_fragmentShader = R"EOT(
#version 410 core

uniform vec3 color;
uniform sampler2DArray texSampler;

flat in int instanceID;
in vec2 texCoord;

out vec4 out_color;

void main()
{
    out_color = vec4(color * (instanceID / 4096.0f), 1.0) * texture2DArray(texSampler, vec3(texCoord, instanceID%6));
}
)EOT";

// Dummy "application" class
class Dummy
{
    public:
        Dummy() 
            : bgcolor(0.8f, 0.6f, 0.6f)
            , alpha(0.75f)
            , pushed(false)
            , open(true)
        {
            points.resize(4);
        }
        
        void init()
        {
            bool ret;
            
            // Create vertex buffer
            ret = vertexBuffer.create(sizeof(g_cube), (void*)g_cube, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
            if(false == ret)
            {
                return;
            }
            
            // Create vertex buffer for mvp
            ret = mvpBuffer.create(16*16*16*sizeof(float[16]), nullptr, BufferObject::Access::Frequency::STREAM, BufferObject::Access::Type::DRAW);
            if(false == ret)
            {
                return;
            }
            
            // Create vertex stream
            vertexStream.create();
            ret = vertexStream.add(&vertexBuffer, 
                                   {
                                        { 0, Geometry::ComponentType::FLOAT, 3, false, 5*sizeof(float), 0,               0 },
                                        { 1, Geometry::ComponentType::FLOAT, 2, false, 5*sizeof(float), 3*sizeof(float), 0 }
                                    });
            if(false == ret)
            {
                return;
            }
            ret = vertexStream.add(&mvpBuffer, 
                                {
                                       { 2, Geometry::ComponentType::FLOAT, 4, false, 16*sizeof(float),  0,               1 },
                                       { 3, Geometry::ComponentType::FLOAT, 4, false, 16*sizeof(float),  4*sizeof(float), 1 },
                                       { 4, Geometry::ComponentType::FLOAT, 4, false, 16*sizeof(float),  8*sizeof(float), 1 },
                                       { 5, Geometry::ComponentType::FLOAT, 4, false, 16*sizeof(float), 12*sizeof(float), 1 }
                                });
            if(false == ret)
            {
                return;
            }
            ret = vertexStream.compile();
            if(false == ret)
            {
                return;
            }
            
            // Create program.
            ret = program.create( {{Shader::Type::VERTEX_SHADER,   g_vertexShader},
                                   {Shader::Type::FRAGMENT_SHADER, g_fragmentShader}} );
            if(false == ret)
            {
                return;
            }
            ret = program.link();
            if(false == ret)
            {
                return;
            }
            program.begin();
                colorId = program.getUniformLocation("color");
                GLint texId = program.getUniformLocation("texSampler");
                program.uniform(texId, 0);
            program.end();
            
            camera[0].lookAt(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f));
            camera[0].perspective(45.0f, 0.1f, 10.0f);
            
            camera[1] = camera[0];
            
            angle[0] = angle[1] = glm::vec3(0.0f);
            
            depth = 0.1f;
            
            color = glm::vec3(1.0f);
            
            memset(msPerFrame, 0, sizeof(msPerFrame));
            currentFrame = 0;
            msPerFrameAccum = 0.0f;

            Framework::Texture::load(texture, {"tex00.png","tex01.png","tex02.png","tex03.png","tex04.png","tex05.png"});
            texture.bind();
                texture.setMagFilter(Framework::Texture::MagFilter::LINEAR_TEXEL);
                texture.setMinFilter(Framework::Texture::MinFilter::LINEAR_TEXEL);
                texture.setWrap(Framework::Texture::Wrap::CLAMP_TO_EDGE, Framework::Texture::Wrap::CLAMP_TO_EDGE);
            texture.unbind();
            
            glfwSwapInterval(1); // vsync
        }
        
        void gui()
        {
            ImGui::Begin("DumbFramework + ImGui Test", &open, ImVec2(640, 360), alpha, 0);
                if(ImGui::Button("Push me!"))
                {
                    pushed = !pushed;
                }
                ImGui::SameLine();
                ImGui::Text(pushed ? "How dare you!" : "Move along!");
                ImGui::SliderFloat("Window alpha", &alpha, 0.0f,1.0f);
                
                back = ImGui::Button("Back");
                ImGui::SameLine();
                forward = ImGui::Button("Forward");
                
                angleUpdate = ImGui::SliderFloat3("Angle", glm::value_ptr(angle[1]), -180.0f, 180.0f);
                ImGui::ColorEdit3("Cube color", glm::value_ptr(color));
                int size = points.size();
                ImGui::InputInt("Point count", &size, 1, 10);
                points.resize(size);
                ImGui::PlotHistogram("Points", &points[0], points.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0,128));
                ImVec2 bmin = ImGui::GetItemBoxMin();
                ImVec2 bmax = ImGui::GetItemBoxMax();
                ImVec2 mpos = ImGui::GetMousePos();
                
                if( ((mpos.x > bmin.x) && (mpos.y > bmin.y)) &&
                    ((mpos.x < bmax.x) && (mpos.y < bmax.y)) )
                {
                    float scaled = (mpos.x - bmin.x) / (float)(bmax.x - bmin.x);
                    int bucket = scaled * points.size();
                    
                    float height = (mpos.y - bmin.y) / (float)(bmax.y - bmin.y);
                    points[bucket] = 1.0f - height;
                }
                
                msPerFrameAccum -= msPerFrame[currentFrame];
                msPerFrame[currentFrame] = ImGui::GetIO().DeltaTime * 1000.0f;
                msPerFrameAccum += msPerFrame[currentFrame];
                currentFrame = (currentFrame + 1) % 120;
                const float msPerFrameAvg = msPerFrameAccum / 120.0f;
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", msPerFrameAvg, 1000.0f / msPerFrameAvg);
            ImGui::End();
        }
        
        void render()
        {
            ImGuiIO& io = ImGui::GetIO();
            Framework::Renderer& renderer = Framework::Renderer::instance();
            
            glViewport(0, 0,(int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(bgcolor.r, bgcolor.g, bgcolor.b, 1.0);
            
            // Draw a cube 
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer.culling(false);
            renderer.depthTest(true);
            
            if(angleUpdate)
            {
                glm::vec3 dA = angle[1] - angle[0];
                glm::fquat rotXYZ = glm::angleAxis(glm::radians(dA.x), glm::vec3(1.0f, 0.0, 0.0f)) *
                                    glm::angleAxis(glm::radians(dA.y), glm::vec3(0.0f, 1.0, 0.0f)) *
                                    glm::angleAxis(glm::radians(dA.z), glm::vec3(0.0f, 0.0, 1.0f));
                
                angle[0] = angle[1];
                camera[1].orientation  *=  rotXYZ;
            }
            if(back)
            {
                camera[1].eye -= depth * camera[1].forward();
                angle[0] = angle[1] = glm::vec3(0.0f);
            }
            if(forward)
            {
                camera[1].eye += depth * camera[1].forward();
                angle[0] = angle[1] = glm::vec3(0.0f);
            }
            
            float *mvp = (float*)mvpBuffer.map(BufferObject::Access::Policy::WRITE_ONLY);
            glm::mat4 projView = camera[1].projectionMatrix(glm::ivec2(io.DisplaySize.x, io.DisplaySize.y)) * camera[1].viewMatrix();
            for(size_t k=0; k<16; k++)
            {
                for(size_t j=0; j<16; j++)
                {
                    for(size_t i=0; i<16; i++, mvp+=16)
                    {
                        glm::mat4 model = glm::scale( glm::translate(glm::mat4(), glm::vec3(k*0.2f, i*0.2f, j*0.2f)), glm::vec3(0.07f));
                        memcpy(mvp, glm::value_ptr(projView * model), 16*sizeof(float));
                    }
                }
            }
            mvpBuffer.unmap();
            
            renderer.setActiveTextureUnit(0);
            texture.bind();
            program.begin();
                vertexStream.bind();
                    program.uniform(colorId, color);
                    //vertexStream.draw(Geometry::Primitive::TRIANGLES, 0, sizeof(g_cube)/sizeof(g_cube[0]));
                    glDrawArraysInstanced(GL_TRIANGLES, 0, 12*3, 16*16*16);
                vertexStream.unbind();
            program.end();
            texture.unbind();
        }
        
        void destroy()
        {
            program.destroy();
            vertexBuffer.destroy();
            mvpBuffer.destroy();
            vertexStream.destroy();
            points.clear();
            colorId = 0;
        }
        
    public:
        glm::vec3 bgcolor;
        glm::vec3 color;
        std::vector<float> points;
        float alpha;
        bool pushed;
        bool open;
        Framework::VertexBuffer vertexBuffer;
        Framework::VertexBuffer mvpBuffer;
        Framework::VertexStream vertexStream;
        Framework::Program program;
        GLint colorId;
        glm::vec3 angle[2];
        float depth;
        Framework::Camera camera[2];
        bool back, forward, angleUpdate;
        
        Framework::Texture2D texture;
        
        float msPerFrame[120];
        int   currentFrame;
        float msPerFrameAccum;
};

int main()
{
    Log::ConsoleOutputPolicy outputPolicy;
    Log::LogBuilder<Log::AllPassFilter, Log::SimpleMessageFormat> msgBuilder;
    Log::LogProcessor& logProcessor = Log::LogProcessor::instance();
    logProcessor.start(&msgBuilder, &outputPolicy);
    
    Dummy foobar;
    foobar.bgcolor.r = 0.5;
    
    ImGuiDelegate<Dummy> delegate(1024, 768, "ImGui test", &foobar);
    Wrapper<ImGuiDelegate<Dummy>> wrapper(&delegate);

    wrapper.mouse.onMouseButton +=
        [](Framework::Input::Mouse::Button button, bool state, int modifier)
        {
            Log_Info(Module::Base, "button %x %s %d", button, state ? "pressed " : "released", modifier);
        };

    wrapper.start();
    
    logProcessor.stop();
    return 0;
}
