// SystemMutex.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_PROCESS_SYSTEMMUTEX_H
#define CORE_PROCESS_SYSTEMMUTEX_H

// Includes
//------------------------------------------------------------------------------
#include "../Strings/AString.h"

// SystemMutex
//------------------------------------------------------------------------------
class SystemMutex
{
public:
	SystemMutex( const char * );
	~SystemMutex();

	bool TryLock();
    bool IsLocked() const;
	void Unlock();

private:    
	#if defined( __WINDOWS__ )
		void * m_Handle;
    #elif defined( __LINUX__ ) || defined( __APPLE__ )
        int m_Handle;
	#endif
	AString m_Name;
};

//------------------------------------------------------------------------------
#endif // CORE_PROCESS_SYSTEMMUTEX_H
