#ifndef _DUMB_FW_FILE_
#define _DUMB_FW_FILE_

#include <stdio.h>
#include <sys/types.h>
#include <string>

/**
 * File wrapper.
 */
class File
{
	public:
		/**
 		 * File open mode.
 		 */
		enum OpenMode
		{
			INVALID    = 0x00,
			READ_ONLY  = 0x01,
			WRITE_ONLY = 0x02,
			READ_WRITE = READ_ONLY | WRITE_ONLY,
			APPEND     = 0x04,
			TRUNCATE   = 0x08,
			TEXT       = 0x10
		};

		/**
 		 * File offset origin.
 		 */
		enum Origin
		{
			CURRENT,
			START,
			END
		};

	public:
		/** Constructor. */
		File();
		/** Destructor. */
		~File();

		/**
 		 * Set filename.
		 * The file is closed if it was previously opened.
		 * @param [in] name Filename.
		 */
		template <typename T>
		void setFilename(const T& name);

		/** 
 		 * Set opening mode.
		 * The file is closed if it was previously opened.
		 * @param [in] mode File opening mode.
		 * @return true if the mode was valid and successfully set.
		 */
		bool setOpenMode(File::OpenMode mode);
		
		/**
		 * Open file.
		 * filename and opening mode must have been previously set.
		 * @return true if the file was successfully opened. Check errno if it failed.
		 */
		bool open();

		/**
 		 * Open file.
		 * The filename must have been previously set.
		 * @param [in] mode File mode. 
		 * @return true if the file was successfully opened. Check errno if it failed.
		 */
		bool open(File::OpenMode mode);

		/** 
		 * Open file.
		 * @param [in] mode name Filename.
		 * @param [in] mode File mode. 
		 * @return true if the file was successfully opened. Check errno if it failed.
		 */
		template <typename T>
		bool open(const T& name, File::OpenMode mode);
		
		/** Close file. */
		void close();

		/** Check if the file is opened. */
		bool isOpened() const;

		/** @return filename. */
		const char* name() const;
		/** @return file size in bytes. */
		size_t size() const;
		/** @return current file offset in bytes. */
		off_t offset() const;
		/** @return current open mode. */
		File::OpenMode mode() const;
		
		/** Read data from file.
		 * @param [in] buffer Destination buffer.
		 * @param [in] len Number of bytes to read.
		 * @return Number of bytes read.
		 */
		size_t read (void* buffer, size_t len);

		/**
		 * Write data to file.
		 * @param [out] buffer Source buffer.
		 * @param [in] len Number of bytes to be written.
		 * @return Number of bytes effectively written.
		 */
		size_t write(void* buffer, size_t len);

		/**
		 * Move file offset.
		 * @param [in] offset Offset in bytes from the starting point.
		 * @param [in] origin Offset displacement starting point.
		 * @return true if the file offset was successfully moved.
		 */
		bool seek(off_t offset, File::Origin origin);

	protected:
		FILE *_handle;         /**< Handle. */
		File::OpenMode _mode;  /**< Open mode. */
		char _modeString[4];   /**< Open mode string. */
		std::string _filename; /**< Filename. */
};

/**
 * Set filename.
 * The file is closed if it was previously opened.
 * @param [in] name Filename.
 */
template <typename T>
void File::setFilename(const T& name)
{
	close();
	_filename = name;
}

/** 
 * Open file.
 * @param [in] mode name Filename.
 * @param [in] mode File mode. 
 * @return true if the file was successfully opened. Check errno if it failed.
 */
template <typename T>
bool File::open(const T& name, File::OpenMode mode)
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
	if(NULL == _handle)
	{
		return false;
	}
	return true;
}

#endif /* _DUMB_FW_FILE_ */
