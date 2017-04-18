#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"
#include "SISAssemBlockItem.h"

#define SISAssemBlockProp SISAssemBlock

class SISAssemBlockProp : public SISShipNoInfo
{
public:
	SISAssemBlockItem m_assemBlockItem;
	CMap<CString, LPCTSTR, CString, LPCTSTR> m_mapProp;
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssemBlockProp() : SISShipNoInfo()
	{
		m_mapProp.RemoveAll();
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssemBlockProp& operator = (const SISAssemBlockProp &other)
	{
		SISShipNoInfo::operator=(other);
		
		m_assemBlockItem = other.m_assemBlockItem;
		
		CString strVarName = _T("");
		CString strData = _T("");
		m_mapProp.RemoveAll();
		POSITION mapPos = other.m_mapProp.GetStartPosition();
		while (mapPos)
		{
			other.m_mapProp.GetNextAssoc(mapPos, strVarName, strData);
			m_mapProp.SetAt(strVarName, strData);
		}
	
		return *this;
	}

	/**
	@brief Set assembly block information
	@param assemBlockItem [in]Assembly block information
	@return
	@remark
	*/
	void SetAssemBlockItem(SISAssemBlockItem assemBlockItem)
	{
		m_assemBlockItem = assemBlockItem;
	}

	/**
	@brief Get assembly block information
	@param
	@return Assembly block information
	@remark
	*/
	SISAssemBlockItem GetAssemBlockItem()
	{
		return m_assemBlockItem;
	}

	/**
	@brief Set assembly block property list
	@param assemBlockProp [in]Assembly block properties list
	@return
	@remark
	*/
	void SetAssemBlockProp(CMap<CString, LPCTSTR, CString, LPCTSTR>& assemBlockProp)
	{
		m_mapProp.RemoveAll();

		CString strVarName = _T("");
		CString strData = _T("");
		POSITION mapPos = assemBlockProp.GetStartPosition();
		while (mapPos)
		{
			assemBlockProp.GetNextAssoc(mapPos, strVarName, strData);
			m_mapProp.SetAt(strVarName, strData);
		}
	}

	/**
	@brief Get assembly block property list
	@param assemBlockProp [out]Assembly block properties list
	@return
	@remark
	*/
	void GetAssemBlockProp(CMap<CString, LPCTSTR, CString, LPCTSTR>& assemBlockProp)
	{
		assemBlockProp.RemoveAll();

		CString strVarName = _T("");
		CString strData = _T("");
		POSITION mapPos = m_mapProp.GetStartPosition();
		while (mapPos)
		{
			m_mapProp.GetNextAssoc(mapPos, strVarName, strData);
			assemBlockProp.SetAt(strVarName, strData);
		}
	}
};