#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISLength
{
protected:
	double m_dX;
	double m_dY;
	double m_dZ;
	double m_dXY;
	double m_dXYZ;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISLength()
	{
		m_dX = 0.0;
		m_dY = 0.0;
		m_dZ = 0.0;
		m_dXY = 0.0;
		m_dXYZ = 0.0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISNLength
	@return
	@remark Initialize member variable using value of input object
	*/
	SISLength(SISLength& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISLength& operator = (const SISLength &other)
	{
		m_dX = other.m_dX;
		m_dY = other.m_dY;
		m_dZ = other.m_dZ;
		m_dXY = other.m_dXY;
		m_dXYZ = other.m_dXYZ;

		return *this;
	}

	/**
	@brief Set length information
	@param dX [in]X length
	@param dY [in]Y length
	@param dZ [in]Z length
	@param dXY [in]XY length
	@param dXYZ [in]XYZ length
	@return
	@remark
	*/
	void SetLength(double dX, double dY, double dZ, double dXY, double dXYZ)
	{
		m_dX = dX;
		m_dY = dY;
		m_dZ = dZ;
		m_dXY = dXY;
		m_dXYZ = dXYZ;
	}

	/**
	@brief Get length information
	@param dX [out]X length
	@param dY [out]Y length
	@param dZ [out]Z length
	@param dXY [out]XY length
	@param dXYZ [out]XYZ length
	@return
	@remark
	*/
	void GetLength(double& dX, double& dY, double& dZ, double& dXY, double& dXYZ)
	{
		dX = m_dX;
		dY = m_dY;
		dZ = m_dZ;
		dXY = m_dXY;
		dXYZ = m_dXYZ;
	}
};