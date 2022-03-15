#include "utilities.h"


std::string stringReplaceAll(std::string str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
std::string floatToString(float  value, int precision)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << value;
	return ss.str();
}

float mapF(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double mapD(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void setFilteredValue(double &value, double newValue, double newValueWeight)
{
	value =  value * (1.f - newValueWeight) + newValue * newValueWeight;
}


std::chrono::time_point<std::chrono::high_resolution_clock> now() {
	return std::chrono::high_resolution_clock::now();
}

template <typename T>
double milliseconds(T t) {
	return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t).count() / 1000000;
}
std::string timeToString(double timeMs)
{
	if (timeMs <= 0)
		return "0 ms";

	std::string timeStr = "";

	if (timeMs < 1)
	{
		timeMs *= 1000;
		int us = timeMs;
		timeMs -= (double)us;
		timeMs *= 1000;
		int ns = timeMs;
		timeMs -= (double)ns;

		timeStr = std::to_string(us) + " us";
		if (ns)
			timeStr += " " + std::to_string(ns) + " ns";
	}
	else
	{
		size_t ms = (size_t)timeMs % 1000;
		//timeMs = (timeMs-ms) * 1000.l;
		timeMs /= 1000;
		size_t rawS = (size_t)timeMs;

		size_t s = rawS % 60;
		size_t m = (rawS / 60) % 60;
		size_t h = rawS / 3600;

		if (h)
			timeStr = std::to_string(h) + " h";
		if (m || h)
			timeStr += " " + std::to_string(m) + " min";
		if (m || h || s)
			timeStr += " " + std::to_string(s) + " s";
		timeStr += " " + std::to_string(ms) + " ms";

	}
	return timeStr;
}
DebugFunctionTime::DebugFunctionTime(const std::string& funcName)
{
	m_functionName = funcName;

	std::cout << m_functionName << " begin\n";
		t1 = now();

}
DebugFunctionTime::~DebugFunctionTime()
{
	auto t2 = now();
	std::cout << m_functionName << " end time: " << timeToString(milliseconds(t2 - t1)) << "\n";
}