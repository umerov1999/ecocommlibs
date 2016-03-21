#pragma once
/*-----------------------------------------------------------------------
 본 클래스는 EcoMarine G2 버전의 데이터베이스 입출력 자료구조를 가지고 있는
클래스이다. 각 클래스는 각각의 테이블에 해당하는 필드값을 가지고 있다.
아래 클래스는 자료구조만을 가지고 있고 실제 입출력을 담당하는 CEcoDataBaseManager
클래스에서 데이터의 입출력을 위해 사용된다.
/*-----------------------------------------------------------------------

/* Definition of Data Prototype */
//
//
#if defined(ECOTERMINALTBCLIENT) || defined(ECO_SERVER_MANAGER) || defined(ECO_BLOCK_G1) || defined(ECO_BLOCK_G2_FOR_CLIENT) || defined(ECO_BLOCK_G3_FOR_CLIENT) || defined(ECO_BLOCK_G2_FOR_SERVER) || defined(ECO_OTS_G1) || defined(ECO_OTS_G2_FOR_CLIENT) || defined(ECO_OTS_G3_FOR_CLIENT) || defined(ECO_OTS_G2_FOR_SERVER) || defined(ECO_PM_VIEWING)
#include "StdAfx.h"
#include "HUtility.h"
#else
#include "HPoint.h"
#endif

typedef struct _object_property
{
	CString object_type;
	CString object_name;
	CString object_side;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(object_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(object_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(object_side);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			object_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			object_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			object_side = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}
} OBJECT_PROPERTY;

//-->하남국-20151109, 부재이름 정보를 담는 구조체 생성[PROD-279]
typedef struct _object_name_property
{
	_object_name_property& operator = (const _object_name_property &other)
	{
		strObjType = other.strObjType;
		strObjName = other.strObjName;
		strObjProductName = other.strObjProductName;
		strObjSuperiorName = other.strObjSuperiorName;
		strObjModelName = other.strObjModelName;
		strObjModuleName = other.strObjModuleName;
		return *this;
	}

	CString strObjType;
	CString strObjName;
	CString strObjProductName;
	CString strObjSuperiorName;
	CString strObjModelName;
	CString strObjModuleName;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(strObjType);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(strObjName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(strObjProductName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(strObjSuperiorName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(strObjModelName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(strObjModuleName);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			strObjType = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			strObjName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			strObjProductName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			strObjSuperiorName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			strObjModelName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			strObjModuleName = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}
} OBJECT_NAME_PROPERTY;
//<--하남국-20151109



class AFX_EXT_API CEcoTreeBaseRecordSet : public CObject
{

public:
	CEcoTreeBaseRecordSet();
	~CEcoTreeBaseRecordSet();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_id;
			ar & m_parent_id;
			ar & m_depth;
			ar & m_f_id;
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
		}
		else
		{
			ar & m_id;
			ar & m_parent_id;
			ar & m_depth;
			ar & m_f_id;
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

protected:
	//-->하남국-20120719
	UINT											m_id;
	UINT											m_parent_id;
	UINT											m_depth;
	UINT											m_f_id;
	CString											m_name;
	//<--하남국-20120719

public:
	//-->하남국-20120719
	void SetID(UINT id);
	UINT GetID();

	void SetParentID(UINT parent_id);
	UINT GetParentID();

	void SetDepth(UINT depth);
	UINT GetDepth();

	void SetFID(UINT f_id);
	UINT GetFID();

	void SetName(CString name);
	CString GetName();
	//<--하남국-20120719
};

class AFX_EXT_API CEcoRefBaseRecordSet : public CObject
{
public:
	CEcoRefBaseRecordSet();
	~CEcoRefBaseRecordSet();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		ar & m_id;
		ar & m_f_id;
	}

protected:
	//-->하남국-20120719
	UINT	m_id;
	UINT	m_f_id;
	//<--하남국-20120719

public:
	//-->하남국-20120719
	void SetID(UINT id);
	UINT GetID();

	void SetFID(UINT f_id);
	UINT GetFID();
	//<--하남국-20120719
};

class AFX_EXT_API CShipNoInfoRecordSet : public CEcoTreeBaseRecordSet
{
private:
	COleDateTime m_create_date;
	CString m_bActive;
	COleDateTime m_update_date;
	CString m_gentab;
	int m_gentab_size;
public:
	CShipNoInfoRecordSet();
	~CShipNoInfoRecordSet();

	CShipNoInfoRecordSet& operator = (const CShipNoInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_create_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_bActive);
			ar & stdStr;
			ar & m_update_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_gentab);
			ar & stdStr;
			ar & m_gentab_size;
		}
		else
		{
			ar & m_create_date.m_dt;
			ar & stdStr;
			m_bActive = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_update_date.m_dt;
			ar & stdStr;
			m_gentab = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_gentab_size;
		}
	}

public:
	void SetCreateDate(COleDateTime create_date);
	COleDateTime GetCreateDate();

	void SetbActive(CString bActive);
	CString GetbActive();

	void SetUpdateDate(COleDateTime update_date);
	COleDateTime GetUpdateDate();

	void SetGentab(CString gentab);
	CString GetGentab();

	void SetGentabSize(int gentab_size);
	int GetGentabSize();
};

