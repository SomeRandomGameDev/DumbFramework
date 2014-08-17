namespace Framework {
namespace Log {

    // Constructor
    template <class FilterPolicy, class FormatPolicy>
        LogBuilder<FilterPolicy, FormatPolicy>::LogBuilder()
        : BaseLogBuilder()
          , _filter()
          , _format()
    {}

    // Destructor
    template <class FilterPolicy, class FormatPolicy>
        LogBuilder<FilterPolicy, FormatPolicy>::~LogBuilder()
        {}

    // Build log message
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

} // Log
} // Framework
