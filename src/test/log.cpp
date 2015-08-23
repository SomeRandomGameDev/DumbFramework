/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <unittest++/UnitTest++.h>
#include <iostream>
#include <vector>
#include <DumbFramework/log.hpp>

using namespace Dumb;

SUITE(Log)
{
    struct DummyMessageFormat
    {
        void build(std::string & buffer, Module::Identifier const & module, Severity const & severity, Log::SourceInfos const & infos, 
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
    
    struct BufferOutputPolicy : public Log::OutputPolicyBase
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
        output.expected[0] = "[info][Base] RunImpl info    0!";
        output.expected[1] = "[warning][Base] RunImpl warn    1!";
        output.expected[2] = "[error][Base] RunImpl error   2!";

        Log_Info(Module::Base,    "info    %d!", i++);
        Log_Warning(Module::Base, "warn    %d!", i++);
        Log_Error(Module::Base,   "error   %d!", i++);
        
        processor.stop();
    }
    
    struct StringListOutputPolicy : public Log::OutputPolicyBase
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
        
        expected[0] = "[info][Base] RunImpl info    0!";
        expected[1] = "[warning][Base] RunImpl warn    1!";
        expected[2] = "[error][Base] RunImpl error   2!";

        Log::LogProcessor& processor = Log::LogProcessor::instance();
        processor.start(&msgBuilder, &output);
        
        int i = 0;
                
        Log_Info(Module::Base,    "info    %d!", i++);
        Log_Warning(Module::Base, "warn    %d!", i++);
        Log_Error(Module::Base,   "error   %d!", i++);

        processor.flush();

        CHECK_EQUAL(expected[2], output.msgList[2]);
        CHECK_EQUAL(expected[1], output.msgList[1]);
        CHECK_EQUAL(expected[0], output.msgList[0]);

        processor.stop();
    }
}
