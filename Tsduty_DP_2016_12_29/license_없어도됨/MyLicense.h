#ifndef MY_LICENSE_H
#define MY_LICENSE_H
typedef struct License
{
	const char* bytes;
	size_t length;
}
MyLicense;
#ifdef __cplusplus
	extern "C"
#else
	extern
#endif
const MyLicense myLicense;
#endif
