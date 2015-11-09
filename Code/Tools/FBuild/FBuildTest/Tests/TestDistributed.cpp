// TestDistributed.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "../../../../TestFramework/UnitTest.h"

#include "../../FBuildCore/FBuild.h"
#include "../../FBuildCore/Protocol/Protocol.h"
#include "../../FBuildCore/Protocol/Server.h"
#include "../../FBuildCore/WorkerPool/JobQueueRemote.h"

#include "../../../../Core/FileIO/FileIO.h"
#include "../../../../Core/Strings/AStackString.h"

// TestDistributed
//------------------------------------------------------------------------------
class TestDistributed : public UnitTest
{
private:
	DECLARE_TESTS

	void TestWith1RemoteWorkerThread() const;
	void TestWith4RemoteWorkerThreads()	const;
	void WithPCH() const;
	void RegressionTest_RemoteCrashOnErrorFormatting();
	void TestLocalRace();
	void TestForceInclude() const;
	void TestZiDebugFormat() const;
	void TestZiDebugFormat_Local() const;
	void TestHelper( const char * target,
					 uint32_t numRemoteWorkers,
					 bool shouldFail = false,
					 bool allowRace = false ) const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN( TestDistributed )
	REGISTER_TEST( TestWith1RemoteWorkerThread )
	REGISTER_TEST( TestWith4RemoteWorkerThreads )
	REGISTER_TEST( WithPCH )
	REGISTER_TEST( RegressionTest_RemoteCrashOnErrorFormatting )
	REGISTER_TEST( TestLocalRace )
	REGISTER_TEST( TestForceInclude )
	REGISTER_TEST( TestZiDebugFormat )
	REGISTER_TEST( TestZiDebugFormat_Local )
REGISTER_TESTS_END

// Test
//------------------------------------------------------------------------------
void TestDistributed::TestHelper( const char * target, uint32_t numRemoteWorkers, bool shouldFail, bool allowRace ) const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDistributed/fbuild.bff";
	options.m_AllowDistributed = true;
	options.m_NumWorkerThreads = 0;
	options.m_NoLocalConsumptionOfRemoteJobs = true; // ensure all jobs happen on the remote worker
	options.m_AllowLocalRace = allowRace;
	//options.m_ShowProgress = true;
	//options.m_ShowInfo = true;
	//options.m_ShowSummary = true;
	FBuild fBuild( options );

	JobQueueRemote jqr( numRemoteWorkers );

	// start a client to emulate the other end
	Server s;
	s.Listen( Protocol::PROTOCOL_PORT );

	TEST_ASSERT( fBuild.Initialize() );

	// clean up anything left over from previous runs
	Array< AString > files;
	FileIO::GetFiles( AStackString<>( "..\\..\\..\\..\\ftmp\\Test\\Distributed" ), AStackString<>( "*.*" ), true, &files );
	const AString * iter = files.Begin();
	const AString * const end = files.End();
	for ( ; iter != end; ++iter )
	{
		FileIO::FileDelete( iter->Get() );
	}

	if ( !shouldFail )
	{
		TEST_ASSERT( FileIO::FileExists( target ) == false );
	}

	bool pass = fBuild.Build( AStackString<>( target ) );
	if ( !shouldFail )
	{
		TEST_ASSERT( pass );
	}

	// make sure all output files are as expected
	if ( !shouldFail )
	{
		TEST_ASSERT( FileIO::FileExists( target ) );
	}
}

// TestWith1RemoteWorkerThread
//------------------------------------------------------------------------------
void TestDistributed::TestWith1RemoteWorkerThread() const
{
	const char * target( "..\\..\\..\\..\\ftmp\\Test\\Distributed\\dist.lib" );
	TestHelper( target, 1 );
}

// TestWith4RemoteWorkerThreads
//------------------------------------------------------------------------------
void TestDistributed::TestWith4RemoteWorkerThreads() const
{
	const char * target( "..\\..\\..\\..\\ftmp\\Test\\Distributed\\dist.lib" );
	TestHelper( target, 4 );
}

// WithPCH
//------------------------------------------------------------------------------
void TestDistributed::WithPCH() const
{
	const char * target( "..\\..\\..\\..\\ftmp\\Test\\Distributed\\distpch.lib" );
	TestHelper( target, 4 );
}

// RegressionTest_RemoteCrashOnErrorFormatting
//------------------------------------------------------------------------------
void TestDistributed::RegressionTest_RemoteCrashOnErrorFormatting()
{
	const char * target( "badcode" );
	TestHelper( target, 4, true ); // compilation should fail
}

//------------------------------------------------------------------------------
void TestDistributed::TestLocalRace()
{
	{
		const char * target( "..\\..\\..\\..\\ftmp\\Test\\Distributed\\dist.lib" );
		TestHelper( target, 1, false, true ); // allow race
	}
	{
		const char * target( "..\\..\\..\\..\\ftmp\\Test\\Distributed\\dist.lib" );
		TestHelper( target, 4, false, true ); // allow race
	}
	{
		const char * target( "..\\..\\..\\..\\ftmp\\Test\\Distributed\\distpch.lib" );
		TestHelper( target, 4, false, true ); // allow race
	}
	{
		const char * target( "badcode" );
		TestHelper( target, 4, true, true ); // compilation should fail, allow race
	}
}

// TestForceInclude
//------------------------------------------------------------------------------
void TestDistributed::TestForceInclude() const
{
	const char * target( "..\\..\\..\\..\\ftmp\\Test\\Distributed\\ForceInclude\\ForceInclude.lib" );
	TestHelper( target, 4 );
}

// TestZiDebugFormat
//------------------------------------------------------------------------------
void TestDistributed::TestZiDebugFormat() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDistributed/fbuild.bff";
	options.m_AllowDistributed = true;
	options.m_NumWorkerThreads = 0;
	options.m_NoLocalConsumptionOfRemoteJobs = true; // ensure all jobs happen on the remote worker
	options.m_AllowLocalRace = false;
	options.m_ForceCleanBuild = true;
	FBuild fBuild( options );

	JobQueueRemote jqr( 1 );

	// start a client to emulate the other end
	Server s;
	s.Listen( Protocol::PROTOCOL_PORT );

	TEST_ASSERT( fBuild.Initialize() );

	TEST_ASSERT( fBuild.Build( AStackString<>( "remoteZi" ) ) );
}

// TestZiDebugFormat_Local
//------------------------------------------------------------------------------
void TestDistributed::TestZiDebugFormat_Local() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDistributed/fbuild.bff";
	options.m_AllowDistributed = true;
	options.m_ForceCleanBuild = true;
	FBuild fBuild( options );

	JobQueueRemote jqr( 1 );

	// start a client to emulate the other end
	Server s;
	s.Listen( Protocol::PROTOCOL_PORT );

	TEST_ASSERT( fBuild.Initialize() );

	TEST_ASSERT( fBuild.Build( AStackString<>( "remoteZi" ) ) );
}

//------------------------------------------------------------------------------
