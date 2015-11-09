// FunctionCSAssembly
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "FunctionCSAssembly.h"
#include "../../FBuild.h"
#include "../../FLog.h"
#include "../BFFIterator.h"
#include "../BFFParser.h"
#include "../BFFStackFrame.h"
#include "../BFFVariable.h"
#include "../../Graph/NodeGraph.h"
#include "../../Graph/DirectoryListNode.h"
#include "../../Graph/CSNode.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
FunctionCSAssembly::FunctionCSAssembly()
: Function( "CSAssembly" )
{
}

// AcceptsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionCSAssembly::AcceptsHeader() const
{
	return true;
}

// Commit
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionCSAssembly::Commit( const BFFIterator & funcStartIter ) const
{
	// make sure all required variables are defined
	const BFFVariable * compiler;
	const BFFVariable * compilerOptions;
	const BFFVariable * compilerOutput;
	if ( !GetString( funcStartIter, compiler, ".Compiler", true ) ||
		 !GetString( funcStartIter, compilerOptions, ".CompilerOptions", true ) ||
		 !GetString( funcStartIter, compilerOutput, ".CompilerOutput", true ) )
	{
		return false;
	}

	NodeGraph & ng = FBuild::Get().GetDependencyGraph();

	Dependencies staticDeps( 32, true );

	// do we want to build a files in a directory?
	const BFFVariable * inputPath = BFFStackFrame::GetVar( ".CompilerInputPath" );
	if ( inputPath )
	{
		// get the optional pattern and recurse options related to InputPath
		Array< AString > patterns;
		if ( !GetStrings( funcStartIter, patterns, ".CompilerInputPattern", false ) )
		{
			return false; // GetString will have emitted an error
		}
		if ( patterns.IsEmpty() )
		{
			patterns.Append( AStackString<>( "*.cs" ) );
		}

		// recursive?  default to true
		bool recurse = true;
		if ( !GetBool( funcStartIter, recurse, ".CompilerInputPathRecurse", true, false ) )
		{
			return false; // GetBool will have emitted an error
		}

		// Support an exclusion path
		Array< AString > excludePaths;
		if ( !GetFolderPaths( funcStartIter, excludePaths, ".CompilerInputExcludePath", false ) )
		{
			return false; // GetFolderPaths will have emitted an error
		}

        Array< AString > filesToExclude(0, true);
        if ( !GetStrings( funcStartIter, filesToExclude, ".CompilerInputExcludedFiles", false ) ) // not required
        {
            return false; // GetStrings will have emitted an error
        }
	    CleanFileNames( filesToExclude );

		// Input paths
		Array< AString > inputPaths;
		if ( !GetFolderPaths( funcStartIter, inputPaths, ".CompilerInputPath", false ) )
		{
			return false; // GetFolderPaths will have emitted an error
		}

		Dependencies dirNodes( inputPaths.GetSize() );
		if ( !GetDirectoryListNodeList( funcStartIter, inputPaths, excludePaths, filesToExclude, recurse, &patterns, "CompilerInputPath", dirNodes ) )
		{
			return false; // GetDirectoryListNodeList will have emitted an error
		}
		staticDeps.Append( dirNodes );
	}

	// do we want to build a specific list of files?
	if ( !GetNodeList( funcStartIter, ".CompilerInputFiles", staticDeps, false ) )
	{
		// helper will emit error
		return false;
	}

	if ( staticDeps.IsEmpty() )
	{
		Error::Error_1006_NothingToBuild( funcStartIter, this );
		return false;
	}

	// additional references?
	Dependencies extraRefs( 0, true );
	if ( !GetNodeList( funcStartIter, ".CompilerReferences", extraRefs, false ) )
	{
		// helper function will have emitted an error
		return false;
	}

	// Create library node which depends on the single file or list
	if ( ng.FindNode( compilerOutput->GetString() ) )
	{
		Error::Error_1100_AlreadyDefined( funcStartIter, this, compilerOutput->GetString() );
		return false;
	}
	Node * csNode = ng.CreateCSNode( compilerOutput->GetString(),
									 staticDeps,
									 compiler->GetString(),
									 compilerOptions->GetString(),
									 extraRefs );

	// should we create an alias?
	return ProcessAlias( funcStartIter, csNode );
}

//------------------------------------------------------------------------------
