// Main
//------------------------------------------------------------------------------
#pragma once
#ifndef FBUILD_FBUILDWORKER_FBUILDWORKEROPTIONS_H
#define FBUILD_FBUILDWORKER_FBUILDWORKEROPTIONS_H

// Includes
//------------------------------------------------------------------------------
// FBuild
#include "Worker/WorkerSettings.h"

// Core
#include "../../../Core/Env/Types.h"

// Forward Declaration
//------------------------------------------------------------------------------
class AString;

// FBuildWorkerOptions
//------------------------------------------------------------------------------
class FBuildWorkerOptions
{
public:
	FBuildWorkerOptions();

	bool ProcessCommandLine( const AString & commandLine );

	// sub-process control
	bool m_IsSubprocess;	// Process is child? (Internal)
	bool m_UseSubprocess;	// Should we launch a sub-process?

	// resource usage
	bool m_OverrideCPUAllocation;
	uint32_t m_CPUAllocation;
	bool m_OverrideWorkMode;
	WorkerSettings::Mode m_WorkMode;

private:
	void ShowUsageError();
};

//------------------------------------------------------------------------------
#endif // FBUILD_FBUILDWORKER_FBUILDWORKEROPTIONS_H
