#pragma once

#define ERROR_OK 0
#define ERROR_DPHID_SERVICE_NOT_FOUNT -1
#define ERROR_TSTUDY_DIGITAL_PEN_LIC_DLL_NOT_FOUND -2
#define ERROR_LICENSE_VALIDATION_FAILED -3
#define ERROR_SERVICE_START_FAILED -4
#define ERROR_LICENSE_TYPE_NOT_MATCH -5
#define ERROR_LICENSE_NOT_BEGIN -6
#define ERROR_LICENSE_EXPIRED -7
#define ERROR_UNKNOWN -8

typedef void(*PenConnectFun)(unsigned long long time, const char* penSerial, int penType);
typedef void(*PenDisconnectFun)(unsigned long long time,  const char* penSerial, int penType);
typedef void(*PenUpFun)(unsigned long long time,  const char* penSerial, int penType);
typedef void(*PenDownFun)(unsigned long long time,  const char* penSerial, int penType);
typedef void(*PenErrorFun)(unsigned long long time,  const char* penSerial, int penType);
typedef void(*PenBatteryCriticalFun)(unsigned long long time,  const char* penSerial, int penType);
typedef void(*PenBatteryWarningFun)(unsigned long long time,  const char* penSerial, int penType);
typedef void(*PenInfoFun)(unsigned long long time,  const char* penSerial, int penType, const char* productName, const char* btAddr, const char* sw, const char* hw, unsigned short pid, unsigned short vid);
typedef void(*PenCoordinateFun)(unsigned long long time,  const char* penSerial, int penType, const char* pageSerial, unsigned long long pageId, int nx, int ny, unsigned char force);
typedef void(*PenNewPageFun)(unsigned long long time,  const char* penSerial, int penType, const char* pageSerial, unsigned long long pageId);
class  DigitalPenHID
{
public:
	DigitalPenHID(){};
	virtual ~DigitalPenHID(){};
public:
	virtual int Start()=0;
	virtual void Stop()=0;
	virtual void SetNofityMessageWind(void* hWnd, unsigned int message)=0;
	virtual void SetPenConnectEventHandler(PenConnectFun p) = 0;
	virtual void SetPenDisonnectEventHandler(PenDisconnectFun p) = 0;
	virtual void SetPenUpEventHandler(PenUpFun p) = 0;
	virtual void SetPenDownEventHandler(PenDownFun p) = 0;
	virtual void SetPenCoordinateEventHandler(PenCoordinateFun p) = 0;
	virtual void SetPenErrorEventHandler(PenErrorFun p) = 0;
	virtual void SetPenBatteryCriticalEventHandler(PenBatteryCriticalFun p) = 0;
	virtual void SetPenBatteryWarningEventHandler(PenBatteryWarningFun p) = 0;
	virtual void SetPenInfoEventHandler(PenInfoFun p) = 0;
	virtual void SetPenNewPageEventHandler(PenNewPageFun p) = 0;
	virtual void SetHoverMode(bool bHoverMode, const char* penSerial) = 0;
	
};
extern  DigitalPenHID* getDigitalPenHIDInstance(const char* pLicData,int len);
extern void destoryDigitalPenHID(DigitalPenHID**pph);

