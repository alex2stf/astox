/*
 * Date.cpp
 *
 *  Created on: Mar 30, 2015
 *      Author: alexandru
 */




#include "astox.h"



namespace astox {

    const char * Date::monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const char * Date::dayNames[] = {"Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"};
    Date * Date::_dateMinOff = NULL;
    bool Date::_minOffsetCalculated = false;
    int Date::daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    using namespace std;

#define MST (-7)
#define UTC (0)
#define CCT (+8)

    Date::Date() : Value() {
        valueType = ASTOX_VALUE_DATE;
        std::time_t rawtime;
#ifdef ASTOX_OS_WINDOWS
        std::tm timeinfo;
        rawtime = time(0);
        localtime_s(&timeinfo, &rawtime);
        std::tm gmt_timeinfo;
        std::time_t gmt_rawtime;
        gmt_rawtime = time(0);
        gmtime_s(&gmt_timeinfo, &gmt_rawtime);
        digestUTCDate(&gmt_timeinfo);
        digestDate(&timeinfo, true);
#else
        std::time(&rawtime);
        std::time_t t_gmt_time_in;
        digestUTCDate(gmtime(&t_gmt_time_in));
        digestDate(std::localtime(&rawtime));
#endif
    }

    Date::Date(long in) {
		

		std::time_t t = in;
		std::tm tmstrt;
		
		#ifdef ASTOX_OS_WINDOWS
			localtime_s(&tmstrt, &t);
		#else
			localtime_r(&in, &tmstrt); //ok android
		#endif
        digestDate(&tmstrt);
        digestUTCDate(&tmstrt);
    }

    Date::Date(std::tm * mtime) {
        digestDate(mtime);
        digestUTCDate(mtime);
    };

    Date::Date(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds)
    : Value() {
        valueType = ASTOX_VALUE_DATE;

        setupDate(year, month, day, hours, minutes, seconds, milliseconds, year, month, day, hours, minutes, seconds, milliseconds);
    };

    bool Date::isLeapYear(int year) {
        if (year % 400 == 0) {
            return true;
        }
        if (year % 100 == 0) {
            return false;
        }
        if (year % 4 == 0) {
            return true;
        }
        return false;
    };

    int Date::getNumberOfDaysInMonth(int year, int monthIndex) {
        if (monthIndex == 1 && isLeapYear(year)) {
            return 29;
        }
        return Date::daysInMonth[monthIndex];
    };

 

    void Date::setupDate(int year, int month, int day, int hours, int minutes, int seconds, int milliseconds,
            int utc_year, int utc_month, int utc_day, int utc_hours, int utc_minutes, int utc_seconds, int utc_milliseconds) {


		//

        int refixMinDate = 0;

        if (year == 0) {
            year = 1899;
            if (day == 0) {
                day = 31;
            }
            if (month == 0) {
                month = 11;
            }
        }

        if (year < 0) {
            refixMinDate = year;
        }

        if (year >= 0 && year <= 99) {
            year += 1900;
        }


        bool yearBelow = (year < 1970);
        bool dateBelow = (year == 1970 && day <= 0);

        _year = year - 1900;
        _fullYear = year;

		
        std::tm time_in;
        time_in.tm_year = _year;
        time_in.tm_mon = month;
        time_in.tm_mday = day;
        time_in.tm_hour = hours;
        time_in.tm_min = minutes;
        time_in.tm_sec = seconds;
        time_in.tm_isdst = -1;

        std::tm gmt_time_in;
        gmt_time_in.tm_year = _year;
        gmt_time_in.tm_mon = month;
        gmt_time_in.tm_mday = day;
        gmt_time_in.tm_hour = hours;
        gmt_time_in.tm_min = minutes;
        gmt_time_in.tm_sec = seconds;


		#ifdef ASTOX_OS_WINDOWS
			std::time_t time_temp = std::mktime(&time_in);
			std::time_t t_gmt_time_in = std::mktime(&gmt_time_in);
			struct tm ptm;
			gmtime_s(&ptm, &t_gmt_time_in);
			digestUTCDate(&ptm);
			std::tm time_out;
			localtime_s(&time_out, &time_temp);
			digestDate(&time_out, false);
		#else
			gmt_time_in.tm_isdst = -1;
			std::time_t t_gmt_time_in = std::mktime(&gmt_time_in);
			struct tm * ptm = gmtime(&t_gmt_time_in);
			digestUTCDate(ptm);
			std::time_t time_temp = std::mktime(&time_in);
			std::tm *time_out = std::localtime(&time_temp);
			digestDate(time_out);
		#endif
    }

