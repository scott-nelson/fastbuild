// VSProjectGenerator - Generate a Visual Studio project
//------------------------------------------------------------------------------
#pragma once
#ifndef FBUILD_HELPERS_VSPROJECTGENERATOR_H
#define FBUILD_HELPERS_VSPROJECTGENERATOR_H

// Includes
//------------------------------------------------------------------------------
#include "../../../../Core/Containers/Array.h"
#include "../../../../Core/Strings/AString.h"

// Forward Declarations
//------------------------------------------------------------------------------
class IOStream;

// VSProjectConfig
//-----------------------------------------------------------------------------
class VSProjectConfig
{
public:
	AString m_Platform;
	AString m_Config;

	AString m_BuildCommand;
	AString m_RebuildCommand;
	AString m_CleanCommand;
	AString m_Output;
	AString m_PreprocessorDefinitions;
	AString m_IncludeSearchPath;
	AString m_ForcedIncludes;
	AString m_AssemblySearchPath;
	AString m_ForcedUsingAssemblies;
	AString m_AdditionalOptions;
	AString m_OutputDirectory;
	AString m_IntermediateDirectory;
	AString m_LayoutDir;
	AString m_LayoutExtensionFilter;
	AString m_Xbox360DebuggerCommand;
	AString m_DebuggerFlavor;
	AString m_AumidOverride;
	AString m_PlatformToolset;
	AString m_DeploymentType;
	AString m_DeploymentFiles;

	AString m_LocalDebuggerCommandArguments;
	AString m_LocalDebuggerWorkingDirectory;
	AString m_LocalDebuggerCommand;
	AString m_LocalDebuggerEnvironment;

	static bool Load( IOStream & stream, Array< VSProjectConfig > & configs );
	static void Save( IOStream & stream, const Array< VSProjectConfig > & configs );
};

// VSProjectFileType
//------------------------------------------------------------------------------
class VSProjectFileType
{
public:
	AString m_FileType;	// e.g. "CppForm"
	AString m_Pattern;	// e.g. "Code\Forms\*.h" (can be full filename also)

	static bool Load( IOStream & stream, Array< VSProjectFileType > & fileTypes );
	static void Save( IOStream & stream, const Array< VSProjectFileType > & fileTypes );
};

// VSProjectGenerator
//------------------------------------------------------------------------------
class VSProjectGenerator
{
public:
	VSProjectGenerator();
	~VSProjectGenerator();

	void SetProjectName( const AString & name ) { m_ProjectName = name; }
	void SetBasePaths( const Array< AString > & paths );
	void SetAllowedFileExtensions( Array< AString > & extensions ) { m_AllowedFileExtensions = extensions; }

	void AddFile( const AString & file, bool filterByExtension = true );
	void AddFiles( const Array< AString > & files, bool filterByExtension = true );

	void SetRootNamespace( const AString & s )			{ m_RootNamespace = s; }
	void SetProjectGuid( const AString & s )			{ m_ProjectGuid = s; }
	void SetDefaultLanguage( const AString & s )		{ m_DefaultLanguage = s; }
	void SetApplicationEnvironment( const AString & s ) { m_ApplicationEnvironment = s; }
	void SetReferences( const Array< AString > & a )	{ m_References = a; }
	void SetProjectReferences( const Array< AString > & a )	{ m_ProjectReferences = a; }

	const AString & GenerateVCXProj( const AString & projectFile,
									 const Array< VSProjectConfig > & configs,
									 const Array< VSProjectFileType > & fileTypes );
	const AString & GenerateVCXProjFilters( const AString & projectFile );

	static void FormatDeterministicProjectGUID( AString & guid, const AString & projectName );

private:
	// Helper to format some text
	void Write( const char * fmtString, ... );

	// Helpers to format some xml
	void WritePGItem( const char * xmlTag, const AString & value );

	void GetFolderPath( const AString & fileName, AString & folder ) const;

	// project details
	AString m_ProjectName;
	Array< AString > m_BasePaths;
	Array< AString > m_AllowedFileExtensions;

	// Globals
	AString m_RootNamespace;
	AString m_ProjectGuid;
	AString m_DefaultLanguage;
	AString m_ApplicationEnvironment;
	Array< AString > m_References;
	Array< AString > m_ProjectReferences;

	// intermediate data
	Array< AString > m_Files;

	// working buffer
	AString m_Tmp;

	// final output
	AString m_OutputVCXProj;
	AString m_OutputVCXProjFilters;
};

//------------------------------------------------------------------------------
#endif // FBUILD_HELPERS_VSPROJECTGENERATOR_H
