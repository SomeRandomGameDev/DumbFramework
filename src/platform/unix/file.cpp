#include <DumbFramework/config.hpp>
#include <DumbFramework/file.hpp>

namespace Framework {

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

} // Framework
