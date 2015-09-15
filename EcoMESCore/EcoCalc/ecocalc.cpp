#include "ecocalc.h"
#include "Calculate/Calculate.h"

// 20150807-강경완, calc 전역 멤버 선언, 이 놈으로 모든 계산 수행됨
CCalculate  g_Calculate;

EcoCalc::EcoCalc()
{
}

/*-----------------------------------------------------------------------
기능                  : 2선간 각도를 계산하여 반환함
매개변수               : Evector first_line_start_pt    : 1번선의 시작 점
                       Evector first_line_end_pt       : 1번선의 끝점
                       Evector second_line_start_pt    : 2번선의 시작 점
                       Evector second_line_end_pt      : 2번선의 끝점
                       double* angle                   : 계산된 각도 반환
리턴                  : error code
작성내역			     :
강경완-20150807,  최초작성
-----------------------------------------------------------------------*/
int EcoCalc::CalTwoLineAngle(Evector first_line_start_pt, Evector first_line_end_pt, Evector second_line_start_pt, Evector second_line_end_pt, double* angle)
{
    return g_Calculate.CalTwoLineAngle(
                KVector(first_line_start_pt.x, first_line_start_pt.y, first_line_start_pt.z),
                KVector(first_line_end_pt.x, first_line_end_pt.y, first_line_end_pt.z),
                KVector(second_line_start_pt.x, second_line_start_pt.y, second_line_start_pt.z),
                KVector(second_line_end_pt.x, second_line_end_pt.y, second_line_end_pt.z),
                angle);
}

/*-----------------------------------------------------------------------
기능                  : 3점간 각도 계산 후 반환
매개변수               : Evector origin_pt    : 두선분의 교차점
                       Evector pt_1         : 1번선의 끝점
                       Evector pt_2         : 2번선의 끝점
                       double* angle        : 계산된 각도 반환
리턴                  : error code
작성내역			     :
강경완-20150807,  최초작성
-----------------------------------------------------------------------*/
int EcoCalc::CalAngleThreePoint(Evector origin_pt, Evector pt_1, Evector pt_2, double* angle)
{
    return g_Calculate.CalAngleThreePoint(
                KVector(origin_pt.x, origin_pt.y, origin_pt.z),
                KVector(pt_1.x, pt_1.y, pt_1.z),
                KVector(pt_2.x, pt_2.y, pt_2.z),
                angle);
}

/*-----------------------------------------------------------------------
기능                  : 3점을 이루는 원의 원점 반환
매개변수               : Evector pt_1             : 점 1
                       Evector pt_2             : 점 2
                       Evector pt_3             : 점 3
                       Evector center_pt        : 원의 원점 반환
리턴                  : error code
작성내역			     :
강경완-20150807,  최초작성
-----------------------------------------------------------------------*/
int EcoCalc::CalCircleCenterThreePoint(Evector pt_1, Evector pt_2, Evector pt_3, Evector* center_pt)
{
    int iRet;
    KVector kCenPt;

    kCenPt.x = center_pt->x;
    kCenPt.y = center_pt->y;
    kCenPt.z = center_pt->z;
    iRet = g_Calculate.CalCircleCenterThreePoint(
                KVector(pt_1.x, pt_1.y, pt_1.z),
                KVector(pt_2.x, pt_2.y, pt_2.z),
                KVector(pt_3.x, pt_3.y, pt_3.z),
                &kCenPt);

    center_pt->x = kCenPt.x;
    center_pt->y = kCenPt.y;
    center_pt->z = kCenPt.z;

    return iRet;
}

