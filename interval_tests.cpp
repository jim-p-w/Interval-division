#include <iostream>
#include "TimeInterval.h"

using namespace std;

/**
 * \brief Divides one time interval by another
 *
 * Given two TimeInterval objects -- a numerator and a denominator -- computes 
 * the quotient (as a long integer) and the remainder (as another TimeInterval)
 * when numerator is divided by denominator.
 *
 */
void divide(const TimeInterval &numerator,
            const TimeInterval &denominator,
            long &quotient,
            TimeInterval &remainder)
{
	quotient = 0;
	remainder = numerator;

	//
	// The simplest algorithm...
	//
	while (remainder >= denominator) {
		remainder -= denominator;
		quotient++;
	}

	//
	// Are there edge cases that aren't handled by the simple algorithm?
	//

	//
	// Are there algorithms that can arrive at the answer much more efficiently?
	//
}


int main(int argc, char **argv)
{
	TimeInterval numerator;
	TimeInterval denominator;
	TimeInterval remainder;
	long quotient;

	//
	// Add tests here to verify the correctness of your implementation
	// of interval division
	//

	numerator.setInterval(500, 0, 0);    // 500 days
	denominator.setInterval(0, 0, 200000);    // 0.2 seconds

	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;

	return 0;
}
