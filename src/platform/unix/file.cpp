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
#include <DumbFramework/config.hpp>
#include <DumbFramework/file.hpp>

namespace Dumb {

/**
 * @brief Return the current working directory.
 * @return Current working directory.
 */
std::string File::currentWorkingDirectory()
{
    char buffer[1024];
    char *path;
    
    path = getcwd(buffer, sizeof(buffer));
    if(nullptr == path)
    {
        return std::string();
    }
    return std::string(path);
}

/**
 * @brief Return the executable directory.
 * @return Executable directory.
 */
std::string File::executableDirectory()
{
    char buffer[1024]; // May be big enough.
    size_t pathLength;
    
    // readlink does not add '\0' at the end.
    pathLength = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    if(std::string::npos == pathLength)
    {
        return std::string();
    }
    // Append '\0'.
    buffer[pathLength] = '\0';
    return std::string( dirname(buffer) );
}

} // Dumb
