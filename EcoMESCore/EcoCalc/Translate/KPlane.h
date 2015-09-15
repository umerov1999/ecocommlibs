#pragma once
#include "KPosition.h"
#include "kvector.h"

class KPlane
{
public:
	KPlane(void);
	~KPlane(void);
	
	KPlane(KPosition p0, KVector v0);
	KPlane(KPosition position0, KPosition position1, KPosition position2);
	void ComputeCrossProduct(KVector leg1, KVector leg2, KVector* out_vec);
	void SetBasePosition(KPosition base_position);

private:
	KPosition m_BasePosition;
	KVector m_NormalVector;

public:
	void SetNormalVector(KVector normal_vector);
	KVector GetNormalVector(void);
	KPosition GetBasePosition(void);
};
