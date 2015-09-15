#pragma once
#include "kposition.h"

class KSphere
{
public:
	KSphere(void);
	~KSphere(void);
	KPosition m_Center;
	double m_Radius;
	KSphere(KPosition center, double radius);
};
