// TestBuildAndLinkLibrary.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "FBuildTest.h"

#include "../../FBuildCore/FBuild.h"

#include "../../../../Core/FileIO/FileIO.h"
#include "../../../../Core/Process/Process.h"
#include "../../../../Core/Strings/AStackString.h"

// TestBuildAndLinkLibrary
//------------------------------------------------------------------------------
class TestDLL : public FBuildTest
{
private:
	DECLARE_TESTS

	void TestSingleDLL() const;
	void TestSingleDLL_NoRebuild() const;
	void TestTwoDLLs() const;
	void TestTwoDLLs_NoRebuild() const;
	void TestTwoDLLs_NoUnnecessaryRelink() const;
	void TestDLLWithPCH() const;
	void TestDLLWithPCH_NoRebuild() const;
	void TestExeWithDLL() const;
	void TestExeWithDLL_NoRebuild() const;
	void TestValidExeWithDLL() const;

	void TestLinkWithCopy() const;

	const char * GetSingleDLLDBFileName() const { return "../../../../ftmp/Test/DLL/singledll.fdb"; }
	const char * GetTwoDLLsDBFileName() const { return "../../../../ftmp/Test/DLL/twodlls.fdb"; }
	const char * GetDLLWithPCHDBFileName() const { return "../../../../ftmp/Test/DLL/dllwithpch.fdb"; }
	const char * GetExeWithDLLDBFileName() const { return "../../../../ftmp/Test/DLL/dllwithexe.fdb"; }
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN( TestDLL )
	REGISTER_TEST( TestSingleDLL )
	REGISTER_TEST( TestSingleDLL_NoRebuild )
	REGISTER_TEST( TestTwoDLLs )
	REGISTER_TEST( TestTwoDLLs_NoRebuild )
	REGISTER_TEST( TestTwoDLLs_NoUnnecessaryRelink )
	REGISTER_TEST( TestDLLWithPCH )
	REGISTER_TEST( TestDLLWithPCH_NoRebuild )
	REGISTER_TEST( TestExeWithDLL )
	REGISTER_TEST( TestExeWithDLL_NoRebuild )
	REGISTER_TEST( TestValidExeWithDLL )
	REGISTER_TEST( TestLinkWithCopy )
REGISTER_TESTS_END

// TestSingleDLL
//------------------------------------------------------------------------------
void TestDLL::TestSingleDLL() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize() );

	const AStackString<> dll( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dll.dll" );

	// clean up anything left over from previous runs
	EnsureFileDoesNotExist( dll );

	TEST_ASSERT( fBuild.Build( dll ) );
	TEST_ASSERT( fBuild.SaveDependencyGraph( GetSingleDLLDBFileName() ) );	

	// make sure all output files are as expecter
	EnsureFileExists( dll );

	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 3,		1,		Node::FILE_NODE );	// cpp 
	CheckStatsNode ( 1,		1,		Node::COMPILER_NODE );
	CheckStatsNode ( 1,		1,		Node::OBJECT_NODE );
	CheckStatsNode ( 1,		1,		Node::LIBRARY_NODE );
	CheckStatsNode ( 1,		1,		Node::DLL_NODE );
	CheckStatsTotal( 7,		5 );
}

// TestSingleDLL_NoRebuild
//------------------------------------------------------------------------------
void TestDLL::TestSingleDLL_NoRebuild() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize( GetSingleDLLDBFileName() ) );

	const AStackString<> dll( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dll.dll" );

	TEST_ASSERT( fBuild.Build( dll ) );

	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 3,		3,		Node::FILE_NODE );	// cpp + 2 .h
	CheckStatsNode ( 1,		0,		Node::COMPILER_NODE );
	CheckStatsNode ( 1,		0,		Node::OBJECT_NODE );
	CheckStatsNode ( 1,		0,		Node::LIBRARY_NODE );
	CheckStatsNode ( 1,		0,		Node::DLL_NODE );
	CheckStatsTotal( 7,		3 );
}

// TestTwoDLLs
//------------------------------------------------------------------------------
void TestDLL::TestTwoDLLs() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize() );

	const AStackString<> dllA( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dlla.dll" );
	const AStackString<> dllB( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dllb.dll" );

	// clean up anything left over from previous runs
	EnsureFileDoesNotExist( dllA );
	EnsureFileDoesNotExist( dllB );

	// build dllB which depends on dllA
	TEST_ASSERT( fBuild.Build( dllB ) );
	TEST_ASSERT( fBuild.SaveDependencyGraph( GetTwoDLLsDBFileName() ) );

	// make sure all output files are as expecter
	EnsureFileExists( dllA );
	EnsureFileExists( dllB );

	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 5,		2,		Node::FILE_NODE );	// 2 cpp files
	CheckStatsNode ( 1,		1,		Node::COMPILER_NODE );
	CheckStatsNode ( 2,		2,		Node::OBJECT_NODE );
	CheckStatsNode ( 2,		2,		Node::LIBRARY_NODE );
	CheckStatsNode ( 2,		2,		Node::DLL_NODE );
	CheckStatsTotal( 12,	9 );
}

