// Calculate.h: interface for the CCalculate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULATE_H__DC732C29_9D80_4569_BFFA_81773FB0E638__INCLUDED_)
#define AFX_CALCULATE_H__DC732C29_9D80_4569_BFFA_81773FB0E638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Translate/Translate.h"

class CCalculate  
{
public:
	CTranslate			m_Translate;
public:
	int CalTwoLineAngle(KVector first_line_start_pt, KVector first_line_end_pt, KVector second_line_start_pt, KVector second_line_end_pt, double* angle);
	int CalAngleThreePoint(KVector origin_pt, KVector pt_1, KVector pt_2, double* angle);
	int CalCircleCenterThreePoint(KVector pt_1, KVector pt_2, KVector pt_3, KVector* center_pt);
	int CalArcLen(KVector pt_1, KVector pt_2, KVector pt_3, double* arc_len);
    int CalLengthTwoPoint(KVector start_pt, KVector end_pt, double* length);
	int CalPlaneRate(KVector pt_1, KVector pt_2, KVector pt_3, KVector pt_4, double* error_val);
	int CalLinearRate(KVector pt_1, KVector pt_2, KVector pt_3, KVector* ret_pt, double* real_dev_len);
	CCalculate();
	virtual ~CCalculate();

};

#endif // !defined(AFX_CALCULATE_H__DC732C29_9D80_4569_BFFA_81773FB0E638__INCLUDED_)
