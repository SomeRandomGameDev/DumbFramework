#include <errno.h>
#include <string>
#include <iostream>
#include <log.hpp>

namespace Log
{
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
		, _task()
		, _msgQueue()
		, _builder(NULL)
	{}
	/** Constructor. */
	LogProcessor::LogProcessor(LogProcessor const &)
		: _lock(PTHREAD_MUTEX_INITIALIZER)
		, _alive(PTHREAD_MUTEX_INITIALIZER)
		, _task()
		, _msgQueue()
		, _builder(NULL)
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
		LogProcessor& processor = LogProcessor::instance();
		std::string msg;

		OutputPolicyBase *output = (OutputPolicyBase*)param;
		if(param == NULL)
		{
			return NULL;
		}

		while(!processor.mustStop() || ret)
		{
			ret = processor.dequeueMessage(msg);
			if(ret)
			{
				output->write(msg);
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
		pthread_mutex_init(&_lock, NULL);
		pthread_mutex_init(&_alive, NULL);
		pthread_mutex_lock(&_alive);
		int ret = pthread_create(&_task, NULL, taskRoutine, outputPolicy);
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
		pthread_mutex_unlock(&_lock);
		return ret;
	}

	void LogProcessor::write(size_t module, size_t severity, SourceInfos const & infos, char const * format, ...)
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

	bool AllPassFilter::eval(size_t module, size_t severity)
	{
		return true;
	}

	void SimpleMessageFormat::build(std::string & buffer, size_t module, size_t severity, SourceInfos const & infos, char const * format, va_list args)
	{
		char data[1024];
		vsnprintf(data, 1024, format, args);
		buffer = data;
	}

	bool ConsoleOutputPolicy::write(std::string & msg)
	{
		std::cout << msg << std::endl;
		return true;
	}

	/// Write to log file.
	bool FileOutputPolicy::write(std::string & msg)
	{
		static File out;
		File::OpenMode mode;
		if(!out.isOpened())
		{
			mode = File::WRITE_ONLY;
		}
		else
		{
			mode = File::APPEND;
		}
		bool ret = out.open("log.txt", mode);
		if(true != ret) return ret;

		size_t wlen = msg.size();
		size_t rlen = out.write((void*)msg.c_str(), wlen);
		if(rlen != wlen) return false;

		out.close();
		return true;
	}

} // Log