// TestTwoDLLs_NoRebuild
//------------------------------------------------------------------------------
void TestDLL::TestTwoDLLs_NoRebuild() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize( GetTwoDLLsDBFileName() ) );

	// build again
	const AStackString<> dllB( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dllb.dll" );
	TEST_ASSERT( fBuild.Build( dllB ) );

	// Check stats to be sure nothing was built
	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 5,		5,		Node::FILE_NODE );	// 2 cpp files + 3 .h
	CheckStatsNode ( 1,		0,		Node::COMPILER_NODE );
	CheckStatsNode ( 2,		0,		Node::OBJECT_NODE );
	CheckStatsNode ( 2,		0,		Node::LIBRARY_NODE );
	CheckStatsNode ( 2,		0,		Node::DLL_NODE );
	CheckStatsTotal( 12,	5 );
}

// TestTwoDLLs_NoUnnecessaryRelink
//------------------------------------------------------------------------------
void TestDLL::TestTwoDLLs_NoUnnecessaryRelink() const
{
	// 1) Force DLL A to build
	{
		FBuildOptions options;
		options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
		options.m_ShowSummary = true; // required to generate stats for node count checks
		FBuild fBuild( options );
		TEST_ASSERT( fBuild.Initialize( GetTwoDLLsDBFileName() ) );

		const AStackString<> dllA( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dlla.dll" );

		// delete DLL A to have it relink (and regen the import lib)
		EnsureFileDoesNotExist( dllA );
		TEST_ASSERT( fBuild.Build( dllA ) );
		TEST_ASSERT( fBuild.SaveDependencyGraph( GetTwoDLLsDBFileName() ) );

		// Check stats to be sure one dll was built
		//				 Seen,	Built,	Type
		CheckStatsNode ( 3,		3,		Node::FILE_NODE );	// 1 cpp files + 2 .h
		CheckStatsNode ( 1,		0,		Node::COMPILER_NODE );
		CheckStatsNode ( 1,		0,		Node::OBJECT_NODE );
		CheckStatsNode ( 1,		0,		Node::LIBRARY_NODE );
		CheckStatsNode ( 1,		1,		Node::DLL_NODE );
		CheckStatsTotal( 7,		4 );
	}

	// 2) Ensure DLL B does not relink
	{
		FBuildOptions options;
		options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
		options.m_ShowSummary = true; // required to generate stats for node count checks
		FBuild fBuild( options );
		TEST_ASSERT( fBuild.Initialize( GetTwoDLLsDBFileName() ) );

		// build again
		const AStackString<> dllB( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dllb.dll" );
		TEST_ASSERT( fBuild.Build( dllB ) );

		// Check stats to be sure nothing was built
		//				 Seen,	Built,	Type
		CheckStatsNode ( 5,		5,		Node::FILE_NODE );	// 2 cpp files + 3 .h
		CheckStatsNode ( 1,		0,		Node::COMPILER_NODE );
		CheckStatsNode ( 2,		0,		Node::OBJECT_NODE );
		CheckStatsNode ( 2,		0,		Node::LIBRARY_NODE );
		CheckStatsNode ( 2,		0,		Node::DLL_NODE );
		CheckStatsTotal( 12,	5 );
	}
}

// TestDLLWithPCH
//------------------------------------------------------------------------------
void TestDLL::TestDLLWithPCH() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize() );

	const AStackString<> dllPCH( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dllpch.dll" );

	// clean up anything left over from previous runs
	EnsureFileDoesNotExist( dllPCH );

	// build dllB which depends on dllA
	TEST_ASSERT( fBuild.Build( dllPCH ) );
	TEST_ASSERT( fBuild.SaveDependencyGraph( GetDLLWithPCHDBFileName() ) );

	// make sure all output files are as expecter
	EnsureFileExists( dllPCH );

	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 4,		2,		Node::FILE_NODE );	// 1 cpp + 1 pch cpp
	CheckStatsNode ( 1,		1,		Node::COMPILER_NODE );
	CheckStatsNode ( 2,		2,		Node::OBJECT_NODE );// obj + pch obj
	CheckStatsNode ( 1,		1,		Node::LIBRARY_NODE );
	CheckStatsNode ( 1,		1,		Node::DLL_NODE );
	CheckStatsTotal( 9,		7 );
}

