#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/ColorSets/XTPTabColorSetDefault.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2003.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2007.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2007Access.h>


//////////////////////////////////////////////////////////////////////////
//

void CXTPTabColorSetOffice2007Access::RefreshMetrics()
{
	CXTPTabColorSetOffice2007::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		m_clrTextSelected.SetStandardValue(0);
		m_clrTextNormal.SetStandardValue(0);
		m_clrTextHighlight.SetStandardValue(0);

		m_clrFrameBorder.SetStandardValue(pImages->GetImageColor(_T("TabManager"), _T("AccessBorder")));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);

		m_clrHeaderFace.SetStandardValue(pImages->GetImageColor(_T("TabManager"), _T("AccessHeader")));
		m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);

		m_clrButtonText.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText")));
	}
}
