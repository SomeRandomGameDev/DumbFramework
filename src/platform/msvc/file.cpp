#include <DumbFramework/config.hpp>
#include <DumbFramework/file.hpp>

namespace Framework {

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

} // Framework
