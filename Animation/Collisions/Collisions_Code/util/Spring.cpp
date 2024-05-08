#include "Spring.h"

Spring::Spring() :
p_one(-1), p_two(-1),
ks(0.0), kd(0.0), rest_length(0.0)
{
}

Spring::Spring(int p_one, int p_two, double ks, double kd, double rest_length) :
	p_one(p_one), p_two(p_two),
	ks(ks), kd(kd), rest_length(rest_length)
{
}