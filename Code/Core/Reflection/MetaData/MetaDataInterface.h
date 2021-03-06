// MetaDaatInterface.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_REFLECTION_METADATAINTERFACE_H
#define CORE_REFLECTION_METADATAINTERFACE_H

// Includes
//------------------------------------------------------------------------------
#include "../ReflectionMacros.h"
#include "../RefObject.h"

// IMetaData
//------------------------------------------------------------------------------
class IMetaData : public RefObject
{
    REFLECT_DECLARE( IMetaData )
public:
    explicit IMetaData();
    virtual ~IMetaData();

    const IMetaData* GetNext() const { return m_Next; }
protected:
    friend IMetaData & operator + ( IMetaData & a, IMetaData & b );

    IMetaData * m_Next;
};

//------------------------------------------------------------------------------
#endif // CORE_REFLECTION_METADATAINTERFACE_H
