// Dependencies.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
// FBuild
#include "Dependencies.h"

#include "../FBuild.h"
#include "Node.h"
#include "NodeGraph.h"

// Core
#include "../../../../Core/FileIO/IOStream.h"


// operator =
/*//------------------------------------------------------------------------------
void Dependencies::operator = ( const Array< Node * > & nodes )
{
	Clear();
	SetCapacity( nodes.GetSize() );
	auto end = nodes.End();
	for ( auto it = nodes.Begin(); it != end; ++it )
	{
		Node * node = *it;
		Append( Dependency( node ) );
	}
}*/

// Save
//------------------------------------------------------------------------------
void Dependencies::Save( IOStream & stream ) const
{
	size_t numDeps = GetSize();
	stream.Write( (uint32_t)numDeps );

	Iter end = End();
	for ( Iter it = Begin(); it != end; ++it )
	{
		const Dependency & dep = *it;

		// Nodes are saved by index to simplify deserialization
		uint32_t index = dep.GetNode()->GetIndex();
		stream.Write( index );

		// Save weak flag
		bool isWeak = dep.IsWeak();
		stream.Write( isWeak );
	}
}

// Load
//------------------------------------------------------------------------------
bool Dependencies::Load( IOStream & stream )
{
	uint32_t numDeps;
	if ( stream.Read( numDeps ) == false )
	{
		return false;
	}
	if ( GetCapacity() < GetSize() + numDeps )
	{
		SetCapacity( GetSize() + numDeps );
	}
	NodeGraph & ng = FBuild::Get().GetDependencyGraph();
	for ( uint32_t i=0; i<numDeps; ++i )
	{
		// Read node index
		uint32_t index( INVALID_NODE_INDEX );
		if ( stream.Read( index ) == false )
		{
			return false;
		}

		// Convert to Node *
		Node * node = ng.GetNodeByIndex( index );
		ASSERT( node );

		// Read weak flag
		bool isWeak( false );
		if ( stream.Read( isWeak ) == false )
		{
			return false;
		}

		// Recombine dependency info
		Append( Dependency( node, isWeak ) );
	}
	return true;
}
//------------------------------------------------------------------------------