    void Date::digestUTCDate(std::tm* utc_timeinfo) {
        _utcHours = utc_timeinfo->tm_hour;
        _utcMins = utc_timeinfo->tm_min;
        _utcDay = utc_timeinfo->tm_mday;
        _utcMonth = utc_timeinfo->tm_mon;
        _utcHours = utc_timeinfo->tm_hour;
        _utcWeekDay = utc_timeinfo->tm_wday;
        _utcYear = utc_timeinfo->tm_year;
        _utcFullYear = _utcYear + 1900;
    };

    void Date::digestDate(std::tm * timeinfo, bool global) {
        _day = timeinfo->tm_mday;
        _month = timeinfo->tm_mon;
        _year = timeinfo->tm_year;
        _fullYear = _year + 1900;
        _hours = timeinfo->tm_hour;
        _minutes = timeinfo->tm_min;
        _seconds = timeinfo->tm_sec;
        _miliseconds = _seconds * 1000;
		//_weekDay = timeinfo->tm_wday; //this is another stadndard
	
		#ifdef ASTOX_OS_WINDOWS
			TIME_ZONE_INFORMATION TimeZoneInfo;
			GetTimeZoneInformation(&TimeZoneInfo);
			wstring ws(TimeZoneInfo.StandardName);
			string StandardNameStr(ws.begin(), ws.end());
			strStandardName = StandardNameStr;
			SYSTEMTIME GmtTime;
			SYSTEMTIME LocalTime;
			GmtTime.wDay = LocalTime.wDay = timeinfo->tm_wday;
			GmtTime.wDayOfWeek = LocalTime.wDayOfWeek = timeinfo->tm_wday;
			GmtTime.wHour = LocalTime.wHour = timeinfo->tm_hour;
			GmtTime.wMinute = LocalTime.wMinute = timeinfo->tm_min;
			GmtTime.wMonth = LocalTime.wMonth = timeinfo->tm_mon;
			GmtTime.wSecond = LocalTime.wSecond = timeinfo->tm_sec;
			GmtTime.wYear = LocalTime.wYear = _year;
			GetSystemTime(&LocalTime);
			TzSpecificLocalTimeToSystemTime(&TimeZoneInfo,
					&LocalTime,
					&GmtTime);

			_timeZoneOffset = ((_utcHours * 60 + _utcMins) - (_hours * 60 + _minutes));

			if (_timeZoneOffset > 0) {
				_timeZoneOffset = TimeZoneInfo.Bias;
			}
			_hourOffset = -(_timeZoneOffset / 60);
		#else
			_timeZoneOffset = -(timeinfo->tm_gmtoff / 60);
			_hourOffset = abs(_timeZoneOffset) / 60;
			strStandardName = timeinfo->tm_zone;
		#endif
			_weekDay = getDayOfWeek(_day, _month, _year);
    };

    std::string Date::toString() {
        string result; // string which will contain the result
        ostringstream convert; // stream used for the conversion
        convert << Date::dayNames[abs(_weekDay)]; // insert the textual representation of 'Number' in the characters in the stream
        convert << " ";
        convert << Date::monthNames[abs(_month)];
        convert << " ";
        if (_day < 10) {
            convert << "0";
        }
        convert << _day;
        convert << " ";
        convert << _fullYear;
        convert << " ";
        if (_hours < 10) {
            convert << "0";
        }
        convert << _hours;
        convert << ":";
        if (_minutes < 10) {
            convert << "0";
        }
        convert << _minutes;
        convert << ":";
        if (_seconds < 10) {
            convert << "0";
        }
        convert << _seconds;
        _commonStr = convert.str();
        convert << " GMT+";
        int hoff = getHourOffset();
        if (hoff < 999) {
            convert << "0";
        }
        convert << hoff;
        convert << " (";
        convert << strStandardName;
        convert << ")";

        result = convert.str();
        return result; //"Mon Mar 30 2015 10:56:48 GMT+0300 (EEST)";
    }

    std::string Date::getCommonStr() {
        toString();
        return _commonStr;
    };

    int Date::getDate() {
        return _day;
    };

    int Date::getDay() {
        return _weekDay;
    };

    int Date::getMonth() {
        return _month;
    };

    int Date::getYear() {
        return _year;
    };

    int Date::getHours() {
        return _hours;
    };

    int Date::getMinutes() {
        return _minutes;
    };

    int Date::getTimezoneOffset() {
        return _timeZoneOffset;
    };

    int Date::getSeconds() {
        return _seconds;
    };

    int Date::getUTCDate() {
        return _utcDay;
    };

    int Date::getUTCMonth() {
        return _utcMonth;
    };

    int Date::getUTCDay() {
        return _utcWeekDay;
    };

    int Date::getUTCMinutes() {
        return _utcMins;
    };

