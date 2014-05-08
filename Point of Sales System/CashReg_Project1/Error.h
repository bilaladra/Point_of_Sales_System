#ifndef _Error
#define _Error

#include <windows.h>
#include <string>

using namespace std;

struct Error
{
public:
	char ErrorCode[3];
	char ErrorDescription[22];
};

#endif;