// NetworkStartupHelper
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_NETWORK_NETWORKSTARTUPHELPER_H
#define CORE_NETWORK_NETWORKSTARTUPHELPER_H

// Includes
//------------------------------------------------------------------------------
#if defined( __WINDOWS__ )
    #include <winsock2.h>
#endif

#include "../Env/Types.h"
#include "../Process/Mutex.h"

// NetworkStartup
//------------------------------------------------------------------------------
class NetworkStartupHelper
{
public:
    // ensure the network is up around the scope of this object
	NetworkStartupHelper();
    ~NetworkStartupHelper();

private:
#if defined( __WINDOWS__ )
	static WSADATA s_WSAData;
#endif
	static Mutex s_Mutex;
    static uint32_t s_RefCount;
};

//------------------------------------------------------------------------------
#endif // CORE_NETWORK_NETWORKSTARTUPHELPER_H
