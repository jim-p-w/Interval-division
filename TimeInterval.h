/**
 * \class TimeInterval
 *
 * \brief Storage and manipulation of time intervals
 *
 * This class stores time intervals that may be as long as 10^16 years
 * (when sizeof(long) == 8) while having microsecond resolution.
 * Basic arithmetic for these intervals is supported for the following
 * operations:
 *     - addition
 *     - subtraction
 *     - multiplication of an interval by an integer
 *     - comparison (==, !=, <, <=, >, >=)
 *
 * Printing of a TimeInterval is supported through the extraction
 * operator <<, which causes the interval to be printed as [X days, X.XXXXXX s].
 *
 * The value of a time interval is specified as a sum of days, seconds,
 * and microseconds. Negative values for any of these components are
 * supported, e.g., -1 day plus 500000 microseconds is equivalent to
 * an interval of -86399.5 seconds.
 *
 * Examples:
 *
 *     TimeInterval a(1, 0, 0);      // Interval of 1 day
 *     TimeInterval b(0, 1, 500000); // Interval of 1.5 seconds
 *     TimeInterval zero;            // Intervals default to 0 days 0.0 s
 *
 *     while (a > zero) {
 *         a -= b;
 *         b *= 2;
 *         std::cout << a << std::endl;
 *     }
 *
 * \author Michael G. Duda, NCAR/MMM
 *
 * \date 8 July 2018
 *
 */

#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <iostream>

class TimeInterval
{

public:
	//
	// Constructors and destructors
	//
	TimeInterval();
	TimeInterval(long days, long seconds, long u_seconds);
	~TimeInterval();

	//
	// Set an interval to a specified length of time
	//
	void setInterval(long days, long seconds, long u_seconds);

	//
	// Operators
	//
	TimeInterval operator+(const TimeInterval &inc) const;
	TimeInterval& operator+=(const TimeInterval &inc);
	TimeInterval operator-(const TimeInterval &dec) const;
	TimeInterval& operator-=(const TimeInterval &dec);
	TimeInterval operator*(long n) const;
	TimeInterval& operator*=(long n);
	friend bool operator==(const TimeInterval &t1, const TimeInterval &t2);
	friend bool operator!=(const TimeInterval &t1, const TimeInterval &t2);
	friend bool operator<(const TimeInterval &t1, const TimeInterval &t2);
	friend bool operator<=(const TimeInterval &t1, const TimeInterval &t2);
	friend bool operator>(const TimeInterval &t1, const TimeInterval &t2);
	friend bool operator>=(const TimeInterval &t1, const TimeInterval &t2);
	friend std::ostream& operator<<(std::ostream& output, const TimeInterval& p);
	friend TimeInterval operator*(long n, const TimeInterval &t);

private:
	long intervalDays;		// Number of days in the interval
	long intervalSeconds;		// Number of seconds in the interval
	long intervalMicroSeconds;	// Number of microseconds in the interval

	//
	// Private methods
	//
	void normalize();		// Keep microseconds in the range [0,999999], keep seconds in the range [0,86399]
};

#endif
