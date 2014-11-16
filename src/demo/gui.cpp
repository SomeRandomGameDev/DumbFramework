#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/wrapper.hpp>
#include <DumbFramework/imguidelegate.hpp>

using namespace Framework;

void dummyRenderGUI()
{
    bool dummy = true;
    ImGui::ShowTestWindow(&dummy);
}

class Dummy
{
    public:
        Dummy() 
            : bgcolor(0.8f, 0.6f, 0.6f, 1.0f)
        {}
        void render()
        {
            ImGuiIO& io = ImGui::GetIO();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        
    public:
        glm::vec4 bgcolor;
};

int main()
{
    Log::ConsoleOutputPolicy outputPolicy;
    Log::LogBuilder<Log::AllPassFilter, Log::SimpleMessageFormat> msgBuilder;
    Log::LogProcessor& logProcessor = Log::LogProcessor::instance();
    logProcessor.start(&msgBuilder, &outputPolicy);
    
    Dummy foobar;
    foobar.bgcolor.r = 0.5;
    
    ImGuiDelegate delegate(1024, 768, "ImGui test", std::bind(std::mem_fn(&Dummy::render), foobar), dummyRenderGUI);
    Wrapper<ImGuiDelegate> wrapper(&delegate);
    wrapper.start();
    
    logProcessor.stop();
    return 0;
}