// TestDLLWithPCH_NoRebuild
//------------------------------------------------------------------------------
void TestDLL::TestDLLWithPCH_NoRebuild() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize( GetDLLWithPCHDBFileName() ) );

	const AStackString<> dllPCH( "..\\..\\..\\..\\ftmp\\Test\\DLL\\dllpch.dll" );

	// build dllB which depends on dllA
	TEST_ASSERT( fBuild.Build( dllPCH ) );

	// Check we build what was expected
	//				 Seen,	Built,	Type
	CheckStatsNode ( 4,		4,		Node::FILE_NODE );	// 1 cpp + 1 pch cpp + 2 .h
	CheckStatsNode ( 1,		0,		Node::COMPILER_NODE );
	CheckStatsNode ( 2,		0,		Node::OBJECT_NODE );// obj + pch obj
	CheckStatsNode ( 1,		0,		Node::LIBRARY_NODE );
	CheckStatsNode ( 1,		0,		Node::DLL_NODE );
	CheckStatsTotal( 9,		4 );
}

// TestExeWithDLL
//------------------------------------------------------------------------------
void TestDLL::TestExeWithDLL() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize() );

	const AStackString<> exe( "..\\..\\..\\..\\ftmp\\Test\\DLL\\exe.exe" );

	// clean up anything left over from previous runs
	EnsureFileDoesNotExist( exe );

	// build executable with depends on DLLA
	TEST_ASSERT( fBuild.Build( exe ) );
	TEST_ASSERT( fBuild.SaveDependencyGraph( GetExeWithDLLDBFileName() ) );

	// make sure all output files are as expecter
	EnsureFileExists( exe );

	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 4,		2,		Node::FILE_NODE );		// 2 cpp
	CheckStatsNode ( 1,		1,		Node::COMPILER_NODE );
	CheckStatsNode ( 2,		2,		Node::OBJECT_NODE );	// exe.obj + a.obj
	CheckStatsNode ( 2,		2,		Node::LIBRARY_NODE );	// exe lib + dll lib
	CheckStatsNode ( 1,		1,		Node::DLL_NODE );
	CheckStatsNode ( 1,		1,		Node::EXE_NODE );
	CheckStatsTotal( 11,	9 );
}

// TestExeWithDLL_NoRebuild
//------------------------------------------------------------------------------
void TestDLL::TestExeWithDLL_NoRebuild() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize( GetExeWithDLLDBFileName() ) );

	const AStackString<> exe( "..\\..\\..\\..\\ftmp\\Test\\DLL\\exe.exe" );

	// build executable with depends on DLLA
	TEST_ASSERT( fBuild.Build( exe ) );

	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 4,		4,		Node::FILE_NODE );		// 2 cpp + a.h + precompiledheader.h
	CheckStatsNode ( 1,		0,		Node::COMPILER_NODE );
	CheckStatsNode ( 2,		0,		Node::OBJECT_NODE );	// exe.obj + a.obj
	CheckStatsNode ( 2,		0,		Node::LIBRARY_NODE );	// exe lib + dll lib
	CheckStatsNode ( 1,		0,		Node::DLL_NODE );
	CheckStatsNode ( 1,		0,		Node::EXE_NODE );
	CheckStatsTotal( 11,	4 );
}

// TestValidExeWithDLL
//------------------------------------------------------------------------------
void TestDLL::TestValidExeWithDLL() const
{
	const AStackString<> exe( "..\\..\\..\\..\\ftmp\\Test\\DLL\\exe.exe" );

	Process p;
	p.Spawn( exe.Get(), nullptr, nullptr, nullptr );
	int ret = p.WaitForExit();
	TEST_ASSERT( ret == 12345678 );
}

// TestLinkWithCopy
//------------------------------------------------------------------------------
void TestDLL::TestLinkWithCopy() const
{
	FBuildOptions options;
	options.m_ConfigFile = "Data/TestDLL/fbuild.bff";
	options.m_ShowSummary = true; // required to generate stats for node count checks
	options.m_ForceCleanBuild = true;
	FBuild fBuild( options );
	TEST_ASSERT( fBuild.Initialize() );

	//const AStackString<> exe( "..\\..\\..\\..\\ftmp\\Test\\DLL\\Copy\\exe.exe" );

	// build executable with depends on DLLA
	TEST_ASSERT( fBuild.Build( AStackString<>( "DllBUsingCopy" ) ) );

	// Check stats
	//				 Seen,	Built,	Type
	CheckStatsNode ( 2,		2,		Node::COPY_NODE );
	CheckStatsNode ( 5,		2,		Node::FILE_NODE );
	CheckStatsNode ( 1,		1,		Node::COMPILER_NODE );
	CheckStatsNode ( 2,		2,		Node::OBJECT_NODE );
	CheckStatsNode ( 2,		2,		Node::LIBRARY_NODE );
	CheckStatsNode ( 2,		2,		Node::DLL_NODE );
	CheckStatsNode ( 1,		1,		Node::ALIAS_NODE );
	CheckStatsTotal( 15,	12 );
}

//------------------------------------------------------------------------------
