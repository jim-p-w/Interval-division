#include <cassert>
#include <iostream>
#include "TimeInterval.h"

using namespace std;

// factor for increasing the count of denominators
static const int DENOM_FACTOR = 2;

/**
 * \brief Reduce the provided numerator by succesive factors of the provided denominator.
 *
 * \return the amount the numerator was reduced by.
 */
long reduce_by_factor(long factor,
					  const TimeInterval &denominator,
					  TimeInterval &numerator)
{
	TimeInterval total = denominator;
	long quotient = 1;
	long delta = 1;
	int operations = 0;
	if (factor <= 1) {
		return 0;
	}

	// determine largest factor that denominator goes into numerator, and compute how many times
	// the denominator goes into that.
	// e.g. computing 15/3 using a factor of 2 would yield 4
	//    (try 2*3, then try 4*3, then try 8*3)
	while (numerator >= total) {
		delta *= factor;
		total = denominator * delta;
		operations += 2;
		//cout << __LINE__ << " delta:" << delta << " total:" << total << endl;
	}
	delta /= factor;
	total = denominator * delta;
	operations += 2;
	quotient = delta;
	//cout << "delta:" << delta << " total:" << total << " numerator:" << numerator << endl;

	// compute the number of times the the denominator goes into
	//   numerator - (largest factor * denominator)
	// by successively lowering the multiplier
	// e.g. computing 15/3, the largest factor found above is 4 and the total is 12
	// so try 12 + (4*3), which won't work,
	// then try 12 * (2*3) which won't work,
	// then try 12 + (1*3) which wil fit
	while (delta >= 1) {
		while (numerator >= total) {
			total += (denominator * delta);
			quotient += delta;
			operations += 2;
			//cout << "       delta:" << delta << " total:" << total << endl;
		}
		total -= (denominator * delta);
		quotient -= delta;
		delta /= factor;
		operations += 3;
		//cout << "delta:" << delta << " total:" << total << " numerator:" << numerator << endl;
	}

	//cout << "delta:" << delta << " total:" << total << " numerator:" << numerator << endl;
	//cout << "operations:" << operations << endl;
	numerator -= total;
	return quotient;
}

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

	const TimeInterval zero(0, 0, 0);

	if (denominator == zero) {
		throw std::invalid_argument("Denominator cannot be zero");
	}

	if (denominator < zero) {
		return;	// should an exception get thrown here?
	}

	if (numerator < zero) {
		return;	// should an exception get thrown here?
	}

#if 1
	quotient = reduce_by_factor(DENOM_FACTOR, denominator, remainder);
#else
	//
	// The simplest algorithm...
	//
	while (remainder >= denominator) {
		remainder -= denominator;
		quotient++;
	}
#endif

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
	const TimeInterval zero(0, 0, 0);
	long quotient;
	double seconds = 0.20;
	//
	// Add tests here to verify the correctness of your implementation
	// of interval division
	//

	numerator.setInterval(500, 0, 0);    // 500 days
	denominator.setInterval(0, 0, 200000);    // 0.2 seconds
	long expectedQuotient = (500 * 24 * 60 * 60) / seconds;

	// check for zero denominator
	try {
		divide(numerator, zero, quotient, remainder);
		assert(false);	// should not get here
	} catch(exception &e) {
		cout << "divide threw: " << e.what() << endl;
	}

	// check for numerator > denominator
	divide(numerator, denominator, quotient, remainder);
	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	cout << "exp q:" << expectedQuotient << endl;
	assert(quotient == expectedQuotient);
	assert(remainder == zero);

	// check for numerator < denominator
	divide(denominator, numerator, quotient, remainder);
	cout << numerator << " divides " << denominator << " " << quotient << " times with a remainder of " << remainder << endl;
	assert(quotient == 0);
	assert(remainder == denominator);

	// check for numerator == denominator
	divide(numerator, numerator, quotient, remainder);
	cout << numerator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	assert(quotient == 1);
	assert(remainder == zero);

	// some random tests
	time_t now = time(NULL);
	srandom(now);

	for (int i = 0; i < 10; i++) {
		int nDays = random() % 700;
		int nSeconds = random() % (24 * 60 * 60);
		int nUseconds = random() % 900000;
		long daySeconds = (nDays * 24 * 60 * 60) + nSeconds;
		TimeInterval days(nDays, nSeconds, 0);
		TimeInterval uSeconds(0, 0, nUseconds);
		long expectedQuotient = daySeconds / ( 0.000001 * nUseconds);
		long expectedRemainInt = daySeconds * 1000000 - ((nUseconds ) * expectedQuotient) ;
		TimeInterval expectedRemainder(0, 0, expectedRemainInt );

		divide(days, uSeconds, quotient, remainder);
		cout << uSeconds << " divides " << days << " " << quotient << " times, remainder:" << remainder << endl;
		assert(quotient == expectedQuotient);
		assert(expectedRemainder == remainder);

		divide(uSeconds, days, quotient, remainder);
		cout << days << " divides " << uSeconds << " " << quotient << " times, remainder:" << remainder << endl << endl;
		assert(quotient == 0);
		assert(remainder == uSeconds);
	}

	return 0;
}
