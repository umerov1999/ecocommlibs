#include ".\kstraight.h"

KStraight::KStraight(void)
: m_BasePosition()
{
}

KStraight::~KStraight(void)
{
}

KStraight::KStraight(KPosition p0, KVector v0)
{
	m_BasePosition  = p0;

	v0.Nomalize();

	m_TangentVector = v0; 
}

/*-----------------------------------------------------------------------
클래스명		: KStraight
함수명			: SetBasePosition
기능			: Base Position 입력
매개변수		: KPosition base_position => 입력 Position
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KStraight::SetBasePosition(KPosition base_position)
{
	m_BasePosition = base_position;
}

/*-----------------------------------------------------------------------
클래스명		: KStraight
함수명			: SetTangentVector
기능			: 접선 입력
매개변수		: KVector tangent_vector => 입력 벡터
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KStraight::SetTangentVector(KVector tangent_vector)
{
	KVector temp;

	temp = tangent_vector;

	temp.Nomalize();

	m_TangentVector = temp;
}

/*-----------------------------------------------------------------------
클래스명		: KStraight
함수명			: GetBasePosition
기능			: Base position 리턴
매개변수		: 없음
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
KPosition KStraight::GetBasePosition(void)
{
	return m_BasePosition;
}

/*-----------------------------------------------------------------------
클래스명		: KStraight
함수명			: GetTangentVector
기능			: Tangent Vector 리턴
매개변수		: 엇음
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
KVector KStraight::GetTangentVector(void)
{
	return m_TangentVector;
}