/*-----------------------------------------------------------------------
클래스명		: CTemplateTbRecordSet
기능			: 도면 템플릿 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-01-19
마지막 수정일자	: 2010-01-19
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CTemplateTbRecordSet : public CObject
{
private:
	CString											m_template_name;
	CString											m_template;
	int												m_template_size;
	CString											m_description;
	CString											m_thumbnail_path;
	int												m_thumbnail_size;
public:
	CTemplateTbRecordSet();
	~CTemplateTbRecordSet();

	CTemplateTbRecordSet& operator = (const CTemplateTbRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_template_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_template);
			ar & stdStr;
			ar & m_template_size;
			stdStr = CStringConverter::CStringWToCStringA(m_description);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_thumbnail_path);
			ar & stdStr;
			ar & m_thumbnail_size;
		}
		else
		{
			ar & stdStr;
			m_template_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_template = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_template_size;
			ar & stdStr;
			m_description = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_thumbnail_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_thumbnail_size;
		}
	}

public:
	void SetTemplateName(CString template_name);
	CString GetTemplateName();

	void SetTemplate(CString template_path);
	CString GetTemplate();

	void SetTemplateSize(int template_size);
	int GetTemplateSize();

	void SetDescription(CString description);
	CString GetDescription();

	void SetThumbnailPath(CString thumbnail_path);
	CString GetThumbnailPath();

	void SetThumbnailSize(int thumbnail_size);
	int GetThumbnailSize();

};

/*-----------------------------------------------------------------------
클래스명		: CSystemDrawingRecordSet
기능			: tb_system_drawing 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2011-03-15
마지막 수정일자	: 2011-03-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CSystemDrawingRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_name;
	CString											m_system_stored_type;
	CString											m_format_type;
	CString											m_data_path;
	int												m_data_size;
public:
	CSystemDrawingRecordSet();
	~CSystemDrawingRecordSet();

	CSystemDrawingRecordSet& operator = (const CSystemDrawingRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_system_stored_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_format_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_data_path);
			ar & stdStr;
			ar & m_data_size;
		}
		else
		{
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_system_stored_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_format_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_data_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_data_size;
		}
	}

public:
	void SetDrawingName(CString val);
	CString GetDrawingName();

	void SetSystemStoredType(CString val);
	CString GetSystemStoredType();

	void SetFormatType(CString val);
	CString GetFormatType();

	void SetDataPath(CString val);
	CString GetDataPath();

	void SetDataSize(int val);
	int GetDataSize();
};



/*-----------------------------------------------------------------------
클래스명		: CUserInfoRecordSet
기능			: tb_user_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-01-29
마지막 수정일자	: 2010-07-20
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CUserInfoRecordSet : public CObject
{
private:
	CString											m_id;
	CString											m_company;
	CString											m_dept;
	CString											m_name;
	CString											m_approval;
	CString											m_select_priv;
	CString											m_update_priv;
	CString											m_modify_priv;
	CString											m_password;
	CString											m_phone_no;
	CString											m_delete_priv;

//2012-02-02 by ha
	CString											m_update_priv_tpl;		
	CString											m_modify_priv_tpl;
	CString											m_delete_priv_tpl;

	//-->하남국-20130108, 기존 스케줄 권한들을 EcoProductionManager 에서 프로젝트 추가, 수정, 삭제 권한으로 변경.
	CString											m_modify_priv_pjt_in_epm;
	CString											m_delete_priv_pjt_in_epm;
	CString											m_update_priv_pjt_in_epm;
// 	CString											m_modify_priv_scd;
// 	CString											m_delete_priv_scd;
// 	CString											m_mng_res_priv_scd;
	//<--하남국-20130108
	//-->손지훈-20131111, 어셈블리 트리 관리 권한 추가
	CString											m_at_mng_priv_epm;
	//<--손지훈-20131111
	//-->최조영-20140411
	CString											m_admin_priv;
	//<--최조영-20140411
public:
	CUserInfoRecordSet();
	~CUserInfoRecordSet();

	CUserInfoRecordSet& operator = (const CUserInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_id);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_company);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_dept);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_approval);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_select_priv);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_update_priv);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_modify_priv);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_password);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_phone_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_delete_priv);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_update_priv_tpl);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_modify_priv_tpl);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_delete_priv_tpl);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_modify_priv_pjt_in_epm);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_delete_priv_pjt_in_epm);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_update_priv_pjt_in_epm);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_at_mng_priv_epm);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_admin_priv);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_company = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_dept = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_approval = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_select_priv = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_update_priv = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_modify_priv = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_password = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_phone_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_delete_priv = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_update_priv_tpl = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_modify_priv_tpl = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_delete_priv_tpl = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_modify_priv_pjt_in_epm = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_delete_priv_pjt_in_epm = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_update_priv_pjt_in_epm = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_at_mng_priv_epm = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_admin_priv = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetUserID(CString id);
	CString GetUserID();

	void SetCompany(CString company);
	CString GetCompany();

	void SetDept(CString dept);
	CString GetDept();

	void SetName(CString name);
	CString GetName();

	void SetApproval(CString bApproved);
	CString GetApproval();

	void SetSelectPriv(CString priv);
	CString GetSelectPriv();

	void SetModifyPriv(CString priv);
	CString GetModifyPriv();

	void SetUpdatePriv(CString priv);
	CString GetUpdatePriv();

	void SetDeletePriv(CString priv);
	CString GetDeletePriv();

	void SetPassword(CString password);
	CString GetPassword();

	void SetPhoneNo(CString no);
	CString GetPhoneNo();

//2012-02-02 by ha
	void SetUpdatePrivTpl(CString no);
	CString GetUpdatePrivTpl();

	void SetModifyPrivTpl(CString no);
	CString GetModifyPrivTpl();

	void SetDeletePrivTpl(CString no);
	CString GetDeletePrivTpl();

	//-->하남국-20130108, 기존 스케줄 권한들을 EcoProductionManager 에서 프로젝트 추가, 수정, 삭제 권한으로 변경.
	void SetModifyPrivPjtInEPM(CString no);
	CString GetModifyPrivPjtInEPM();

	void SetDeletePrivPjtInEPM(CString no);
	CString GetDeletePrivPjtInEPM();

	void SetUpdatePrivPjtInEPM(CString no);
	CString GetUpdatePrivPjtInEPM();

// 	void SetModifyPrivScd(CString no);
// 	CString GetModifyPrivScd();
// 
// 	void SetDeletePrivScd(CString no);
// 	CString GetDeletePrivScd();
// 
// 	void SetMngResPrivScd(CString no);
// 	CString GetMngResPrivScd();
	//<--하남국-20130108
	//-->손지훈-20131111, 어셈블리 트리 관리 권한 추가
	void SetAtMngPrivEpm(CString no);
	CString GetAtMngPrivEpm();
	//<--손지훈-20131111, 어셈블리 트리 관리 권한 추가
	//-->최조영-20140411
	void SetAdminPriv(CString strAdmin);
	CString GetAdminPriv();
	//<--최조영-20140411
};


/*-----------------------------------------------------------------------
클래스명		: CMeasurePointRecordSet
기능			: MeasurePoint 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-13
마지막 수정일자	: 2009-10-13
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CMeasurePointRecordSet : public CEcoTreeBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_measure_name;
	//<--하남국-20120719
	CString											m_user_id;
	COleDateTime									m_measure_date;
	CString											m_measure_data_up_down_path;
	int												m_measure_data_size;
	//-->하남국-20120719
	int												m_measure_data_count;
	//<--하남국-20120719
public:
	CMeasurePointRecordSet();
	~CMeasurePointRecordSet();

	CMeasurePointRecordSet& operator = (const CMeasurePointRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_user_id);
			ar & stdStr;
			ar & m_measure_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_data_up_down_path);
			ar & stdStr;
			ar & m_measure_data_size;
			ar & m_measure_data_count;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_user_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_measure_date.m_dt;
			ar & stdStr;
			m_measure_data_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_measure_data_size;
			ar & m_measure_data_count;
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	void SetUserID(CString user_id);
	CString GetUserID();

	//-->하남국-20120719
// 	void SetMeasureName(CString measure_name);
// 	CString GetMeasureName();
	//<--하남국-20120719

	void SetMeasureDate(COleDateTime measure_date);
	COleDateTime GetMeasureDate();

	void SetMeasureData(CString measure_data);
	CString GetMeasureData();

	void SetMeasureDataSize(int measure_data_size);
	int GetMeasureDataSize();

	//-->하남국-20120719
	void SetMeasureDataCount(int measure_data_count);
	int GetMeasureDataCount();
	//<--하남국-20120719
};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockRelationInfoRecordSet
기능			: EcoBlockRelationInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-14
마지막 수정일자	: 2009-10-14
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockRelationInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_design_point_no;
	CString											m_measure_point_no;
	CString											m_connection_type;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719

public:
	CEcoBlockRelationInfoRecordSet();
	~CEcoBlockRelationInfoRecordSet();

	CEcoBlockRelationInfoRecordSet& operator = (const CEcoBlockRelationInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_design_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_connection_type);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_design_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_connection_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetDesignPointName(CString design_point_name);
	CString GetDesignPointName();

	void SetMeasurePointName(CString measure_point_name);
	CString GetMeasurePointName();

	void SetConnectionType(CString connection_type);
	CString GetConnectionType();
	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockProjectRecordSet
기능			: EcoBlock Project 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-12
마지막 수정일자	: 2009-10-12
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockProjectRecordSet : public CEcoTreeBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_project_type;
	CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>		m_object_query_list;
	CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>		m_object_delete_list;
	int												m_object_delete_list_size;
	CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>		m_object_hide_list;
	int												m_object_hide_list_size;
	CString											m_hoops_segment_up_down_path;
	int												m_hoops_segment_size;
	//-->하남국-20120826-03-584
	double											m_design_model_modeling_matrix[16];
	//float											m_design_model_modeling_matrix[16];
	//<--하남국-20120826-03-584
	int												m_design_model_modeling_matrix_size;
	//-->하남국-20120719
	UINT											m_f_id_measure_point;
	//CString											m_measure_name;
	//<--하남국-20120719
	//-->하남국-20120826-03-584
	double											m_measure_model_modeling_matrix[16];
	//float											m_measure_model_modeling_matrix[16];
	//<--하남국-20120826-03-584
	int												m_measure_model_modeling_matrix_size;
	int												m_projection_face_cnt;
	CString											m_success_info;
	COleDateTime									m_update_date;
	COleDateTime									m_success_info_update_date;
	CString											m_update_user_id;
	CString											m_success_remark;
	double											m_tolerance_x_min;
	double											m_tolerance_x_max;
	double											m_tolerance_y_min;
	double											m_tolerance_y_max;
	double											m_tolerance_z_min;
	double											m_tolerance_z_max;
	CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>		m_related_msr_part_list;
	int												m_related_msr_part_len;
	CString											m_hsf_path;
	int												m_hsf_size;

private:
	int m_nObjectQueryListCnt;
	int m_nObjectDeleteListCnt;
	int m_nObjectHideListCnt;
	int m_nRelatedMsrPartListCnt;


public:
	CEcoBlockProjectRecordSet();
	~CEcoBlockProjectRecordSet();

	CEcoBlockProjectRecordSet& operator = (const CEcoBlockProjectRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_project_type);
			ar & stdStr;

			m_nObjectQueryListCnt = m_object_query_list.GetCount();
			ar & m_nObjectQueryListCnt;
			for(int i = 0; i < m_nObjectQueryListCnt; i++)
				m_object_query_list.GetAt(i).serialize(ar, version, bSendNRecv);

			m_nObjectDeleteListCnt = m_object_delete_list.GetCount();
			ar & m_nObjectDeleteListCnt;
			for(int i = 0; i < m_nObjectDeleteListCnt; i++)
				m_object_delete_list.GetAt(i).serialize(ar, version, bSendNRecv);

			ar & m_object_delete_list_size;

			m_nObjectHideListCnt = m_object_hide_list.GetCount();
			ar & m_nObjectHideListCnt;
			for(int i = 0; i < m_nObjectHideListCnt; i++)
				m_object_hide_list.GetAt(i).serialize(ar, version, bSendNRecv);

			ar & m_object_hide_list_size;
			stdStr = CStringConverter::CStringWToCStringA(m_hoops_segment_up_down_path);
			ar & stdStr;
			ar & m_hoops_segment_size;

			for(int i = 0; i < 16; i++)
				ar & m_design_model_modeling_matrix[i];

			ar & m_design_model_modeling_matrix_size;
			ar & m_f_id_measure_point;

			for(int i = 0; i < 16; i++)
				ar & m_measure_model_modeling_matrix[i];

			ar & m_measure_model_modeling_matrix_size;
			ar & m_projection_face_cnt;
			stdStr = CStringConverter::CStringWToCStringA(m_success_info);
			ar & stdStr;
			ar & m_update_date.m_dt;
			ar & m_success_info_update_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_update_user_id);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_success_remark);
			ar & stdStr;
			ar & m_tolerance_x_min;
			ar & m_tolerance_x_max;
			ar & m_tolerance_y_min;
			ar & m_tolerance_y_max;
			ar & m_tolerance_z_min;
			ar & m_tolerance_z_max;

			m_nRelatedMsrPartListCnt = m_related_msr_part_list.GetCount();
			ar & m_nRelatedMsrPartListCnt;
			for(int i = 0; i < m_nRelatedMsrPartListCnt; i++)
				m_related_msr_part_list.GetAt(i).serialize(ar, version, bSendNRecv);

			ar & m_related_msr_part_len;
			stdStr = CStringConverter::CStringWToCStringA(m_hsf_path);
			ar & stdStr;
			ar & m_hsf_size;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_project_type = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nObjectQueryListCnt;
			OBJECT_PROPERTY stru; m_object_query_list.RemoveAll();
			for(int i = 0; i < m_nObjectQueryListCnt; i++)
			{
				stru.serialize(ar, version, bSendNRecv);
				m_object_query_list.Add(stru);
			}

			ar & m_nObjectDeleteListCnt;
			m_object_delete_list.RemoveAll();
			for(int i = 0; i < m_nObjectDeleteListCnt; i++)
			{
				stru.serialize(ar, version, bSendNRecv);
				m_object_delete_list.Add(stru);
			}

			ar & m_object_delete_list_size;

			ar & m_nObjectHideListCnt;
			m_object_hide_list.RemoveAll();
			for(int i = 0; i < m_nObjectHideListCnt; i++)
			{
				stru.serialize(ar, version, bSendNRecv);
				m_object_hide_list.Add(stru);
			}

			ar & m_object_hide_list_size;
			ar & stdStr;
			m_hoops_segment_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_hoops_segment_size;

			for(int i = 0; i < 16; i++)
				ar & m_design_model_modeling_matrix[i];

			ar & m_design_model_modeling_matrix_size;
			ar & m_f_id_measure_point;

			for(int i = 0; i < 16; i++)
				ar & m_measure_model_modeling_matrix[i];

			ar & m_measure_model_modeling_matrix_size;
			ar & m_projection_face_cnt;
			ar & stdStr;
			m_success_info = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_update_date.m_dt;
			ar & m_success_info_update_date.m_dt;
			ar & stdStr;
			m_update_user_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_success_remark = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_tolerance_x_min;
			ar & m_tolerance_x_max;
			ar & m_tolerance_y_min;
			ar & m_tolerance_y_max;
			ar & m_tolerance_z_min;
			ar & m_tolerance_z_max;

			ar & m_nRelatedMsrPartListCnt;
			m_related_msr_part_list.RemoveAll();
			for(int i = 0; i < m_nRelatedMsrPartListCnt; i++)
			{
				stru.serialize(ar, version, bSendNRecv);
				m_related_msr_part_list.Add(stru);
			}

			ar & m_related_msr_part_len;
			ar & stdStr;
			m_hsf_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_hsf_size;
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetProjectType(CString project_type);
	CString GetProjectType();

	void SetObjectQueryList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY&>* object_query_list);
	void GetObjectQueryList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>* object_query_list);

	void SetObjectDeleteList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY&>* object_delete_list);
	void GetObjectDeleteList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>* object_delete_list);

	void SetObjectHideList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY&>* object_hide_list);
	void GetObjectHideList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>* object_hide_list);

	void SetHoopsSegment(CString upload_file_path);
	CString GetHoopsSegment();

	//-->하남국-20120826-03-584
	void SetDesignModelModelingMatrix(double* matrix);
	void GetDesignModelModelingMatrix(double* matrix);
	// 	void SetDesignModelModelingMatrix(float* matrix);
	// 	void GetDesignModelModelingMatrix(float* matrix);
	//<--하남국-20120826-03-584

	//-->하남국-20120719
	void SetMeasurePointTbID(UINT f_id_measure_point);
	UINT GetMeasurePointTbID();
	//<--하남국-20120719

	//-->하남국-20120826-03-584
	void SetMeasureModelModelingMatrix(double* matrix);
	void GetMeasureModelModelingMatrix(double* matrix);
	// 	void SetMeasureModelModelingMatrix(float* matrix);
	// 	void GetMeasureModelModelingMatrix(float* matrix);
	//<--하남국-20120826-03-584

	void SetProjectionFaceCnt(int face_cnt);
	int GetProjectionFaceCnt();

	void SetSuccessInfo(CString success_info);
	CString GetSuccessInfo();

	void SetUpdateDate(COleDateTime date);
	COleDateTime GetUpdateDate();

	void SetUpdateUserID(CString id);
	CString GetUpdateUserID();

	void SetObjectDeleteListSize(int size);
	int GetObjectDeleteListSize();

	void SetObjectHideListSize(int size);
	int GetObjectHideListSize();

	void SetHoopsSegmentSize(int size);
	int GetHoopsSegmentSize();

	void SetDesignModelMMatirxSize(int size);
	int GetDesignModelMMatirxSize();

	void SetMeasureModelMMatirxSize(int size);
	int GetMeasureModelMMatirxSize();

	void SetSuccessRemark(CString remark);
	CString GetSuccessRemark();

	void SetSuccessInfoUpdateDate(COleDateTime date);
	COleDateTime GetSuccessInfoUpdateDate();

	void SetTolerance(double x_min, double x_max, double y_min, double y_max, double z_min, double z_max);
	void GetTolerance(double* x_min, double* x_max, double* y_min, double* y_max, double* z_min, double* z_max);

	void SetRelatedMsrPartList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY&>* related_msr_part_list);
	void GetRelatedMsrPartList(CArray<OBJECT_PROPERTY, OBJECT_PROPERTY>* related_msr_part_list);

	void SetRelatedMsrPartListSize(int size);
	int GetRelatedMsrPartListSize();

	void SetHsfPath(CString path);
	CString GetHsfPath();

	void SetHsfSize(int size);
	int GetHsfSize();

};


/*-----------------------------------------------------------------------
클래스명		: CEcoBlockDesignPointGeneralInfoRecordSet
기능			: EcoBlockDesignPointGeneralInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-14
마지막 수정일자	: 2009-10-14
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockDesignPointGeneralInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_design_point_no;
	double											m_x_coord;
	double											m_y_coord;
	double											m_z_coord;
	CString											m_create_method;
	//-->하남국-20151109
	OBJECT_NAME_PROPERTY									m_parent_part_one;
	OBJECT_NAME_PROPERTY									m_parent_part_two;
	//기존코드
	//OBJECT_PROPERTY									m_parent_part_one;
	//OBJECT_PROPERTY									m_parent_part_two;
	//<--하남국-20151109
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockDesignPointGeneralInfoRecordSet();
	~CEcoBlockDesignPointGeneralInfoRecordSet();

	CEcoBlockDesignPointGeneralInfoRecordSet& operator = (const CEcoBlockDesignPointGeneralInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_design_point_no);
			ar & stdStr;
			ar & m_x_coord;
			ar & m_y_coord;
			ar & m_z_coord;
			stdStr = CStringConverter::CStringWToCStringA(m_create_method);
			ar & stdStr;
			m_parent_part_one.serialize(ar, version, bSendNRecv);
			m_parent_part_two.serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_design_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_x_coord;
			ar & m_y_coord;
			ar & m_z_coord;
			ar & stdStr;
			m_create_method = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_parent_part_one.serialize(ar, version, bSendNRecv);
			m_parent_part_two.serialize(ar, version, bSendNRecv);
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetDesignPointName(CString design_point_name);
	CString GetDesignPointName();

	void SetDesignPointCoord(double x, double y, double z);
	void GetDesignPointCoord(double* x, double* y, double* z);

	void SetCreateMethod(CString create_method);
	CString GetCreateMethod();

	//-->하남국-20151109
	void SetParentPart(OBJECT_NAME_PROPERTY one, OBJECT_NAME_PROPERTY two);
	void GetParentPart(OBJECT_NAME_PROPERTY* one, OBJECT_NAME_PROPERTY* two);
	//기존코드
	//void SetParentPart(OBJECT_PROPERTY one, OBJECT_PROPERTY two);
	//void GetParentPart(OBJECT_PROPERTY* one, OBJECT_PROPERTY* two);
	//<--하남국-20151109

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719
};

/*-----------------------------------------------------------------------
클래스명		: CEcoBlockManualMesManagerRecordSet
기능			: EcoBlockManualMesManager 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2011-02-25
마지막 수정일자	: 2011-02-25
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockManualMesManagerRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719

	int												m_dev_type;
	CString											m_des_pair;
	CString											m_dev;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockManualMesManagerRecordSet();
	~CEcoBlockManualMesManagerRecordSet();

	CEcoBlockManualMesManagerRecordSet& operator = (const CEcoBlockManualMesManagerRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			ar & m_dev_type;
			stdStr = CStringConverter::CStringWToCStringA(m_des_pair);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_dev);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_dev_type;
			ar & stdStr;
			m_des_pair = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_dev = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetDevType(int dev_type);
	int GetDevType();

	void SetDesPair(CString des_pair);
	CString GetDesPair();

	void SetDev(CString dev);
	CString GetDev();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719
};


/*-----------------------------------------------------------------------
클래스명		: CEcoBlockControlPointCoordInfoRecordSet
기능			: EcoBlockControlPointCoordInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-14
마지막 수정일자	: 2009-10-14
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockControlPointCoordInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_control_point_no;
	CString											m_property;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockControlPointCoordInfoRecordSet();
	~CEcoBlockControlPointCoordInfoRecordSet();

	CEcoBlockControlPointCoordInfoRecordSet& operator = (const CEcoBlockControlPointCoordInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_control_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_property);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_control_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_property = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetControlPointNo(CString control_point_no);
	CString GetControlPointNo();

	void SetProperty(CString control_point_property);
	CString GetProperty();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719
};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockControlPointDistanceInfoRecordSet
기능			: EcoBlockControlPointDistanceInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-14
마지막 수정일자	: 2009-10-14
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockControlPointDistanceInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CArray<CString, CString&>						m_control_point_pair;
	CString											m_property;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719

private:
	int m_nControlPointPairCnt;

public:
	CEcoBlockControlPointDistanceInfoRecordSet();
	~CEcoBlockControlPointDistanceInfoRecordSet();

	CEcoBlockControlPointDistanceInfoRecordSet& operator = (const CEcoBlockControlPointDistanceInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;

			m_nControlPointPairCnt = m_control_point_pair.GetCount();
			ar & m_nControlPointPairCnt;
			for(int i = 0; i < m_nControlPointPairCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_control_point_pair.GetAt(i));
				ar & stdStr;
			}

			stdStr = CStringConverter::CStringWToCStringA(m_property);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nControlPointPairCnt;
			CString str; m_control_point_pair.RemoveAll();
			for(int i = 0; i < m_nControlPointPairCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_control_point_pair.Add(str);
			}

			ar & stdStr;
			m_property = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetControlPointPair(CArray<CString, CString&>* control_point_pair);
	void GetControlPointPair(CArray<CString, CString&>* control_point_pair);

	void SetProperty(CString control_point_property);
	CString GetProperty();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719
};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockControlPointCurveInfoRecordSet
기능			: EcoBlockControlPointCurveInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2011-02-25
마지막 수정일자	: 2011-02-25
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockControlPointCurveInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	
	CString											m_name;
	HPoint											m_target1;
	HPoint											m_target2;
	HPoint											m_pos;
	double											m_length;
	CString											m_edge_up_down_path;

	CString											m_property;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockControlPointCurveInfoRecordSet();
	~CEcoBlockControlPointCurveInfoRecordSet();

	CEcoBlockControlPointCurveInfoRecordSet& operator = (const CEcoBlockControlPointCurveInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
			m_target1.serialize(ar, version, bSendNRecv);
			m_target2.serialize(ar, version, bSendNRecv);
			m_pos.serialize(ar, version, bSendNRecv);
			ar & m_length;
			stdStr = CStringConverter::CStringWToCStringA(m_edge_up_down_path);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_property);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_target1.serialize(ar, version, bSendNRecv);
			m_target2.serialize(ar, version, bSendNRecv);
			m_pos.serialize(ar, version, bSendNRecv);
			ar & m_length;
			ar & stdStr;
			m_edge_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_property = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetName(CString name);
	CString GetName();

	void SetTarget(HPoint target1, HPoint target2);
	void GetTarget(HPoint* target1, HPoint* target2);

	void SetPos(HPoint pos);
	void GetPos(HPoint* pos);

	void SetLength(double length);
	double GetLength();

	void SetEdgePath(CString path);
	CString GetEdgePath();

	void SetProperty(CString control_point_property);
	CString GetProperty();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockControlPointPerpInfoRecordSet
기능			: EcoBlockControlPointPerpInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-01-08
마지막 수정일자	: 2010-01-08
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockControlPointPerpInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CArray<CString, CString&>						m_control_point_pair;
	CString											m_property;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719

private:
	int m_nControlPointPairCnt;

public:
	CEcoBlockControlPointPerpInfoRecordSet();
	~CEcoBlockControlPointPerpInfoRecordSet();

	CEcoBlockControlPointPerpInfoRecordSet& operator = (const CEcoBlockControlPointPerpInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;

			m_nControlPointPairCnt = m_control_point_pair.GetCount();
			ar & m_nControlPointPairCnt;
			for(int i = 0; i < m_nControlPointPairCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_control_point_pair.GetAt(i));
				ar & stdStr;
			}

			stdStr = CStringConverter::CStringWToCStringA(m_property);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nControlPointPairCnt;
			CString str; m_control_point_pair.RemoveAll();
			for(int i = 0; i < m_nControlPointPairCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_control_point_pair.Add(str);
			}

			ar & stdStr;
			m_property = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetControlPointPair(CArray<CString, CString&>* control_point_pair);
	void GetControlPointPair(CArray<CString, CString&>* control_point_pair);

	void SetProperty(CString control_point_property);
	CString GetProperty();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};




/*-----------------------------------------------------------------------
클래스명		: CEcoBlockControlPointAngleInfoRecordSet
기능			: EcoBlockControlPointAngleInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-14
마지막 수정일자	: 2009-10-14
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockControlPointAngleInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_create_angle_method;
	CArray<CString, CString&>						m_control_point_pair;
	CString											m_property;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719

private:
	int m_nControlPointPairCnt;

public:
	CEcoBlockControlPointAngleInfoRecordSet();
	~CEcoBlockControlPointAngleInfoRecordSet();

	CEcoBlockControlPointAngleInfoRecordSet& operator = (const CEcoBlockControlPointAngleInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_create_angle_method);
			ar & stdStr;

			m_nControlPointPairCnt = m_control_point_pair.GetCount();
			ar & m_nControlPointPairCnt;
			for(int i = 0; i < m_nControlPointPairCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_control_point_pair.GetAt(i));
				ar & stdStr;
			}

			stdStr = CStringConverter::CStringWToCStringA(m_property);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_create_angle_method = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nControlPointPairCnt;
			CString str; m_control_point_pair.RemoveAll();
			for(int i = 0; i < m_nControlPointPairCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_control_point_pair.Add(str);
			}

			ar & stdStr;
			m_property = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetCreateAngleMethod(CString method);
	CString GetCreateAngleMethod();

	void SetControlPointPair(CArray<CString, CString&>* control_point_pair);
	void GetControlPointPair(CArray<CString, CString&>* control_point_pair);

	void SetProperty(CString control_point_property);
	CString GetProperty();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};


/*-----------------------------------------------------------------------
클래스명		: CEcoBlockControlPointFlatInfoRecordSet
기능			: EcoBlockControlPointFlatInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-14
마지막 수정일자	: 2009-10-14
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockControlPointFlatInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CArray<CString, CString&>						m_plane_point_pair;
	CArray<CString, CString&>						m_control_point_list;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719

private:
	int m_nPlanePointPairCnt;
	int m_nControlPointListCnt;

public:
	CEcoBlockControlPointFlatInfoRecordSet();
	~CEcoBlockControlPointFlatInfoRecordSet();

	CEcoBlockControlPointFlatInfoRecordSet& operator = (const CEcoBlockControlPointFlatInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;

			m_nPlanePointPairCnt = m_plane_point_pair.GetCount();
			ar & m_nPlanePointPairCnt;
			for(int i = 0; i < m_nPlanePointPairCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_plane_point_pair.GetAt(i));
				ar & stdStr;
			}

			m_nControlPointListCnt = m_control_point_list.GetCount();
			ar & m_nControlPointListCnt;
			for(int i = 0; i < m_nControlPointListCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_control_point_list.GetAt(i));
				ar & stdStr;
			}
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nPlanePointPairCnt;
			CString str; m_plane_point_pair.RemoveAll();
			for(int i = 0; i < m_nPlanePointPairCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_plane_point_pair.Add(str);
			}

			ar & m_nControlPointListCnt;
			m_control_point_list.RemoveAll();
			for(int i = 0; i < m_nControlPointListCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_control_point_list.Add(str);
			}
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetPlanePointPair(CArray<CString, CString&>* plane_point_pair);
	void GetPlanePointPair(CArray<CString, CString&>* plane_point_pair);

	void SetControlPointList(CArray<CString, CString&>* control_point_list);
	void GetControlPointList(CArray<CString, CString&>* control_point_list);

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719
};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockCameraInfoRecordSet
기능			: EcoBlockCameraInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2009-10-14
마지막 수정일자	: 2009-10-14
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockCameraInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_view_name;
	float											m_position_x;
	float											m_position_y;
	float											m_position_z;
	float											m_target_x;
	float											m_target_y;
	float											m_target_z;
	float											m_up_vector_x;
	float											m_up_vector_y;
	float											m_up_vector_z;
	float											m_field_width;
	float											m_field_height;
	CString											m_projection_mode;
	CString											m_edge_list;
	int												m_edge_list_size;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockCameraInfoRecordSet();
	~CEcoBlockCameraInfoRecordSet();

	CEcoBlockCameraInfoRecordSet& operator = (const CEcoBlockCameraInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_view_name);
			ar & stdStr;
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			stdStr = CStringConverter::CStringWToCStringA(m_projection_mode);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_edge_list);
			ar & stdStr;
			ar & m_edge_list_size;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_view_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			ar & stdStr;
			m_projection_mode = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_edge_list = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_edge_list_size;
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetViewName(CString view_name);
	CString GetViewName();

	void SetPosition(float x, float y, float z);
	void GetPosition(float* x, float* y, float* z);

	void SetTarget(float x, float y, float z);
	void GetTarget(float* x, float* y, float* z);

	void SetUpVector(float x, float y, float z);
	void GetUpVector(float* x, float* y, float* z);

	void SetField(float width, float height);
	void GetField(float* width, float* height);

	void SetProjectionMode(CString projection_mode);
	CString GetProjectionMode();

	void SetEdgeList(CString path);
	CString GetEdgeList();

	void SetEdgeListSize(int size);
	int GetEdgeListSize();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockProjectionFaceListRecordSet
기능			: EcoBlockProjectionFaceList 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-01-11
마지막 수정일자	: 2010-01-11
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockProjectionFaceListRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_projection_face_name;
	float											m_position_x;
	float											m_position_y;
	float											m_position_z;
	float											m_target_x;
	float											m_target_y;
	float											m_target_z;
	float											m_up_vector_x;
	float											m_up_vector_y;
	float											m_up_vector_z;
	float											m_field_width;
	float											m_field_height;
	CString											m_projection_mode;
	CString											m_edge_list_up_down_path;
	int												m_edge_list_size;
	float											m_modeling_matrix[16];
	int												m_modeling_matrix_size;	
	CString											m_projection_type;
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockProjectionFaceListRecordSet();
	~CEcoBlockProjectionFaceListRecordSet();

	CEcoBlockProjectionFaceListRecordSet& operator = (const CEcoBlockProjectionFaceListRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_projection_face_name);
			ar & stdStr;
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			stdStr = CStringConverter::CStringWToCStringA(m_projection_mode);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_edge_list_up_down_path);
			ar & stdStr;
			ar & m_edge_list_size;

			for(int i = 0; i < 16; i++)
				ar & m_modeling_matrix[i];

			ar & m_modeling_matrix_size;
			stdStr = CStringConverter::CStringWToCStringA(m_projection_type);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_projection_face_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			ar & stdStr;
			m_projection_mode = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_edge_list_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_edge_list_size;

			for(int i = 0; i < 16; i++)
				ar & m_modeling_matrix[i];

			ar & m_modeling_matrix_size;
			ar & stdStr;
			m_projection_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetProjectionFaceName(CString projection_face_name);
	CString GetProjectionFaceName();

	void SetPosition(float x, float y, float z);
	void GetPosition(float* x, float* y, float* z);

	void SetTarget(float x, float y, float z);
	void GetTarget(float* x, float* y, float* z);

	void SetUpVector(float x, float y, float z);
	void GetUpVector(float* x, float* y, float* z);

	void SetField(float width, float height);
	void GetField(float* width, float* height);

	void SetProjectionMode(CString projection_mode);
	CString GetProjectionMode();

	void SetEdgeList(CString path);
	CString GetEdgeList();

	void SetEdgeListSize(int size);
	int GetEdgeListSize();

	void SetModelingMatrix(float* matrix);
	void GetModelingMatrix(float* matrix);

	void SetModelingMatrixSize(int size);
	int GetModelingMatrixSize();

	void SetProjectionType(CString type);
	CString GetProjectionType();

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};



/*-----------------------------------------------------------------------
클래스명		: CEcoBlockAddMeasurePointInfoRecordSet
기능			: tb_EcoBlock_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoBlockAddMeasurePointInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_measure_point_no;
	//-->하남국-20120826-03-584
	double											m_coord_x;
	double											m_coord_y;
	double											m_coord_z;
	//HPoint											m_coord;
	//<--하남국-20120826-03-584
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockAddMeasurePointInfoRecordSet();
	~CEcoBlockAddMeasurePointInfoRecordSet();

	CEcoBlockAddMeasurePointInfoRecordSet& operator = (const CEcoBlockAddMeasurePointInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_no);
			ar & stdStr;
			ar & m_coord_x;
			ar & m_coord_y;
			ar & m_coord_z;
		}
		else
		{
			ar & stdStr;
			m_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_coord_x;
			ar & m_coord_y;
			ar & m_coord_z;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetMeasurePointNo(CString no);
	CString GetMeasurePointNo();

	//-->하남국-20120826-03-584
	void SetCoord(double x, double y, double z);
	void GetCoord(double& x, double& y, double& z);
	// 	void SetCoord(HPoint coord);
	// 	HPoint GetCoord();
	//<--하남국-20120826-03-584

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};




/*-----------------------------------------------------------------------
클래스명		: CEcoBlockDeleteMeasurePointInfoRecordSet
기능			: tb_EcoBlock_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoBlockDeleteMeasurePointInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_measure_point_no;
	//-->하남국-20120826-03-584
	double											m_coord_x;
	double											m_coord_y;
	double											m_coord_z;
	//HPoint											m_coord;
	//<--하남국-20120826-03-584
	//-->하남국-20120719
	//CString											m_project_type;
	//<--하남국-20120719
public:
	CEcoBlockDeleteMeasurePointInfoRecordSet();
	~CEcoBlockDeleteMeasurePointInfoRecordSet();

	CEcoBlockDeleteMeasurePointInfoRecordSet& operator = (const CEcoBlockDeleteMeasurePointInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_no);
			ar & stdStr;
			ar & m_coord_x;
			ar & m_coord_y;
			ar & m_coord_z;
		}
		else
		{
			ar & stdStr;
			m_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_coord_x;
			ar & m_coord_y;
			ar & m_coord_z;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetMeasurePointNo(CString no);
	CString GetMeasurePointNo();

	//-->하남국-20120826-03-584
	void SetCoord(double x, double y, double z);
	void GetCoord(double& x, double& y, double& z);
	// 	void SetCoord(HPoint coord);
	// 	HPoint GetCoord();
	//<--하남국-20120826-03-584

	//-->하남국-20120719
// 	void SetProjectType(CString project_type);
// 	CString GetProjectType();
	//<--하남국-20120719

};



/*-----------------------------------------------------------------------
클래스명		: CSystemBlockListRecordSet
기능			: tb_system_block_list 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2011-01-27
마지막 수정일자	: 2011-01-27
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CSystemBlockListRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_block_name;
	CString											m_pe_info;
public:
	CSystemBlockListRecordSet();
	~CSystemBlockListRecordSet();

	CSystemBlockListRecordSet& operator = (const CSystemBlockListRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_pe_info);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_pe_info = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetPeInfo(CString pe_info);
	CString GetPeInfo();

};


/*-----------------------------------------------------------------------
클래스명		: CEcoOtsProjectRecordSet
기능			: tb_ecoots_project 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoOtsProjectRecordSet : public CEcoTreeBaseRecordSet
{
private:
	
	CString											m_project_group;
	CString											m_ship_no;
	//-->하남국-20120719
// 	CString											m_project_name;
	//<--하남국-20120719
	CString											m_hoops_segment_up_down_path;
	int												m_hoops_segment_size;
	int												m_projection_face_cnt;
	COleDateTime									m_update_date;
	CString											m_update_user_id;

public:
	CEcoOtsProjectRecordSet();
	~CEcoOtsProjectRecordSet();

	CEcoOtsProjectRecordSet& operator = (const CEcoOtsProjectRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_hoops_segment_up_down_path);
			ar & stdStr;
			ar & m_hoops_segment_size;
			ar & m_projection_face_cnt;
			ar & m_update_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_update_user_id);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_hoops_segment_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_hoops_segment_size;
			ar & m_projection_face_cnt;
			ar & m_update_date.m_dt;
			ar & stdStr;
			m_update_user_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetHoopsSegment(CString path);
	CString GetHoopsSegment();

	void SetHoopsSegmentSize(int size);
	int GetHoopsSegmentSize();

	void SetProjectionFaceCnt(int cnt);
	int GetProjectionFaceCnt();

	void SetUpdateDate(COleDateTime date);
	COleDateTime GetUpdateDate();

	void SetUpdateUserID(CString id);
	CString GetUpdateUserID();

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsBlockListRecordSet
기능			: tb_ecoots_block_list 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoOtsBlockListRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString											m_project_name;
	CString											m_block_name;
	CString											m_without_part_list;

	HPoint											m_block_color;
	HPoint											m_block_transparency;
	bool											m_block_visibility;

	CString											m_measure_point_marker_symbol;
	HPoint											m_measure_point_marker_color;
	double											m_measure_point_marker_size;
	HPoint											m_measure_point_text_color;
	double											m_measure_point_text_size;

	HPoint											m_correction_val;
	
	bool											m_bexist_model;
	bool											m_bcalc_bound;
	HPoint											m_bound_min;
	HPoint											m_bound_max;
	float											m_last_des_modeling_matrix[16];
	int												m_last_des_modeling_matrix_size;	
	//-->하남국-20120826-03-584
	double											m_init_des_modeling_matrix[16];
	//float											m_init_des_modeling_matrix[16];
	//<--하남국-20120826-03-584
	int												m_init_des_modeling_matrix_size;	
	
	UINT											m_f_id_ecoblock_project;
	CString											m_ecoblock_project_name;
	CString											m_ecoblock_project_type;
	
	CString											m_block_load_option;
	
public:
	CEcoOtsBlockListRecordSet();
	~CEcoOtsBlockListRecordSet();

	CEcoOtsBlockListRecordSet& operator = (const CEcoOtsBlockListRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_without_part_list);
			ar & stdStr;
			m_block_color.serialize(ar, version, bSendNRecv);
			m_block_transparency.serialize(ar, version, bSendNRecv);
			ar & m_block_visibility;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_marker_symbol);
			ar & stdStr;
			m_measure_point_marker_color.serialize(ar, version, bSendNRecv);
			ar & m_measure_point_marker_size;
			m_measure_point_text_color.serialize(ar, version, bSendNRecv);
			ar & m_measure_point_text_size;
			m_correction_val.serialize(ar, version, bSendNRecv);
			ar & m_bexist_model;
			ar & m_bcalc_bound;
			m_bound_min.serialize(ar, version, bSendNRecv);
			m_bound_max.serialize(ar, version, bSendNRecv);

			for(int i = 0; i < 16; i++)
				ar & m_last_des_modeling_matrix[i];

			ar & m_last_des_modeling_matrix_size;

			for(int i = 0; i < 16; i++)
				ar & m_init_des_modeling_matrix[i];

			ar & m_init_des_modeling_matrix_size;	
			ar & m_f_id_ecoblock_project;
			stdStr = CStringConverter::CStringWToCStringA(m_ecoblock_project_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ecoblock_project_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_block_load_option);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_without_part_list = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_block_color.serialize(ar, version, bSendNRecv);
			m_block_transparency.serialize(ar, version, bSendNRecv);
			ar & m_block_visibility;
			ar & stdStr;
			m_measure_point_marker_symbol = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_measure_point_marker_color.serialize(ar, version, bSendNRecv);
			ar & m_measure_point_marker_size;
			m_measure_point_text_color.serialize(ar, version, bSendNRecv);
			ar & m_measure_point_text_size;
			m_correction_val.serialize(ar, version, bSendNRecv);
			ar & m_bexist_model;
			ar & m_bcalc_bound;
			m_bound_min.serialize(ar, version, bSendNRecv);
			m_bound_max.serialize(ar, version, bSendNRecv);

			for(int i = 0; i < 16; i++)
				ar & m_last_des_modeling_matrix[i];

			ar & m_last_des_modeling_matrix_size;

			for(int i = 0; i < 16; i++)
				ar & m_init_des_modeling_matrix[i];

			ar & m_init_des_modeling_matrix_size;	
			ar & m_f_id_ecoblock_project;
			ar & stdStr;
			m_ecoblock_project_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ecoblock_project_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_block_load_option = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:

	void SetProjectName(CString project_name);
	CString GetProjectName();
	

	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetWithOutPartList(CString list);
	CString GetWithOutPartList();

	void SetBlockColor(HPoint color);
	HPoint GetBlockColor();

	void SetBlockTransparency(HPoint transparency);
	HPoint GetBlockTransparency();

	void SetBlockVisibility(bool visibility);
	bool GetBlockVisibility();

	void SetMeasurePointMarkerSymbol(CString symbol);
	CString GetMeasurePointMarkerSymbol();

	void SetMeasurePointMarkerColor(HPoint color);
	HPoint GetMeasurePointMarkerColor();

	void SetMeasurePointMarkerSize(double size);
	double GetMeasurePointMarkerSize();

	void SetMeasurePointTextColor(HPoint color);
	HPoint GetMeasurePointTextColor();

	void SetMeasurePointTextSize(double size);
	double GetMeasurePointTextSize();

	void SetCorrectionVal(HPoint val);
	HPoint GetCorrectionVal();
	
	void SetbExistModel(bool bexist);
	bool GetbExistModel();
	
	void SetbCalcBound(bool bcalc);
	bool GetbCalcBound();

	void SetBound(HPoint min, HPoint max);
	void GetBound(HPoint* min, HPoint* max);

	void SetLastDesModelingMatrix(float* matrix);
	void GetLastDesModelingMatrix(float* matrix);

	void SetLastDesModelingMatrixSize(int size);
	int GetLastDesModelingMatrixSize();

//-->하남국-20120826-03-584
	void SetInitDesModelingMatrix(double* matrix);
	void GetInitDesModelingMatrix(double* matrix);
	// 	void SetInitDesModelingMatrix(float* matrix);
	// 	void GetInitDesModelingMatrix(float* matrix);
	//<--하남국-20120826-03-584

	void SetInitDesModelingMatrixSize(int size);
	int GetInitDesModelingMatrixSize();

	void SetEcoBlockPjtTbID(UINT f_id_ecoblock_pjt);
	UINT GetEcoBlockPjtTbID();

	void SetEcoBlockProjectName(CString project_name);
	CString GetEcoBlockProjectName();

	void SetEcoBlockProjectType(CString project_type);
	CString GetEcoBlockProjectType();

	void SetBlockLoadOption(CString option);
	CString GetBlockLoadOption();

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsAddMeasurePointInfoRecordSet
기능			: tb_ecoots_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoOtsAddMeasurePointInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_block_name;
	CString											m_measure_point_no;
	HPoint											m_coord;
public:
	CEcoOtsAddMeasurePointInfoRecordSet();
	~CEcoOtsAddMeasurePointInfoRecordSet();

	CEcoOtsAddMeasurePointInfoRecordSet& operator = (const CEcoOtsAddMeasurePointInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_no);
			ar & stdStr;
			m_coord.serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_coord.serialize(ar, version, bSendNRecv);
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetMeasurePointNo(CString no);
	CString GetMeasurePointNo();

	void SetCoord(HPoint coord);
	HPoint GetCoord();

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsDeleteMeasurePointInfoRecordSet
기능			: tb_ecoots_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoOtsDeleteMeasurePointInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_block_name;
	CString											m_measure_point_no;
	HPoint											m_coord;
public:
	CEcoOtsDeleteMeasurePointInfoRecordSet();
	~CEcoOtsDeleteMeasurePointInfoRecordSet();

	CEcoOtsDeleteMeasurePointInfoRecordSet& operator = (const CEcoOtsDeleteMeasurePointInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_no);
			ar & stdStr;
			m_coord.serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_coord.serialize(ar, version, bSendNRecv);
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetMeasurePointNo(CString no);
	CString GetMeasurePointNo();

	void SetCoord(HPoint coord);
	HPoint GetCoord();

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsDesignPointGeneralInfoRecordSet
기능			: EcoOtsDesignPointGeneralInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoOtsDesignPointGeneralInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_block_name;
	CString											m_design_point_no;
	double											m_x_coord;
	double											m_y_coord;
	double											m_z_coord;
	CString											m_create_method;
	OBJECT_PROPERTY									m_parent_part_one;
	OBJECT_PROPERTY									m_parent_part_two;
public:
	CEcoOtsDesignPointGeneralInfoRecordSet();
	~CEcoOtsDesignPointGeneralInfoRecordSet();

	CEcoOtsDesignPointGeneralInfoRecordSet& operator = (const CEcoOtsDesignPointGeneralInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_design_point_no);
			ar & stdStr;
			ar & m_x_coord;
			ar & m_y_coord;
			ar & m_z_coord;
			stdStr = CStringConverter::CStringWToCStringA(m_create_method);
			ar & stdStr;
			m_parent_part_one.serialize(ar, version, bSendNRecv);
			m_parent_part_two.serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_design_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_x_coord;
			ar & m_y_coord;
			ar & m_z_coord;
			ar & stdStr;
			m_create_method = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_parent_part_one.serialize(ar, version, bSendNRecv);
			m_parent_part_two.serialize(ar, version, bSendNRecv);
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetDesignPointName(CString design_point_name);
	CString GetDesignPointName();

	void SetDesignPointCoord(double x, double y, double z);
	void GetDesignPointCoord(double* x, double* y, double* z);

	void SetCreateMethod(CString create_method);
	CString GetCreateMethod();

	void SetParentPart(OBJECT_PROPERTY one, OBJECT_PROPERTY two);
	void GetParentPart(OBJECT_PROPERTY* one, OBJECT_PROPERTY* two);
};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsRelationByDesMsrRecordSet
기능			: tb_ecoots_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoOtsRelationByDesMsrRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_block_name;
	CString											m_desing_point_no;
	CString											m_measure_point_no;
	CString											m_visible_type;
public:
	CEcoOtsRelationByDesMsrRecordSet();
	~CEcoOtsRelationByDesMsrRecordSet();

	CEcoOtsRelationByDesMsrRecordSet& operator = (const CEcoOtsRelationByDesMsrRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_desing_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_visible_type);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_desing_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_visible_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetDesignPointNo(CString no);
	CString GetDesignPointNo();

	void SetMeasurePointNo(CString no);
	CString GetMeasurePointNo();

	void SetVisibleType(CString type);
	CString GetVisibleType();

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsRelationByMsrMsrRecordSet
기능			: tb_ecoots_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoOtsRelationByMsrMsrRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_base_block_name;
	CString											m_base_measure_point_no;
	CString											m_erection_block_name;
	CString											m_erection_measure_point_no;

	CString											m_r_element_relation_type;
	CString											m_r_element_visible_type;

	CString											m_dev_element_visible_type;
	HPoint											m_dev_element_pos;

	//-->하남국-20120605-03-450
	double											m_r_element_base_gap;
	CString											m_r_element_dev_type;
	//<--하남국-20120605-03-450
	//-->최조영-20120703-03-460
	double											m_relation_box_size_ind;
	double											m_relation_box_size_dep;
	//<--최조영-20120703-03-460
public:
	CEcoOtsRelationByMsrMsrRecordSet();
	~CEcoOtsRelationByMsrMsrRecordSet();

	CEcoOtsRelationByMsrMsrRecordSet& operator = (const CEcoOtsRelationByMsrMsrRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_base_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_base_measure_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_erection_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_erection_measure_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_r_element_relation_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_r_element_visible_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_dev_element_visible_type);
			ar & stdStr;
			m_dev_element_pos.serialize(ar, version, bSendNRecv);
			ar & m_r_element_base_gap;
			stdStr = CStringConverter::CStringWToCStringA(m_r_element_dev_type);
			ar & stdStr;
			ar & m_relation_box_size_ind;
			ar & m_relation_box_size_dep;
		}
		else
		{
			ar & stdStr;
			m_base_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_base_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_erection_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_erection_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_r_element_relation_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_r_element_visible_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_dev_element_visible_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_dev_element_pos.serialize(ar, version, bSendNRecv);
			ar & m_r_element_base_gap;
			ar & stdStr;
			m_r_element_dev_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_relation_box_size_ind;
			ar & m_relation_box_size_dep;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetBaseBlockName(CString block_name);
	CString GetBaseBlockName();

	void SetBaseMeasurePointNo(CString no);
	CString GetBaseMeasurePointNo();

	void SetErectionBlockName(CString block_name);
	CString GetErectionBlockName();

	void SetErectionMeasurePointNo(CString no);
	CString GetErectionMeasurePointNo();

	void SetRElementRelationType(CString type);
	CString GetRElementRelationType();

	void SetRElementVisibleType(CString type);
	CString GetRElementVisibleType();

	void SetDevElementVisibleType(CString type);
	CString GetDevElementVisibleType();

	void SetDevElementPos(HPoint pos);
	HPoint GetDevElementPos();

	//-->하남국-20120605-03-450
	void SetRElementBaseGap(double dblGap);
	double GetRElementBaseGap();
	void SetRElementDevType(CString type);
	CString GetRElementDevType();
	//<--하남국-20120605-03-450
	//-->최조영-20120703-03-460
	void SetRelationBoxSizeind(double dsize);
	double GetRelationBoxSizeind();

	void SetRelationBoxSizedep(double dsize);
	double GetRelationBoxSizedep();
	//<--최조영-20120703-03-460
};




/*-----------------------------------------------------------------------
클래스명		: CEcoOtsPointCondRecordSet
기능			: tb_ecoots_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoOtsPointCondRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_block_name;

	CString											m_measure_point_no;
	HPoint											m_position;
	CString											m_visible_type;
	//-->최조영-20120703-03-460
	double											m_element_box_size;
	//<--최조영-20120703-03-460
public:
	CEcoOtsPointCondRecordSet();
	~CEcoOtsPointCondRecordSet();

	CEcoOtsPointCondRecordSet& operator = (const CEcoOtsPointCondRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_point_no);
			ar & stdStr;
			m_position.serialize(ar, version, bSendNRecv);
			stdStr = CStringConverter::CStringWToCStringA(m_visible_type);
			ar & stdStr;
			ar & m_element_box_size;
		}
		else
		{
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_position.serialize(ar, version, bSendNRecv);
			ar & stdStr;
			m_visible_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_element_box_size;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetMeasurePointNo(CString no);
	CString GetMeasurePointNo();

	void SetPosition(HPoint position);
	HPoint GetPosition();

	void SetVisibleType(CString type);
	CString GetVisibleType();

	//-->최조영-20120703-03-460
	void SetboxSize(double dsize);
	double GetboxSize();
	//<--최조영-20120703-03-460

};


/*-----------------------------------------------------------------------
클래스명		: CEcoOtsDistanceCondRecordSet
기능			: tb_ecoots_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoOtsDistanceCondRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_first_block_name;
	CString											m_first_measure_point_no;

	CString											m_second_block_name;
	CString											m_second_measure_point_no;

	HPoint											m_position;
	CString											m_visible_type;
	//-->최조영-20120703-03-460
	double											m_element_box_size;
	//<--최조영-20120703-03-460
public:
	CEcoOtsDistanceCondRecordSet();
	~CEcoOtsDistanceCondRecordSet();

	CEcoOtsDistanceCondRecordSet& operator = (const CEcoOtsDistanceCondRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_first_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_first_measure_point_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_second_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_second_measure_point_no);
			ar & stdStr;
			m_position.serialize(ar, version, bSendNRecv);
			stdStr = CStringConverter::CStringWToCStringA(m_visible_type);
			ar & stdStr;
			ar & m_element_box_size;
		}
		else
		{
			ar & stdStr;
			m_first_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_first_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_second_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_second_measure_point_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_position.serialize(ar, version, bSendNRecv);
			ar & stdStr;
			m_visible_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_element_box_size;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetFirstBlockName(CString block_name);
	CString GetFirstBlockName();

	void SetFirstMeasurePointNo(CString no);
	CString GetFirstMeasurePointNo();

	void SetSecondBlockName(CString block_name);
	CString GetSecondBlockName();

	void SetSecondMeasurePointNo(CString no);
	CString GetSecondMeasurePointNo();

	void SetPosition(HPoint position);
	HPoint GetPosition();

	void SetVisibleType(CString type);
	CString GetVisibleType();

	//-->최조영-20120703-03-460
	void SetboxSize(double dsize);
	double GetboxSize();
	//<--최조영-20120703-03-460

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsDrawingElementRecordSet
기능			: tb_ecoots_add_measure_point_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoOtsDrawingElementRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_element_up_down_path;
	int												m_element_size;
public:
	CEcoOtsDrawingElementRecordSet();
	~CEcoOtsDrawingElementRecordSet();

	CEcoOtsDrawingElementRecordSet& operator = (const CEcoOtsDrawingElementRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_element_up_down_path);
			ar & stdStr;
			ar & m_element_size;
		}
		else
		{
			ar & stdStr;
			m_element_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_element_size;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetElement(CString path);
	CString GetElement();

	void SetElementSize(int size);
	int GetElementSize();
};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsCameraInfoRecordSet
기능			: EcoOtsCameraInfo 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-15
마지막 수정일자	: 2010-06-15
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoOtsCameraInfoRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_view_name;
	float											m_position_x;
	float											m_position_y;
	float											m_position_z;
	float											m_target_x;
	float											m_target_y;
	float											m_target_z;
	float											m_up_vector_x;
	float											m_up_vector_y;
	float											m_up_vector_z;
	float											m_field_width;
	float											m_field_height;
	CString											m_projection_mode;
	CString											m_edge_list;
	int												m_edge_list_size;
public:
	CEcoOtsCameraInfoRecordSet();
	~CEcoOtsCameraInfoRecordSet();

	CEcoOtsCameraInfoRecordSet& operator = (const CEcoOtsCameraInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_view_name);
			ar & stdStr;
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			stdStr = CStringConverter::CStringWToCStringA(m_projection_mode);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_edge_list);
			ar & stdStr;
			ar & m_edge_list_size;
		}
		else
		{
			ar & stdStr;
			m_view_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			ar & stdStr;
			m_projection_mode = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_edge_list = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_edge_list_size;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetViewName(CString view_name);
	CString GetViewName();

	void SetPosition(float x, float y, float z);
	void GetPosition(float* x, float* y, float* z);

	void SetTarget(float x, float y, float z);
	void GetTarget(float* x, float* y, float* z);

	void SetUpVector(float x, float y, float z);
	void GetUpVector(float* x, float* y, float* z);

	void SetField(float width, float height);
	void GetField(float* width, float* height);

	void SetProjectionMode(CString projection_mode);
	CString GetProjectionMode();

	void SetEdgeList(CString path);
	CString GetEdgeList();

	void SetEdgeListSize(int size);
	int GetEdgeListSize();

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsProjectionFaceListRecordSet
기능			: EcoOtsProjectionFaceList 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-16
마지막 수정일자	: 2010-06-16
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoOtsProjectionFaceListRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_projection_face_name;
	float											m_position_x;
	float											m_position_y;
	float											m_position_z;
	float											m_target_x;
	float											m_target_y;
	float											m_target_z;
	float											m_up_vector_x;
	float											m_up_vector_y;
	float											m_up_vector_z;
	float											m_field_width;
	float											m_field_height;
	CString											m_projection_mode;
	CString											m_edge_list_up_down_path;
	int												m_edge_list_size;
	CString											m_projection_type;
public:
	CEcoOtsProjectionFaceListRecordSet();
	~CEcoOtsProjectionFaceListRecordSet();

	CEcoOtsProjectionFaceListRecordSet& operator = (const CEcoOtsProjectionFaceListRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_projection_face_name);
			ar & stdStr;
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			stdStr = CStringConverter::CStringWToCStringA(m_projection_mode);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_edge_list_up_down_path);
			ar & stdStr;
			ar & m_edge_list_size;
			stdStr = CStringConverter::CStringWToCStringA(m_projection_type);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_projection_face_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_position_x;
			ar & m_position_y;
			ar & m_position_z;
			ar & m_target_x;
			ar & m_target_y;
			ar & m_target_z;
			ar & m_up_vector_x;
			ar & m_up_vector_y;
			ar & m_up_vector_z;
			ar & m_field_width;
			ar & m_field_height;
			ar & stdStr;
			m_projection_mode = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_edge_list_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_edge_list_size;
			ar & stdStr;
			m_projection_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetProjectionFaceName(CString projection_face_name);
	CString GetProjectionFaceName();

	void SetPosition(float x, float y, float z);
	void GetPosition(float* x, float* y, float* z);

	void SetTarget(float x, float y, float z);
	void GetTarget(float* x, float* y, float* z);

	void SetUpVector(float x, float y, float z);
	void GetUpVector(float* x, float* y, float* z);

	void SetField(float width, float height);
	void GetField(float* width, float* height);

	void SetProjectionMode(CString projection_mode);
	CString GetProjectionMode();

	void SetEdgeList(CString path);
	CString GetEdgeList();

	void SetEdgeListSize(int size);
	int GetEdgeListSize();

	void SetProjectionType(CString type);
	CString GetProjectionType();

};



/*-----------------------------------------------------------------------
클래스명		: CEcoOtsProjectionFaceModelingMatrixListRecordSet
기능			: EcoOtsProjectionFaceList 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2010-06-16
마지막 수정일자	: 2010-06-16
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoOtsProjectionFaceModelingMatrixListRecordSet : public CEcoRefBaseRecordSet
{
private:
	//-->하남국-20120719
	//CString											m_project_name;
	//<--하남국-20120719
	CString											m_projection_face_name;
	CString											m_block_name;
	float											m_modeling_matrix[16];
	int												m_modeling_matrix_size;	
public:
	CEcoOtsProjectionFaceModelingMatrixListRecordSet();
	~CEcoOtsProjectionFaceModelingMatrixListRecordSet();

	CEcoOtsProjectionFaceModelingMatrixListRecordSet& operator = (const CEcoOtsProjectionFaceModelingMatrixListRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_projection_face_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;

			for(int i = 0; i < 16; i++)
				ar & m_modeling_matrix[i];

			ar & m_modeling_matrix_size;	
		}
		else
		{
			ar & stdStr;
			m_projection_face_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());

			for(int i = 0; i < 16; i++)
				ar & m_modeling_matrix[i];

			ar & m_modeling_matrix_size;
		}
	}

public:
	//-->하남국-20120719
// 	void SetProjectName(CString project_name);
// 	CString GetProjectName();
	//<--하남국-20120719

	void SetProjectionFaceName(CString projection_face_name);
	CString GetProjectionFaceName();

	void SetBlockName(CString block_name);
	CString GetBlockName();

	void SetModelingMatrix(float* matrix);
	void GetModelingMatrix(float* matrix);

	void SetModelingMatrixSize(int size);
	int GetModelingMatrixSize();

};

/*-----------------------------------------------------------------------
기능			: CAssemblyLevelRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CAssemblyLevelRecordSet : public CEcoTreeBaseRecordSet
{
private:
	UINT m_order;
public:
	CAssemblyLevelRecordSet();
	~CAssemblyLevelRecordSet();

	CAssemblyLevelRecordSet& operator = (const CAssemblyLevelRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		ar & m_order;
	}

public:
	void SetOrder(UINT order);
	UINT GetOrder();
};

/*-----------------------------------------------------------------------
기능			: CAssemblyTreeRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CAssemblyTreeRecordSet : public CEcoTreeBaseRecordSet
{
private:

public:
	CAssemblyTreeRecordSet();
	~CAssemblyTreeRecordSet();

	CAssemblyTreeRecordSet& operator = (const CAssemblyTreeRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

	}

public:

};

/*-----------------------------------------------------------------------
기능			: CVariableGroupRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
하남국-20121123, 자식그룹의 ID를 저장하는 컬럼 추가(1@@2@@3 와 같이 저장함)
-----------------------------------------------------------------------*/
class AFX_EXT_API CVariableGroupRecordSet : public CEcoTreeBaseRecordSet
{
private:
	CString m_data;
	CString m_child_id;	//-->하남국-20121123
public:
	CVariableGroupRecordSet();
	~CVariableGroupRecordSet();

