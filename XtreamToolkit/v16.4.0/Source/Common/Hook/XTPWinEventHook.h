class CXTPWinEventHook
{
public:
	DECLARE_HANDLE(HWINEVENTHOOK);
	typedef void (CALLBACK* WINEVENTPROC)(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime);

public:
	CXTPWinEventHook();
	virtual ~CXTPWinEventHook();

public:
	void SetEventHook(UINT eventMin, UINT eventMax, WINEVENTPROC pfnEventProc);
	void RemoveEventHook();

protected:
	typedef HWINEVENTHOOK (WINAPI* LPFNSETWINEVENTHOOK)(UINT eventMin, UINT eventMax, HMODULE hmodWinEventProc, WINEVENTPROC lpfnWinEventProc, DWORD idProcess, DWORD idThread, UINT dwflags);
	typedef BOOL (WINAPI* LPFNUNHOOKWINEVENT)(HWINEVENTHOOK hWinEventHook);

	static void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook,  DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

protected:
	HWINEVENTHOOK m_hWinEventHook;      // HWINEVENTHOOK value that identifies event hook instance
	LPFNSETWINEVENTHOOK m_pSetWinEventHook;
	LPFNUNHOOKWINEVENT m_pUnhookWinEvent;

public:
	LONG m_nCount;
};
