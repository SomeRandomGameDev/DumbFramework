#include <errno.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <DumbFramework/log.hpp>

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
    /**
     * Log processor thread routine.
     * @param [in] param  Opaque pointer to log processor.
     */
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
     * @param [in] builder       Log message builder.
     * @param [in] outputPolicy  Filter and log output.
     */
    bool LogProcessor::start(BaseLogBuilder* builder, OutputPolicyBase* outputPolicy)
    {
        if((NULL == builder) || (NULL == outputPolicy))
        {
            return false;
        }
        _builder = builder;
        _output  = outputPolicy;
        if(!_output->setup())
        {
            return false;
        }
        pthread_mutex_init(&_lock, NULL);
        pthread_mutex_init(&_alive, NULL);
        pthread_mutex_lock(&_alive);
        int ret = pthread_create(&_task, NULL, taskRoutine, this);
        return (ret == 0);
    }
    /** Stop logging task. **/
    bool LogProcessor::stop()
    {
        int   ret;
        void *data;
        pthread_mutex_unlock(&_alive);
        ret = pthread_join(_task, &data);
        if(_output) { _output->teardown(); }
        return (ret == 0);
    }
    /** 
     * @brief Flush log output.
     *
     * Wait until all emitted log messages have been processed.
     */
    void LogProcessor::flush()
    {
        pthread_mutex_lock(&_lock);
        pthread_cond_wait(&_empty, &_lock);
        pthread_mutex_unlock(&_lock);
    }
    /** Return true if the thread must stop. **/
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
    /**
     * Add string to message queue.
     * @param [in] msg  Log message.
     */
    void LogProcessor::queueMessage(std::string const & msg)
    {
        pthread_mutex_lock(&_lock);
        _msgQueue.push_back(msg);
        pthread_mutex_unlock(&_lock);
    }
    /**
     * Retrieve log message from queue.
     * @param [out] msg  Log message.
     * @return false if the queue is empty.
     */
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

    /**
     * Emit log message.
     * @param [in]  module   Module ID. 
     * @param [in]  severity Log severity (warning, info, error, ...).
     * @param [in]  infos    Source information (file, line number, ...).
     * @param [in]  format   Format string.
     * @param [in]  ...      Format string arguments.
     */
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
        : filename("")
        , line(0)
        , function("")
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
        oss << '[' << severity.toString() << "][" << module.toString() << "][" << infos.function << "]";
        oss << data;
        buffer = oss.str();
    }

    /**
     * Write to standard output.
     * @param [in] msg Log message.
     */
    bool ConsoleOutputPolicy::write(std::string & msg)
    {
        std::cout << msg;
        if(msg[msg.size()-1] != '\n')
        {
            std::cout << std::endl;
        }
        return true;
    }

    /** Contructor. **/
    FileOutputPolicy::FileOutputPolicy()
        : _filename("")
    {}
    /** Destructor. **/
    FileOutputPolicy::~FileOutputPolicy()
    {}
    /** 
     * Build log filename using current datetime.
     * @return Allways true
     */
    bool FileOutputPolicy::setup()
    {
        std::time_t  t  = std::time(NULL);
        std::tm     *tm = std::localtime(&t);
        
        char buffer[256];
        std::strftime(buffer, sizeof(buffer), "log_%H%M%S_%d%m%Y.txt", tm);
        _filename = buffer;
        std::cout << _filename << std::endl;
        return true;
    }
    /**
     * Write to log file.
     * @param [in] msg Log message.
     * @return true if the message was succesfully written to
     *         the file.
     */
    bool FileOutputPolicy::write(std::string & msg)
    {
        static File::OpenMode mode = File::WRITE_ONLY;
        File out;

        bool ret = out.open(_filename.c_str(), mode);
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