	CVariableGroupRecordSet& operator = (const CVariableGroupRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_data);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_child_id);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_data = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_child_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetData(CString data);
	CString GetData();

	//-->하남국-20121123
	void SetChildID(CString strIDs);
	CString GetChildID(void);
	//<--하남국-20121123
};

/*-----------------------------------------------------------------------
기능			: CVariableOfGroupRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20131113, 함수 최초 작성
               , name 항목은 사용하지 않음.
-----------------------------------------------------------------------*/
class AFX_EXT_API CVariableOfGroupRecordSet : public CEcoTreeBaseRecordSet
{
private:
	UINT m_f_id_user_variable;
	BOOL m_essential;
	UINT m_view_order;
	BOOL m_buse_pjt_id;
public:
	CVariableOfGroupRecordSet();
	~CVariableOfGroupRecordSet();

	CVariableOfGroupRecordSet& operator = (const CVariableOfGroupRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		ar & m_f_id_user_variable;
		ar & m_essential;
		ar & m_view_order;
		ar & m_buse_pjt_id;
	}

public:
	void SetFIDUserVariable(UINT f_id_user_variable);
	UINT GetFIDUserVariable();

	void SetEssential(BOOL bEssential);
	BOOL GetEssential();

	void SetViewOrder(UINT view_order);
	UINT GetViewOrder();

