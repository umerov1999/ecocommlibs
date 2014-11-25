#pragma once

#include "StdAfx.h"

class CPKOptions// : public CObject
{
public:
	CPKOptions(void)
	{
		m_bSendNRecv = TRUE;
	}
	~CPKOptions(void)
	{

	}

	CPKOptions(const CPKOptions& s)
	{
		*this = s;
	}

	CPKOptions& operator = (const CPKOptions &other)
	{
		m_bSendNRecv = other.m_bSendNRecv;
		return *this;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		;
	}

public:
	void SetSendNRecv(BOOL bSendNRecv)
	{
		m_bSendNRecv = bSendNRecv;
	}

protected:
	BOOL m_bSendNRecv;
};