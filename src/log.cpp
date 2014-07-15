#include <errno.h>
#include <string>
#include <iostream>
#include <sstream>
#include <log.hpp>

namespace Framework {
namespace Log {
    
    /** Constructor. */
    BaseLogBuilder::BaseLogBuilder()
    {}
    /** Destructor. */
    BaseLogBuilder::~BaseLogBuilder()
    {}
    /** Constructor. */
    LogProcessor::LogProcessor()
        : _lock(PTHREAD_MUTEX_INITIALIZER)
        , _alive(PTHREAD_MUTEX_INITIALIZER)
        , _empty(PTHREAD_COND_INITIALIZER)
        , _task()
        , _msgQueue()
        , _builder(NULL)
        , _output(NULL)
    {}
    /** Constructor. */
    LogProcessor::LogProcessor(LogProcessor const &)
        : _lock(PTHREAD_MUTEX_INITIALIZER)
        , _alive(PTHREAD_MUTEX_INITIALIZER)
        , _empty(PTHREAD_COND_INITIALIZER)
        , _task()
        , _msgQueue()
        , _builder(NULL)
        , _output(NULL)
    {}
    /** Destructor. */
    LogProcessor::~LogProcessor()
    {}
    /** Copy operator. */
    LogProcessor& LogProcessor::operator= (LogProcessor const &)
    { return *this; }
    /** Get log processor instance. */
    LogProcessor& LogProcessor::instance()
    {
        static LogProcessor processor;
        return processor;
    }
    /** Logger thread callback */
    void* LogProcessor::taskRoutine(void *param)
    {
        bool ret = true;
        LogProcessor* processor = reinterpret_cast<LogProcessor*>(param);
        std::string msg;

        if((NULL == param) || (NULL == processor))
        {
            return NULL;
        }
        if(NULL == processor->_output)
        {
            return NULL;
        }
        while(!processor->mustStop() || ret)
        {
            ret = processor->dequeueMessage(msg);
            if(ret)
            {
                processor->_output->write(msg);
            }
        }
        return NULL;
    }
    /**
     * Start logging task.
     */
    bool LogProcessor::start(BaseLogBuilder* builder, OutputPolicyBase* outputPolicy)
    {
        _builder = builder;
        _output  = outputPolicy;
        pthread_mutex_init(&_lock, NULL);
        pthread_mutex_init(&_alive, NULL);
        pthread_mutex_lock(&_alive);
        int ret = pthread_create(&_task, NULL, taskRoutine, this);
        return (ret == 0);
    }
    /**
     * Stop logging task.
     */
    bool LogProcessor::stop()
    {
        int   ret;
        void *data;
        pthread_mutex_unlock(&_alive);
        ret = pthread_join(_task, &data);
        return (ret == 0);
    }
    /** Flush message queue. **/
    void LogProcessor::flush()
    {
        pthread_mutex_lock(&_lock);
        pthread_cond_wait(&_empty, &_lock);
        pthread_mutex_unlock(&_lock);
    }
    bool LogProcessor::mustStop()
    {
        int ret = pthread_mutex_trylock(&_alive);
        if(ret == EBUSY)
        {
            return false;
        }
        else if(ret == 0)
        {
            pthread_mutex_unlock(&_alive);
        }
        return true;
    }

    void LogProcessor::queueMessage(std::string const & msg)
    {
        pthread_mutex_lock(&_lock);
        _msgQueue.push_back(msg);
        pthread_mutex_unlock(&_lock);
    }

    bool LogProcessor::dequeueMessage(std::string & msg)
    {
        bool ret;
        pthread_mutex_lock(&_lock);
        ret = !_msgQueue.empty();
        if(ret)
        {
            msg = _msgQueue.front();
            _msgQueue.pop_front();
        }
        else
        {
            pthread_cond_signal(&_empty);
        }
        pthread_mutex_unlock(&_lock);
        return ret;
    }

    void LogProcessor::write(Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, ...)
    {
        if(NULL == _builder)
        {
            return;
        }

        std::string buffer;
        va_list args;
        va_start(args, format);
        if(_builder->build(buffer, module, severity, infos, format, args))
        {
            queueMessage(buffer);
        }
        va_end(args);
    }

    SourceInfos::SourceInfos()
        : filename(__FILE__)
        , line(__LINE__)
        , function(__FUNCTION__)
    {}

    SourceInfos::SourceInfos(char const * name, size_t num, char const * fnctl)
        : filename(name)
        , line(num)
        , function(fnctl)
    {}

    SourceInfos::SourceInfos(SourceInfos const & infos)
        : filename(infos.filename)
        , line(infos.line)
        , function(infos.function)
    {}

    SourceInfos& SourceInfos::operator= (SourceInfos const & infos)
    {
        filename = infos.filename;
        line     = infos.line;
        function = infos.function;
        return *this;
    }

    /**
     * Evaluate filter.
     * @param [in] module Module id.
     * @param [in] severity Severity.
     */
    bool AllPassFilter::eval(Framework::Module const & /*module*/, Framework::Severity const & /*severity*/)
    {
        return true;
    }

    /** Maximum buffer length. **/
    const size_t SimpleMessageFormat::MaxBufferLen = 1024;

    /**
     * Build log string.
     * @param [out] buffer String buffer.
     * @param [in]  module Module id.
     * @param [in]  severity Severity.
     * @param [in]  infos Log information.
     * @param [in]  format String format.
     * @param [in]  args Format argument list.
     */
    void SimpleMessageFormat::build(std::string & buffer, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args)
    {
        char data[SimpleMessageFormat::MaxBufferLen];
        vsnprintf(data, SimpleMessageFormat::MaxBufferLen, format, args);
        
        std::ostringstream oss;
        oss << '[' << severity.toString() << "][" << module.toString() << ']' << ' ' << infos.function << ' ';
        oss << data;
        buffer = oss.str();
    }

    /**
     * Write to standard output.
     * @param [in] msg Log message.
     */
    bool ConsoleOutputPolicy::write(std::string & msg)
    {
        std::cout << msg << std::endl;
        return true;
    }

    /**
     * Write to log file.
     * @param [in] msg Log message.
     */
    bool FileOutputPolicy::write(std::string & msg)
    {
        static File::OpenMode mode = File::WRITE_ONLY;
        File out;

        bool ret = out.open("log.txt", mode);
        if(true != ret) return ret;
        mode = File::APPEND;

        size_t wlen = msg.size();
        size_t rlen = out.write((void*)msg.c_str(), wlen);
        if(rlen != wlen) return false;

        out.close();
        return true;
    }

} // Log
} // Framework
