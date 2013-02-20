#ifndef _DUMB_FW_LOG_
#define _DUMB_FW_LOG_

#include <config.hpp>
#include <string>
#include <stdarg.h>
#include <file.hpp>

/**
 * Log source.
 */
struct SourceInfos
{
	char const* filename; /**< Filename. */
	size_t      line;     /**< Line number. */
	char const* function; /**< Function name. */
	// more?
};

/**
 * Logger interface.
 */
class BaseLogger
{
	public:
		/**
		 * Output log message.
		 * @param [in] module   Module ID. 
		 * @param [in] severity Log severity (warning, trace, error, ...).
		 * @param [in] infos    Source information (file, line number, ...).
		 * @param [in] format   Format string.
		 * @param [in] args     Format string arguments.
		 */
		virtual void log(size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args) = 0;
};


/**
 * Log message.
 */
template <class FilterPolicy, class FormatPolicy, class OutputPolicy>
class Logger : public BaseLogger
{
	public:
		/**
		 * Constructor.
		 */
		Logger();
		/**
		 * Destructor.
		 */
		virtual ~Logger();

		/**
		 * Output log message.
		 * @param [in] module   Module ID. 
		 * @param [in] severity Log severity (warning, trace, error, ...).
		 * @param [in] infos    Source information (file, line number, ...).
		 * @param [in] format   Format string.
		 * @param [in] args     Format string arguments.
		 */
		virtual void log(size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args);

	private:
		FilterPolicy _filter; /**< Message filter. */
		FormatPolicy _format; /**< Used to build log string. */
		OutputPolicy _output; /**< Log output. */
		std::string  _buffer; /**< String buffer. */
};

/**
 * No filter.
 */
struct AllPassFilter
{
	/**
	 * Evaluate filter.
	 * @param [in] module Module id.
	 * @param [in] severity Severity.
	 */
	bool eval(size_t module, size_t severity);
};

/**
 * Simple message format.
 */
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

/**
 * Console output.
 */
struct ConsoleOutput
{
	/**
	 * Write buffer to console.
	 * @param [in] buffer String buffer.
	 */
	void write(std::string const & buffer);
};

/** 
 * Constructor.
 */
template <class FilterPolicy, class FormatPolicy, class OutputPolicy>
Logger<FilterPolicy, FormatPolicy, OutputPolicy>::Logger()
	: BaseLogger()
	, _filter()
	, _format()
	, _output()
	, _buffer()
{}

/** 
 * Destructor.
 */
template <class FilterPolicy, class FormatPolicy, class OutputPolicy>
Logger<FilterPolicy, FormatPolicy, OutputPolicy>::~Logger()
{}

/**
 * Output log message.
 * @param [in] module   Module ID. 
 * @param [in] severity Log severity (warning, trace, error, ...).
 * @param [in] infos    Source information (file, line number, ...).
 * @param [in] format   Format string.
 * @param [in] args     Format string arguments.
 */
template <class FilterPolicy, class FormatPolicy, class OutputPolicy>
void Logger<FilterPolicy, FormatPolicy, OutputPolicy>::log(size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args)
{
	if(_filter.eval(module, severity))
	{
		_format.build(_buffer, module, severity, infos, format, args);
		_output.write(_buffer);
	}
}


#endif /* _DUMB_FW_LOG_ */
