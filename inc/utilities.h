#pragma once

#include <string>
#include <vector>

std::string stringReplaceAll(std::string str, const std::string& from, const std::string& to);

float mapF(float x, float in_min, float in_max, float out_min, float out_max);
double mapD(double x, double in_min, double in_max, double out_min, double out_max);





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
