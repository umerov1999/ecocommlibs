#include ".\kplane.h"

KPlane::KPlane(void)
: m_BasePosition()
{
}

KPlane::~KPlane(void)
{
}

KPlane::KPlane(KPosition p0, KVector v0)
{
	m_BasePosition  = p0;
	v0.Nomalize(); 	
	m_NormalVector = v0; 
}

/*-----------------------------------------------------------------------
클래스명		: KPlane
함수명			: KPlane
기능			: 3점을 지나는 평면 생성
매개변수		: KPosition position0 => 점 1
				  KPosition position1 => 점 2
				  KPosition position2 => 점 3
리턴			: 없음
작성일자		: 2007-04-27
마지막 수정일자	: 2007-04-27
작성자			: 김덕은
-----------------------------------------------------------------------*/
KPlane::KPlane(KPosition position0, KPosition position1, KPosition position2)
{
	KVector leg1, leg2;
	KVector normal_vec;

	leg1 = position1 - position0;
	leg2 = position2 - position0;

	ComputeCrossProduct(leg1, leg2, &normal_vec);
	m_BasePosition = position0;

	normal_vec.Nomalize();

    m_NormalVector = normal_vec;

}

/*-----------------------------------------------------------------------
클래스명		: KPlane
함수명			: ComputeCrossProduct
기능			: 두 벡터의 외적계산
매개변수		: KVector leg1 => 벡터 1
				  KVector leg2 => 벡터 2
				  KVector* out_vec => 결과 벡터
리턴			: 없음
작성일자		: 2007-04-27
마지막 수정일자	: 2007-04-27
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KPlane::ComputeCrossProduct(KVector leg1, KVector leg2, KVector* out_vec)
{
	out_vec->x = leg1.y * leg2.z - leg1.z * leg2.y;
	out_vec->y = leg1.z * leg2.x - leg1.x * leg2.z;
	out_vec->z = leg1.x * leg2.y - leg1.y * leg2.x;
}

/*-----------------------------------------------------------------------
클래스명		: KPlane
함수명			: SetBasePosition
기능			: Base Position 입력
매개변수		: KPosition base_position => 입력 position
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KPlane::SetBasePosition(KPosition base_position)
{
	m_BasePosition = base_position;

}

/*-----------------------------------------------------------------------
클래스명		: KPlane
함수명			: SetNormalVector
기능			: Normal Vector 입력
매개변수		: KVector normal_vector => 입력벡터
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KPlane::SetNormalVector(KVector normal_vector)
{
	KVector temp;

	temp = normal_vector;

	temp.Nomalize();

	m_NormalVector = temp;
}

/*-----------------------------------------------------------------------
클래스명		: KPlane
함수명			: GetNormalVector
기능			: Normal Vector 리턴
매개변수		: 없음
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
KVector KPlane::GetNormalVector(void)
{
	return m_NormalVector;
}

/*-----------------------------------------------------------------------
클래스명		: KPlane
함수명			: GetBasePosition
기능			: Base Position 리턴
매개변수		: 없음
리턴			: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
KPosition KPlane::GetBasePosition(void)
{
	return m_BasePosition;
}
