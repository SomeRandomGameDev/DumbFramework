#ifndef _DUMB_FW_LOG_
#define _DUMB_FW_LOG_

#include <string>
#include <list>
#include <stdarg.h>
#include <pthread.h>

#include <DumbFramework/config.hpp>
#include <DumbFramework/module.hpp>
#include <DumbFramework/severity.hpp>
#include <DumbFramework/file.hpp>

/**
 * @defgroup DUMB_FW_LOG Logging system.
 * Application logging system.
 * 
 * In order to use the logging system in your application, you must 
 * provide an object that implements Framework::Log::BaseLogBuilder
 * interface and an output policy to the Framework::Log::LogProcessor
 * instance.
 * Here is an example using the convenience classes provided:
 * @code
 * #include <DumbFramework/log.hpp>
 * 
 * using namespace Framework;
 * 
 * int main()
 * {
 *     Log::LogBuilder<Log::AllPassFilter, Log::SimpleMessageFormat> msgBuilder;
 *     Log::FileOutputPolicy output;
 *     
 *     Log::LogProcessor& processor = Log::LogProcessor::instance();
 *     processor.start(&msgBuilder, &output);
 *     
 *     // [...]
 *     
 *     processor.stop();
 *     
 *     return 0;
 * }
 * @endcode
 * 
 * Now that the log processor is initialized and started, you can emit
 * log messages using one of the 4 macro provided.
 *      - Log_Info (module, format, ...)
 *      - Log_Warning (module, format, ...)
 *      - Log_Error (module, format, ...)
 *      - Log_Ex (module, severity, format, ...)
 */

/**
 * @def  Log_Info(module, format, ...)
 * @hideinitializer
 * @ingroup DUMB_FW_LOG
 * Emit log with the severity set to Framework::Severity::Info.
 * 
 * @a module is one of the module defined in Framework::Module. \n
 * @a format is similar is similar to @c printf. \n
 *
 * The source information (file, line number, function name) is
 * automatically set. 
 */

/**
 * @def  Log_Warning(module, format, ...)
 * @hideinitializer
 * @ingroup DUMB_FW_LOG
 * Emit log with the severity set to Framework::Severity::Warning.
 * 
 * @a module is one of the module defined in Framework::Module. \n
 * @a format is similar to @c printf. \n
 *
 * The source information (file, line number, function name) is
 * automatically set. 
 */

/**
 * @def  Log_Error(module, format, ...)
 * @hideinitializer
 * @ingroup DUMB_FW_LOG
 * Emit log with the severity set to Framework::Severity::Error.
 * 
 * @a module is one of the module defined in Framework::Module. \n
 * @a format is similar to @c printf. \n
 * 
 * The source information (file, line number, function name) is
 * automatically set. 
 */

/**
 * @def Log_Ex(module, severity, format, ...)
 * @hideinitializer
 * @ingroup DUMB_FW_LOG
 * Emit log.
 * This macro may be used in case where the severity is determined by
 * the outcome of some computations.
 * 
 * For example, we get the result of a system via @b errno and the
 * associated message with @b strerror. Depending on the value of
 * @b errno, we can decide that this is an error, or warning, or just
 * an information.
 * @code
 * Framework::Severity severity;
 * int ret;
 * 
 * ret = some_system_function();
 * 
 * if(0 == ret)
 * {
 *    severity = Framework::Severity::Infos;
 * }
 * else if((EINPROGRESS == errno) || (EALREADY == errno))
 * {
 *    severity = Framework::Severity::Warning;
 * }
 * else
 * {
 *     severity = Framework::Severity::Error;
 * }
 * 
 * Log_Ex(Framework::Module::App, severity, "some_system_function result: %s", strerror(errno));
 * 
 * @endcode
 * 
 * @a module is one of the module defined in DumbFramework::Module. \n
 * @a severity is one of the module defined in DumbFramework::Severity. \n
 * @a format is similar to @c printf. \n
 * 
 * The source information (file, line number, function name) is
 * automatically set. 
 */

