// Ray3
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_MATH_RAY3_H
#define CORE_MATH_RAY3_H

// Includes
//------------------------------------------------------------------------------
#include "Vec3.h"

// Ray3
//------------------------------------------------------------------------------
class Ray3
{
public:
	inline explicit Ray3( const Vec3 & origin, const Vec3 & dir )
		: m_Origin( origin )
		, m_Direction( dir )
	{}
	inline Ray3() {}
	inline ~Ray3() {}

	inline const Vec3 & GetOrigin() const { return m_Origin; }
	inline const Vec3 & GetDir() const { return m_Direction; }

private:
	Vec3 m_Origin;
	Vec3 m_Direction;
};

//------------------------------------------------------------------------------
#endif // CORE_MATH_RAY3_H
