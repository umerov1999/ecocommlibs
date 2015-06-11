#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/Hook/XTPWinEventHook.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneScrollBar.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneSlider.h>

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarSliderPane

IMPLEMENT_DYNAMIC(CXTPStatusBarSliderPane, CXTPStatusBarScrollBarPane)

CXTPStatusBarSliderPane::CXTPStatusBarSliderPane()
{

}

CXTPStatusBarSliderPane::~CXTPStatusBarSliderPane()
{

}

CXTPScrollBarPaintManager* CXTPStatusBarSliderPane::GetScrollBarPaintManager() const
{
	return m_pStatusBar->GetPaintManager()->GetSliderPaintManager();
}

void CXTPStatusBarSliderPane::SetTicks(double* pTicks, int nCount)
{
	SAFE_DELETE(m_pTicks);

	if (nCount)
	{
		m_pTicks = new XTP_SLIDERTICKS(pTicks, nCount);
	}
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPStatusBarSliderPane, CXTPStatusBarPane)
	DISP_PROPERTY_ID(CXTPStatusBarSliderPane, "Min", 120, m_nMin, VT_I4)
	DISP_PROPERTY_ID(CXTPStatusBarSliderPane, "Max", 121, m_nMax, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarSliderPane, "Value", 122, GetPos, SetPos, VT_I4)
	DISP_FUNCTION_ID(CXTPStatusBarSliderPane, "SetTooltipPart", 123, OleSetTooltipPart, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPStatusBarSliderPane, "SetTicks", 124, OleSetTicks, VT_EMPTY, VTS_VARIANT)
END_DISPATCH_MAP()

// {4579783D-A5D8-4569-8690-592295C5583A}
static const GUID IID_StatusBarSliderPane =
{ 0x4579783d, 0xa5d8, 0x4569, { 0x86, 0x90, 0x59, 0x22, 0x95, 0xc5, 0x58, 0x3a } };

BEGIN_INTERFACE_MAP(CXTPStatusBarSliderPane, CXTPStatusBarPane)
	INTERFACE_PART(CXTPStatusBarSliderPane, IID_StatusBarSliderPane, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPStatusBarSliderPane, IID_StatusBarSliderPane)

void CXTPStatusBarSliderPane::OleSetTooltipPart(int nPart, LPCTSTR lpszTooltip)
{
	SetTooltipPart(nPart, lpszTooltip);
}

void CXTPStatusBarSliderPane::OleSetTicks(const VARIANT& Ticks)
{
	int nCount = 1;
	double* pID = NULL;

	if ((Ticks.vt & VT_ARRAY) != 0)
	{
		int nType = Ticks.vt & (~VT_ARRAY);

		if (nType != VT_VARIANT && nType != VT_I4 && nType != VT_I2 && nType != VT_R4  && nType != VT_R8)
			return;

		COleSafeArray saSrc(Ticks);

		if (saSrc.GetDim() != 1)
			return;

		nCount = saSrc.GetOneDimSize();

		long nLBound = 0;
		saSrc.GetLBound(1, &nLBound);

		if (nCount < 1)
			return;

		pID = new double[nCount];

		for (long nIndex = 0; nIndex < nCount; nIndex++)
		{
			long nElement = nIndex + nLBound;

			if (nType == VT_VARIANT)
			{
				VARIANT varData;
				saSrc.GetElement(&nElement, &varData);

				COleVariant var(varData);
				var.ChangeType(VT_R8);

				pID[nIndex] = V_R8(&var);
			}
			else if (nType == VT_I2 || nType == VT_I4)
			{
				int nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				pID[nIndex] = nItem;
			}
			else if (nType == VT_R4)
			{
				float nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				pID[nIndex] = nItem;
			}
			else if (nType == VT_R8)
			{
				double nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				pID[nIndex] = nItem;
			}
		}
	}
	else
	{
		COleVariant var(Ticks);
		var.ChangeType(VT_R8);

		pID = new double[1];
		pID[0] = V_R8(&var);
	}

	SetTicks(pID, nCount);
	delete[] pID;
}

#endif
