#include "StdAfx.h"

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>


//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerNavigateButtons

CXTPTabManagerNavigateButtons::CXTPTabManagerNavigateButtons()
{

}

CXTPTabManagerNavigateButtons::~CXTPTabManagerNavigateButtons()
{
	RemoveAll();
}

void CXTPTabManagerNavigateButtons::RemoveAll()
{
	for (int i = 0; i < m_arrButtons.GetSize(); i++)
	{
		delete m_arrButtons[i];
	}
	m_arrButtons.RemoveAll();
}

void CXTPTabManagerNavigateButtons::RemoveAt(int nIndex)
{
	delete m_arrButtons[nIndex];
	m_arrButtons.RemoveAt(nIndex);
}
