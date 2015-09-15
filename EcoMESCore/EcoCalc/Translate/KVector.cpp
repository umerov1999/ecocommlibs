#include ".\kvector.h"

#include "math.h"

KVector::KVector(void)
: x(0)
, y(0)
, z(0)
{
}

KVector::~KVector(void)
{
}

KVector::KVector(double ix, double iy, double iz)
{
	x = ix;
	y = iy;
	z = iz;
}

/*-----------------------------------------------------------------------
클래스명		: KVector
함수명			: Nomalize
기능			: 벡터 정규화
매개변수		: 없음
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KVector::Nomalize(void)
{
	double vec_len = 0.0;

	vec_len = GetLength();

	if(vec_len == 0)
		return;

	x = x/vec_len;
	y = y/vec_len;
	z = z/vec_len;

	return;
}

/*-----------------------------------------------------------------------
클래스명		: KVector
함수명			: GetLength
기능			: 벡터 길이 산출
매개변수		: 벡터 길이
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
double KVector::GetLength(void)
{
	return sqrt((x * x) + (y * y) + (z * z));
}

/*-----------------------------------------------------------------------
클래스명		: KVector
함수명			: operator=
기능			: =
매개변수		: const KVector vec => 입력벡터
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KVector::operator=(const KVector vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;

}

/*-----------------------------------------------------------------------
클래스명		: KVector
함수명			: operator=
기능			: =
매개변수		: KPosition pos => 입력 position
작성일자		: 2007-04-28
마지막 수정일자	: 2007-04-28
작성자			: 김덕은
-----------------------------------------------------------------------*/
void KVector::operator=(KPosition pos)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}
