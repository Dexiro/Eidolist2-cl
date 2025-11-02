#pragma once



namespace mathf
{
	static float DegToRad(float deg);


	template<typename T>
	bool in_range(const T& val, const T& min, const T& max)
	{
		return (val >= min && val <= max);
	}
}