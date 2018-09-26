#ifndef _MW_TIMER_H_
#define _MW_TIMER_H_

#include <sys/timeb.h>

#include "mwstring.h"

#define MWTIMER_KILO 1000
#define MWTIMER_MILLION 1000000
#define MWTIMER_SECONDSPERDAY 86400
#define MWTIMER_SECONDSPERHOUR 3600
#define MWTIMER_SECONDSPERMINUTE 60

#define MWTIMER_TIMEZONEHOURS_SHANGHAI 8
#define MWTIMER_TIMESTRING_MAXLENGTH 16

using namespace std;

class MWTimer
{
private:
	timeb _start_time;

public:
	static double get_time(timeb* time_struct_pt = NULL);
	static double endure(double start_time);
	static MWString current_time(const time_t timezone_hours = MWTIMER_TIMEZONEHOURS_SHANGHAI);
	static MWString get_time_string(const time_t milliseconds_since_epoch, const time_t timezone_hours = 0);

	void start();
	double endure();
};

#endif
