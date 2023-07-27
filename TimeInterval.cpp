#include "TimeInterval.h"

#include <iomanip>

TimeInterval::TimeInterval()
{
	intervalDays = 0;
	intervalSeconds = 0;
	intervalMicroSeconds = 0;
}


TimeInterval::TimeInterval(long days, long seconds, long u_seconds)
{
	intervalDays = days;
	intervalSeconds = seconds;
	intervalMicroSeconds = u_seconds;

	this->normalize();
}


TimeInterval::~TimeInterval()
{
	// Nothing to do...
}


void TimeInterval::setInterval(long days, long seconds, long u_seconds)
{
	intervalDays = days;
	intervalSeconds = seconds;
	intervalMicroSeconds = u_seconds;

	this->normalize();
}


TimeInterval TimeInterval::operator+(const TimeInterval &inc) const
{
	TimeInterval result;

	result.intervalDays = intervalDays + inc.intervalDays;
	result.intervalSeconds = intervalSeconds + inc.intervalSeconds;
	result.intervalMicroSeconds = intervalMicroSeconds + inc.intervalMicroSeconds;

	// Normalize the resulting sum
	result.normalize();

	return result;
}


TimeInterval& TimeInterval::operator+=(const TimeInterval &inc)
{
	*this = *this + inc;

	return *this;
}


TimeInterval TimeInterval::operator-(const TimeInterval &dec) const
{
	TimeInterval result;

	result.intervalDays = intervalDays - dec.intervalDays;
	result.intervalSeconds = intervalSeconds - dec.intervalSeconds;
	result.intervalMicroSeconds = intervalMicroSeconds - dec.intervalMicroSeconds;

	// Normalize the resulting difference
	result.normalize();

	return result;
}


TimeInterval& TimeInterval::operator-=(const TimeInterval &dec)
{
	*this = *this - dec;

	return *this;
}


TimeInterval TimeInterval::operator*(long n) const
{
	TimeInterval result;

	result.intervalDays = n * intervalDays;
	result.intervalSeconds = n * intervalSeconds;
	result.intervalMicroSeconds = n * intervalMicroSeconds;

	// Normalize the resulting product
	result.normalize();

	return result;
}


TimeInterval operator*(long n, const TimeInterval &t)
{
	return t * n;
}


TimeInterval& TimeInterval::operator*=(long n)
{
	*this = *this * n;

	return *this;
}


bool operator==(const TimeInterval &t1, const TimeInterval &t2)
{
	return 	(t1.intervalDays == t2.intervalDays) &&
		(t1.intervalSeconds == t2.intervalSeconds) &&
		(t1.intervalMicroSeconds == t2.intervalMicroSeconds);
}


bool operator!=(const TimeInterval &t1, const TimeInterval &t2)
{
	return 	!(t1 == t2);
}


bool operator<(const TimeInterval &t1, const TimeInterval &t2)
{
	//
	// The correctness of this comparison relies on the fact that the signs of
	// each of the components of the interval are the same, and also that the number
	// of microseconds is less than one second and the number of seconds is less
	// than one day. We only need to find a difference in the most significant
	// component (days > seconds > microseconds) to determine the result.
	//
	if (t1.intervalDays < t2.intervalDays) {
		return true;
	}
	else if (t1.intervalDays == t2.intervalDays) {
		if (t1.intervalSeconds < t2.intervalSeconds) {
			return true;
		}
		else if (t1.intervalSeconds == t2.intervalSeconds) {
			if (t1.intervalMicroSeconds < t2.intervalMicroSeconds) {
				return true;
			}
		}
	}

	return false;
}


bool operator<=(const TimeInterval &t1, const TimeInterval &t2)
{
	return !(t1 > t2);
}


bool operator>(const TimeInterval &t1, const TimeInterval &t2)
{
	return (t2 < t1);
}


bool operator>=(const TimeInterval &t1, const TimeInterval &t2)
{
	return !(t1 < t2);
}


std::ostream& operator<<(std::ostream &output, const TimeInterval &t)
{
	//
	// Printing the value of a TimeInterval is complicated by the separate storage
	// of seconds and microseconds, which get printed together in the form X.XXXXXX s.
	// When microseconds are negative, we don't need to print the "-" character in front,
	// since it will be printed in front of the seconds (which may be zero).
	//
	if (t.intervalMicroSeconds >= 0) {
		output << "[" << t.intervalDays << " days, "
			<< t.intervalSeconds << "." << std::setw(6) << std::setfill('0') << t.intervalMicroSeconds << " s]";
	}
	else {
		if (t.intervalSeconds == 0) {
			output << "[" << t.intervalDays << " days, "
				<< "-" << t.intervalSeconds << "." << std::setw(6) << std::setfill('0') << -t.intervalMicroSeconds << " s]";
		}
		else {
			output << "[" << t.intervalDays << " days, "
				<< t.intervalSeconds << "." << std::setw(6) << std::setfill('0') << -t.intervalMicroSeconds << " s]";
		}
	}

	return output;
}


void TimeInterval::normalize()
{
	long seconds;
	long days;

	//
	// Roll microseconds beyond 999999 into seconds
	//
	seconds = intervalMicroSeconds / 1000000;
	intervalMicroSeconds %= 1000000;
	intervalSeconds += seconds;

	//
	// Roll seconds beyond 86399 into days
	//
	days = intervalSeconds / 86400;
	intervalSeconds %= 86400;
	intervalDays += days;

	//
	// Ensure that sign of all components is the same as the most significant
	// non-zero component.
	//
	if (intervalDays < 0) {
		if (intervalMicroSeconds > 0) {
			intervalMicroSeconds -= 1000000;
			intervalSeconds++;
		}

		if (intervalSeconds > 0) {
			intervalSeconds -= 86400;
			intervalDays++;
		}
	}
	else if (intervalDays > 0) {
		if (intervalMicroSeconds < 0) {
			intervalMicroSeconds += 1000000;
			intervalSeconds--;
		}

		if (intervalSeconds < 0) {
			intervalSeconds += 86400;
			intervalDays--;
		}
	}
	else {
		if (intervalSeconds < 0) {
			if (intervalMicroSeconds > 0) {
				intervalMicroSeconds -= 1000000;
				intervalSeconds++;
			}
		}
		else if (intervalSeconds > 0) {
			if (intervalMicroSeconds < 0) {
				intervalMicroSeconds += 1000000;
				intervalSeconds--;
			}
		}
	}
}
