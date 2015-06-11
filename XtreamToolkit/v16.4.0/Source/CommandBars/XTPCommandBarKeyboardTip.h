class CXTPCommandBars;

//===========================================================================
// Summary:
//     CXTPCommandBarKeyboardTip is CWnd derived class, represents Keyboard tip single window
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarKeyboardTip : public CWnd
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarKeyboardTip object
	// Parameters:
	//     pCommandBars - CXTPCommandBars parent object
	//     pOwner - Owner object
	//     lpszCaption - Caption of control
	//     pt - Client coordinates
	//     dwAlign - Align of tip
	//     bEnabled - TRUE if keyboard tip is enabled
	//-----------------------------------------------------------------------
	CXTPCommandBarKeyboardTip(CXTPCommandBars* pCommandBars, CCmdTarget* pOwner, LPCTSTR lpszCaption, CPoint pt, DWORD dwAlign, BOOL bEnabled);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
//}}AFX_CODEJOCK_PRIVATE

public:
	CString m_strTip;           // Tooltip
	CString m_strCaption;       // Caption
	CString m_strPrefix;       // Caption
	CCmdTarget* m_pOwner;       // Owner object
	CPoint m_pt;                // Client coordinates
	BOOL m_bEnabled;            // Enabled flag
	BOOL m_bVisible;            // TRUE if visible
	CXTPCommandBars* m_pCommandBars;    // Parent CXTPCommandBars object
	DWORD m_dwAlign;            // Tip align
	BOOL m_bExplicit;           // TRUE when user specify manually KeyTip; FALSE if calculated automatically
};
