#pragma once

#include <iostream>
#include <string>
#include <sstream> // for using stringstream
#include <iomanip>
#include <vector>
#include <chrono>

std::string stringReplaceAll(std::string str, const std::string& from, const std::string& to);
std::string floatToString(float value, int precision);


float mapF(float x, float in_min, float in_max, float out_min, float out_max);
double mapD(double x, double in_min, double in_max, double out_min, double out_max);
void setFilteredValue(double &value, double newValue, double newValueWeight);

// helper functions for cleaner time measuring code
extern std::chrono::time_point<std::chrono::high_resolution_clock> now();
template <typename T>
extern double milliseconds(T t);
extern std::string timeToString(double timeMs);
class DebugFunctionTime
{
	public:
	DebugFunctionTime(const std::string& funcName);
	~DebugFunctionTime();

	private:
	std::chrono::time_point<std::chrono::high_resolution_clock> t1;
	std::string m_stackSpace;
	std::string m_functionName;

};


#define VECTOR_INSERT_ONCE(vec,elem) \
	for (size_t i = 0; i < vec.size(); ++i) \
		if (vec[i] == elem) \
			goto endVectorInsert; \
	vec.push_back(elem); \
	endVectorInsert:; 

#define VECTOR_REMOVE_ELEM(vec,elem) \
	for (size_t i = 0; i < vec.size(); ++i) \
		if (vec[i] == elem) \
		{ \
			vec.erase(vec.begin() + i); \
			if (i > 0) \
				--i; \
		}
