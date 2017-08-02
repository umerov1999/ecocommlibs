#pragma once

#ifndef _SISAssemBlockInfo_H
#define _SISAssemBlockInfo_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"
#include "SISAssemBlockItem.h"



class SISAssemBlockInfo : public SISShipNoInfo
{
public:
	SISAssemBlockItem m_assemBlockItem;
	SISAssemBlockItem m_parentAssemBlockItem;
	CArray<SISAssemBlockItem, SISAssemBlockItem&> m_arrChildAssemBlockItem;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssemBlockInfo() : SISShipNoInfo()
	{
		m_arrChildAssemBlockItem.RemoveAll();
	}

	/**
	@brief Creator
	@param c [in]Object of SISAssemBlockInfo
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAssemBlockInfo(const SISAssemBlockInfo& c) : SISShipNoInfo()
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssemBlockInfo& operator = (const SISAssemBlockInfo &other)
	{
		SISShipNoInfo::operator=(other);
		
		m_assemBlockItem = other.m_assemBlockItem;
		m_parentAssemBlockItem = other.m_parentAssemBlockItem;
		m_arrChildAssemBlockItem.Copy(other.m_arrChildAssemBlockItem);
	
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
	@brief Set parent assembly block information
	@param parentAssemBlockItem [in]Parent assembly block information
	@return
	@remark
	*/
	void SetParentAssemBlockItem(SISAssemBlockItem parentAssemBlockItem)
	{
		m_parentAssemBlockItem = parentAssemBlockItem;
	}

	/**
	@brief Get parent assembly block information
	@param
	@return Parent assembly block information
	@remark
	*/
	SISAssemBlockItem GetParentAssemBlockItem()
	{
		return m_parentAssemBlockItem;
	}

	/**
	@brief Set child assembly block information list
	@param childAssemBlockItem [in]Child assembly block information list
	@return
	@remark
	*/
	void SetChildAssemBlockItem(CArray<SISAssemBlockItem, SISAssemBlockItem&>& childAssemBlockItem)
	{
		m_arrChildAssemBlockItem.Copy(childAssemBlockItem);
	}

	/**
	@brief Get child assembly block information list
	@param childAssemBlockItem [out]Child assembly block information list
	@return
	@remark
	*/
	void GetChildAssemBlockItem(CArray<SISAssemBlockItem, SISAssemBlockItem&>& childAssemBlockItem)
	{
		childAssemBlockItem.Copy(m_arrChildAssemBlockItem);
	}
};

#endif