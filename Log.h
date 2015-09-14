#pragma once
#include <stdio.h>
class Log
{
public:
	Log();
	~Log();
	static void error(const char *msg)
	{
		fprintf(stderr, msg);
	}
	
};

