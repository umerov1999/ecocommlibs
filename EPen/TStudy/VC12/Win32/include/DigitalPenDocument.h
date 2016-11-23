#pragma once

class DigitalPenDocument
{
public:
	DigitalPenDocument(){};
	virtual ~DigitalPenDocument(){};
public:
	
	virtual bool LoadAfd(const char* afdFileName) = 0;
	virtual void UnLoadAfd() = 0;
	virtual bool ExtractBackgroundsImages(const char *desFolder) = 0;
	virtual int GetAfdPageCount() = 0;
	virtual void GetPageSize(int&width, int&height) = 0;
	virtual void GetPageRange(unsigned long long &startPage, unsigned long long &endPage) = 0;
	virtual int GetPageIndex(unsigned long long uPageAddressID) = 0;
	virtual int GetPageIndex(const char* pageAddressSerial) = 0;
	virtual bool IsPageInRange(unsigned long long uPageAddressID) = 0;
	virtual bool IsPageInRange(const char* pageAddressSerial) = 0;
	virtual void PageID2Serial(unsigned long long uPageAddressID, char* pageAddressSerial) = 0;
	virtual void PageSerial2ID(const char*pageAddressSerial, unsigned long long &uPageAddressID) = 0;
	virtual int PageSubtract(unsigned long long uEndPageID, unsigned long long uStartPageID) = 0;
	
};
extern DigitalPenDocument* getDigitalPenDocument(const char* pLicData);
extern void destoryDigitalPenDocument(DigitalPenDocument *&pDocument);
