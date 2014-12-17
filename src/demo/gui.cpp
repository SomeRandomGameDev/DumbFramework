#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/wrapper.hpp>
#include <DumbFramework/renderer.hpp>
#include <DumbFramework/imguidelegate.hpp>
#include <DumbFramework/camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Framework;

static const float g_cube[] =
{
    -1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f
};

static const char* g_vertexShader = R"EOT(
#version 410 core
uniform mat4 modelviewproj;

layout (location=0) in vec3 vs_position;

void main()
{
    gl_Position = modelviewproj * vec4(vs_position, 1.0);
}
)EOT";

static const char* g_fragmentShader = R"EOT(
#version 410 core

uniform vec3 color;

out vec4 out_color;

void main()
{
    out_color = vec4(color, 1.0);
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
            
            // Create vertex stream
            ret = vertexStream.build(&vertexBuffer, { { 0, Geometry::ComponentType::FLOAT, 3, false, 3*sizeof(float), 0, 0 } } );
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
                mvpId = program.getUniformLocation("modelviewproj");
                colorId = program.getUniformLocation("color");
            program.end();
            
            camera[0].lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f));
            camera[0].perspective(45.0f, 0.1f, 10.0f);
            
            camera[1] = camera[0];
            
            angle[0] = angle[1] = glm::vec3(0.0f);
            
            depth = 0.1f;
        }
        
        void render()
        {
            Framework::Renderer& renderer = Framework::Renderer::instance();

            // First build the interface
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Begin("DumbFramework + ImGui Test", &open, ImVec2(640, 360), alpha, 0);
                if(ImGui::Button("Push me!"))
                {
                    pushed = !pushed;
                }
                ImGui::SameLine();
                ImGui::Text(pushed ? "How dare you!" : "Move along!");
                ImGui::SliderFloat("Window alpha", &alpha, 0.0f,1.0f);
                
                bool back = ImGui::Button("Back");
                ImGui::SameLine();
                bool forward = ImGui::Button("Forward");
                
                bool angleUpdate = ImGui::SliderFloat3("Angle", glm::value_ptr(angle[1]), -180.0f, 180.0f);
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
            
            ImGui::End();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
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
            glm::mat4 mvp = camera[1].projectionMatrix(glm::ivec2(io.DisplaySize.x, io.DisplaySize.y)) * camera[1].viewMatrix();

            program.begin();
                vertexStream.bind();
                    program.uniform(mvpId, false, mvp);
                    program.uniform(colorId, color);
                    
                    vertexStream.draw(Geometry::Primitive::TRIANGLES, 0, sizeof(g_cube)/sizeof(g_cube[0]));
                    
                    program.uniform(mvpId, false, glm::translate(mvp, glm::vec3(2.25f, 0.0f, 0.0f)));
                    program.uniform(colorId, glm::vec3(1.0f)-color);
                    
                    vertexStream.draw(Geometry::Primitive::TRIANGLES, 0, sizeof(g_cube)/sizeof(g_cube[0]));
                vertexStream.unbind();
            program.end();
            
            // Render interface.
            ImGui::Render();
        }
        
        void destroy()
        {
            program.destroy();
            vertexBuffer.destroy();
            vertexStream.destroy();
            points.clear();
            mvpId = colorId = 0;
        }
        
    public:
        glm::vec3 bgcolor;
        glm::vec3 color;
        std::vector<float> points;
        float alpha;
        bool pushed;
        bool open;
        Framework::VertexBuffer vertexBuffer;
        Framework::VertexStream vertexStream;
        Framework::Program program;
        GLint mvpId, colorId;
        glm::vec3 angle[2];
        float depth;
        Framework::Camera camera[2];
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
