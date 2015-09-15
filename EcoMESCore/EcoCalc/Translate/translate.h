#pragma once

#include "KVector.h"
#include "KPosition.h"
#include "KStraight.h"
#include "KSphere.h"
#include "KPlane.h"

#define		M_PI		3.1415926
#define		X_DIR		0
#define		Y_DIR		1
#define		Z_DIR		2


class CTranslate
{
public:
	CTranslate(void);
	~CTranslate(void);

public:
	void GetMatrixTranslate(double in_matrix[4][4], double tx, double ty, double tz, double out_matrix[4][4]);
	void GetMatrixRotateOffAxis(double in_matrix[4][4], double vec_x, double vec_y, double vec_z, double angle, double out_matrix[4][4]);
	
	void MatrixMultiply(double in_matrix1[4][4], double in_matrix2[4][4], double out_matrix[4][4]);
	
	void ComputeCrossProduct(KVector leg1, KVector leg2, KVector* out_vec);
	double ComputeDotProduct(KVector leg1, KVector leg2);
	void ComputeVectorNormalize(KVector in_vector, KVector* out_vector);
	double ComputeVectorLength(KVector vec);
	double ComputeVectorAngle(KVector leg1, KVector leg2);

public:
	void InitMatrix(double tmatrix[4][4]);
	void GetTran1Pt1Axis(bool level, double inmatrix[4][4], KVector OriginPt, KVector OriginAxis, KVector TargetPt, KVector TargetAxis, double tmatrix[4][4]);
	void EvaluateVector(KVector in_pt, double tmatrix[4][4], KVector* out_pt);
	void GetTran1Pt1Axis1Plane(double in_matrix[4][4], KVector OriginPt, KVector OriginAxis, KVector OriginPlane, KVector TargetPt, KVector TargetAxis, KVector TargetPlane, double out_matrix[4][4]);
	void Compute2PtTarget(KVector pt1, KVector pt2, double length_pt1_pt2, double length_pt2_end, double* error, KVector* target_pt);
	int InvMatrix(int n, const double *A, double *b);
	int Intersect2Planes(KPlane plane1, KPlane plane2, KStraight* result_straight);
	double ComputeDotProduct(KVector leg1, KPosition leg2);
	int IntersectStraightSphere(KStraight straight, KSphere sphere, KPosition result_positions[2]);
	double ComputeSquare(double value);
	double ComputePerPosition(KStraight straight, KPosition position, KPosition* result_position);
	double ComputeDotProduct(KPosition leg1, KPosition leg2);
	double ComputeDistance(KPosition position1, KPosition position2);
        bool GetTran2PtRotateAngle(KPosition AxisPt1, KPosition AxisPt2, KPosition RotatePt, double Angle, double tmatrix[4][4]);
        bool GetTran2PtRotate(KPosition AxisPt1, KPosition AxisPt2, KPosition RotatePt, int Direction, double Distance, double tmatrix[4][4]);
	void EvaluatePosition(KPosition in_pt, double tmatrix[4][4], KPosition* out_pt);
	double Compute2PlanesAngle(KPosition axis_pt1, KPosition axis_pt2, KPosition plane_pt1, KPosition plane_pt2);
	double Compute2PlanesAngle(KPlane plane1 , KPlane plane2);
};
