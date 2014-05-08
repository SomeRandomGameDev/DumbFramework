#ifndef _DUMB_FW_LOG_
#define _DUMB_FW_LOG_

#include <config.hpp>
#include <string>
#include <list>
#include <stdarg.h>
#include <pthread.h>

#include <file.hpp>

namespace Log
{
    /** Log severity. */
    enum SEVERITY
    {
        TRACE = 0,
        WARNING,
        ERROR
    };

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
            virtual bool build(std::string & out, size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args) = 0;
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
            virtual bool build(std::string & out, size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args);

        private:
            FilterPolicy _filter; /**< Message filter. */
            FormatPolicy _format; /**< Used to build log string. */
    };

    /// Log message builder/manager.
    class LogProcessor
    {
        public:
            ~LogProcessor();

            static LogProcessor& instance();

            void write(size_t module, size_t severity, SourceInfos const & infos, char const * format, ...);

            bool start(BaseLogBuilder* builder, OutputPolicyBase* outputPolicy);
            bool stop();

        protected:
            static void* taskRoutine(void *param);

            void queueMessage(std::string const & msg);
            bool dequeueMessage(std::string & msg);

            bool mustStop();

        private:
            LogProcessor();
            LogProcessor(LogProcessor const &);
            LogProcessor& operator= (LogProcessor const &);

        private:
            pthread_mutex_t  _lock;
            pthread_mutex_t  _alive;
            pthread_t        _task;

            std::list<std::string> _msgQueue;

            BaseLogBuilder *_builder;
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
        bool LogBuilder<FilterPolicy, FormatPolicy>::build(std::string & out, size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args)
        {
            if(_filter.eval(module, severity))
            {
                LogProcessor& processor = LogProcessor::instance();
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
        bool eval(size_t module, size_t severity);
    };

    /** Simple message format. */
    struct SimpleMessageFormat
    {
        /**
         * Build log string.
         * @param [out] buffer String buffer.
         * @param [in]  module Module id.
         * @param [in]  severity Severity.
         * @param [in]  infos Log information.
         * @param [in]  format String format.
         * @param [in]  args Format argument list.
         */
        void build(std::string & buffer, size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args);
    };

    struct ConsoleOutputPolicy : public OutputPolicyBase
    {
        bool write(std::string & msg);
    };

    struct FileOutputPolicy : public OutputPolicyBase
    {
        bool write(std::string & msg);
    };

} // Log

#define Log_Trace(module, format, ...)   do { Log::LogProcessor::instance().write(module, Log::TRACE,   Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);
#define Log_Warning(module, format, ...) do { Log::LogProcessor::instance().write(module, Log::WARNING, Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);
#define Log_Error(module, format, ...)   do { Log::LogProcessor::instance().write(module, Log::ERROR,   Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);

#endif /* _DUMB_FW_LOG_ */
