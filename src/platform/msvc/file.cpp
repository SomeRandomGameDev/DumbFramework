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
    DWORD ret;
    size_t pathLength;
    
    pathLength = GetCurrentDirectory(0, nullptr);
    if(0 == pathLength)
    {
        return std::string();
    }
    std::string path(pathLength, '\0');
    pathLength = GetCurrentDirectory(string.size(), &size[0]);
    return path;
}

/**
 * @brief Return the executable directory.
 * @return Executable directory.
 */
std::string File::executableDirectory()
{
    HMODULE module = GetModuleHandle(NULL);
    if(nullptr == module)
    {
        return std::string();
    }
    char buffer[1024]; // May be big enough.
    GetModuleFileName(module , buffer, sizeof(buffer));
    return std::string(buffer);
}

} // Dumb
