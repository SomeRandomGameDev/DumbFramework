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
namespace Dumb {
namespace Log  {

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
    bool LogBuilder<FilterPolicy, FormatPolicy>::build(std::string & out, Dumb::Module::Identifier const & module, Dumb::Severity const & severity, SourceInfos const & infos, char const * format, va_list args)
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
} // Dumb
