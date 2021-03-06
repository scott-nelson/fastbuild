// RefObject.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "RefObject.h"
#include "ReflectionInfo.h"

// Reflection
//------------------------------------------------------------------------------
// RefObject_ReflectionInfo
//------------------------------------------------------------------------------
class RefObject_ReflectionInfo : public ReflectionInfo
{
public:
	explicit RefObject_ReflectionInfo() { SetTypeName( "RefObject" ); m_IsAbstract = true; }
	virtual ~RefObject_ReflectionInfo() {}
};
RefObject_ReflectionInfo g_RefObject_ReflectionInfo;
void RefObject_ReflectionInfo_Bind()
{
	ReflectionInfo::BindReflection( g_RefObject_ReflectionInfo );
}

// DynamicCastHelper
//------------------------------------------------------------------------------
/*static*/ bool RefObject::CanDynamicCast( const ReflectionInfo * dst, const ReflectionInfo * src )
{
	while ( src )
	{
		if ( src == dst )
		{
			return true;
		}
		src = src->GetSuperClass();
	}
	return false;
}

//------------------------------------------------------------------------------
