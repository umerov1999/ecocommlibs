// Calculate.cpp: implementation of the CCalculate class.
//
//////////////////////////////////////////////////////////////////////
#include "../ecocalc.h"
#include "Calculate.h"
#include <QtMath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalculate::CCalculate()
{

}

CCalculate::~CCalculate()
{

}

int CCalculate::CalLengthTwoPoint(KVector start_pt, KVector end_pt, double *length)
{
    int ret = ECO_OK;
	double tmp_matrix[4][4];
	double std_matrix[4][4];

	KVector trans_start_pt, trans_end_pt;
	KVector target_pt, target_axis;

	m_Translate.InitMatrix(tmp_matrix);
	m_Translate.InitMatrix(std_matrix);

	target_pt.x = 0.0;
	target_pt.y = 0.0;
	target_pt.z = 0.0;

	target_axis.x = 1000.0;
	target_axis.y = 0.0;
	target_axis.z = 0.0;

    m_Translate.GetTran1Pt1Axis(false, tmp_matrix, start_pt, end_pt, target_pt, target_axis, std_matrix);

	m_Translate.EvaluateVector(start_pt, std_matrix, &trans_start_pt);
	m_Translate.EvaluateVector(end_pt, std_matrix, &trans_end_pt);

	(*length) = trans_end_pt.x - trans_start_pt.x;

	if((*length) < 0.0)
		(*length) *= -1.0;

	return ret;
}

int CCalculate::CalAngleThreePoint(KVector origin_pt, KVector pt_1, KVector pt_2, double* angle)
{
    int ret = ECO_OK;
	double len_a, len_b, len_c;
	double cos_theta;

    CalLengthTwoPoint(origin_pt, pt_1, &len_a);
    CalLengthTwoPoint(origin_pt, pt_2, &len_b);
    CalLengthTwoPoint(pt_1, pt_2, &len_c);

	//2011-02-08 하남국
	//1.#QNAN0 출력에대한 예외처리
	if(len_a == 0 || len_b == 0 || len_c == 0)
	{
		(*angle) = 0.0;
		ret = -1;
		return ret;
	}


	cos_theta = (pow(len_a, 2.0) + pow(len_b, 2.0) - pow(len_c, 2.0)) 
				/ (2 * len_a * len_b);

	(*angle) = acos(cos_theta) * 180.0 / M_PI;

	return ret;
}

