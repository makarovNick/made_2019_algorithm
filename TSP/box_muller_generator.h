#pragma once

#include <cstdlib>
#include <cmath>

double boxMueller(double max_value) 
{
	double x1 = 0;
	double x2 = 0;
	double w = 0;

    do 
    {
        x1 = 2.0 * ((double)std::rand() / RAND_MAX) - 1.0;
        x2 = 2.0 * ((double)std::rand() / RAND_MAX) - 1.0;
        w = x1 * x1 + x2 * x2;
    } while (w >= 1.0); 

	return std::fabs(x1 * std::sqrt((-2.0 * std::log(w)) / w) * max_value);
}