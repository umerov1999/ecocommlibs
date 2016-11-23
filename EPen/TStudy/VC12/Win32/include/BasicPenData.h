#pragma once
#include <string>
class  BasicPenData
{
public:
	BasicPenData(){};
	~BasicPenData(){};

public:
	virtual int GetEventType() = 0;
	virtual unsigned long long GetPageId() = 0;
	virtual const char* GetPenSerial() = 0;
	virtual const char* GetPageSerial() = 0;
	virtual int GetCoordinateX() = 0;
	virtual int GetCoordinateY() = 0;
	virtual char GetForce() = 0;
	virtual unsigned long long GetStampTime() = 0;
	virtual int GetPenType() = 0;
	virtual const char* GetBluetoothAddr() = 0;
	virtual const char* GetProductName() = 0;
	virtual const char* GetSoftwareVersion() = 0;
	virtual const char* GetHardwareVersion() = 0;
	virtual unsigned short GetPruductId() = 0;
	virtual unsigned short GetVenderId() = 0;
	
};

