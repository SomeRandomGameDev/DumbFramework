#include <unittest++/UnitTest++.h>
#include <iostream>
#include <vector>
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
    
    struct StringListOutputPolicy : public Framework::Log::OutputPolicyBase
    {
        std::vector<std::string> msgList;

        bool write(std::string & msg)
        {
            msgList.push_back(msg);
            return true;
        }
    };
    
    TEST(Flush)
    {
        Log::LogBuilder<Log::AllPassFilter, DummyMessageFormat> msgBuilder;
        StringListOutputPolicy output;
        std::string expected[3];
        
        expected[0] = "[Trace][Base] RunImpl trace   0!";
        expected[1] = "[Warning][Base] RunImpl warn    1!";
        expected[2] = "[Error][Base] RunImpl error   2!";

        Log::LogProcessor& processor = Log::LogProcessor::instance();
        processor.start(&msgBuilder, &output);
        
        int i = 0;
                
        Log_Trace(Framework::Module::Base,   "trace   %d!", i++);
        Log_Warning(Framework::Module::Base, "warn    %d!", i++);
        Log_Error(Framework::Module::Base,   "error   %d!", i++);

        processor.flush();

        CHECK_EQUAL(expected[2], output.msgList[2]);
        CHECK_EQUAL(expected[1], output.msgList[1]);
        CHECK_EQUAL(expected[0], output.msgList[0]);

        processor.stop();
    }
}
