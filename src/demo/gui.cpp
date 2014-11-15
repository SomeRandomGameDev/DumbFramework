#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/wrapper.hpp>
#include <DumbFramework/imguidelegate.hpp>

using namespace Framework;

int main()
{
    Log::ConsoleOutputPolicy outputPolicy;
    Log::LogBuilder<Log::AllPassFilter, Log::SimpleMessageFormat> msgBuilder;
    Log::LogProcessor& logProcessor = Log::LogProcessor::instance();
    logProcessor.start(&msgBuilder, &outputPolicy);
    
    ImGuiDelegate delegate;
    Wrapper<ImGuiDelegate> wrapper(&delegate);
    wrapper.start();
    
    logProcessor.stop();
    return 0;
}
