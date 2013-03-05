#include <file.hpp>

/** Constructor. */
File::File()
	: _handle(NULL)
	, _mode(File::INVALID)
	, _filename()
{}

/** Destructor. */
File::~File()
{
	close();
}

/**
 * Set opening mode.
 * The file is closed if it was previously opened.
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
	if(!(mode & File::TEXT))
	{
		str[1] = 'b';
		last = 2;
	}
	else
	{
		last = 1;
	}

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
 * Open file.
 * filename and opening mode must have been previously set.
 * @return true if the file was successfully opened. Check errno if it failed.
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
 * Open file.
 * @param [in] mode File mode. 
 * @return true if the file was successfully opened. Check errno if it failed.
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
 * Close file.
 */
void File::close()
{
	if(_handle)
	{
		fclose(_handle);
		_handle = NULL;
	}
}

/** Check if the file is opened. */
bool File::isOpened() const
{
	return (_handle != NULL);
}

/** @return filename. */
const char* File::name() const
{
	return _filename.c_str();
}
/** @return file size in bytes. */
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

/** @return current file offset in bytes. */
off_t File::offset() const
{
	if(_handle)
	{
		return ftell(_handle);
	}
	return 0;
}

/** @return current open mode. */
File::OpenMode File::mode() const
{
	return _mode;
}
		
/**
 * Read data from file.
 * @param [in] buffer Destination buffer.
 * @param [in] len Number of bytes to read.
 * @return Number of bytes read.
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
 * Write data to file.
 * @param [out] buffer Source buffer.
 * @param [in] len Number of bytes to be written.
 * @return Number of bytes effectively written.
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
 * Move file offset.
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
