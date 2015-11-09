// Network
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_NETWORK_NETWORK_H
#define CORE_NETWORK_NETWORK_H

// Includes
//------------------------------------------------------------------------------
#include "../Strings/AStackString.h"
#include "../Process/Mutex.h"
#include "../Process/Thread.h"

// Forward Declarations
//------------------------------------------------------------------------------
class AString;

// Network
//------------------------------------------------------------------------------
class Network
{
public:	
	static void GetHostName( AString & hostName );

	static uint32_t GetHostIPFromName( const AString & hostName, uint32_t timeoutMS = 1000 );

private:
	static uint32_t NameResolutionThreadFunc( void * userData );

	struct NameResolutionData
	{
		AStackString<>	hostName;
		bool			safeToFree;
	};
	static Mutex s_GetHostByNameMutex;
};

//------------------------------------------------------------------------------
#endif // CORE_NETWORK_NETWORK_H