	void SetbUseProjectID(BOOL bUsePjtID);
	BOOL GetbUseProjectID();
};

/*-----------------------------------------------------------------------
기능			: CUserVariableRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CUserVariableRecordSet : public CEcoTreeBaseRecordSet
{
private:
	UINT m_target_property;
	UINT m_variable_type;
	BOOL m_essential;
	CString m_data;
	BOOL m_duplicate;
	CString m_init_value;
	UINT m_view_order;
	BOOL m_buse_pjt_id;
public:
	CUserVariableRecordSet();
	~CUserVariableRecordSet();

	CUserVariableRecordSet& operator = (const CUserVariableRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_target_property;
			ar & m_variable_type;
			ar & m_essential;
			stdStr = CStringConverter::CStringWToCStringA(m_data);
			ar & stdStr;
			ar & m_duplicate;
			stdStr = CStringConverter::CStringWToCStringA(m_init_value);
			ar & stdStr;
			ar & m_view_order;
			ar & m_buse_pjt_id;
		}
		else
		{
			ar & m_target_property;
			ar & m_variable_type;
			ar & m_essential;
			ar & stdStr;
			m_data = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_duplicate;
			ar & stdStr;
			m_init_value = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_view_order;
			ar & m_buse_pjt_id;
		}
	}

public:
	void SetTargetProperty(UINT target_property);
	UINT GetTargetProperty();

	void SetVariableType(UINT variable_type);
	UINT GetVariableType();

	void SetEssential(BOOL bEssential);
	BOOL GetEssential();

	void SetData(CString data);
	CString GetData();

	void SetDuplicate(BOOL bDuplicate);
	BOOL GetDuplicate();

	void SetInitValue(CString init_value);
	CString GetInitValue();

	void SetViewOrder(UINT view_order);
	UINT GetViewOrder();

	void SetbUseProjectID(BOOL bUsePjtID);
	BOOL GetbUseProjectID();
};

/*-----------------------------------------------------------------------
기능			: CProjectPropertyRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CProjectPropertyRecordSet : public CEcoTreeBaseRecordSet
{
private:
	UINT m_f_id_user_variable;
	CString m_data;
public:
	CProjectPropertyRecordSet();
	~CProjectPropertyRecordSet();

	CProjectPropertyRecordSet& operator = (const CProjectPropertyRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_f_id_user_variable;
			stdStr = CStringConverter::CStringWToCStringA(m_data);
			ar & stdStr;
		}
		else
		{
			ar & m_f_id_user_variable;
			ar & stdStr;
			m_data = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetFIDUserVariable(UINT f_id_user_variable);
	UINT GetFIDUserVariable();

	void SetData(CString data);
	CString GetData();
};

/*-----------------------------------------------------------------------
기능			: CBlockPropertyRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CBlockPropertyRecordSet : public CEcoTreeBaseRecordSet
{
private:
	UINT m_f_id_user_variable;
	CString m_data;
public:
	CBlockPropertyRecordSet();
	~CBlockPropertyRecordSet();

	CBlockPropertyRecordSet& operator = (const CBlockPropertyRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_f_id_user_variable;
			stdStr = CStringConverter::CStringWToCStringA(m_data);
			ar & stdStr;
		}
		else
		{
			ar & m_f_id_user_variable;
			ar & stdStr;
			m_data = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetFIDUserVariable(UINT f_id_user_variable);
	UINT GetFIDUserVariable();

	void SetData(CString data);
	CString GetData();
};

/*-----------------------------------------------------------------------
기능			: CShipnoPropertyRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CShipnoPropertyRecordSet : public CEcoTreeBaseRecordSet
{
private:
	UINT m_f_id_user_variable;
	CString m_data;
public:
	CShipnoPropertyRecordSet();
	~CShipnoPropertyRecordSet();

	CShipnoPropertyRecordSet& operator = (const CShipnoPropertyRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_f_id_user_variable;
			stdStr = CStringConverter::CStringWToCStringA(m_data);
			ar & stdStr;
		}
		else
		{
			ar & m_f_id_user_variable;
			ar & stdStr;
			m_data = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetFIDUserVariable(UINT f_id_user_variable);
	UINT GetFIDUserVariable();

	void SetData(CString data);
	CString GetData();
};

/*-----------------------------------------------------------------------
기능			: CEcoBlockProjectTreeRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  	: 
하남국-20120719, 함수 최초 작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoBlockProjectTreeRecordSet : public CEcoTreeBaseRecordSet
{
private:

public:
	CEcoBlockProjectTreeRecordSet();
	~CEcoBlockProjectTreeRecordSet();

	CEcoBlockProjectTreeRecordSet& operator = (const CEcoBlockProjectTreeRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

	}
};













/*-----------------------------------------------------------------------
클래스명		: CUploadFileRecordSet
기능			: tb_upload_file 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2012-04-15
마지막 수정일자	: 2012-04-15
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CUploadFileRecordSet : public CEcoTreeBaseRecordSet
{
private:
//	int												m_depth;
//	CString											m_parent_name;
//	CString											m_name;
	CString											m_index_type;
//	CString											m_index_string;
	CString											m_file_name;
	CString											m_file_path;
	int												m_file_size;
	CString											m_reserv_1;
	CString											m_reserv_2;
	CString											m_reserv_3;
	CString											m_reserv_4;
	CString											m_reserv_5;
public:
	CUploadFileRecordSet();
	~CUploadFileRecordSet();

	CUploadFileRecordSet& operator = (const CUploadFileRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_index_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_file_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_file_path);
			ar & stdStr;
			ar & m_file_size;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_1);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_2);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_3);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_4);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_5);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_index_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_file_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_file_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_file_size;
			ar & stdStr;
			m_reserv_1 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_2 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_3 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_4 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_5 = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
// 	void SetDepth(int val);
// 	int GetDepth();
// 	
// 	void SetParentName(CString val);
// 	CString GetParentName();
// 
// 	void SetName(CString val);
// 	CString GetName();

	void SetIndexType(CString val);
	CString GetIndexType();

// 	void SetIndexString(CString val);
// 	CString GetIndexString();

	void SetFileName(CString val);
	CString GetFileName();

	void SetFilePath(CString val);
	CString GetFilePath();

	void SetFileSize(int val);
	int GetFileSize();

	void SetReserv1(CString val);
	CString GetReserv1();

	void SetReserv2(CString val);
	CString GetReserv2();

	void SetReserv3(CString val);
	CString GetReserv3();

	void SetReserv4(CString val);
	CString GetReserv4();

	void SetReserv5(CString val);
	CString GetReserv5();
};











/*-----------------------------------------------------------------------
클래스명		: CAutoUpdateFileRercordSet
기능			: tb_auto_update_file 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2011-09-06
마지막 수정일자	: 2011-09-06
작성자			: 강경완
-----------------------------------------------------------------------*/
class AFX_EXT_API CAutoUpdateFileRercordSet : public CObject
{
private:
	CString											m_name;
	CString											m_file_path;
	int												m_file_size;
	COleDateTime									m_update_date;
	CString											m_property;
	CString											m_reserve_1;
	CString											m_reserve_2;
	CString											m_reserve_3;
	CString											m_reserve_4;
	CString											m_reserve_5;
public:
	CAutoUpdateFileRercordSet();
	~CAutoUpdateFileRercordSet();