int CCalculate::CalTwoLineAngle(KVector first_line_start_pt, KVector first_line_end_pt, KVector second_line_start_pt, KVector second_line_end_pt, double *angle)
{
    int ret = ECO_OK;
	double tmp_matrix[4][4];
	double std_matrix[4][4];

	double len_a, len_b, len_c, tmp_len;
	double cos_theta;

	KVector origin_pt;
	KVector pt_1;
	KVector pt_2;

	KVector trans_first_start_pt, trans_first_end_pt;
	KVector trans_second_start_pt, trans_second_end_pt;
	KVector target_pt, target_axis_1, target_axis_2;

	m_Translate.InitMatrix(tmp_matrix);
	m_Translate.InitMatrix(std_matrix);

	target_pt.x = 0.0;
	target_pt.y = 0.0;
	target_pt.z = 0.0;

	target_axis_1.x = 1000.0;
	target_axis_1.y = 0.0;
	target_axis_1.z = 0.0;

	target_axis_2.x = 0.0;
	target_axis_2.y = 1000.0;
	target_axis_2.z = 0.0;

	m_Translate.GetTran1Pt1Axis1Plane(tmp_matrix, first_line_start_pt, first_line_end_pt, second_line_start_pt,
		target_pt, target_axis_1, target_axis_2, std_matrix);

	m_Translate.EvaluateVector(first_line_start_pt, std_matrix, &trans_first_start_pt);
	m_Translate.EvaluateVector(first_line_end_pt, std_matrix, &trans_first_end_pt);

	m_Translate.EvaluateVector(second_line_start_pt, std_matrix, &trans_second_start_pt);
	m_Translate.EvaluateVector(second_line_end_pt, std_matrix, &trans_second_end_pt);

	// 4번째점 레벨 0로 세팅
	trans_second_end_pt.z = 0.0;

	// y = ax + b
	double second_a, second_b;

	if(trans_second_end_pt.x - trans_second_start_pt.x != 0.0)
	{
		second_a = (trans_second_end_pt.y - trans_second_start_pt.y) 
				 / (trans_second_end_pt.x - trans_second_start_pt.x);

		second_b = trans_second_end_pt.y - (second_a * trans_second_end_pt.x);

		origin_pt.x = (second_b / second_a) * -1.0;
		origin_pt.y = 0.0;
		origin_pt.z = 0.0;
	}
	else
	{
		origin_pt.x = trans_second_start_pt.x;
		origin_pt.y = 0.0;
		origin_pt.z = 0.0;
	}

    CalLengthTwoPoint(origin_pt, trans_first_start_pt, &len_a);
    CalLengthTwoPoint(origin_pt, trans_first_end_pt, &tmp_len);
	if(len_a < tmp_len)
	{
		pt_1 = trans_first_end_pt;
		len_a = tmp_len;
	}
	else
	{
		pt_1 = trans_first_start_pt;
	}

    CalLengthTwoPoint(origin_pt, trans_second_start_pt, &len_b);
    CalLengthTwoPoint(origin_pt, trans_second_end_pt, &tmp_len);
	if(len_b < tmp_len)
	{
		pt_2 = trans_second_end_pt;
		len_b = tmp_len;
	}
	else
	{
		pt_2 = trans_second_start_pt;
	}

    CalLengthTwoPoint(pt_1, pt_2, &len_c);
	

	cos_theta = (pow(len_a, 2.0) + pow(len_b, 2.0) - pow(len_c, 2.0)) 
				/ (2 * len_a * len_b);

	(*angle) = acos(cos_theta) * 180.0 / M_PI;

	return ret;
}
int CCalculate::CalArcLen(KVector pt_1, KVector pt_2, KVector pt_3, double* arc_len)
{
	double tmp_matrix[4][4];
	double std_matrix[4][4];
	double inv_matrix[4][4];

	KVector center_pt;
	KVector tmp_pt;
	KVector trans_pt_1, trans_pt_2, trans_pt_3;
	KVector target_pt, target_axis_1, target_axis_2;

	m_Translate.InitMatrix(tmp_matrix);
	m_Translate.InitMatrix(std_matrix);

	target_pt.x = 0.0;
	target_pt.y = 0.0;
	target_pt.z = 0.0;

	target_axis_1.x = 1000.0;
	target_axis_1.y = 0.0;
	target_axis_1.z = 0.0;

	target_axis_2.x = 0.0;
	target_axis_2.y = 1000.0;
	target_axis_2.z = 0.0;

	m_Translate.GetTran1Pt1Axis1Plane(tmp_matrix, pt_1, pt_2, pt_3,
		target_pt, target_axis_1, target_axis_2, std_matrix);
	m_Translate.InvMatrix(4, (double*)std_matrix, (double*)inv_matrix);

	m_Translate.EvaluateVector(pt_1, std_matrix, &trans_pt_1);
	m_Translate.EvaluateVector(pt_2, std_matrix, &trans_pt_2);
	m_Translate.EvaluateVector(pt_3, std_matrix, &trans_pt_3);

	KVector first_center, second_center, third_center;

	double a1, a2, a3; // Y = aX + b;
	double b1, b2, b3;
	double a, b, c, d;
	double cos_theta, theta;

	first_center.x = 
		(trans_pt_1.x + trans_pt_2.x) / 2.0;
	first_center.y = 
		(trans_pt_1.y + trans_pt_2.y) / 2.0;

	second_center.x = 
		(trans_pt_3.x + trans_pt_2.x) / 2.0;
	second_center.y = 
		(trans_pt_3.y + trans_pt_2.y) / 2.0;

	third_center.x = 
		(trans_pt_3.x + trans_pt_1.x) / 2.0;
	third_center.y = 
		(trans_pt_3.y + trans_pt_1.y) / 2.0;


	if(trans_pt_1.x > trans_pt_2.x && (trans_pt_1.y > trans_pt_2.y || trans_pt_1.y < trans_pt_2.y || (trans_pt_1.y == trans_pt_2.y))) {

		a1 = -(double)(trans_pt_1.x - trans_pt_2.x) / (double)(trans_pt_1.y - trans_pt_2.y);
		b1 = (double)first_center.y + (-a1) * first_center.x;

	} else {
		
		a1 = -(double)(trans_pt_2.x - trans_pt_1.x) / (double)(trans_pt_2.y - trans_pt_1.y);
		b1 = (double)first_center.y + (-a1) * first_center.x;
	} 

	
	if(trans_pt_2.x > trans_pt_3.x && (trans_pt_2.y > trans_pt_3.y || trans_pt_2.y < trans_pt_3.y || (trans_pt_2.y == trans_pt_3.y))) {

		a2 = -(double)(trans_pt_2.x - trans_pt_3.x) / (double)(trans_pt_2.y - trans_pt_3.y);
		b2 = (double)second_center.y + (-a2) * second_center.x;

	} else {
		
		a2 = -(double)(trans_pt_3.x - trans_pt_2.x) / (double)(trans_pt_3.y - trans_pt_2.y);
		b2 = (double)second_center.y + (-a2) * second_center.x;
	}

 
	if(trans_pt_1.x > trans_pt_3.x && (trans_pt_1.y > trans_pt_3.y || trans_pt_1.y < trans_pt_3.y || (trans_pt_1.y == trans_pt_3.y))) {

		a3 = -(double)(trans_pt_1.x - trans_pt_3.x) / (double)(trans_pt_1.y - trans_pt_3.y);
		b3 = (double)third_center.y + (-a3) * third_center.x;

	} else {
		
		a3 = -(double)(trans_pt_3.x - trans_pt_1.x) / (double)(trans_pt_3.y - trans_pt_1.y);
		b3 = (double)third_center.y + (-a3) * third_center.x;
	}


	if(trans_pt_1.y == trans_pt_2.y) {
		center_pt.x = (b2 - b3) / (a3 - a2);
		center_pt.y = ((a2 * center_pt.x + b2) + (a3 * center_pt.x + b3)) / 2.0;

	} else if(trans_pt_2.y == trans_pt_3.y) {
		center_pt.x = (b3 - b1) / (a1 - a3);
		center_pt.y = ((a1 * center_pt.x + b1) + (a3 * center_pt.x + b3)) / 2.0;

	} else if(trans_pt_3.y == trans_pt_1.y) {
		center_pt.x = (b2 - b1) / (a1 - a2);
		center_pt.y = ((a2 * center_pt.x + b2) + (a1 * center_pt.x + b1)) / 2.0;

	} else if(abs(trans_pt_1.y - trans_pt_2.y) < abs(trans_pt_2.y - trans_pt_3.y) && abs(trans_pt_1.y - trans_pt_2.y) < abs(trans_pt_1.y - trans_pt_3.y)) {
		center_pt.x = (((b3 - b1) / (a1 - a3)) + ((b2 - b3) / (a3 - a2))) / 2.0;
		center_pt.y = ((a2 * center_pt.x + b2) + (a3 * center_pt.x + b3)) / 2.0;
		
	} else if(abs(trans_pt_2.y - trans_pt_3.y) < abs(trans_pt_1.y - trans_pt_2.y) && abs(trans_pt_2.y - trans_pt_3.y) < abs(trans_pt_1.y - trans_pt_3.y)) {
		center_pt.x = (((b3 - b1) / (a1 - a3)) + ((b2 - b1) / (a1 - a2))) / 2.0;
		center_pt.y = ((a1 * center_pt.x + b1) + (a3 * center_pt.x + b3)) / 2.0;	
	} else if(abs(trans_pt_1.y - trans_pt_3.y) < abs(trans_pt_2.y - trans_pt_3.y) && abs(trans_pt_1.y - trans_pt_3.y) < abs(trans_pt_1.y - trans_pt_2.y)) {
		center_pt.x = (((b2 - b1) / (a1 - a2)) + ((b2 - b3) / (a3 - a2))) / 2.0;
		center_pt.y = ((a2 * center_pt.x + b2) + (a1 * center_pt.x + b1)) / 2.0;
	}

	m_Translate.EvaluateVector(center_pt, inv_matrix, &center_pt);



	m_Translate.InitMatrix(tmp_matrix);
	m_Translate.InitMatrix(std_matrix);

	target_pt.x = 0.0;
	target_pt.y = 0.0;
	target_pt.z = 0.0;

	target_axis_1.x = 1000.0;
	target_axis_1.y = 0.0;
	target_axis_1.z = 0.0;

	target_axis_2.x = 0.0;
	target_axis_2.y = 1000.0;
	target_axis_2.z = 0.0;
/*
	m_Translate.GetTran1Pt1Axis1Plane(tmp_matrix, trans_pt_1, center_pt, trans_pt_3,
		target_pt, target_axis_1, target_axis_2, std_matrix);

	m_Translate.EvaluateVector(trans_pt_3, std_matrix, &tmp_pt);

	a = sqrt(pow(center_pt.x - trans_pt_1.x, 2.0) + pow(center_pt.y - trans_pt_1.y, 2.0));
	b = sqrt(pow(center_pt.x - trans_pt_3.x, 2.0) + pow(center_pt.y - trans_pt_3.y, 2.0));
	c = sqrt(pow(trans_pt_3.x - trans_pt_1.x, 2.0) + pow(trans_pt_3.y - trans_pt_1.y, 2.0));
*/
	m_Translate.GetTran1Pt1Axis1Plane(tmp_matrix, pt_1, center_pt, pt_3,
		target_pt, target_axis_1, target_axis_2, std_matrix);

	m_Translate.EvaluateVector(pt_1, std_matrix, &trans_pt_1);
	m_Translate.EvaluateVector(pt_2, std_matrix, &trans_pt_2);
	m_Translate.EvaluateVector(pt_3, std_matrix, &trans_pt_3);
	m_Translate.EvaluateVector(center_pt, std_matrix, &center_pt);

//	m_Translate.EvaluateVector(pt_3, std_matrix, &tmp_pt);

	a = sqrt(pow(center_pt.x - trans_pt_1.x, 2.0) + pow(center_pt.y - trans_pt_1.y, 2.0));
	b = sqrt(pow(center_pt.x - trans_pt_3.x, 2.0) + pow(center_pt.y - trans_pt_3.y, 2.0));
	c = sqrt(pow(trans_pt_3.x - trans_pt_1.x, 2.0) + pow(trans_pt_3.y - trans_pt_1.y, 2.0));
	d = sqrt(pow(trans_pt_2.x - trans_pt_1.x, 2.0) + pow(trans_pt_2.y - trans_pt_1.y, 2.0));
	if(trans_pt_3.y == 0.0)
	{
		theta = 180.0;
	}
	else
	{
		cos_theta = (pow(a, 2.0) + pow(b, 2.0) - pow(c, 2.0)) / (2.0 * a * b);
		theta = acos(cos_theta) * 180.0 / 3.1415926;
/*
		if(trans_pt_2.y * trans_pt_3.y < 0)
			theta = 360.0- theta;
*/
		if((c < d) || (trans_pt_2.y * trans_pt_3.y < 0))
			theta = 360.0- theta;
	}

	(*arc_len) = (2.0 * M_PI * a) * (theta / 360.0);

    return ECO_OK;
}
int CCalculate::CalCircleCenterThreePoint(KVector pt_1, KVector pt_2, KVector pt_3, KVector* center_pt)
{
	double tmp_matrix[4][4];
	double std_matrix[4][4];
	double inv_matrix[4][4];

	KVector trans_pt_1, trans_pt_2, trans_pt_3;
	KVector target_pt, target_axis_1, target_axis_2;

	m_Translate.InitMatrix(tmp_matrix);
	m_Translate.InitMatrix(std_matrix);

	target_pt.x = 0.0;
	target_pt.y = 0.0;
	target_pt.z = 0.0;

	target_axis_1.x = 1000.0;
	target_axis_1.y = 0.0;
	target_axis_1.z = 0.0;

	target_axis_2.x = 0.0;
	target_axis_2.y = 1000.0;
	target_axis_2.z = 0.0;

	m_Translate.GetTran1Pt1Axis1Plane(tmp_matrix, pt_1, pt_2, pt_3,
		target_pt, target_axis_1, target_axis_2, std_matrix);
	m_Translate.InvMatrix(4, (double*)std_matrix, (double*)inv_matrix);

	m_Translate.EvaluateVector(pt_1, std_matrix, &trans_pt_1);
	m_Translate.EvaluateVector(pt_2, std_matrix, &trans_pt_2);
	m_Translate.EvaluateVector(pt_3, std_matrix, &trans_pt_3);

	KVector first_center, second_center, third_center;

	double a1, a2, a3; // Y = aX + b;
	double b1, b2, b3;


	first_center.x = 
		(trans_pt_1.x + trans_pt_2.x) / 2.0;
	first_center.y = 
		(trans_pt_1.y + trans_pt_2.y) / 2.0;

	second_center.x = 
		(trans_pt_3.x + trans_pt_2.x) / 2.0;
	second_center.y = 
		(trans_pt_3.y + trans_pt_2.y) / 2.0;

	third_center.x = 
		(trans_pt_3.x + trans_pt_1.x) / 2.0;
	third_center.y = 
		(trans_pt_3.y + trans_pt_1.y) / 2.0;


	if(trans_pt_1.x > trans_pt_2.x && (trans_pt_1.y > trans_pt_2.y || trans_pt_1.y < trans_pt_2.y || (trans_pt_1.y == trans_pt_2.y))) {

		a1 = -(double)(trans_pt_1.x - trans_pt_2.x) / (double)(trans_pt_1.y - trans_pt_2.y);
		b1 = (double)first_center.y + (-a1) * first_center.x;

	} else {
		
		a1 = -(double)(trans_pt_2.x - trans_pt_1.x) / (double)(trans_pt_2.y - trans_pt_1.y);
		b1 = (double)first_center.y + (-a1) * first_center.x;
	} 

	
	if(trans_pt_2.x > trans_pt_3.x && (trans_pt_2.y > trans_pt_3.y || trans_pt_2.y < trans_pt_3.y || (trans_pt_2.y == trans_pt_3.y))) {

		a2 = -(double)(trans_pt_2.x - trans_pt_3.x) / (double)(trans_pt_2.y - trans_pt_3.y);
		b2 = (double)second_center.y + (-a2) * second_center.x;

	} else {
		
		a2 = -(double)(trans_pt_3.x - trans_pt_2.x) / (double)(trans_pt_3.y - trans_pt_2.y);
		b2 = (double)second_center.y + (-a2) * second_center.x;
	}

 
	if(trans_pt_1.x > trans_pt_3.x && (trans_pt_1.y > trans_pt_3.y || trans_pt_1.y < trans_pt_3.y || (trans_pt_1.y == trans_pt_3.y))) {

		a3 = -(double)(trans_pt_1.x - trans_pt_3.x) / (double)(trans_pt_1.y - trans_pt_3.y);
		b3 = (double)third_center.y + (-a3) * third_center.x;

	} else {
		
		a3 = -(double)(trans_pt_3.x - trans_pt_1.x) / (double)(trans_pt_3.y - trans_pt_1.y);
		b3 = (double)third_center.y + (-a3) * third_center.x;
	}


	if(trans_pt_1.y == trans_pt_2.y) {
		(*center_pt).x = (b2 - b3) / (a3 - a2);
		(*center_pt).y = ((a2 * (*center_pt).x + b2) + (a3 * (*center_pt).x + b3)) / 2.0;

	} else if(trans_pt_2.y == trans_pt_3.y) {
		(*center_pt).x = (b3 - b1) / (a1 - a3);
		(*center_pt).y = ((a1 * (*center_pt).x + b1) + (a3 * (*center_pt).x + b3)) / 2.0;

	} else if(trans_pt_3.y == trans_pt_1.y) {
		(*center_pt).x = (b2 - b1) / (a1 - a2);
		(*center_pt).y = ((a2 * (*center_pt).x + b2) + (a1 * (*center_pt).x + b1)) / 2.0;

	} else if(abs(trans_pt_1.y - trans_pt_2.y) < abs(trans_pt_2.y - trans_pt_3.y) && abs(trans_pt_1.y - trans_pt_2.y) < abs(trans_pt_1.y - trans_pt_3.y)) {
		(*center_pt).x = (((b3 - b1) / (a1 - a3)) + ((b2 - b3) / (a3 - a2))) / 2.0;
		(*center_pt).y = ((a2 * (*center_pt).x + b2) + (a3 * (*center_pt).x + b3)) / 2.0;
		
	} else if(abs(trans_pt_2.y - trans_pt_3.y) < abs(trans_pt_1.y - trans_pt_2.y) && abs(trans_pt_2.y - trans_pt_3.y) < abs(trans_pt_1.y - trans_pt_3.y)) {
		(*center_pt).x = (((b3 - b1) / (a1 - a3)) + ((b2 - b1) / (a1 - a2))) / 2.0;
		(*center_pt).y = ((a1 * (*center_pt).x + b1) + (a3 * (*center_pt).x + b3)) / 2.0;	
	} else if(abs(trans_pt_1.y - trans_pt_3.y) < abs(trans_pt_2.y - trans_pt_3.y) && abs(trans_pt_1.y - trans_pt_3.y) < abs(trans_pt_1.y - trans_pt_2.y)) {
		(*center_pt).x = (((b2 - b1) / (a1 - a2)) + ((b2 - b3) / (a3 - a2))) / 2.0;
		(*center_pt).y = ((a2 * (*center_pt).x + b2) + (a1 * (*center_pt).x + b1)) / 2.0;
	}

	m_Translate.EvaluateVector((*center_pt), inv_matrix, center_pt);

    return ECO_OK;
}


