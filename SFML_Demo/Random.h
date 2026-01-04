#pragma once
#include <random>
class Random
{
	// Static means the variable belongs to the class, not the instance of the class. All instances share this variable
	static std::mt19937 random;
public:

	//only one of this fuction (static)
	static int Int(int min, int max);
	static float Float(float min, float max);

};