	CAutoUpdateFileRercordSet& operator = (const CAutoUpdateFileRercordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_file_path);
			ar & stdStr;
			ar & m_file_size;
			ar & m_update_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_property);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserve_1);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserve_2);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserve_3);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserve_4);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserve_5);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_file_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_file_size;
			ar & m_update_date.m_dt;
			ar & stdStr;
			m_property = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserve_1 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserve_2 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserve_3 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserve_4 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserve_5 = CStringConverter::CStringAToCStringW(stdStr.c_str());

		}
	}

public:
	void SetFileName(CString val);
	CString GetFileName();

	void SetFilePath(CString val);
	CString GetFilePath();

	void SetFileSize(int val);
	int GetFileSize();

	void SetUpdateDate(COleDateTime val);
	COleDateTime GetUpdateDate();

	void SetProperty(CString val);
	CString GetProperty();

	void SetReserve_1(CString val);
	CString GetReserve_1();

	void SetReserve_2(CString val);
	CString GetReserve_2();

	void SetReserve_3(CString val);
	CString GetReserve_3();

	void SetReserve_4(CString val);
	CString GetReserve_4();

	void SetReserve_5(CString val);
	CString GetReserve_5();
};



/*-----------------------------------------------------------------------
클래스명		: CEcoModelRecordSet
기능			: tb_ecomodel_%s 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2011-05-30
마지막 수정일자	: 2011-05-30
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CEcoModelRecordSet : public CObject
{
public:
	CString						m_type;
	CString						m_name;
	CString						m_product_name;
	CString						m_superior_name;
	CString						m_module_name;
	CString						m_side;
	CString						m_ass_name;
	CString						m_contour_data;
	int							m_contour_data_size;
	COleDateTime				m_update_date;
	CString						m_hsf_data;
	int							m_hsf_data_size;
	CString						m_q_index;

public:
	CEcoModelRecordSet();
	~CEcoModelRecordSet();

	CEcoModelRecordSet& operator = (const CEcoModelRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_product_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_superior_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_module_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_side);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ass_name);
			ar & stdStr;
			ar & m_contour_data_size;
			stdStr = CStringConverter::CStringWToCStringA(m_contour_data);
			ar & stdStr;
			ar & m_update_date.m_dt;
			ar & m_hsf_data_size;
			stdStr = CStringConverter::CStringWToCStringA(m_hsf_data);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_q_index);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_product_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_superior_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_module_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_side = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ass_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_contour_data_size;
			ar & stdStr;
			m_contour_data = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_update_date.m_dt;
			ar & m_hsf_data_size;
			ar & stdStr;
			m_hsf_data = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_q_index = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetType(CString val);
	CString GetType();

	void SetName(CString val);
	CString GetName();

	void SetProductName(CString val);
	CString GetProductName();

	void SetSuperiorName(CString val);
	CString GetSuperiorName();

	void SetModuleName(CString val);
	CString GetModuleName();

	void SetSide(CString val);
	CString GetSide();

	void SetAssName(CString val);
	CString GetAssName();

	void SetContourData(CString val);
	CString GetContourData();

	void SetContourDataSize(int val);
	int GetContourDataSize();

	void SetUpdateDate(COleDateTime val);
	COleDateTime GetUpdateDate();

	void SetHsfData(CString val);
	CString GetHsfData();

	void SetHsfDataSize(int val);
	int GetHsfDataSize();

	void SetQueryIndex(CString val);
	CString GetQueryIndex();

};


/*-----------------------------------------------------------------------
클래스명		: CAssBlockInspectionInfoRecordSet
기능			: tb_ass_block_inspection_info 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2011-01-27
마지막 수정일자	: 2011-01-27
작성자			: 강경완
-----------------------------------------------------------------------*/