namespace Framework {
namespace Log {
    /**
     * @brief Log source informations.
     * @ingroup DUMB_FW_LOG
     * 
     * This structure provides various informations about the current log
     * message. It contains the name of the file, the line number and the
     * function name where the log message was emitted.
     */
    struct SourceInfos
    {
        /**
         * @brief Default constructor.
         *
         * Initialize the @a filename, @a line and @a function with
         * dummy values. The file name is the empty string. The line 
         * number is 0 and the function name is also an empty string.
         */
        SourceInfos();
        /**
         * @brief Constructor.
         * 
         * Constructs the source infos.
         * @param [in] name  File name.
         * @param [in] num   Line number.
         * @param [in] fnctl Function name.
         */
        SourceInfos(char const* name, size_t num, char const* fnctl);
        /**
         * @brief Copy constructor.
         * 
         * Constructs the source infos from anothed one.
         * @param [in] infos Source infos.
         */
        SourceInfos(SourceInfos const & infos);

        /**
         * @brief Copy operator.
         * 
         * Copy the data contained in @a infos.
         * @param [in] infos Source infos.
         * @return Reference to the calling instance.
         */
        SourceInfos& operator= (SourceInfos const & infos);

        /** Source filename where the log message was emitted. */
        char const* filename;
        /** Number of the line where the log message was emitted. */
        size_t      line;
        /** Name of the function/method where the log message was emitted. */
        char const* function;
        // more?
    };

    /**
     * @brief Log output policy.
     * @ingroup DUMB_FW_LOG
     * 
     * Defines the way log messages are output.
     */
    struct OutputPolicyBase
    {
        /**
         * @brief Destructor.
         *
         * Destructs any allocated resources used to output log messages.
         */
        virtual ~OutputPolicyBase() {}
        /**
         * @brief Setup output.
         * 
         * Initialize, setup or create resources necessary for log output.
         * @return false if an error occured.
         */
        virtual bool setup()    { return true; }
        /**
         * @brief Release resources.
         */
        virtual void teardown() {}
        /**
         * @brief Output log message.
         * @param [in] msg Log message.
         * @return false if an error occured.
         */
        virtual bool write(std::string & msg) = 0;
    };

    /**
     * @brief Base log builder interface.
     * @ingroup DUMB_FW_LOG
     * 
     * Build log message based on the issuing module, severity, source
     * infos and format data.
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
             * @param [in]  severity Log severity (warning, info, error, ...).
             * @param [in]  infos    Source information (file, line number, ...).
             * @param [in]  format   Format string.
             * @param [in]  args     Format string arguments.
             * @return false if an error occured.
             */
            virtual bool build(std::string & out, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args) = 0;
    };

    /**
     * @brief Log builder.
     * @ingroup DUMB_FW_LOG
     * 
     * Build log message based on the filter and format policy.
     *
     * The @a FilterPolicy must implement an @c eval method. This method
     * is used to filter log entries based on the module id and the
     * severity. It may return false if the current log entry does not
     * match the filter criterias. The log builder will dismiss every
     * log entry that failed filter evaluation.
     * @code
     * bool eval(Framework::Module const & module, Framework::Severity const & severity);
     * @endcode
     * 
     * The @a FormatPolicy must implement a @c build method. This method
     * creates a string based on the module id, severy, source infos and
     * format data.
     * @code
     * void build(std::string & buffer, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args);
     * @endcode
     * 
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
             * Build log message.
             * @param [out] out      Output string.
             * @param [in]  module   Module ID. 
             * @param [in]  severity Log severity (warning, info, error, ...).
             * @param [in]  infos    Source information (file, line number, ...).
             * @param [in]  format   Format string.
             * @param [in]  args     Format string arguments.
             * @return false if an error occured.
             */
            virtual bool build(std::string & out, Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, va_list args);

