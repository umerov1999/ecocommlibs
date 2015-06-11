class CXTPResource
{
// Construction / Destruction
public:

	CXTPResource();

	virtual ~CXTPResource();

// Methods
public:

	BOOL LoadResource(HMODULE hInstance, LPCTSTR pszName, LPCTSTR pszType);

// Members
public:
   /** Handle to the resource info block */
   HRSRC m_hResource;

   /** Handle to the resource data */
   HGLOBAL m_hResourceData;

   /** Pointer to the first byte of the resource data */
   LPVOID m_pvResourceData;

   /** Size of the resource in bytes */
   DWORD m_dwSize;
};