class AFX_EXT_API CAssBlockInspectionInfoRecordSet : public CObject
{
private:
	CString										m_project_group;
	CString										m_ship_no;
	CString										m_record_id;
	bool										m_approval;
	CString										m_block_name;
	CString										m_assembly_name;
	CString										m_ass_stage;
	CString										m_inspection_item;
	CString										m_block_area;
	CString										m_block_type;
	CString										m_outsourcing_name;
	CString										m_outsourcing_group;
	CString										m_yard_name;
	CString										m_ass_step;
	CString										m_result;
	CString										m_cause_of_fail;
	COleDateTime								m_time_of_plan;
	COleDateTime								m_time_of_real;
	double										m_weld_percent;
	CString										m_inspection_dept;
	CString										m_inspectior;
	CString										m_comment;
	CString										m_checksheet_hash;
	CString										m_dac_hash;
	bool										m_UpdateNotPM;
	CString										m_block_or_pe;
	bool										m_bUploadFile;
	CString										m_update_user_id;
public:
	CAssBlockInspectionInfoRecordSet();
	~CAssBlockInspectionInfoRecordSet();

	CAssBlockInspectionInfoRecordSet& operator = (const CAssBlockInspectionInfoRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_record_id);
			ar & stdStr;
			ar & m_approval;
			stdStr = CStringConverter::CStringWToCStringA(m_block_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_assembly_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ass_stage);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_inspection_item);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_block_area);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_block_type);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_outsourcing_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_outsourcing_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_yard_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ass_step);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_result);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_cause_of_fail);
			ar & stdStr;
			ar & m_time_of_plan.m_dt;
			ar & m_time_of_real.m_dt;
			ar & m_weld_percent;
			stdStr = CStringConverter::CStringWToCStringA(m_inspection_dept);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_inspectior);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_comment);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_checksheet_hash);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_dac_hash);
			ar & stdStr;
			ar & m_UpdateNotPM;
			stdStr = CStringConverter::CStringWToCStringA(m_block_or_pe);
			ar & stdStr;
			ar & m_bUploadFile;
			stdStr = CStringConverter::CStringWToCStringA(m_update_user_id);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_record_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_approval;
			ar & stdStr;
			m_block_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_assembly_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ass_stage = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_inspection_item = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_block_area = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_block_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_outsourcing_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_outsourcing_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_yard_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ass_step = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_result = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_cause_of_fail = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_time_of_plan.m_dt;
			ar & m_time_of_real.m_dt;
			ar & m_weld_percent;
			ar & stdStr;
			m_inspection_dept = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_inspectior = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_comment = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_checksheet_hash = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_dac_hash = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_UpdateNotPM;
			ar & stdStr;
			m_block_or_pe = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_bUploadFile;
			ar & stdStr;
			m_update_user_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	void SetRecordId(CString val);
	CString GetRecordId();

	void SetApproval(bool val);
	bool GetApproval();

	void SetBlockName(CString val);
	CString GetBlockName();

	void SetAssemblyName(CString val);
	CString GetAssemblyName();

	void SetAssStage(CString val);
	CString GetAssStage();

	void SetInspectionItem(CString val);
	CString GetInspectionItem();

	void SetBlockArea(CString val);
	CString GetBlockArea();

	void SetBlockType(CString val);
	CString GetBlockType();

	void SetOutSourcingName(CString val);
	CString GetOutSourcingName();

	void SetOutSourcingGroup(CString val);
	CString GetOutSourcingGroup();

	void SetYardName(CString val);
	CString GetYardName();

	void SetAssStep(CString val);
	CString GetAssStep();

	void SetResult(CString val);
	CString GetResult();

	void SetCauseOfFail(CString val);
	CString GetCauseOfFail();

	void SetTimeOfPlan(COleDateTime val);
	COleDateTime GetTimeOfPlan();

	void SetTimeOfReal(COleDateTime val);
	COleDateTime GetTimeOfReal();

	void SetWeldPercent(double val);
	double GetWeldPercent();

	void SetInspectioinDept(CString val);
	CString GetInspectioinDept();

	void SetInspector(CString val);
	CString GetInspector();

	void SetComment(CString val);
	CString GetComment();

	void SetCheckSheetHash(CString val);
	CString GetCheckSheetHash();

	void SetDacHash(CString val);
	CString GetDacHash();

	void SetUpdateNotPM(bool val);
	bool GetUpdateNotPM();

	void SetBlockOrPE(CString val);
	CString GetBlockOrPE();

	void SetBuploadFile(bool val);
	bool GetBuploadFile();

	void SetUpdateUserID(CString val);
	CString GetUpdateUserID();
};

