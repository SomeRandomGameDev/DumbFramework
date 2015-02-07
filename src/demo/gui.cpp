#include <DumbFramework/config.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <DumbFramework/log.hpp>
#include <DumbFramework/wrapper.hpp>
#include <DumbFramework/imguidelegate.hpp>
#include <DumbFramework/camera.hpp>
#include <DumbFramework/render/renderer.hpp>
#include <DumbFramework/render/texture.hpp>
#include <DumbFramework/render/textureloader.hpp>
#include <DumbFramework/render/mesh.hpp>
#include <DumbFramework/render/rendercontext.hpp>
#include <DumbFramework/render/material.hpp>
#include <DumbFramework/render/mesh.hpp>
#include <DumbFramework/render/dummy.hpp>
#include <DumbFramework/render/geometrypass.hpp>
#include <DumbFramework/render/light/lightpass.hpp>

using namespace Framework;

// Dummy "application" class
class Dummy
{
    public:
        Dummy() 
            : bgcolor(0.5f, 0.1f, 0.1f)
            , alpha(0.24f)
            , open(true)
        {}
        
        void init()
        {
            bool ret;
            
            ret = geometrypass.create(glm::ivec2(1024, 768));
            if(false == ret)
            {
                return;
            }
            
            ret = lightpass.create(geometrypass.output(), geometrypass.depthbuffer());
            
            ret = Render::createCube(mesh);

            ret = material.create("dummy");
            Render::Texture::load(material.diffuseMap,  "cubeTex.png");
            Render::Texture::load(material.specularMap, "cubeTexSpec.png");
            Render::Texture::load(material.normalMap,   "cubeTexNormal.png");
            material.diffuseMap.bind();
                material.diffuseMap.setMagFilter(Render::Texture::MagFilter::LINEAR_TEXEL);
                material.diffuseMap.setMinFilter(Render::Texture::MinFilter::LINEAR_TEXEL);
                material.diffuseMap.setWrap(Render::Texture::Wrap::CLAMP_TO_EDGE, Render::Texture::Wrap::CLAMP_TO_EDGE);
            material.diffuseMap.unbind();
            material.specularMap.bind();
                material.specularMap.setMagFilter(Render::Texture::MagFilter::LINEAR_TEXEL);
                material.specularMap.setMinFilter(Render::Texture::MinFilter::LINEAR_TEXEL);
                material.specularMap.setWrap(Render::Texture::Wrap::CLAMP_TO_EDGE, Render::Texture::Wrap::CLAMP_TO_EDGE);
            material.specularMap.unbind();
            material.normalMap.bind();
                material.normalMap.setMagFilter(Render::Texture::MagFilter::LINEAR_TEXEL);
                material.normalMap.setMinFilter(Render::Texture::MinFilter::LINEAR_TEXEL);
                material.normalMap.setWrap(Render::Texture::Wrap::CLAMP_TO_EDGE, Render::Texture::Wrap::CLAMP_TO_EDGE);
            material.normalMap.unbind();
            
            material.culling = false;
            
            camera[0].lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f));
            camera[0].perspective(45.0f, 0.1f, 10.0f);
            
            camera[1] = camera[0];
            
            angle[0] = angle[1] = glm::vec3(0.0f);
            
            depth = 0.1f;
            
            memset(msPerFrame, 0, sizeof(msPerFrame));
            currentFrame = 0;
            msPerFrameAccum = 0.0f;

            rot = 0.0f;
            
            //glfwSwapInterval(1); // vsync
        }
        
        void gui()
        {
            ImGui::Begin("DumbFramework + ImGui Test", &open, ImVec2(640, 360), alpha, 0);
                back = ImGui::Button("Back");
                ImGui::SameLine();
                forward = ImGui::Button("Forward");
                
                angleUpdate = ImGui::SliderFloat3("Angle", glm::value_ptr(angle[1]), -180.0f, 180.0f);

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
            
            rot += ImGui::GetIO().DeltaTime * 1.2f;
            glm::mat4 model  = glm::rotate(glm::mat4(), rot, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));
            
            geometrypass.begin(camera[1]);
                geometrypass.render(material, model, normal, mesh);
            geometrypass.end();

            Render::PointLight light;
            light.radius    = 0.825f;
            light.intensity = 1.0f;

            lightpass.clear();
 
            light.position  = glm::vec3(-1.2f, 1.2f, 1.2f);
            light.color = glm::vec3(1.0f, 0.0f, 0.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 0.0f, 1.2f, 1.2f);
            light.color = glm::vec3(0.0f, 1.0f, 0.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 1.2f, 1.2f, 1.2f);
            light.color = glm::vec3(0.0f, 0.0f, 1.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 1.2f, 1.2f, 0.0f);
            light.color = glm::vec3(1.0f, 1.0f, 0.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 0.2f, 1.2f, 0.0f);
            light.color = glm::vec3(1.0f, 0.0f, 1.0f);
            lightpass.add(light);
            light.position  = glm::vec3(-1.2f, 1.2f, 0.0f);
            light.color = glm::vec3(1.0f, 1.0f, 1.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 1.2f, 1.2f,-1.2f);
            light.color = glm::vec3(0.0f, 1.0f, 1.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 0.2f, 1.2f,-1.2f);
            light.color = glm::vec3(1.0f, 0.5f, 0.5f);
            lightpass.add(light);
            light.position  = glm::vec3(-1.2f, 1.2f,-1.2f);
            light.color = glm::vec3(1.0f, 0.5f, 1.0f);
            lightpass.add(light);

            light.position  = glm::vec3(-1.2f,-1.2f, 1.2f);
            light.color = glm::vec3(0.5f, 0.5f, 1.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 0.0f,-1.2f, 1.2f);
            light.color = glm::vec3(0.5f, 1.0f, 1.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 1.2f,-1.2f, 1.2f);
            light.color = glm::vec3(0.5f, 1.0f, 0.5f);
            lightpass.add(light);
            light.position  = glm::vec3( 1.2f,-1.2f, 0.0f);
            light.color = glm::vec3(1.0f, 0.25f, 0.5f);
            lightpass.add(light);
            light.position  = glm::vec3( 0.2f,-1.2f, 0.0f);
            light.color = glm::vec3(1.0f, 0.5f, 0.25f);
            lightpass.add(light);
            light.position  = glm::vec3(-1.2f,-1.2f, 0.0f);
            light.color = glm::vec3(0.0f, 1.0f, 0.5f);
            lightpass.add(light);
            light.position  = glm::vec3( 1.2f,-1.2f,-1.2f);
            light.color = glm::vec3(0.5f, 0.25f, 1.0f);
            lightpass.add(light);
            light.position  = glm::vec3( 0.2f,-1.2f,-1.2f);
            light.color = glm::vec3(1.0f, 0.5f, 0.25f);
            lightpass.add(light);
            light.position  = glm::vec3(-1.2f,-1.2f,-1.2f);
            light.color = glm::vec3(0.5f, 1.0f, 0.25f);
            lightpass.add(light);


            lightpass.draw(camera[1]);

            glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::ivec2 halfSize(io.DisplaySize.x/2, io.DisplaySize.y/2);
            geometrypass.debug(Framework::Render::GeometryPass::ALBEDO,   glm::ivec2(0),             halfSize);
            geometrypass.debug(Framework::Render::GeometryPass::SPECULAR, glm::ivec2(halfSize.x, 0), glm::ivec2(halfSize.x, 0)+halfSize);
            geometrypass.debug(Framework::Render::GeometryPass::NORMAL,   glm::ivec2(0, halfSize.y), glm::ivec2(0, halfSize.y)+halfSize);
            lightpass.debug   (glm::ivec2(halfSize.x, halfSize.y), glm::ivec2(halfSize.x, halfSize.y)+halfSize);
        }
        
        void destroy()
        {
            mesh.destroy();
            geometrypass.destroy();
            lightpass.destroy();
            material.destroy();
            colorId = 0;
        }
        
    public:
        glm::vec3 bgcolor;
        float alpha;
        bool open;

        Render::Mesh mesh;
        Render::Material material;
        Render::GeometryPass geometrypass;
        Render::LightPass    lightpass;
        
        GLint colorId;
        glm::vec3 angle[2];
        float rot;
        float depth;
        Camera camera[2];
        bool back, forward, angleUpdate;
        
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
