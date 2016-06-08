#pragma once

//-->최조영-20151217, 파이프 설계점 속성 구조체[PROD-384]
typedef struct _object_pipe_property
{
	struct _object_pipe_property()
	{
		dblObjDiameter = 0.0;
		strObjComponent = _T("");
		strObjDescription = _T("");
		strDirect = _T("");
	}

	_object_pipe_property& operator = (const _object_pipe_property &other)
	{
		dblObjDiameter = other.dblObjDiameter;
		strObjComponent = other.strObjComponent;
		strDirect = other.strDirect;
		strObjDescription = other.strObjDescription;

		return *this;
	}

	double dblObjDiameter;
	CString strObjComponent;
	CString strObjDescription;
	CString strDirect;
} OBJECT_PIPE_PROPERTY;
//<--최조영-20151109


typedef struct _WASAIRDIRECTION
{
	HPoint vec;
	BOOL bAirDirection;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		if(bSendNRecv == TRUE)
		{
			vec.serialize(ar, version, bSendNRecv);
			ar & bAirDirection;
		}
		else
		{
			vec.serialize(ar, version, bSendNRecv);
			ar & bAirDirection;
		}
	}

} WASAIRDIRECTION;

typedef struct _WASNormalAxis
{
	struct _WASAIRDIRECTION normalU;
	struct _WASAIRDIRECTION normalV;
	struct _WASAIRDIRECTION normalW;

	void InitWorld()
	{
		normalU.bAirDirection = TRUE;
		normalU.vec.x = 1.0;
		normalU.vec.y = 0.0;
		normalU.vec.z = 0.0;

		normalV.bAirDirection = TRUE;
		normalV.vec.x = 0.0;
		normalV.vec.y = 1.0;
		normalV.vec.z = 0.0;

		normalW.bAirDirection = TRUE;
		normalW.vec.x = 0.0;
		normalW.vec.y = 0.0;
		normalW.vec.z = 1.0;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		if(bSendNRecv == TRUE)
		{
			normalU.serialize(ar, version, bSendNRecv);
			normalV.serialize(ar, version, bSendNRecv);
			normalW.serialize(ar, version, bSendNRecv);
		}
		else
		{
			normalU.serialize(ar, version, bSendNRecv);
			normalV.serialize(ar, version, bSendNRecv);
			normalW.serialize(ar, version, bSendNRecv);
		}
	}

}WASNormalAxis;