/*-----------------------------------------------------------------------
클래스명		: CProjectRecordSet
기능			: tb_project 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스

작성일자		: 2012-11-07
마지막 수정일자	: 2012-11-07
작성자			: 하남국
-----------------------------------------------------------------------*/
class AFX_EXT_API CProjectRecordSet : public CEcoTreeBaseRecordSet
{
private:
	UINT m_f_id_variable_group;
	UINT m_project_type;
	COleDateTime m_update_date;
	CString m_update_user_id;
	BOOL m_update_not_pm;
	
public:
	CProjectRecordSet();
	~CProjectRecordSet();

	CProjectRecordSet& operator = (const CProjectRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_f_id_variable_group;
			ar & m_project_type;
			ar & m_update_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_update_user_id);
			ar & stdStr;
			ar & m_update_not_pm;
		}
		else
		{
			ar & m_f_id_variable_group;
			ar & m_project_type;
			ar & m_update_date.m_dt;
			ar & stdStr;
			m_update_user_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_update_not_pm;
		}
	}

public:
	void SetFIDVariableGroup(UINT nVarGroupID);
	UINT GetFIDVariableGroup();

	void SetProjectType(UINT nPjtTypeID);
	UINT GetProjectType();

	void SetUpdateDate(COleDateTime date);
	COleDateTime GetUpdateDate();

	void SetUpdateUserID(CString strID);
	CString GetUpdateUserID();

	void SetUpdateNotPm(BOOL bPmCreate);
	BOOL GetUpdateNotPm();
};


/*-----------------------------------------------------------------------
클래스명		: CUserInputBlockListRecordSet
기능			: tb_project 테이블의 레코드 필드 값들의 입출력 데이터
				포맷을 가지고 있는 클래스
작성일자		: 2012-12-18
마지막 수정일자	: 2012-12-18
작성자			: 하남국
//-->하남국-20121218, 최초생성
-----------------------------------------------------------------------*/
class AFX_EXT_API CUserInputBlockListRecordSet : public CEcoRefBaseRecordSet
{
public:
	CUserInputBlockListRecordSet();
	~CUserInputBlockListRecordSet();

	CUserInputBlockListRecordSet& operator = (const CUserInputBlockListRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

private:
	CString											m_name;

public:

	void SetName(CString name);
	CString GetName();
};


/*-----------------------------------------------------------------------
클래스명		: CEcoModelNameManagerRecordSet
기능			: tb_ecomodel_manager 테이블의 레코드 필드 값들의 입출력 데이터
포맷을 가지고 있는 클래스

작성일자		: 2012-12-17
마지막 수정일자	: 2012-12-17
작성자			: 최조영
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoModelNameManagerRecordSet : public CObject
{
private:
	
	UINT											m_id;
	CString											m_name;
public:
	CEcoModelNameManagerRecordSet();
	~CEcoModelNameManagerRecordSet();

	CEcoModelNameManagerRecordSet& operator = (const CEcoModelNameManagerRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_id;
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
		}
		else
		{
			ar & m_id;
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetID(UINT uid);
	UINT GetID();

	void SetTbName(CString tableName);
	CString GetTbName();

};

/*-----------------------------------------------------------------------
클래스명		: CEcoModelHierarchytreeRecordSet
기능			: tb_hierarchy_tree 테이블의 레코드 필드 값들의 입출력 데이터
포맷을 가지고 있는 클래스

작성일자		: 2012-12-20
마지막 수정일자	: 2012-12-20
작성자			: 최조영
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoModelHierarchytreeRecordSet : public CObject
{
private:

	UINT											m_id;
	UINT											m_pid;
	UINT											m_depth;
	CString											m_name;
	UINT											m_tbNick;
public:
	CEcoModelHierarchytreeRecordSet();
	~CEcoModelHierarchytreeRecordSet();

	CEcoModelHierarchytreeRecordSet& operator = (const CEcoModelHierarchytreeRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_id;
			ar & m_pid;
			ar & m_depth;
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
			ar & m_tbNick;
		}
		else
		{
			ar & m_id;
			ar & m_pid;
			ar & m_depth;
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_tbNick;
		}
	}

public:
	void SetID(UINT uid);
	UINT GetID();

	void SetPID(UINT uid);
	UINT GetPID();

	void SetDepth(UINT uid);
	UINT GetDepth();

	void SetName(CString Name);
	CString GetName();

	void SetTbNickName(UINT uid);
	UINT GetTbNickName();
};


/*-----------------------------------------------------------------------
클래스명		: CEcoModelDataTableSet
기능			: tb_ecomodel_??? 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스

작성일자		: 2012-12-20
마지막 수정일자	: 2012-12-20
작성자			: 최조영
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoModelDataTableSet : public CObject
{
private:
	UINT											m_fid;
	CString											m_name;//
	CString											m_type;//
	COleDateTime									m_update_date;
	CString											m_hsf_path;
	int												m_hsf_size;
	double											m_measure_x_min;
	double											m_measure_x_max;
	double											m_measure_x_center;
	double											m_measure_y_min;
	double											m_measure_y_max;
	double											m_measure_y_center;
	double											m_measure_z_min;
	double											m_measure_z_max;
	double											m_measure_z_center;
	CString											m_reserv_1;
	CString											m_reserv_2;
	CString											m_reserv_3;
	CString											m_reserv_4;
	CString											m_reserv_5;


public:
	CEcoModelDataTableSet();
	~CEcoModelDataTableSet();

	CEcoModelDataTableSet& operator = (const CEcoModelDataTableSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_fid;
			stdStr = CStringConverter::CStringWToCStringA(m_name);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_type);
			ar & stdStr;
			ar & m_update_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_hsf_path);
			ar & stdStr;
			ar & m_hsf_size;
			ar & m_measure_x_min;
			ar & m_measure_x_max;
			ar & m_measure_x_center;
			ar & m_measure_y_min;
			ar & m_measure_y_max;
			ar & m_measure_y_center;
			ar & m_measure_z_min;
			ar & m_measure_z_max;
			ar & m_measure_z_center;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_1);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_2);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_3);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_4);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_reserv_5);
			ar & stdStr;
		}
		else
		{
			ar & m_fid;
			ar & stdStr;
			m_name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_type = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_update_date.m_dt;
			ar & stdStr;
			m_hsf_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_hsf_size;
			ar & m_measure_x_min;
			ar & m_measure_x_max;
			ar & m_measure_x_center;
			ar & m_measure_y_min;
			ar & m_measure_y_max;
			ar & m_measure_y_center;
			ar & m_measure_z_min;
			ar & m_measure_z_max;
			ar & m_measure_z_center;
			ar & stdStr;
			m_reserv_1 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_2 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_3 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_4 = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_reserv_5 = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void SetFid(UINT fid);
	UINT GetFid();

	void SetDataName(CString name);
	CString GetDataName();

	void SetType(CString type);
	CString GetType();

	void SetUpdateDate(COleDateTime date);
	COleDateTime GetUpdateDate();

	void SetHsfPath(CString path);
	CString GetHsfPath();

	void SetHsfSize(int size);
	int GetHsfSize();

	void SetMeasure(double x_min, double x_max, double x_center, double y_min, double y_max, double y_center, double z_min, double z_max, double z_center);
	void GetMeasure(double* x_min, double* x_max, double* x_center, double* y_min, double* y_max, double* y_center, double* z_min, double* z_max, double* z_center);

	void SetReserv1(CString val);
	CString GetReserv1();

	void SetReserv2(CString val);
	CString GetReserv2();

	void SetReserv3(CString val);
	CString GetReserv3();

	void SetReserv4(CString val);
	CString GetReserv4();

	void SetReserv5(CString val);
	CString GetReserv5();
};



/*-----------------------------------------------------------------------
클래스명		: CVariableGroupParentCheckSet
기능			: tb_variable_group 테이블의 레코드 필드 값들의 입출력 데이터
포맷을 가지고 있는 클래스

작성일자		: 2013-04-10
마지막 수정일자	: 2013-04-10
작성자			: 최조영
-----------------------------------------------------------------------*/
class AFX_EXT_API CVariableGroupParentCheckSet : public CObject
{
private:

