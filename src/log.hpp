#ifndef _DUMB_FW_LOG_
#define _DUMB_FW_LOG_

#include "config.hpp"
#include <string>
#include <list>
#include <stdarg.h>
#include <pthread.h>

#include "module.hpp"
#include "severity.hpp"
#include "file.hpp"

namespace Framework {
namespace Log {
    /**
     * Log source.
     */
    struct SourceInfos
    {
        SourceInfos();
        SourceInfos(char const * name, size_t num, char const * fnctl);
        SourceInfos(SourceInfos const & infos);

        SourceInfos& operator= (SourceInfos const & infos);

        char const* filename; /**< Filename. */
        size_t      line;     /**< Line number. */
        char const* function; /**< Function name. */
        // more?
    };

    /**
     * Log output policy.
     */
    struct OutputPolicyBase
    {
        virtual ~OutputPolicyBase() {}
        virtual bool setup()    { return true; }
        virtual void teardown() {}
        virtual bool write(std::string & msg) = 0;
    };

    /**
     * Log builder
     */
    class BaseLogBuilder
    {
        public:
            /** Constructor. */
            BaseLogBuilder();
            /** Destructor. */
            virtual ~BaseLogBuilder();
            /**
             * Build log message.
             * @param [out] out      Output string.
             * @param [in]  module   Module ID. 
             * @param [in]  severity Log severity (warning, trace, error, ...).
             * @param [in]  infos    Source information (file, line number, ...).
             * @param [in]  format   Format string.
             * @param [in]  args     Format string arguments.
             */
            virtual bool build(std::string & out, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args) = 0;
    };

    /**
     * Log builder.
     */
    template <class FilterPolicy, class FormatPolicy>
        class LogBuilder : public BaseLogBuilder
    {
        public:
            /** Constructor. */
            LogBuilder();
            /** Destructor. */
            virtual ~LogBuilder();

            /**
             * Output log message.
             * @param [out] out      Output string.
             * @param [in]  module   Module ID. 
             * @param [in]  severity Log severity (warning, trace, error, ...).
             * @param [in]  infos    Source information (file, line number, ...).
             * @param [in]  format   Format string.
             * @param [in]  args     Format string arguments.
             */
            virtual bool build(std::string & out, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args);

        private:
            FilterPolicy _filter; /**< Message filter. */
            FormatPolicy _format; /**< Used to build log string. */
    };

    /**
     * Log message builder/manager.
     */
    class LogProcessor
    {
        public:
            /** Destructor. **/
            ~LogProcessor();
            /** Get log processor instance. */
            static LogProcessor& instance();

            void write(Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, ...);
            /**
             * Start logging task.
             * @param [in] builder       Log message builder.
             * @param [in] outputPolicy  Filter and log output.
             */
            bool start(BaseLogBuilder* builder, OutputPolicyBase* outputPolicy);
            /** Stop logging task. **/
            bool stop();
            /** Flush message queue. **/
            void flush();
            
        protected:
            /**
             * Log processor thread routine.
             * @param [in] param  Opaque pointer to log processor.
             */
            static void* taskRoutine(void *param);
            /**
             * Add string to message queue.
             * @param [in] msg  Log message.
             */
            void queueMessage(std::string const & msg);
            /**
             * Retrieve log message from queue.
             * @param [out] msg  Log message.
             * @return false if the queue is empty.
             */
            bool dequeueMessage(std::string & msg);
            /** Return true if the thread must stop. **/
            bool mustStop();

        private:
            LogProcessor();
            LogProcessor(LogProcessor const &);
            LogProcessor& operator= (LogProcessor const &);

        private:
            pthread_mutex_t  _lock;
            pthread_mutex_t  _alive;
            pthread_cond_t   _empty;
            pthread_t        _task;

            std::list<std::string> _msgQueue;

            BaseLogBuilder   *_builder;
            OutputPolicyBase *_output; 
    };

    /** 
     * Constructor.
     */
    template <class FilterPolicy, class FormatPolicy>
        LogBuilder<FilterPolicy, FormatPolicy>::LogBuilder()
        : BaseLogBuilder()
          , _filter()
          , _format()
    {}

    /** 
     * Destructor.
     */
    template <class FilterPolicy, class FormatPolicy>
        LogBuilder<FilterPolicy, FormatPolicy>::~LogBuilder()
        {}

    /**
     * Build log message.
     * @param [out] out      Output string.
     * @param [in]  module   Module ID. 
     * @param [in]  severity Log severity (warning, trace, error, ...).
     * @param [in]  infos    Source information (file, line number, ...).
     * @param [in]  format   Format string.
     * @param [in]  args     Format string arguments.
     */
    template <class FilterPolicy, class FormatPolicy>
        bool LogBuilder<FilterPolicy, FormatPolicy>::build(std::string & out, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args)
        {
            if(_filter.eval(module, severity))
            {
                LogProcessor& processor = LogProcessor::instance();
                (void)processor;
                _format.build(out, module, severity, infos, format, args);
                return true;
            }
            return false;
        }

    /** No filter. */
    struct AllPassFilter
    {
        /**
         * Evaluate filter.
         * @param [in] module Module id.
         * @param [in] severity Severity.
         */
        bool eval(Framework::Module const & module, Framework::Severity const & severity);
    };

    /** Simple message format. */
    struct SimpleMessageFormat
    {
        /** Maximum buffer length. **/
        static const size_t MaxBufferLen;
        /**
         * Build log string.
         * @param [out] buffer String buffer.
         * @param [in]  module Module id.
         * @param [in]  severity Severity.
         * @param [in]  infos Log information.
         * @param [in]  format String format.
         * @param [in]  args Format argument list.
         */
        void build(std::string & buffer, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args);
    };

    /** Write log to console. **/
    struct ConsoleOutputPolicy : public OutputPolicyBase
    {
        /**
         * Write to standard output.
         * @param [in] msg Log message.
         */
        bool write(std::string & msg);
    };

    /** Write log to file. **/
    class FileOutputPolicy : public OutputPolicyBase
    {
        public:
            /** Contructor. **/
            FileOutputPolicy();
            /** Destructor. **/
            ~FileOutputPolicy();
            /** Build log filename using current datetime. **/
            bool setup();
            /**
             * Write to log file.
             * @param [in] msg Log message.
             */
            bool write(std::string & msg);
        private:
            std::string _filename; /**< Log filename. **/
    };

} // Log
} // Framework

#define Log_Trace(module, format, ...)   do { Framework::Log::LogProcessor::instance().write(module, Framework::Severity::Trace,   Framework::Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);
#define Log_Warning(module, format, ...) do { Framework::Log::LogProcessor::instance().write(module, Framework::Severity::Warning, Framework::Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);
#define Log_Error(module, format, ...)   do { Framework::Log::LogProcessor::instance().write(module, Framework::Severity::Error,   Framework::Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);

#endif /* _DUMB_FW_LOG_ */
