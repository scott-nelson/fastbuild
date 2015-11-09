// NetworkStartupHelper
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "NetworkStartupHelper.h"

#include "../Env/Assert.h"

//------------------------------------------------------------------------------
// Static Data
//------------------------------------------------------------------------------
#if defined( __WINDOWS__ )
    /*static*/ WSADATA NetworkStartupHelper::s_WSAData;
#endif
/*static*/ Mutex NetworkStartupHelper::s_Mutex;
/*static*/ uint32_t NetworkStartupHelper::s_RefCount( 0 );

// CONSTRUCTOR
//------------------------------------------------------------------------------
NetworkStartupHelper::NetworkStartupHelper()
{
	MutexHolder mh( s_Mutex );

    s_RefCount++;
    if ( s_RefCount > 1 )
    {
        return; // already previously started
    }

    // start up
    #if defined( __WINDOWS__ )
        VERIFY( WSAStartup( MAKEWORD( 2, 2 ), &s_WSAData ) == 0 );
    #endif
}

// DESTRUCTOR
//------------------------------------------------------------------------------
NetworkStartupHelper::~NetworkStartupHelper()
{
	MutexHolder mh( s_Mutex );

    ASSERT( s_RefCount > 0 );
    s_RefCount--;
    if ( s_RefCount > 0 )
    {
        return; // still in use
    }

    // clean up
    #if defined( __WINDOWS__ )
        WSACleanup();
    #endif
}

//------------------------------------------------------------------------------
