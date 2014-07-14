#include <unittest++/UnitTest++.h>
#include <iostream>
#include "log.hpp"

using namespace Framework;

SUITE(Log)
{
    struct DummyMessageFormat
    {
        void build(std::string & buffer, Framework::Module const & module, Framework::Severity const & severity, Framework::Log::SourceInfos const & infos, 
                   char const * format, va_list args)
        {
            char data[1024];
            vsnprintf(data, 1024, format, args);
            
            std::ostringstream oss;
            oss << '[' << severity.toString() << "][" << module.toString() << ']' << ' ' << infos.function << ' ';
            oss << data;
            buffer = oss.str();
        }
    };
    
    struct BufferOutputPolicy : public Framework::Log::OutputPolicyBase
    {
        std::string expected[3];
        size_t index;
        
        bool write(std::string & msg)
        {
            CHECK_EQUAL(expected[index], msg);
            index++;
            return true;
        }
    };
    
    TEST(BufferOutput)
    {
        Log::LogBuilder<Log::AllPassFilter, DummyMessageFormat> msgBuilder;
        BufferOutputPolicy output;

        Log::LogProcessor& processor = Log::LogProcessor::instance();
        processor.start(&msgBuilder, &output);
        
        int i = 0;
        output.index = 0;
        output.expected[0] = "[Trace][Base] RunImpl trace   0!";
        output.expected[1] = "[Warning][Base] RunImpl warn    1!";
        output.expected[2] = "[Error][Base] RunImpl error   2!";

        Log_Trace(Framework::Module::Base,   "trace   %d!", i++);
        Log_Warning(Framework::Module::Base, "warn    %d!", i++);
        Log_Error(Framework::Module::Base,   "error   %d!", i++);
        
        processor.stop();
    }
}
