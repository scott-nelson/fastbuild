// Sort.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_CONTAINERS_SORT_H
#define CORE_CONTAINERS_SORT_H

// Includes
//------------------------------------------------------------------------------
#include "../Env/Types.h"

// AscendingCompare
//------------------------------------------------------------------------------
class AscendingCompare
{
public:
	template < class T >
	inline bool operator () ( const T & a, const T & b ) const
	{
		return ( a < b );
	}
};

// AscendingCompareDeref
//------------------------------------------------------------------------------
class AscendingCompareDeref
{
public:
	template < class T >
	inline bool operator () ( const T & a, const T & b ) const
	{
		return ( ( *a ) < ( *b ) );
	}
};

// ShellSort
//------------------------------------------------------------------------------
template < class T, class COMPARE >
void ShellSort( T * begin, T * end, const COMPARE & compare )
{
	size_t numItems = end - begin;
	size_t increment = 3;
	while ( increment > 0 )
	{
		for ( size_t i=0; i < numItems; i++ )
		{
			size_t j = i;
			T temp( begin[ i ] );
			while ( ( j >= increment ) && ( compare( temp, begin[ j - increment ]  ) ) )
			{
				begin[ j ] = begin[ j - increment ];
				j = j - increment;
			}
			begin[ j ] = temp;
		}
		if ( increment / 2 != 0 )
		{
			increment = increment / 2 ;
		}
		else if ( increment == 1 )
		{
			increment = 0;
		}
		else
		{
			increment = 1;
		}
	}    
}

//------------------------------------------------------------------------------
#endif // CORE_CONTAINERS_SORT_H
