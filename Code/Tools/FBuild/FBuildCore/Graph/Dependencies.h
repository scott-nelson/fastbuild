// Dependencies.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FBUILD_GRAPH_DEPENDENCY_H
#define FBUILD_GRAPH_DEPENDENCY_H

// Includes
//------------------------------------------------------------------------------
#include "../../../../Core/Containers/Array.h"

// Forward Declarations
//------------------------------------------------------------------------------
class IOStream;
class Node;

// Dependency
//------------------------------------------------------------------------------
class Dependency
{
public:
	explicit Dependency( Node * node )
		: m_Node( node )
		, m_IsWeak( false )
	{}
	explicit Dependency( Node * node, bool isWeak )
		: m_Node( node )
		, m_IsWeak( isWeak )
	{}

	inline Node * GetNode() const { return m_Node; }
	inline bool IsWeak() const { return m_IsWeak; }

private:
	Node * m_Node;	// Node being depended on
	bool m_IsWeak;	// Is node used for build ordering, but not triggering a rebuild
};

//  Dependencies
//------------------------------------------------------------------------------
class Dependencies : public Array< Dependency >
{
public:
	explicit inline Dependencies()
		: Array< Dependency >()
	{}
	explicit inline Dependencies( size_t initialCapacity, bool resizeable = false )
		: Array< Dependency >( initialCapacity, resizeable )
	{}
	explicit inline Dependencies( Dependency * begin, Dependency * end )
		: Array< Dependency >( begin, end )
	{}

	//void operator = ( const Array< Node * > & nodes );

	void Save( IOStream & stream ) const;
	bool Load( IOStream & stream );
};

//------------------------------------------------------------------------------
#endif // FBUILD_GRAPH_DEPENDENCY_H
