/*
 * Dates.h
 *
 *  Created on: Feb 14, 2017
 *      Author: ChadMello
 */
#include <stdio.h>
#include <math.h>

typedef struct dates
{
	int month;
	int day;
	int year;
	int hour;
	int minutes;
	int seconds;
	double julian_day;
} date;

const date d0 = {0,0,0,0,0,0,0.0};

#define ZERO_DATE d0

 date addDays (double nDays, const date *d1);
 date addMonths (double nMonths, const date *d1);
 date addYears (double nYears, const date *d1);
 date addWeeks (double nWeeks, const date *d1);
 date addHours (double nHours, const date *d1);
 date addMinutes (double nMinutes, const date *d1);
 date addSeconds (double nSeconds, const date *d1);

 double getDayDiff(const date *d1, const date *d2);

//calculates and sets the julian_day member of a date
void setJulianDay (date *d);

// returns julian day (including time) from date
double getFullJulianDay(const date *d);

// returns a date struct from a julian day
date getDateFromJulian (double jd);

date addMinutes (double nMinutes, const date *d1)
{
	// now, create new date with Julian day + nMinutes
	return addDays((nMinutes/1440.0),d1);
}

date addSeconds (double nSeconds, const date *d1)
{
	// now, create new date with Julian day + nSeconds
	return addDays((nSeconds/86400.0),d1);
}

date addHours (double nHours, const date *d1)
{
	// now, create new date with Julian day + nHours
	return addDays((nHours/24.0),d1);
}

date addYears (double nYears, const date *d1)
{
	// now, create new date with Julian day + nYears
	return addDays((nYears*365.0),d1);
}

date addMonths (double nMonths, const date *d1)
{
	// now, create new date with Julian day + nMonths
	return addDays((nMonths*30.44),d1);
}

date addWeeks (double nWeeks, const date *d1)
{
	// now, create new date with Julian day + nWeeks
	return addDays((nWeeks*7),d1);
}

date addDays (double nDays, const date *d1)
{
	double jd = getFullJulianDay(d1);

	// now, create new date with Julian day + nDays
	return getDateFromJulian(jd + nDays);
}

double getDayDiff(const date *d1, const date *d2)
{
	double jd1 = getFullJulianDay(d1);
	double jd2 = getFullJulianDay(d2);
	return jd2-jd1;
}

double getFullJulianDay(const date *d)
{
	int B, A, Y, M;
	double JD;

	Y = d->year;
	M = d->month;

	if(M<=2)
	{
		Y--;
	 	M+=12;
	}

	A = floor(Y/100.0);
	B = 2-A+floor(A/4.0);

	//Calculate "day" part of JD
    JD = floor((365.25*(Y+4716.0)))
         +floor((30.6001*(M+1)))+d->day+B-1524.5;

    // Now, calculate "time" part of JD
	//  hour: 0-23, minutes: 0-59, seconds: 0-59
	JD+= (((d->hour*60.0f)*60.0f)+(d->minutes*60.0f)+d->seconds)/86400.0f;

    return JD;
}

void setJulianDay (date *d)
{
    d->julian_day=getFullJulianDay(d);
    return;
 }

date getDateFromJulian (double jd)
{
	int a,b,c,d,e,alpha;
	double z, f;
	date newDate;
	jd+=.5;

	// For now, we are not including the time of day in our calculations.
	// If we were, d would be a pointer to a float
	f = modf(jd, &z);

	if (z<2299161)
		a=z;
	else
	{
		alpha = floor((z-1867216.25)/36524.25);
		a = z+1+alpha-floor(alpha/4);
	}

	//perform setup calculations
	b = a + 1524;
	c = floor((b-122.1)/365.25);
	d = floor(365.25*c);
	e = floor((b-d)/30.6001);

	// Now, let's find the calendar components...
	// (day)
	(newDate.day) = b-d-floor(30.6001 * e)+f;

	// (month)
	if (e<14)
		(newDate.month) = e-1;
	else
		(newDate.month)=e-13;

	// (year)
	if ((newDate.month)>2)
		(newDate.year)=c-4716;
	else
		(newDate.year)=c-4715;

	newDate.julian_day = jd;

	// time of day
	if (f>0.0)
	{
		int seconds = f*86400.0;
		newDate.hour = (int)(seconds/3600);
		newDate.minutes = (int)((seconds%3600)/60);
		newDate.seconds = modf((seconds%3600)/60.0,&z)*60;
	}
	else
	{
		newDate.hour = 0;
		newDate.minutes = 0;
		newDate.seconds = 0;
	}

	return newDate;
}
