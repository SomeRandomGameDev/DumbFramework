#include <string>
#include <iostream>
#include <log.hpp>

/**
 * Evaluate filter.
 * @param [in] module Module id.
 * @param [in] severity Severity.
 */
bool AllPassFilter::eval(size_t, size_t )
{
	return true;
}

/**
 * Build log string.
 * @param [out] buffer String buffer.
 * @param [in]  module Module id.
 * @param [in]  severity Severity.
 * @param [in]  infos Log information.
 * @param [in]  format String format.
 * @param [in]  args Format argument list.
*/
void SimpleMessageFormat::build(std::string & buffer, size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args)
{
  char str[1024];
  
  va_start(args, format);
  vsnprintf(str, 1024, format, args);
  va_end(args);

  buffer = str;
}

/**
 * Write buffer to console.
 * @param [in] buffer String buffer.
 */
void ConsoleOutput::write(std::string const & buffer)
{
	std::cout << buffer.c_str() << std::endl;
}
