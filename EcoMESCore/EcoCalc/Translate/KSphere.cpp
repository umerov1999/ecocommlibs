#include ".\ksphere.h"

KSphere::KSphere(void)
: m_Radius(0)
{
}

KSphere::~KSphere(void)
{
}

KSphere::KSphere(KPosition center, double radius)
{
	m_Center = center;
	m_Radius = radius;
}