int CCalculate::CalPlaneRate(KVector pt_1, KVector pt_2, KVector pt_3, KVector pt_4, double* error_val)
{
	double tmp_matrix[4][4];
	double std_matrix[4][4];

	KVector trans_pt_1, trans_pt_2, trans_pt_3, trans_pt_4;
	KVector target_pt, target_axis_1, target_axis_2;

	m_Translate.InitMatrix(tmp_matrix);
	m_Translate.InitMatrix(std_matrix);

	target_pt.x = 0.0;
	target_pt.y = 0.0;
	target_pt.z = 0.0;

	target_axis_1.x = 1000.0;
	target_axis_1.y = 0.0;
	target_axis_1.z = 0.0;

	target_axis_2.x = 0.0;
	target_axis_2.y = 1000.0;
	target_axis_2.z = 0.0;

	m_Translate.GetTran1Pt1Axis1Plane(tmp_matrix, pt_1, pt_2, pt_3,
		target_pt, target_axis_1, target_axis_2, std_matrix);

	m_Translate.EvaluateVector(pt_1, std_matrix, &trans_pt_1);
	m_Translate.EvaluateVector(pt_2, std_matrix, &trans_pt_2);
	m_Translate.EvaluateVector(pt_3, std_matrix, &trans_pt_3);
	m_Translate.EvaluateVector(pt_4, std_matrix, &trans_pt_4);

	(*error_val) = trans_pt_4.z;

    return ECO_OK;
}

int CCalculate::CalLinearRate(KVector pt_1, KVector pt_2, KVector pt_3, KVector* ret_pt, double* real_dev_len)
{
	KPosition origin_pos(pt_1.x, pt_1.y, pt_1.z);

	KStraight straight(origin_pos, KVector(pt_2.x - origin_pos.x, pt_2.y - origin_pos.y, pt_2.z - origin_pos.z));
	KPosition target_pos(pt_3.x, pt_3.y, pt_3.z);
	KPosition ret_pos;
	KVector tmp_vt;

	m_Translate.ComputePerPosition(straight, target_pos, &ret_pos);

	ret_pt->x = pt_3.x - ret_pos.x;
	ret_pt->y = pt_3.y - ret_pos.y;
	ret_pt->z = pt_3.z - ret_pos.z;

	tmp_vt.x = ret_pos.x;
	tmp_vt.y = ret_pos.y;
	tmp_vt.z = ret_pos.z;

    CalLengthTwoPoint(pt_3, tmp_vt, real_dev_len);

    return ECO_OK;
}
