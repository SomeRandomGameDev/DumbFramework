#include <unittest++/UnitTest++.h>
#include "log.hpp"

using namespace Framework;

SUITE(Log)
{
    TEST(Classify)
    {
        Log::LogBuilder<Log::AllPassFilter, Log::SimpleMessageFormat> msgBuilder;
        Log::ConsoleOutputPolicy consoleOutput;

        Log::LogProcessor& processor = Log::LogProcessor::instance();
        processor.start(&msgBuilder, &consoleOutput);
        
        int i = 0;
        
        Log_Trace  (Framework::Module::Base, "trace   %d!", i++);
        Log_Warning(Framework::Module::Base, "warning %d!", i++);
        Log_Error  (Framework::Module::Base, "error   %d!", i++);
        
        processor.stop();
    }
}
