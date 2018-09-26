#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include "mwtimer.h"

double MWTimer::get_time(timeb* time_struct_pt)
{
	timeb* a_time_struct_pt;
	if (time_struct_pt == NULL)
	{
		a_time_struct_pt = new timeb;
		ftime(a_time_struct_pt);
	}
	else
	{
		a_time_struct_pt = time_struct_pt;
	}

	double ret = a_time_struct_pt->time + a_time_struct_pt->millitm / (double) MWTIMER_KILO;
	
	if (time_struct_pt == NULL)
	{
		delete a_time_struct_pt;
	}
	return ret;
}

MWString MWTimer::get_time_string(const time_t milliseconds_since_epoch, const time_t timezone_hours)
{
	time_t in_day_seconds((milliseconds_since_epoch / MWTIMER_KILO + timezone_hours * MWTIMER_SECONDSPERHOUR) % MWTIMER_SECONDSPERDAY);
	time_t in_hour_seconds(in_day_seconds % MWTIMER_SECONDSPERHOUR);
	time_t in_minute_seconds(in_hour_seconds % MWTIMER_SECONDSPERMINUTE);
	char time_char[MWTIMER_TIMESTRING_MAXLENGTH];
	snprintf(time_char, MWTIMER_TIMESTRING_MAXLENGTH, "%02u:%02u:%02u.%03u", static_cast<unsigned int>(in_day_seconds / MWTIMER_SECONDSPERHOUR), static_cast<unsigned int>(in_hour_seconds / MWTIMER_SECONDSPERMINUTE), static_cast<unsigned int>(in_minute_seconds), static_cast<unsigned int>(milliseconds_since_epoch % MWTIMER_KILO));
	return time_char;
}

MWString MWTimer::current_time(const time_t timezone_hours)
{
	timeval time_struct;
	gettimeofday(& time_struct, NULL);
	
	time_t in_day_seconds((time_struct.tv_sec + timezone_hours * MWTIMER_SECONDSPERHOUR) % MWTIMER_SECONDSPERDAY);
	time_t in_hour_seconds(in_day_seconds % MWTIMER_SECONDSPERHOUR);
	time_t in_minute_seconds(in_hour_seconds % MWTIMER_SECONDSPERMINUTE);
	char time_char[MWTIMER_TIMESTRING_MAXLENGTH];
	snprintf(time_char, MWTIMER_TIMESTRING_MAXLENGTH, "%02u:%02u:%02u.%06u", static_cast<unsigned int>(in_day_seconds / MWTIMER_SECONDSPERHOUR), static_cast<unsigned int>(in_hour_seconds / MWTIMER_SECONDSPERMINUTE), static_cast<unsigned int>(in_minute_seconds), static_cast<unsigned int>(time_struct.tv_usec));
	return time_char;
}

void MWTimer::start()
{
	ftime(& _start_time);
}

double MWTimer::endure()
{
	return endure(get_time(& _start_time));
}

double MWTimer::endure(double start_time)
{
	timeb current_time;
	ftime(& current_time);
	return get_time(& current_time) - start_time;
}
