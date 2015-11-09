// SystemMutex
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "SystemMutex.h"
#include "../Strings/AStackString.h"

// system
#if defined( __WINDOWS__ )
    #include <windows.h>
    #include "../Env/Assert.h"
#endif
#if defined( __LINUX__ ) || defined( __APPLE__ )
    #include <errno.h>
    #include <sys/file.h>
#endif

// CONSTRUCTOR
//------------------------------------------------------------------------------
SystemMutex::SystemMutex( const char * name ) :
    #if defined( __WINDOWS__ )
        m_Handle( INVALID_HANDLE_VALUE ),
    #elif defined( __LINUX__ ) || defined( __APPLE__ )
        m_Handle( -1 ),
    #endif
	m_Name( name )
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
SystemMutex::~SystemMutex()
{
    if ( IsLocked() )
    {
        Unlock();
    }
}

// Lock
//------------------------------------------------------------------------------
bool SystemMutex::TryLock()
{ 
    #if defined( __WINDOWS__ )
		void * handle = (void *)CreateMutex( nullptr, TRUE, m_Name.Get() );
		if ( GetLastError() == ERROR_ALREADY_EXISTS )
		{
			if ( ( handle != INVALID_HANDLE_VALUE ) && ( handle != 0 ) )
			{
				CloseHandle( handle );
			}
			return false;
		}
		m_Handle = handle;
		return true;
    #elif defined( __LINUX__ ) || defined( __APPLE__ )
        AStackString<> tempFileName;
        tempFileName.Format( "/var/run/%s.lock", m_Name.Get() );
        m_Handle = open( tempFileName.Get(), O_CREAT | O_RDWR, 0666 );
        int rc = flock( m_Handle, LOCK_EX | LOCK_NB );
        if ( rc )
        {
            if ( errno == EWOULDBLOCK )
            {
                return false; // locked by another process
            }
        }
        return true; // we own it now
    #else
        #error Unknown platform
    #endif
}

// IsLocked
//------------------------------------------------------------------------------
bool SystemMutex::IsLocked() const
{
    #if defined( __WINDOWS__ )
        return ( m_Handle != INVALID_HANDLE_VALUE );
    #elif defined( __LINUX__ ) || defined( __APPLE__ )
        return ( m_Handle != -1 );
    #else
        #error Unknown platform
    #endif
}

// Unlock
//------------------------------------------------------------------------------
void SystemMutex::Unlock()
{
    #if defined( __WINDOWS__ )
		ASSERT( m_Handle != INVALID_HANDLE_VALUE );
		CloseHandle( m_Handle );
		m_Handle = INVALID_HANDLE_VALUE;
    #elif defined( __LINUX__ ) || defined( __APPLE__ )
        ASSERT( m_Handle != -1 );
        VERIFY( flock( m_Handle, LOCK_UN ) == 0 );
        m_Handle = -1;
    #else
        #error Unknown platform
    #endif
}

//------------------------------------------------------------------------------
