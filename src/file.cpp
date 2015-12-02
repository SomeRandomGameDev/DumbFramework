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
#include <DumbFramework/file.hpp>

namespace Dumb {
/** 
 * @brief Default constructor. 
 *
 * Set file attributes to default values.
 * The current file mode is set to @a INVALID. Read, write, seek
 * will fail.
 */
File::File()
    : _handle(NULL)
    , _mode(File::INVALID)
    , _filename()
{}

/** 
 * @brief Destructor. 
 *
 * Close the file if it is opened and release resources. 
 */
File::~File()
{
    close();
}
/**
 * @brief Set filename.
 * 
 * @note The file is closed if it was previously opened.
 * @param [in] name Filename.
 */
void File::setFilename(std::string const& name)
{
    close();
    _filename = name;
}

/** 
 * @brief Set opening mode.
 * 
 * @note The file is closed if it was previously opened.
 * @param [in] mode File opening mode.
 * @return true if the mode was valid and successfully set.
 */
bool File::setOpenMode(File::OpenMode mode)
{
    if(File::INVALID == mode)
    {
        return false;
    }
    
    // Validate mode and build mode string.
    char str[4];
    int last;
    
    str[1] = 'b';
    last = 2;

    if(mode & File::APPEND)
    {
        str[0] = 'a';
        if(mode & File::READ_ONLY)
        {
            str[last++] = '+';
        }
    }
    else
    {
        switch(mode & File::READ_WRITE)
        {
            case File::READ_WRITE:
                str[0] = (mode & File::TRUNCATE) ? 'w' : 'r';
                str[last++] = '+';
                break;
            case File::READ_ONLY:
                str[0] = 'r';
                break;
            case File::WRITE_ONLY:
                str[0] = 'w';
                break;
            default:
                return false;
        }
    }
    str[last] = '\0';

    close();
    _mode = mode;
    
    for(int i=0; i<=last; i++)
    {
        _modeString[i] = str[i];
    }
    return true;
}

/**
 * @brief Open file.
 * 
 * Open the file whose name was set by File::setFilename using
 * the mode specified by File::setOpenMode.
 * This method will fail if these two methods were not previously
 * called.
 * @return 
 *     - true if the file was successfully opened.
 *     - false otherwise (check @c errno for more informations)
 */
bool File::open()
{
    if(_filename.empty() && (File::INVALID == _mode))
    {
        return false;
    }
    
    // Open file.
    _handle = fopen(_filename.c_str(), _modeString);
    return (NULL != _handle);
}

/**
 * @brief Open file.
 * 
 * Open the file whose name was set by File::setFilename using 
 * the mode given as argument.
 * @param [in] mode File mode. 
 * @return
 *     - true if the file was successfully opened.
 *     - false otherwise (check @c errno for more informations)
 */
bool File::open(File::OpenMode mode)
{
    if(_filename.empty())
    {
        return false;
    }

    // Validate mode.
    if(!setOpenMode(mode))
    {
        return false;
    }
    
    // Open file.
    _handle = fopen(_filename.c_str(), _modeString);
    return (NULL != _handle);
}

/** 
 * @brief Open file.
 * 
 * Open the file name whose name and opening are set by the 
 * method arguments.
 * This method is an alias to the successive calls of 
 * File::SetFilename, File::SetOpenMode and File::open.
 * @param [in] name Filename.
 * @param [in] mode File opening mode. 
 * @return
 *     - true if the file was successfully opened.
 *     - false otherwise (check @c errno for more informations).
 */
bool File::open(std::string const& name, File::OpenMode mode)
{
    // Set filename
    setFilename(name);
    
    // Validate mode.
    if(!setOpenMode(mode))
    {
        return false;
    }
    
    // Open file.
    _handle = fopen(_filename.c_str(), _modeString);
    return (NULL != _handle);
}

/** 
 * @brief Close file. 
 *
 * Close the file.
 * File name and opening mode are not reset. 
 * If File::open is called, the same file will be reopened using
 * the same mode. 
 */
void File::close()
{
    if(_handle)
    {
        fclose(_handle);
        _handle = NULL;
    }
}

/** @brief Check if the file is opened. */
bool File::isOpened() const
{
    return (_handle != NULL);
}

/** @brief Get filename. */
std::string const& File::name() const
{
    return _filename;
}
/** @brief Get file size in bytes. */
size_t File::size() const
{
    if(_handle)
    {
        size_t size;
        off_t currentOffset = ftell(_handle);
        
        fseek(_handle, 0, SEEK_END);
        size = ftell(_handle);
        fseek(_handle, 0, SEEK_SET);
        size -= ftell(_handle);
        fseek(_handle, currentOffset, SEEK_SET);
        
        return size;
    }
    
    return 0;
}

/** @brief Get current file offset in bytes. */
off_t File::tell() const
{
    if(_handle)
    {
        return ftell(_handle);
    }
    return 0;
}

/** @brief Get current opening mode. */
File::OpenMode File::mode() const
{
    return _mode;
}

/** 
 * @brief Read data from file.
 * 
 * Read @a len bytes from file and store them at the location
 * pointed by @a buffer. 
 * @param [in] buffer Destination buffer.
 * @param [in] len Number of bytes to read.
 * @return Number of bytes read. A value less than @a len means
 * that an error occured or the end-of-file is reached. Check
 * the value or @c errno of File::eof to determine which occured.
 */
size_t File::read (void* buffer, size_t len)
{
    if(NULL == _handle)
    {
        return 0;
    }
    
    return fread(buffer, 1, len, _handle);
}

/**
 * @brief Write data to file.
 * 
 * Write @a len bytes to file frome the location pointer by 
 * @a buffer.
 * @param [out] buffer Source buffer.
 * @param [in] len Number of bytes to be written.
 * @return Number of bytes effectively written. A value less 
 * than @a len means that an error occured or the end-of-file is
 * reached. Check the value of @c errno or File::eof to determine
 * which occured.
 */
size_t File::write(void* buffer, size_t len)
{
    if(NULL == _handle)
    {
        return 0;
    }
    return fwrite(buffer, 1, len, _handle);
}

/**
 * @brief Move file offset.
 *
 * Move file offset relatively to the @a origin.
 * @param [in] offset Offset in bytes from the starting point.
 * @param [in] origin Offset displacement starting point.
 * @return true if the file offset was successfully moved.
 */
bool File::seek(off_t offset, File::Origin origin)
{
    int whence;
    switch(origin)
    {
        case File::CURRENT:
            whence = SEEK_CUR;
            break;
        case File::START:
            whence = SEEK_SET;
            break;
        case File::END:
            whence = SEEK_END;
            break;
        default:
            return false;
    }
    if(fseek(_handle, offset, whence) < 0)
    {
        return false;
    }
    return true;
}

/**
 * @brief Check if the end-of-file is reached.
 * 
 * Check if the file offset is positioned at the end of the file.
 * @return true if we reached the end of the file.
 */
bool File::eof()
{
    if(NULL == _handle)
    {
        return true;
    }
    return (0 != feof(_handle));
}

} // Dumb
