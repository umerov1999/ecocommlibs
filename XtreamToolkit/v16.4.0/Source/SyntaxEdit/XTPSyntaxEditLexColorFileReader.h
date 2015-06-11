// XTPSyntaxEditLexColorFileReader.h: interface for the CXTLexColorFileReader class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITLEXCOLORFILEREADER_H__)
#define __XTPSYNTAXEDITLEXCOLORFILEREADER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPSyntaxEditColorTheme;
class CXTPNotifyConnection;
class CXTPNotifyConnectionMT;
struct XTP_EDIT_SCHEMAFILEINFO;
class CXTPSyntaxEditSchemaFileInfoList;

//-----------------------------------------------------------------------
// Summary:
//      This enumeration describes allowable configuration managers flags.
//-----------------------------------------------------------------------
enum XTPSyntaxEditCfgFlags
{
	xtpEditCfgFileAdd       = 0x0001, // file should be added
	xtpEditCfgFileRemove    = 0x0002, // file should be removed
};

//===========================================================================
// Summary:
//      This storage class is designed to contain information from the color
//      settings file section. It helps easy retrieve color information.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorInfo
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//      Object constructor.
	// Parameters:
	//      pTheme : [in] Pointer to CXTPSyntaxEditColorTheme object (theme
	//                      this color information belongs to)
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorInfo(CXTPSyntaxEditColorTheme* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//      Object constructor.
	// Parameters:
	//      strClassName : [in] Lex Class name
	//      pTheme : [in] Pointer to CXTPSyntaxEditColorTheme object (theme
	//                      this color information belongs to)
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorInfo(const CString& strClassName, CXTPSyntaxEditColorTheme* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//      Destroys a CXTPSyntaxEditColorTheme() object, handles cleanup and
	//      de-allocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorInfo ();

	//-----------------------------------------------------------------------
	// Summary:
	//      Adds parameter.
	// Parameters:
	//      strName     : [in] Parameter name.
	//      strValue    : [in] Parameter value.
	//-----------------------------------------------------------------------
	void AddParam(const CString& strName, const CString& strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns string of parameter value.
	// Parameters:
	//      strName     : [in] Parameter name.
	//      bDynamic    : [in] If TRUE - search parameter value in parent
	//                         themes (dynamically), otherwise it is searched
	//                         only in the current theme.
	// Returns:
	//      CString object with parameter value.
	//-----------------------------------------------------------------------
	const CString GetParam(const CString& strName, BOOL bDynamic = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns hexadecimal value.
	// Parameters:
	//      strName     : [in] Parameter name.
	//      bDynamic    : [in] If TRUE - search parameter value in parent
	//                         themes (dynamically), otherwise it is searched
	//                         only in the current theme.
	// Returns:
	//      DWORD parameter value.
	//-----------------------------------------------------------------------
	DWORD GetHexParam(const CString& strName, BOOL bDynamic = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns firs parameter position.
	// Returns:
	//      POSITION value.
	//-----------------------------------------------------------------------
	POSITION GetFirstParamNamePosition();

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns next parameter position.
	// Parameters:
	//      pos : [in] position.
	// Returns:
	//      POSITION value.
	//-----------------------------------------------------------------------
	const CString GetNextParamName(POSITION& pos);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this function to get lex class name.
	// Returns:
	//      A lex class name
	//-----------------------------------------------------------------------
	const CString GetClassName() const;

private:
	CString m_strClassName;     // lex class name
	CMapStringToString m_mapParams; // name/value parameters map
	CXTPSyntaxEditColorTheme* m_pTheme; // pointer to the theme this color information belongs to
};

//===========================================================================
// Summary:
//      This collection class hold the color info objects.
//==========================================================================
class CXTPSyntaxEditColorInfoArray : public
			CArray<CXTPSyntaxEditColorInfo*, CXTPSyntaxEditColorInfo*>
{
	typedef CArray<CXTPSyntaxEditColorInfo*, CXTPSyntaxEditColorInfo*> TBase;
public:
	//===========================================================================
	// Summary:
	//      Constructor, create and initializes the object.
	//==========================================================================
	CXTPSyntaxEditColorInfoArray(){};

	//===========================================================================
	// Summary:
	//      Destructor, do the cleaups and destroys the object.
	//==========================================================================
	virtual ~CXTPSyntaxEditColorInfoArray()
	{
		RemoveAll();
	};

	//===========================================================================
	// Summary:
	//      Call this function to remove all the color info objects.
	//==========================================================================
	void RemoveAll()
	{
		for(int i = 0; i < GetSize(); i++)
		{
			if(GetAt(i))
			{
				delete GetAt(i);
			}
		}
		TBase::RemoveAll();
	}
};

class CXTPSyntaxEditColorThemesManager;
class CXTPSyntaxEditFileChangesMonitor;
class CXTPSyntaxEditPropertiesTheme;

//===========================================================================
// Summary:
//      This class provides functionality to read configuration information
//      from configuration files with predefined structure. Its information
//      describes color settings for corresponding lexical items for given
//      language.
//      You construct instance of CXTPSyntaxEditColorTheme by calling
//      constructor. Then you should call Load() member function
//      to read data from persistent storage (file for this implementation),
//      CXTPSyntaxEditLexColorInfo structures.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorTheme
{
	friend class CXTPSyntaxEditPropertiesTheme;

	//===========================================================================
	// Summary:
	//      Determines custom array type to store CXTPSyntaxEditColorInfo structures.
	// See also:
	//      CXTPSyntaxEditColorInfo.
	//===========================================================================
	typedef CMap<CString, LPCTSTR, CXTPSyntaxEditColorInfo *, CXTPSyntaxEditColorInfo *> CXTPSyntaxEditMapLexColorInfo;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	// Parameters:
	//      pThemesManager : [in] Pointer to the parent themes manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorTheme(CXTPSyntaxEditColorThemesManager* pThemesManager);

	//-----------------------------------------------------------------------
	// Summary:
	//      Destroys a CXTPSyntaxEditColorTheme() object, handles cleanup and
	//      de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//      Parses associated color schema file and fills internal color info
	//      structures.
	// Parameters:
	//      csFileName : [in] configuration file name string.
	//-----------------------------------------------------------------------
	virtual void Load(const CString& csFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//      used in non-file mode config to Load Color Sheme from string
	// Parameters:
	//      arPassedString : [in] CStringArray of Color Scheme attributets.
	//-----------------------------------------------------------------------
	virtual void LoadFromString(CStringArray& arPassedString);
	//-----------------------------------------------------------------------

	// Summary:
	//      Returns name of the parent schema.
	// Returns:
	//      CString containing name of the parent schema.
	//-----------------------------------------------------------------------
	const CString GetParentThemeName();

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns parent schema.
	// Returns:
	//      Pointer to CXTPSyntaxEditColorTheme object if exists;
	//      NULL lf parent schema doesn't exist.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorTheme* GetParentTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns color information.
	// Parameters:
	//      strLexClass         : [in] Lex Class name string
	//      strThemeFilename    : [in] configuration file name string.
	//      bDynamic            : [in] If TRUE - search color info in parent
	//                                 themes (dynamically), otherwise it is
	//                                 searched only in this theme.
	// Returns:
	//      Pointer to CXTPSyntaxEditColorInfo object.
	// Remarks:
	//      strThemeFilename is required to avoid calling recursion.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorInfo* GetColorInfo(const CString& strLexClass,
											const CString& strThemeFilename,
											BOOL bDynamic = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns configuration file name.
	// Returns:
	//      CString containing file name.
	//-----------------------------------------------------------------------
	CString GetFileName();

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this method to save colors to configuration file.
	// Parameters:
	//      strParentThemeName  - The parent theme name.
	//      pColorInfoArray_new - Colors information array.
	// Returns:
	//      TRUE if successful, FALSE otherwise.
	// See Also: GetFileName
	//-----------------------------------------------------------------------
	BOOL WriteCfgFile(CString strParentThemeName,
					  CXTPSyntaxEditColorInfoArray* pColorInfoArray_new);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Cleans up collection.
	//-----------------------------------------------------------------------
	void Cleanup();

	//-----------------------------------------------------------------------
	// Summary:
	//      Parses a single section and creates CXTPSyntaxEditColorInfo
	//      object from it.
	// Parameters:
	//      csSection : [in] section name (in configuration file) string.
	// Returns:
	//      Pointer to CXTPSyntaxEditColorInfo with resulting colors set.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorInfo* ParseSection(const CString& csSection);

	//-----------------------------------------------------------------------
	// Summary:
	//      used for non-file mode config to parses a single section
	//      and creates CXTPSyntaxEditColorInfo object from it.
	// Parameters:
	//      csSection : [in] section name (in configuration file) string.
	//      arPassedString : [in] CStringArray of passed string
	// Returns:
	//      Pointer to CXTPSyntaxEditColorInfo with resulting colors set.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorInfo* ParseSectionFromString(const CString& csSection, CStringArray& arPassedString);

private:
	CStringList m_arSections;   // All section names from the theme config file
	CString m_csFileName;       // filename of the corresponding theme config file
	CXTPSyntaxEditMapLexColorInfo m_mapLexColorInfo;    // internal map - lex class name to color info
	CXTPSyntaxEditColorThemesManager* m_pThemesManager; // pointer to the parent themes manager
};

AFX_INLINE CString CXTPSyntaxEditColorTheme::GetFileName()
{
	return m_csFileName;
}

//===========================================================================
// Summary: This class provides functionality to store set of all color themes
//          that would be written from configuration files. For short it is
//          custom collection class.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorThemesManager
{
	typedef CMap<CString, LPCTSTR, CXTPSyntaxEditColorTheme*, CXTPSyntaxEditColorTheme*> CXTPSyntaxEditMapThemes;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorThemesManager();
	//-----------------------------------------------------------------------
	// Summary:
	//      Destroys a CXTPSyntaxEditColorThemesManager() object,
	//      handles cleanup and de-allocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorThemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Loads theme by given theme name and configuration file.
	// Parameters:
	//      strThemeName    : [in] theme name string.
	//      strThemeFilename: [in] configuration file name.
	//-----------------------------------------------------------------------
	void LoadTheme(const CString& strThemeName, const CString& strThemeFilename);

	//-----------------------------------------------------------------------
	// Summary:
	//      Loads theme by given theme name and used in non-file mode config
	// Parameters:
	//      strThemeName   : [in] theme name string.
	//      arPassedString : [in] array of attributes strings.
	//-----------------------------------------------------------------------
	void LoadThemeFromString(const CString& strThemeName, CStringArray& arPassedString);
	//-----------------------------------------------------------------------
	// Summary:
	//      Adds new entity to "theme name - configuration file" map.
	// Parameters:
	//      strThemeName    : [in] theme name string.
	//      strThemeFilename: [in] configuration file name.
	//-----------------------------------------------------------------------
	void AddThemeInfo(const CString& strThemeName, const CString& strThemeFilename);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns color theme by given name.
	// Parameters:
	//      strThemeName    : [in] theme name string.
	// Returns:
	//      Pointer to CXTPSyntaxEditColorTheme.
	// See also:
	//      class CXTPSyntaxEditColorTheme
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorTheme* GetTheme(const CString& strThemeName);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns CStringArray with themes names.
	// Returns:
	//      CStringArray with themes names.
	//-----------------------------------------------------------------------
	CStringArray& GetThemes();

	//-----------------------------------------------------------------------
	// Summary:
	//      Reloads configuration settings from the specified configuration file.
	// Parameters:
	//      csFileName : [in] configuration file name.
	//      nCfgFlags   : [in] action flags.
	// Returns:
	//      Reloaded Theme name if found, or empty string if theme was not found.
	// Remarks:
	//      Does nothing if specified file is not found in the collection of
	//      files from the main configuration file.
	// See also:
	//      enum XTPSyntaxEditCfgFileRemove.
	//-----------------------------------------------------------------------
	CString ReloadFile(const CString& csFileName, int nCfgFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//      Remove all loaded data.
	//-----------------------------------------------------------------------
	void RemoveAll();

private:
	CXTPSyntaxEditMapThemes     m_mapThemes;        // Themes map
	CStringArray        m_arThemeNames;     // Themes names array
	CMapStringToString  m_mapFileToTheme;   // Theme name to configuration file name map
};

typedef XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexTextSchema CXTPSyntaxEditTextSchema;
typedef CXTPSmartPtrInternalT<XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexTextSchema> CXTPSyntaxEditLexTextSchemaPtr;
typedef CXTPSyntaxEditLexTextSchemaPtr CXTPSyntaxEditTextSchemaPtr;

//===========================================================================
// Summary:
//      This class provides functionality to store set of all color themes
//      that would be written from configuration files. For short it is
//      custom collection class.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditTextSchemesManager
{
	typedef CMap<CString, LPCTSTR,
				CXTPSyntaxEditTextSchemaPtr,
				CXTPSyntaxEditTextSchemaPtr&> CXTPSyntaxEditMapSchemes;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Destroys a CXTPSyntaxEditTextSchemesManager() object,
	//      handles cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditTextSchemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Add information about text scheme.
	// Parameters:
	//      info : [in] A reference to schema information.
	//-----------------------------------------------------------------------
	void AddTextSchemeInfo(XTP_EDIT_SCHEMAFILEINFO& info);

	//-----------------------------------------------------------------------
	// Summary:
	//      Implements common logic on creating new text scheme object and
	//      loading it from file or resources.
	// Parameters:
	//      info : [in] A reference to schema information.
	//-----------------------------------------------------------------------
	void LoadTextScheme(XTP_EDIT_SCHEMAFILEINFO& info);

	//-----------------------------------------------------------------------
	// Summary:
	//  Implements common logic on creating new text scheme object and
	//  loading it string - used fo non-file configuration mode.
	// Parameters:
	//  info : [in] A reference to schema information.
	//  p_sSyntaxScheme : pointer to syntax scheme string
	//-----------------------------------------------------------------------
	void LoadTextStringScheme(XTP_EDIT_SCHEMAFILEINFO& info, CString* p_sSyntaxScheme);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns schema by its name.
	// Parameters:
	//      strSchemeName       : [in] schema name.
	// Returns:
	//      CXTPSyntaxEditTextSchemaPtr object.
	// See also:
	//      CXTPSyntaxEditTextSchemaPtr.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemaPtr GetSchema(const CString& strSchemeName);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns schema name by its file name.
	// Parameters:
	//      csFileName  : [in] file name.
	// Returns:
	//      CString object.
	//-----------------------------------------------------------------------
	CString GetSchemaName(const CString& csFileName) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns file name by its schema name.
	// Parameters:
	//      strSchemeName   : [in] schema name.
	// Returns:
	//      CString object.
	//-----------------------------------------------------------------------
	CString GetSchemaFileName(const CString& strSchemeName) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns all schemes names
	// Returns:
	//      Reference to CStringArray.
	//-----------------------------------------------------------------------
	CStringArray& GetSchemes();

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to get schemas list.
	// Returns:
	//      A reference to CXTPSyntaxEditSchemaFileInfoList object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditSchemaFileInfoList* GetSchemaList() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Finds a schema by file extension.
	// Parameters:
	//      strFileExt : [in] file extension string.
	// Returns:
	//      CXTPSyntaxEditTextSchemaPtr object.
	// See also:
	//      CXTPSyntaxEditTextSchemaPtr.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemaPtr FindSchema(const CString& strFileExt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Applys theme to all text schemes.
	// Parameters:
	//      pTheme : [in] Pointer to theme to be set.
	// See also:
	//      class CXTPSyntaxEditColorTheme.
	//-----------------------------------------------------------------------
	void SetTheme(CXTPSyntaxEditColorTheme* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//      Reloads configuration settings from the specified configuration file.
	// Parameters:
	//      csFileName : [in] configuration file name.
	//      nCfgFlags  : [in] action flags.
	// Does nothing if specified file is not found in the collection of files
	// from the main configuration file
	//-----------------------------------------------------------------------
	BOOL ReloadFile(const CString& csFileName, int nCfgFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	// Remove all loaded data.
	//-----------------------------------------------------------------------
	void RemoveAll();

private:
	CXTPSyntaxEditMapSchemes        m_mapSchemes;
	CXTPSyntaxEditSchemaFileInfoList*    m_plistSchemes;
	CStringArray                    m_arSchemeNames;
	CCriticalSection                m_csReadFile;
};

AFX_INLINE CStringArray& CXTPSyntaxEditTextSchemesManager::GetSchemes()
{
	return m_arSchemeNames;
}

AFX_INLINE CXTPSyntaxEditSchemaFileInfoList* CXTPSyntaxEditTextSchemesManager::GetSchemaList() const {
	return m_plistSchemes;
}

//===========================================================================
// Summary: This class manages all configuration parameters of the control.
//          It worries about read configuration files from disk when the Edit
//          control is starting, store it in internal structures and refresh
//          parameters in case configuration files are changed after control
//          was started.
// See Also: CXTPSyntaxEditFileChangesMonitor, CXTPSyntaxEditColorThemesManager
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditConfigurationManager : public CXTPCmdTarget
{
public:
	// Configuration manager sub-objects
	enum XTPSyntaxEditCfgObjects
	{
		xtpEditCfgObjSchMan     = 0x0001, // Schemes manager
		xtpEditCfgObjThemeMan   = 0x0002, // Themes manager
		xtpEditCfgObjMainConfig = 0x0004, // Main configuration file
	};

	//-----------------------------------------------------------------------
	// Summary: Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditConfigurationManager();

	//-----------------------------------------------------------------------
	// Summary: Destroys a CXTPSyntaxEditConfigurationManager() object,
	//          handles cleanup and de-allocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditConfigurationManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Reloads settings from provided file.
	// Parameters:
	//      strConfigFilePath - string of file's full path.
	// Remarks:
	//      Reloads configuration settings from the specified new configuration file.
	//-----------------------------------------------------------------------
	void ReloadConfig(const CString& strConfigFilePath);

	//-----------------------------------------------------------------------
	// Summary:
	//      Reloads configuration settings from the stored main configuration file.
	//-----------------------------------------------------------------------
	void ReloadConfig();

	//-----------------------------------------------------------------------
	// Summary:
	//      Asynchronously reloads configuration settings from the stored main
	//      configuration file.
	// Parameters:
	//      nAsyncThreadPriority - The priority for the working thread which
	//                             reloads configuration.
	//-----------------------------------------------------------------------
	void ReloadConfigAsync(int nAsyncThreadPriority = THREAD_PRIORITY_NORMAL);

	//--------------------------------------------------------------------
	// Summary:     Get main configuration file name.
	// Remarks:     Return configuration file name previously stored by
	//              ReloadConfig(configFile).
	// Returns:     Main configuration file name.
	// See also:    CXTPSyntaxEditConfigurationManager::ReloadConfig().
	//--------------------------------------------------------------------
	CString GetConfigFile() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Reloads configuration settings from the specified configuration file.
	// Parameters:
	//      csFileName : [in] configuration file name.
	//      dwOwnerFlags : [in] owner flags
	//      nCfgFlags   : [in] action flags.
	// Remarks:
	//      Does nothing if specified file is not found in the collection of
	//      files from the main configuration file.
	// See also:
	//      enum XTPSyntaxEditCfgFileRemove.
	//-----------------------------------------------------------------------
	void ReloadFile(CString csFileName, DWORD dwOwnerFlags, int nCfgFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns color theme manager.
	// Returns:
	//      Reference to CXTPSyntaxEditColorThemesManager object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorThemesManager& GetThemeManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns text theme manager.
	// Returns:
	//      Reference to CXTPSyntaxEditTextSchemesManager object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemesManager& GetTextSchemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to set current theme.
	// Parameters:
	//      strThemeName : [in] Theme name.
	//      pActiveSch   : [in] A pointer to active schema to apply current theme.
	//-----------------------------------------------------------------------
	void SetTheme(const CString& strThemeName, CXTPSyntaxEditTextSchema* pActiveSch = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Closes configuration monitor session.
	//-----------------------------------------------------------------------
	void Close();
	//-----------------------------------------------------------------------
	// Summary:
	//      Returns current theme name.
	//-----------------------------------------------------------------------
	CString GetCurrentTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns notification connection.
	// Returns:
	//      CXTPNotifyConnectionPtr pointer.
	//-----------------------------------------------------------------------
	CXTPNotifyConnection* GetConnection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this function to set the syntax and color scheme.
	// Parameters:
	//      pSyntaxScheme - The syntax scheme.
	//      pColorScheme  - The color scheme.
	//-----------------------------------------------------------------------
	void SetSyntaxAndColorScheme(CString* pSyntaxScheme, CString* pColorScheme)
	{
		p_sSyntaxScheme = pSyntaxScheme;
		p_sColorScheme = pColorScheme;
	}
	BOOL m_bUseMonitor;         //TRUE if control use special thread to synchronize syntax rules with files with Syntax and Color schemes
	                            //FALSE if control works "off-line"
	BOOL m_bConfigFileMode;     // TRUE if control read Config file with links to detailed setting (Syntax and Color schemes)
	                            // FALSE if Syntax and Color schemes passed as strings using special functions
	CString m_sIniSet;          //The ini file settings.
	CString* p_sSyntaxScheme;   //Pointer to the syntax scheme.
	CString* p_sColorScheme;    //Pointer to the color scheme.

private:

	static UINT AFX_CDECL ReloadConfigAsync_Proc(LPVOID pThis);

	CString m_strMainIniFilename;   // Stores main configuration file name.

	CXTPSyntaxEditFileChangesMonitor* m_pFolderMonitor;      // Stores folder monitor object.
	CXTPSyntaxEditColorThemesManager m_ColorThemeManager;   // Stores color theme manager.
	CXTPSyntaxEditTextSchemesManager m_TextSchemesManager;  // Stores text theme manager.

	CString m_strCurrentThemeName;  // Current theme name

	CXTPNotifyConnectionMT*         m_pConnectMT; // Notification connection.
	CCriticalSection                m_DataLockerCS; // Critical section

	HANDLE  m_hReloadThread;        // Stores handle of monitoring thread.
	CEvent* m_pBreakReloadEvent;    // Stores pointer to event.
};

AFX_INLINE const CString CXTPSyntaxEditColorInfo::GetClassName() const
{
	return m_strClassName;
}

AFX_INLINE CXTPSyntaxEditColorThemesManager& CXTPSyntaxEditConfigurationManager::GetThemeManager()
{
	return m_ColorThemeManager;
}

AFX_INLINE CXTPSyntaxEditTextSchemesManager& CXTPSyntaxEditConfigurationManager::GetTextSchemesManager()
{
	return m_TextSchemesManager;
}

AFX_INLINE CString CXTPSyntaxEditConfigurationManager::GetCurrentTheme() const
{
	return m_strCurrentThemeName;
}

#endif // !defined(__XTPSYNTAXEDITLEXCOLORFILEREADER_H__)
