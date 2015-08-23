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
#ifndef _DUMB_FW_FILE_
#define _DUMB_FW_FILE_

#include <stdio.h>
#include <sys/types.h>
#include <string>

namespace Dumb {

/**
 * @brief File wrapper.
 * 
 * This is mainly a wrapper around the POSIX stream functions.
 */
class File
{
    public:
        /**
         * @brief File open modes.
         * 
         * This modes are similar to the one defined in the libc fopen function.
         * They can be combined using a bitwise or. Note that some
         * combinations are invalid.
         */
        enum OpenMode
        {
            /**
             * Invalid mode.
             * This mode is used for internal sanity checks.
             */
            INVALID    = 0x00,
            /** 
             * Opened the file for reading.
             * The file offset is positioned at the beginning of the file.
             */
            READ_ONLY  = 0x01,
            /** 
             * Open the file for writing. The file is truncated to zero
             * if it exists, otherwise it is created.
             * The file offset is positioned at the beginning of the file.
             */
            WRITE_ONLY = 0x02,
            /**
             * Open the file for reading and writing. The file is created
             * if it does not exists.
             * The file offset is positioned at the beginning of the file.
             * @note If @a TRUNCATE is also specified, the file will be
             * truncated if it exists.
             */ 
            READ_WRITE = READ_ONLY | WRITE_ONLY,
            /**
             * Open the file for appending. The file is created is created
             * if it does not exists.
             * The file offset is positioned at the end of the file.
             * @note If the @a READ_ONLY mode is also specified, 
             * the file will also be opened for reading. Read will start 
             * at the beginning of the file, and output at the end of the file. 
             */
            APPEND     = 0x04,
            /**
             * This mode is only valid if it is used in conjonction with
             * @a READ_WRITE. @see READ_WRITE
             */
            TRUNCATE   = 0x08,
        };

        /**
         * File offset origin.
         */
        enum Origin
        {
            /** Offsets will be relative to the current file position. **/
            CURRENT,
            /** Offsets will be relative to the beginning of the file. **/
            START,
            /** Offsets will be relative to the end of the file. **/
            END
        };

    public:
        /** 
         * @brief Default constructor. 
         *
         * Set file attributes to default values.
         * The current file mode is set to @a INVALID. Read, write, seek
         * will fail.
         */
        File();
        /** 
         * @brief Destructor. 
         *
         * Close the file if it is opened and release resources. 
         */
        ~File();

        /**
         * @brief Set filename.
         * 
         * @note The file is closed if it was previously opened.
         * @param [in] name Filename.
         */
        void setFilename(std::string const& name);

        /** 
         * @brief Set opening mode.
         * 
         * @note The file is closed if it was previously opened.
         * @param [in] mode File opening mode.
         * @return true if the mode was valid and successfully set.
         */
        bool setOpenMode(File::OpenMode mode);
        
        /**
         * @brief Open file.
         * 
         * Open the file whose name was set by File::setFilename using
         * the mode specified by File::setOpenMode.
         * This method will fail if both filename and opening mode were
         * not previously set.
         * @return 
         *     - true if the file was successfully opened.
         *     - false otherwise (check @c errno for more informations).
         */
        bool open();

        /**
         * @brief Open file.
         * 
         * Open the file whose name was set by File::setFilename using 
         * the mode given as argument.
         * This method will fail if the filename was not previously set.
         * @param [in] mode File opening mode. 
         * @return
         *     - true if the file was successfully opened.
         *     - false otherwise (check @c errno for more informations).
         */
        bool open(File::OpenMode mode);

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
        bool open(std::string const& name, File::OpenMode mode);
        
        /** 
         * @brief Close file. 
         *
         * Close the file.
         * File name and opening mode are not reset. 
         * If File::open is called, the same file will be reopened using
         * the same mode. 
         */
        void close();

        /** @brief Check if the file is opened. */
        bool isOpened() const;

        /** @brief Get filename. */
        std::string const& name  () const;
        /** @brief Get file size in bytes. */
        size_t             size  () const;
        /** @brief Get current file offset in bytes. */
        off_t              offset() const;
        /** @brief Get current opening mode. */
        File::OpenMode     mode  () const;
        
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
        size_t read (void* buffer, size_t len);

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
        size_t write(void* buffer, size_t len);

        /**
         * @brief Move file offset.
         *
         * Move file offset relatively to the @a origin.
         * @param [in] offset Offset in bytes from the starting point.
         * @param [in] origin Offset displacement starting point.
         * @return true if the file offset was successfully moved.
         */
        bool seek(off_t offset, File::Origin origin);

        /**
         * @brief Check if the end-of-file is reached.
         * 
         * Check if the file offset is positioned at the end of the file.
         * @return true if we reached the end of the file.
         */
        bool eof();
    
        /**
         * @brief Return the current working directory.
         * @return Current working directory.
         */
        static std::string currentWorkingDirectory();
        /**
         * @brief Return the executable directory.
         * @return Executable directory.
         */
        static std::string executableDirectory();
        
    protected:
        FILE *_handle;         /**< Handle. */
        File::OpenMode _mode;  /**< Open mode. */
        char _modeString[4];   /**< Open mode string. */
        std::string _filename; /**< Filename. */
};

} // Dumb

#endif /* _DUMB_FW_FILE_ */