/*-----------------------------------------------------------------------
기능                  : 3점을 이루는 호의 길이 반환
매개변수               : Evector pt_1             : 점 1
                       Evector pt_2             : 점 2
                       Evector pt_3             : 점 3
                       double* arc_len        : 호의 길이 반환
리턴                  : error code
작성내역			     :
강경완-20150807,  최초작성
-----------------------------------------------------------------------*/
int EcoCalc::CalArcLen(Evector pt_1, Evector pt_2, Evector pt_3, double* arc_len)
{
    return g_Calculate.CalArcLen(
                KVector(pt_1.x, pt_1.y, pt_1.z),
                KVector(pt_2.x, pt_2.y, pt_2.z),
                KVector(pt_3.x, pt_3.y, pt_3.z),
                arc_len);
}

/*-----------------------------------------------------------------------
기능                  : 2점을 이루는 선분의 길이 반환
매개변수               : Evector start_pt             : 점 1
                       Evector end_pt                : 점 2
                       double* length               : 선분 길이 반환
리턴                  : error code
작성내역			     :
강경완-20150807,  최초작성
-----------------------------------------------------------------------*/
int EcoCalc::CalLengthTwoPoint(Evector start_pt, Evector end_pt, double* length)
{
    return g_Calculate.CalLengthTwoPoint(
                KVector(start_pt.x, start_pt.y, start_pt.z),
                KVector(end_pt.x, end_pt.y, end_pt.z),
                length);
}

/*-----------------------------------------------------------------------
기능                  : pt1~3 으로 생성된 평면에 대해 pt_4 의 수선의 발을 내려 오차값 반환
                       * 1,2,3 순서로 반 시계방향으로 평면이 생성됨
                       * pt_4 가 평면의 법선 방향쪽이면 error_val 은 + 가 되고 반대쪽이면 - 임
매개변수               : Evector pt_1                : 평면을 이루는 점 1
                       Evector pt_2                : 평면을 이루는 점 2
                       Evector pt_3                : 평면을 이루는 점 3
                       Evector pt_4                : 계산을 위한 점
                       double* error_val           : 평면의 normal 방향으로의 오차값 반환
리턴                  : error code
작성내역			     :
강경완-20150807,  최초작성
-----------------------------------------------------------------------*/
int EcoCalc::CalPlaneRate(Evector pt_1, Evector pt_2, Evector pt_3, Evector pt_4, double* error_val)
{
    return g_Calculate.CalPlaneRate(
                KVector(pt_1.x, pt_1.y, pt_1.z),
                KVector(pt_2.x, pt_2.y, pt_2.z),
                KVector(pt_3.x, pt_3.y, pt_3.z),
                KVector(pt_4.x, pt_4.y, pt_4.z),
                error_val);
}

/*-----------------------------------------------------------------------
기능                  : pt1~2 로 생성된 선에 대해 pt_3 의 수선의 발을 내린 교차점과 오차값 반환
매개변수               : Evector pt_1                : 선을 이루는 점 1
                       Evector pt_2                : 선을 이루는 점 2
                       Evector pt_3                : 계산을 위한 점
                       Evector* ret_pt             : pt_3에서 pt1-2로 생성된 선분에 대해 수선의 발을 내린 교차점 반환
                       double* real_dev_len        : pt_3 에서 ret_pt 까지의 길이 반환
리턴                  : error code
작성내역			     :
강경완-20150807,  최초작성
-----------------------------------------------------------------------*/
int EcoCalc::CalLinearRate(Evector pt_1, Evector pt_2, Evector pt_3, Evector* ret_pt, double* real_dev_len)
{
    int iRet;
    KVector kRetPt;

    kRetPt.x = ret_pt->x;
    kRetPt.y = ret_pt->y;
    kRetPt.z = ret_pt->z;
    iRet = g_Calculate.CalLinearRate(
                KVector(pt_1.x, pt_1.y, pt_1.z),
                KVector(pt_2.x, pt_2.y, pt_2.z),
                KVector(pt_3.x, pt_3.y, pt_3.z),
                &kRetPt, real_dev_len);

    ret_pt->x = kRetPt.x;
    ret_pt->y = kRetPt.y;
    ret_pt->z = kRetPt.z;

    return iRet;
}
