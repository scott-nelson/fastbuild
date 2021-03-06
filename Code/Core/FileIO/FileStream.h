// FileStream
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_FILEIO_FILESTREAM_H
#define CORE_FILEIO_FILESTREAM_H

//------------------------------------------------------------------------------
#include "IOStream.h"

// FileStream
//------------------------------------------------------------------------------
class FileStream : public IOStream
{
public:
	explicit FileStream();
	~FileStream();

	enum FileMode
	{
		READ_ONLY		= 0x1,
		WRITE_ONLY		= 0x2,
		TEMP			= 0x4,
		DELETE_ON_CLOSE	= 0x8,
		SHARE_READ		= 0x10,
		SHARE_WRITE		= 0x20,
		SHARE_DELETE	= 0x40,
		NO_RETRY_ON_SHARING_VIOLATION = 0x80,
	};

	bool Open( const char * fileName, uint32_t mode = FileStream::READ_ONLY );
	void Close();

	bool IsOpen() const;

	// raw read/write functions
	virtual uint64_t ReadBuffer( void * buffer, uint64_t bytesToRead );
	virtual uint64_t WriteBuffer( const void * buffer, uint64_t bytesToWrite );
	virtual void Flush();

	// size/position
	virtual uint64_t Tell() const;
	virtual bool Seek( uint64_t pos ) const;
	virtual uint64_t GetFileSize() const;

	// file specific
	bool SetLastWriteTime( uint64_t lastWriteTime );

	// Access to handle
	inline void * GetHandle() const { return m_Handle; }

private:
	void * m_Handle;
};

//------------------------------------------------------------------------------
#endif // CORE_FILEIO_FILESTREAM_H