    int Date::getUTCHours() {
        return _utcHours;
    };

    int Date::getHourOffset() {
        return _hourOffset * 100;
    };

    int Date::getUTCFullYear() {
        return _utcFullYear;
    };

    int Date::getFullYear() {
        return _fullYear;
    };

    Date * Date::getMinOffset() {
        if (!_minOffsetCalculated) {
            _dateMinOff = new Date(1970, 0, 1);
            _minOffsetCalculated = true;
        }
        return _dateMinOff;
    }

	int Date::getMilliseconds() {
		return _miliseconds;
	};


	

	void Date::setMilliseconds(int pmsc) {
		printf("_miliseconds before: %i, input=%i\n", _miliseconds, pmsc);
		if (pmsc > 999) {
			_seconds += pmsc / 1000;
			pmsc = pmsc % 1000;
		}
		if (pmsc < 0) {
			printf("_miliseconds: less than 1000 %i\n", pmsc);
			_seconds -= (pmsc * -1) / 1000;
			pmsc = (pmsc * -1) % 1000;
			if (pmsc > 0) {
				pmsc = 1000 - pmsc;
				_seconds -= 1;
			}
		}

		_miliseconds = pmsc;
		printf("_miliseconds: %i\n", _miliseconds);
		setSeconds(_seconds);
	};

	void Date::setSeconds(int sec) {
		printf("_seconds before: %i, input=%i\n", _seconds, sec);
		if (sec > 59) {
			_minutes += sec / 60;
			sec = sec % 60;
		}

		if (sec < 0) {
			printf("sec: less than 0 %i\n", sec);
			_minutes -= (sec * -1) / 60;
			sec = (sec * -1) % 60;
			if (sec > 0) {
				sec = 60 - sec;
				_minutes -= 1;
			}
		}
		_seconds = sec;
		setMinutes(_minutes);
		printf("SECONDS: %i\n", _seconds);
	};

	void Date::setMinutes(int min) {
		if (min > 59) {
			_hours += min / 60;
			min = min % 60;
		}
		_minutes = min;
		printf("_minutes: %i\n", _minutes);
		printf("hours: %i\n", _hours);
		setHours(_hours);
	};


	void Date::setHours(int hrs) {
		if (hrs > 23) {
			_day += hrs / 24;
			hrs = hrs % 24;
		}

		_hours = hrs;
		printf("day: %i\n", _day);
		setDay(_day);
	};


	void Date::setDay(int dy) {
		int maxDays = getNumberOfDaysInMonth( _fullYear, _month);
		printf("maxDay allowed = %i\n", maxDays);
		if (dy > maxDays - 1) {
			_month += dy / maxDays;
			dy = dy / maxDays;
		}
		_day = dy;
		setMonth(_month);

		printf("day: %i\n", _day);
		printf("_month: %i\n", _month);
	};


	void Date::setMonth(int mth) {
		if (mth > 12) {
			_fullYear += mth / 12;
			mth = mth / 12;
		}
		_month = mth;
		printf("_month: %i\n", _month);
		_weekDay = getDayOfWeek(_day, _month, _fullYear);
		printf("_weekDay: %i\n", _weekDay);
	};


    int Date::getDayOfWeek(int d, int mnth, int y) {
		//0,   1,    2,   3,    4,    5,     6,    7,     8,     9,     10,    11
		//jan  feb   mar  apr   may   jun    jul   aug    sept   oct    nov    dec
		static int defaultTable[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
		static int leapTable[] = {6, 2, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

		int m;
		if (isLeapYear(y)) {
			m = leapTable[mnth];
		} else {
			m = defaultTable[mnth];
		}
		int c = y / 100;
		
		//return  (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;
		return (d + m + y + (y / 4) + c) % 7;
	}

    std::string Date::getStandardName() {
        return strStandardName;
    };


	
#ifdef ASTOX_OS_WINDOWS
int Date::SystemTime2StructTM(SYSTEMTIME *st, struct tm * ptm) {
        //	printf("YEAR: %i\n", st->wYear);
        ptm->tm_isdst = -1; /* mktime() computes whether this is */
        /* during Standard or Daylight time. */
        ptm->tm_sec = (int) st->wSecond;
        ptm->tm_min = (int) st->wMinute;
        ptm->tm_hour = (int) st->wHour;
        ptm->tm_mday = (int) st->wDay;
        ptm->tm_mon = (int) st->wMonth - 1;
        ptm->tm_year = (int) st->wYear - 1900;
        ptm->tm_wday = (int) st->wDayOfWeek;
        if ((time_t) mktime(ptm) == (time_t) - 1) {
            return 0;
        }
        return 1;
    }
#endif

}
