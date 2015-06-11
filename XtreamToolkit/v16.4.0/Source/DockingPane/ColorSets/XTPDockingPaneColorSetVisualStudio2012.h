class CXTPResourceImages;


class _XTP_EXT_CLASS CXTPDockingPaneColorSet
{
public:
	CXTPPaintManagerColor m_clrBorder;       // Border color
	CXTPPaintManagerColor m_clrBackground;   // Background color
	CXTPPaintManagerColor m_clrCaption;      // Caption color
	CXTPPaintManagerColor m_clrText;         // Text color
	CXTPPaintManagerColor m_clrGripper;      // Gripper color
	CXTPPaintManagerColor m_clrSplitter;     // Splitter color

	CXTPColorSetCaptionButton m_csCaptionButtonNormal;
	CXTPColorSetCaptionButton m_csCaptionButtonHot;
	CXTPColorSetCaptionButton m_csCaptionButtonPressed;
};

class _XTP_EXT_CLASS CXTPDockingPaneColorSetVisualStudio2012 : public CXTPDockingPaneColorSet
{
public:
	virtual void RefreshMetrics(CXTPResourceImages *pImages, LPCTSTR lpszSectionName);
};


class _XTP_EXT_CLASS CXTPDockingPaneColorSetVisualStudio2012Light : public CXTPDockingPaneColorSetVisualStudio2012
{
public:
	virtual void RefreshMetrics(CXTPResourceImages *pImages, LPCTSTR lpszSectionName);
};


class _XTP_EXT_CLASS CXTPDockingPaneColorSetVisualStudio2012Dark : public CXTPDockingPaneColorSetVisualStudio2012
{
public:
	virtual void RefreshMetrics(CXTPResourceImages *pImages, LPCTSTR lpszSectionName);
};
