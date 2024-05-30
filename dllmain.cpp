#include <algorithm>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <string_view>

#include "RVExtensionUtil.hpp"

#ifndef _WIN32
#include <signal.h>
#define debugBreak raise(SIGTRAP)
#else
#define debugBreak __debugbreak()
#endif

extern void ImguiRenderTick();

void CALL_CONVENTION RVExtension(char *output, unsigned int outputSize, const char *function)
{
	if (std::string_view(function) == "rendr")
	{
		ImguiRenderTick();
	}

	output[outputSize - 1] = 0;
}

void CALL_CONVENTION RVExtensionVersion(char *output, unsigned int outputSize)
{
	std::strncpy(output, "Test-Extension v1.0", outputSize - 1);
}