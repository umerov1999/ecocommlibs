#pragma once
#include "KPosition.h"

#include "KPosition.h"
#include "kvector.h"

class KStraight
{
public:
	KStraight(void);
	~KStraight(void);

	KStraight(KPosition p0, KVector v0);
	void SetBasePosition(KPosition base_position);
	void SetTangentVector(KVector tangent_vector);

private:
	KPosition m_BasePosition;
	KVector m_TangentVector;
public:
	KPosition GetBasePosition(void);
	KVector GetTangentVector(void);
};
