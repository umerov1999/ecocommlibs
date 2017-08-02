#pragma once

#ifndef _SISPerpendicular_H
#define _SISPerpendicular_H

#include <afxwin.h>  
#include <afxext.h>

class SISPerpendicular
{
protected:
	double m_dX;
	double m_dY;
	double m_dXY;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISPerpendicular()
	{
		m_dX = 0.0;
		m_dY = 0.0;
		m_dXY = 0.0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISNPerpendicular
	@return
	@remark Initialize member variable using value of input object
	*/
	SISPerpendicular(SISPerpendicular& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPerpendicular& operator = (const SISPerpendicular &other)
	{
		m_dX = other.m_dX;
		m_dY = other.m_dY;
		m_dXY = other.m_dXY;

		return *this;
	}

	/**
	@brief Set perpendicular information
	@param dX [in]Perpendicular X value
	@param dY [in]Perpendicular XY value
	@param dXY [in]Perpendicular XYZ value
	@return
	@remark
	*/
	void SetPerpendicular(double dX, double dY, double dXY)
	{
		m_dX = dX;
		m_dY = dY;
		m_dXY = dXY;
	}

	/**
	@brief Get perpendicular information
	@param dX [out]Perpendicular X value
	@param dY [out]Perpendicular XY value
	@param dXY [out]Perpendicular XYZ value
	@return
	@remark
	*/
	void GetPerpendicular(double& dX, double& dY, double& dXY)
	{
		dX = m_dX;
		dY = m_dY;
		dXY = m_dXY;
	}
};

#endif