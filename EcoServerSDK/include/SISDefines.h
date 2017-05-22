#pragma once
#include <afxwin.h>  
#include <afxext.h>


enum NotificationState
//enum class NotificationState
{ 
	NS_NEW = 1, NS_UNREAD, NS_READ
};


enum UserAffiliation
//enum class UserAffiliation
{
	UA_COMPANY = 1, UA_DEPARTMENT, UA_POSITION, UA_ROLE
};

#define DEFAULT_FIELD_TYPE_CNT 17
enum FieldType
//enum class FieldType
{
	FT_SELECT_BOX = 1, FT_CHECK_BOX, FT_CHECK_LIST,
	FT_DATE, FT_TIME, FT_DATE_TIME,
	FT_TEXT, FT_MULTITEXT,
	FT_NUMBER,
	FT_FILE, FT_MULTIFILE,
	FT_USER, FT_COMPANY, FT_DEPARTMENT, FT_POSITION, FT_ROLE, FT_PROJECT_LINK
};

enum AssemTreeComposition
//enum class AssemTreeComposition
{
	ATC_MASTER = 1, ATC_GROUP, ATC_SHIPNO, ATC_BLOCK
};

enum ProgramPermiStatus
{
	PPS_NO = -1, PPS_OWN = 0, PPS_YES = 1
};

/*
* Project Types
*/
#define PJT_TYPE_PM					0x00000001L
#define PJT_TYPE_BLOCK_G2			0x00000002L
#define PJT_TYPE_OTS_G2				0x00000004L
#define PJT_TYPE_IMAGE				0x00000008L
#define PJT_TYPE_ELECTRONIC_PEN     0x00000010L
#define PJT_TYPE_BLOCK_G3			0x00000020L
#define PJT_TYPE_OTS_G3				0x00000040L