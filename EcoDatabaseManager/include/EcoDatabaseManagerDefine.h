#pragma once

#define streq(a,b)      (!strcmp(a,b))

#define			DB_MYSQL						0
#define			DB_MSSQL						1
#define			DB_ORACLE						2

#define			OK								0x0000
#define			DB_CONNECT_ERROR				0x0001
#define			DB_DISCONNECT_ERROR				0x0002
#define			DB_QUERY_ERRO					0x0003
#define			DB_NO_RES_DATA					0x0004
#define			DB_EXIST_DATA					0x0005
#define			NO_MAC_INFO						0x0006
#define			DB_INSERT_FAIL					0x0007
#define			CAN_NOT_FIND_FILE				0x0009
#define			FTP_CONNECT_ERROR				0x0009
#define			NO_PRIV							0x0010
#define			CAN_NOT_FIND_PARENT				0x0011
#define			LACK_INFORMATION				0X0012

#define			UPDATE_BY_CAT_LIST				1
#define			UPDATE_BY_NEUTRAL_FILE			2

#define			INSERT_ONLY						3
#define			INSERT_AFTER_VERIFY				4

#define			REFRESH_ALL						0
#define			REFRESH_ONLY_ACITVE				1
#define			REFRESH_ONLY_DEACITVE			2

#define			PROPERTY_SHIP_NO				1
#define			PROPERTY_BLOCK					2
#define			PROPERTY_PROJECT				3

#define			 TYPE_NUMBER					101
#define			 TYPE_TEXT						102
#define			 TYPE_ENUM						103
#define			 TYPE_DATE						104
#define			 TYPE_USER						105

#define			PRIVATE_NO	0
#define			PRIVATE_OWN	1
#define			PRIVATE_YES	2