        private:
            FilterPolicy _filter; /**< Message filter. */
            FormatPolicy _format; /**< Used to build log string. */
    };

    /**
     * @brief Log processor.
     * @ingroup DUMB_FW_LOG
     * 
     * This module is in charge of building and writting log messages
     * based on the log builder and output policy.
     * Log messages are written asynchronously. More precisely, the log
     * message string is first built using the log builder and then
     * pushed into a message queue. An asynchronous task (typically a 
     * thread) reads this message queue and outputs the message string
     * using the output policy.
     * 
     * @note This is a singleton. You can only have instance of this
     * object.
     */
    class LogProcessor
    {
        public:
            /** Destructor. **/
            ~LogProcessor();
            /** Get log processor instance. */
            static LogProcessor& instance();

            /**
             * Emit log message.
             * @param [in]  module   Module ID. 
             * @param [in]  severity Log severity (warning, info, error, ...).
             * @param [in]  infos    Source information (file, line number, ...).
             * @param [in]  format   Format string.
             * @param [in]  ...      Format string arguments.
             */
            void write(Framework::Module const & module, Framework::Severity const & severity, SourceInfos const & infos, char const * format, ...);
            /**
             * Start logging task.
             * @param [in] builder       Log message builder.
             * @param [in] outputPolicy  Filter and log output.
             */
            bool start(BaseLogBuilder* builder, OutputPolicyBase* outputPolicy);
            /** Stop logging task. **/
            bool stop();
            /** 
             * @brief Flush log output.
             *
             * Wait until all emitted log messages have been processed.
             */
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
     * @brief All pass filter.
     * @ingroup DUMB_FW_LOG
     * 
     * This filter policy will allow any log message to be emitted.
     */
    struct AllPassFilter
    {
        /**
         * Evaluate filter.
         * @param [in] module Module id.
         * @param [in] severity Severity.
         * @return Allways true.
         */
        bool eval(Framework::Module const & module, Framework::Severity const & severity);
    };

    /** 
     * @brief Simple message string builder.
     * @ingroup DUMB_FW_LOG
     * 
     * This format policy builds log message string by simply appending
     * module id, severity, function name and format string. A typical
     * log message formatted by this class will look like:
     * @verbatim [info][Render][LoadTextures]Loading brick0001.png @endverbatim
     */
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

    /** 
     * @brief Write log to console. 
     * @ingroup DUMB_FW_LOG
     *
     * All log messages will be written to the standard output. 
     */
    struct ConsoleOutputPolicy : public OutputPolicyBase
    {
        /**
         * Write to standard output.
         * @param [in] msg Log message.
         * @return Allways true.
         */
        bool write(std::string & msg);
    };

    /** 
     * @brief Write log to file.
     * @ingroup DUMB_FW_LOG
     *
     * Write log messages to a file. A new file is created at each
     * invocation of FileOutputPolicy::setup. The name of the newly
     * created file is @b log_HHMMSS_ddmmYYYY.txt with
     *      - @b HH   hour between 00 and 23.
     *      - @b MM   minute between 00 and 59
     *      - @b SS   second between 00 and 59
     *      - @b dd   day of the month between 01 and 31
     *      - @b mm   month of the year between 01 and 12
     *      - @b YYYY year
     */
    class FileOutputPolicy : public OutputPolicyBase
    {
        public:
            /** Default contructor. **/
            FileOutputPolicy();
            /** Destructor. **/
            ~FileOutputPolicy();
            /** 
             * Build log filename using current datetime.
             * @return Allways true
             */
            bool setup();
            /**
             * Write to log file.
             * @param [in] msg Log message.
             * @return true if the message was succesfully written to
             *         the file.
             */
            bool write(std::string & msg);
        private:
            std::string _filename; /**< Log filename. **/
    };

} // Log
} // Framework

#define Log_Info(module, format, ...)    do { Framework::Log::LogProcessor::instance().write(module, Framework::Severity::Info,   Framework::Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);
#define Log_Warning(module, format, ...) do { Framework::Log::LogProcessor::instance().write(module, Framework::Severity::Warning, Framework::Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);
#define Log_Error(module, format, ...)   do { Framework::Log::LogProcessor::instance().write(module, Framework::Severity::Error,   Framework::Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);

#define Log_Ex(module, severity, format, ...) do { Framework::Log::LogProcessor::instance().write(module, severity,   Framework::Log::SourceInfos(__FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); } while(0);

#include "log.inl"

#endif /* _DUMB_FW_LOG_ */