	UINT											m_id;
	UINT											m_pid;
	UINT											m_depth;
public:
	CVariableGroupParentCheckSet();
	~CVariableGroupParentCheckSet();

	CVariableGroupParentCheckSet& operator = (const CVariableGroupParentCheckSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		ar & m_id;
		ar & m_pid;
		ar & m_depth;
	}

public:
	void SetID(UINT uid);
	UINT GetID();

	void SetPID(UINT uid);
	UINT GetPID();

	void SetDepth(UINT uid);
	UINT GetDepth();
};

/*-----------------------------------------------------------------------
클래스명		: CEcoUserTemplateInfo
기능			: tb_user_template_info 테이블의 레코드 필드 값들의 입출력 데이터
포맷을 가지고 있는 클래스

작성일자		: 2014-06-10
마지막 수정일자	: 2014-06-10
작성자			: 손지훈
-----------------------------------------------------------------------*/
class AFX_EXT_API CEcoUserTemplateInfo : public CObject
{
private:

	UINT											m_no;
	CString											m_userID;
	CString											m_tempName;
	CString											m_lastDate;
	UINT											m_usedCount;
public:
	CEcoUserTemplateInfo();
	~CEcoUserTemplateInfo();

	CEcoUserTemplateInfo& operator = (const CEcoUserTemplateInfo &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_no;
			stdStr = CStringConverter::CStringWToCStringA(m_userID);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_tempName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_lastDate);
			ar & stdStr;
			ar & m_usedCount;
		}
		else
		{
			ar & m_no;
			ar & stdStr;
			m_userID = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_tempName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_lastDate = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_usedCount;
		}
	}

public:

	void SetNo(UINT uno);
	UINT GetNo();
	void SetID(CString uid);
	CString GetID();
	void SetTempName(CString strTempName);
	CString GetTempName();
	void SetLastDate(CString strLastDate);
	CString GetLastDate();
	void SetUsedCount(UINT count);
	UINT GetUsedCount();

};

typedef struct
{
	CString AM_Path;
	CString Sc001d_Path;
	CString	str_Project;
	CString	str_ID;
	CString str_PW;
	CString str_MDB;
}AMtoTerminalinfo;

typedef struct _ship_no_info
{
	UINT nShipNoID;
	CString group;
	CString ship_no;
	CString create_time;
	CString update_time;
	CString bActive;
	CString str_original_group;
	CString str_original_ship_no;
} SHIP_NO_INFO;

typedef struct _pe_info
{
	CString pe_name;
	CString block_list;
} PE_INFO;

typedef struct _project_with_property
{

private:
	int m_nPjtRSCnt;
	int m_nPjtPropRSCnt;

public:
	CArray<CProjectRecordSet, CProjectRecordSet&> arrPjtRS;
	CArray<CProjectPropertyRecordSet, CProjectPropertyRecordSet&> arrPjtPropRS;
	_project_with_property& operator = (const _project_with_property &other)
	{
		arrPjtRS.Copy(other.arrPjtRS);
		arrPjtPropRS.Copy(other.arrPjtPropRS);
		return *this;
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		if(bSendNRecv == TRUE)
		{
			m_nPjtRSCnt = arrPjtRS.GetCount();
			ar & m_nPjtRSCnt;
			for(int i = 0; i < m_nPjtRSCnt; i++)
				arrPjtRS.GetAt(i).serialize(ar, version, bSendNRecv);

			m_nPjtPropRSCnt = arrPjtPropRS.GetCount();
			ar & m_nPjtPropRSCnt;
			for(int i = 0; i < m_nPjtPropRSCnt; i++)
				arrPjtPropRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nPjtRSCnt;
			CProjectRecordSet rs; arrPjtRS.RemoveAll();
			for(int i = 0; i < m_nPjtRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				arrPjtRS.Add(rs);
			}

			ar & m_nPjtPropRSCnt;
			CProjectPropertyRecordSet rs2; arrPjtPropRS.RemoveAll();
			for(int i = 0; i < m_nPjtPropRSCnt; i++)
			{
				rs2.serialize(ar, version, bSendNRecv);
				arrPjtPropRS.Add(rs2);
			}
		}
	}
} PROJECT_WITH_PROPERTY;

typedef struct _update_model_info
{
	CString	name;
	COleDateTime update_date;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(name);
			ar & stdStr;
			ar & update_date.m_dt;
		}
		else
		{
			ar & stdStr;
			name = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & update_date.m_dt;
		}
	}
} UPDATE_MODEL_INFO;

/*-----------------------------------------------------------------------
클래스명		: Ta510021
기능			: Ta510021 테이블의 레코드 필드 값들의 입출력 데이터
포맷을 가지고 있는 클래스

작성일자		: 2014-06-10
마지막 수정일자	: 2014-06-10
작성자			: 손지훈
-----------------------------------------------------------------------*/
class AFX_EXT_API CJoinTa510021 : public CObject
{
private:

	CString											m_shpno;
	CString											m_shpKnd;
	CString											m_blkno;
	CString											m_blkType;
	CString											m_blknoU;
public:
	CJoinTa510021();
	~CJoinTa510021();

	CJoinTa510021& operator = (const CJoinTa510021 &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_shpno);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_shpKnd);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_blkno);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_blkType);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_blknoU);
			ar & stdStr;
		}
		else
		{
			m_shpno = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_shpKnd = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_blkno = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_blkType = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_blknoU = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
		}
	}

public:

	void SetSHPNo(CString shpno);
	CString GetSHPNo();
	void SetSHPKND(CString shpKnd);
	CString GetSHPKND();
	void SetBLKNo(CString blkno);
	CString GetBLKNo();
	void SetBLK_Type(CString blkType);
	CString GetBLK_Type();
	void SetBLKNo_U(CString blknoU);
	CString GetBLKNo_U();

};

/*-----------------------------------------------------------------------
클래스명		: CTa510020
기능			: CTa510020 테이블의 레코드 필드 값들의 입출력 데이터
포맷을 가지고 있는 클래스

작성일자		: 2014-06-10
마지막 수정일자	: 2014-06-10
작성자			: 손지훈
-----------------------------------------------------------------------*/
class AFX_EXT_API CTa510020 : public CObject
{
private:

	CString											m_shpno;
	CString											m_shpKnd;

public:
	CTa510020();
	~CTa510020();

	CTa510020& operator = (const CTa510020 &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_shpno);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_shpKnd);
			ar & stdStr;

		}
		else
		{
			m_shpno = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_shpKnd = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;

		}
	}

public:

	void SetSHPNo(CString shpno);
	CString GetSHPNo();
	void SetSHPKND(CString shpKnd);
	CString GetSHPKND();

};

/*-----------------------------------------------------------------------
클래스명		: CMeasurePointRecordSet
기능			: MeasurePoint 테이블의 레코드 필드 값들의 입출력 데이터
                  포맷을 가지고 있는 클래스(EcoSystem 2009 ERD 참조)

작성일자		: 2015-08-06
마지막 수정일자	: 2015-08-06
작성자			: 최조영
-----------------------------------------------------------------------*/
class AFX_EXT_API CMESMeasurePointRecordSet : public CEcoTreeBaseRecordSet
{
	CString											m_project_group;
	CString											m_ship_no;
	CString											m_user_id;
	COleDateTime									m_measure_date;
	CString											m_measure_data_up_down_path;
	int												m_measure_data_size;
	int												m_measure_data_count;

public:
	CMESMeasurePointRecordSet();
	~CMESMeasurePointRecordSet();

	CMESMeasurePointRecordSet& operator = (const CMESMeasurePointRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_project_group);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_ship_no);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_user_id);
			ar & stdStr;
			ar & m_measure_date.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_measure_data_up_down_path);
			ar & stdStr;
			ar & m_measure_data_size;
			ar & m_measure_data_count;
		}
		else
		{
			ar & stdStr;
			m_project_group = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_ship_no = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_user_id = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_measure_date.m_dt;
			ar & stdStr;
			m_measure_data_up_down_path = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_measure_data_size;
			ar & m_measure_data_count;
		}
	}

public:
	void SetProjectGroup(CString project_group);
	CString GetProjectGroup();

	void SetShipNo(CString ship_no);
	CString GetShipNo();

	void SetUserID(CString user_id);
	CString GetUserID();

	void SetMeasureDate(COleDateTime measure_date);
	COleDateTime GetMeasureDate();

	void SetMeasureData(CString measure_data);
	CString GetMeasureData();

	void SetMeasureDataSize(int measure_data_size);
	int GetMeasureDataSize();

	void SetMeasureDataCount(int measure_data_count);
	int GetMeasureDataCount();
};

/*-----------------------------------------------------------------------
기능			: 각각의 측정점 정보가 들어가는 tb_measure_point_data 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		:
리턴			:
작성내역		:
하남국-20151118, 최초작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CMeasurePointDataRecordSet : public CEcoRefBaseRecordSet
{
private:
	CString m_strName;
	double	m_dblCoordX;
	double	m_dblCoordY;
	double	m_dblCoordZ;
public:
	CMeasurePointDataRecordSet();
	~CMeasurePointDataRecordSet();

	CMeasurePointDataRecordSet& operator = (const CMeasurePointDataRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoRefBaseRecordSet::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strName);
			ar & stdStr;
			ar & m_dblCoordX;
			ar & m_dblCoordY;
			ar & m_dblCoordZ;
		}
		else
		{
			ar & stdStr;
			m_strName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_dblCoordX;
			ar & m_dblCoordY;
			ar & m_dblCoordZ;
		}
	}

public:
	void SetName(CString strName);
	CString GetName();

	void SetCoord(double dblX, double dblY, double dblZ);
	void GetCoord(double& dblX, double& dblY, double& dblZ);
};

/*-----------------------------------------------------------------------
기능			: CDimensionVariableRecordSet 테이블의 레코드 필드 값들의 입출력 데이터 포맷을 가지고 있는 클래스
매개변수		: 
리턴			: 
작성내역  		: 
하남국-20150824, 최초작성
-----------------------------------------------------------------------*/
class AFX_EXT_API CDimensionVariableRecordSet : public CEcoTreeBaseRecordSet
{
public:
	CDimensionVariableRecordSet();
	~CDimensionVariableRecordSet();

	CDimensionVariableRecordSet& operator = (const CDimensionVariableRecordSet &other);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CEcoTreeBaseRecordSet::serialize(ar, version, bSendNRecv);
	}
};
