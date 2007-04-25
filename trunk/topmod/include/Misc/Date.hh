/* $Id: Date.hh,v 1.2 2000/07/20 00:57:27 vinod Exp $ */

#ifndef _DATE_HH_

#define _DATE_HH_

#include <time.h>
#include "../Base/StreamIO.hh"
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum Day { Sun=1, Mon, Tue, Wed, Thu, Fri, Sat };
enum Month { Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

class Date
{
  private :

        // Static data
     static const int DaysIn[13];                      // No. of days in each month (non-leap year)
     static const int TotalDays[2][13];                // Total days since Jan 1 at beg. of month
     static const char * DayName[8];                   // Names of days
     static const char * MonthName[13];                // Names of months

  public :

        // Static functions
     static void today(int& mm, int& dd, int& yy)      // Today's date
       {
         time_t t = time(NULL);
         struct tm * now = localtime(&t);
         dd = now->tm_mday; mm = now->tm_mon+1; yy = now->tm_year+1900;
       }

     static bool leap_year(int yy)                     // Is the given year a leap year?
       {
         if ( (yy % 400 == 0) || ( (yy % 100 != 0) && (yy % 4 == 0) ) ) return true;
         return false;
       }

     static bool valid(int mm, int dd, int yy)         // Is this a valid date?
       {
         if ( dd < 1 || dd > 31 || mm < Jan || mm > Dec ) return false;
         if ( leap_year(yy) && mm == Feb )
            {
              if ( dd > 29 ) return false;
              return true;
            }
         if ( dd > DaysIn[mm] ) return false;
         return true;
       }

     static Day dayOfWeek(int mm, int dd, int yy)      // What day of week is the given date?
       {
         struct tm tmstr;
  
         tmstr.tm_sec = 1; tmstr.tm_min = 0; tmstr.tm_hour = 0;
         tmstr.tm_mday = dd; tmstr.tm_mon = mm - 1; tmstr.tm_year = yy - 1900;
         mktime(&tmstr);
         return Day(tmstr.tm_wday+1);
       }

     static int daysInMonth(int mm, bool leap)         // No. of days in given month
       {
         if ( mm < Jan || mm > Dec ) return -1;
         if ( leap && mm == Feb ) return 29;
         return DaysIn[mm];
       }

     static int daysRemaining(int mm, int dd, int yy)  // Days remaining in this month?
       {
         return daysInMonth(mm,leap_year(yy)) - dd;
       }

     static bool endOfMonth(int mm, int dd, int yy)    // Are we at the end of the month?
       {
         if ( mm == Feb && leap_year(yy) )
            return (dd == 29);
         return (dd == DaysIn[mm]);
       }

     static int dayOfYear(int mm, int dd, int yy)      // Which day of the year is the given date?
       {
         return dd + TotalDays[ leap_year(yy) ? 1 : 0 ][mm];
       }

  protected :

     int d;                                            // Day of month
     int m;                                            // Month
     int y;                                            // Year

  public :

     Date()
       : m(0), d(0), y(0)
       {
         Date :: today(m,d,y);
       }

     Date(int mm, int dd, int yy)
       : m(mm), d(dd), y(yy)
       {
         if ( !valid(mm,dd,yy) ) Date :: today(m,d,y);
       }
     
     Date(struct tm date)
       {
         mktime(&date);
         d = date.tm_mday; m = date.tm_mon+1; y = date.tm_year+1900;
       }
     
     Date(const Date& date)
       : d(date.d), m(date.m), y(date.y)
       {}

     Date& operator = (const Date& date)
       {
         d = date.d; m = date.m; y = date.y;
         return (*this);
       }

     void set(int mm, int dd, int yy)
       {
            // If 2-digit year was given, adjust year to
            // lie in the 20th or 21st century
            // NOTE: This is the only function which checks this and
            // adjusts the year accordingly
         if ( yy < 100 ) // 2-digit year was given
            {
              if ( yy < 25 ) yy += 2000;
              else yy += 1900;
            }
         if ( Date :: valid(mm,dd,yy) )
            {
              d = dd; m = mm; y = yy;
            }
       }

     void day(int dd)
       {
         d = dd;
       }

     int day(void) const
       {
         return d;
       }

     void month(int mm)
       {
         m = mm;
       }

     int month(void) const
       {
         return m;
       }

     void year(int yy)
       {
         y = yy;
       }

     int year(void) const
       {
         return y;
       }

     bool leap_year(void) const                        // Is this date in a leap year?
       {
         return Date :: leap_year(y);
       }

     bool valid(void) const                            // Is this a valid date?
       {
         return Date :: valid(m,d,y);
       }

     Day dayOfWeek(void) const                         // Day of week for this date
       {
         return Date :: dayOfWeek(m,d,y);
       }
     
     int daysInMonth(void) const                       // No. of days in the month for this date
       {
         return Date :: daysInMonth(m,leap_year(y));
       }

     int daysRemaining(void) const                     // No. of days remaining in this month
       {
         return Date :: daysRemaining(m,d,y);
       }

     bool endOfMonth(void) const                       // Are we at the end of the month for this date?
       {
         return Date :: endOfMonth(m,d,y);
       }

     int dayOfYear(void) const                         // Which day of the year is this?
       {
         return Date :: dayOfYear(m,d,y);
       }

        // Comparison operators
     bool operator == (const Date& date)
       {
         if ( y != date.y || m != date.m || d != date.d ) return false;
         return true;
       }

     bool operator != (const Date& date)
       {
         return ! ( *this == date );
       }

     bool operator < (const Date& date)
       {
         if ( y < date.y ) return true;
         if ( y > date.y ) return false;
         if ( m < date.m ) return true;
         if ( m > date.m ) return false;
         if ( d < date.d ) return true;
         return false;          
       }

     bool operator > (const Date& date)
       {
         if ( *this < date || *this == date ) return false;
         return true;
       }


  protected :

        // Increment and decrement the date by given number of days
     void increment(int numdays=1)
       {
         int remain = daysRemaining();
         d = 1; // Needed for proper calculation of daysRemaining()
         while ( numdays > remain )
            {
                 // Go to next month
              numdays -= remain; numdays--; // to account for first day of next month
              m++;
              if ( m > 12 )
                 {
                   y++; m=1;
                 }
              remain = daysRemaining();
            }
         d = daysInMonth() - remain + numdays;
       }

     void decrement(int numdays=1)
       {
         int passed = d;
         while ( numdays >= passed )
            {
                 // Go to previous month. No need to adjust as in increment
              numdays -= passed; m--;
              if ( m < 1 )
                 {
                   y--; m=12;
                 }
              passed = daysInMonth();
            }
         d = passed - numdays;
       }

  public :
     
        // Operators for changing the date
     void operator += (int days)
       {
         increment(days);
       }

     void operator -= (int days)
       {
         decrement(days);
       }

     friend Date operator + (const Date& date, int days) // Addition of date and number of days
       {
         Date newdate(date); newdate += days;
         return newdate;
       }

     friend Date operator - (const Date& date, int days) // Subtraction of date and number of days
       {
         Date newdate(date); newdate -= days;
         return newdate;
       }

     void operator ++ (void)                           // prefix increment
       {
         increment();
       }

     void operator ++ (int)                            // postfix increment
       {
         increment();
       }
     
     void operator -- (void)                           // prefix decrement
       {
         decrement();
       }

     void operator -- (int)                            // postfix decrement
       {
         decrement();
       }

        // Increment/decrement by month/year
     void next_month(void)
       {
         m++;
         if ( m > 12 )
            {
              m = 1; y++;
            }
         if ( d > daysInMonth() ) d == daysInMonth();
       }

     void previous_month(void)
       {
         m--;
         if ( m < 1 )
            {
              m = 12; y--;
            }
         if ( d > daysInMonth() ) d = daysInMonth();
       }

     void next_year(void)
       {
         y++;
         if ( m == Feb && d == 29 ) d = 28; // no 2 leap years are consecutive
       }

     void previous_year(void)
       {
         y--;
         if ( m == Feb && d == 29 ) d = 28; // no 2 leap years are consecutive
       }

        //*** DO NOT CALL THIS FUNCTION FROM THE DEFAULT CONSTRUCTOR ***//
     static Date today(void)                           // Today's date
       {
         Date now;
         Date :: today(now.m,now.m,now.y);
         return now;
       }

     static Date yesterday(void)
       {
         Date now = Date :: today();
         now--;
         return now;
       }

     static Date tomorrow(void)
       {
         Date now = Date :: today();
         now++;
         return now;
       }

        // Difference between 2 dates, returned in number of days (can be negative)
     int operator - (const Date& date)
       {
            // Find the day number in the current year and use that to find difference
         int mydayno = dayOfYear();
         int dayno = date.dayOfYear();
         int y1 = y, y2 = date.y;

         if ( y1 < y2 )
            {
              while ( y2 > y1 )
                 {
                   y2--; dayno += 365;
                   if ( leap_year(y2) ) dayno++;
                 }
            }
         else if ( y2 < y1 )
            {
              while ( y1 > y2 )
                 {
                   y1--; mydayno += 365;
                   if ( leap_year(y1) ) mydayno++;
                 }
            }
         return (mydayno - dayno);
       }

     friend ostream& operator << (ostream& o, const Date& date)
       {
         o << date.m << '/' << date.d << '/' << date.y;
         return o;
       }

     friend istream& operator >> (istream& i, Date& date)
       {
            // Separating character can be anything
            // If EOL is reached before reading the full date, or if the date
            // is incompletely specified, date is not changed
            // Caveat: Separator char need not be the same between each element
            // Date format is mm-dd-yy
         int mm,dd,yy;
         bool valid = true;
         removeWhiteSpace(i);
         if ( readTillNumeric(i) )
            {
              i >> mm;
              if ( readTillNumeric(i) )
                 {
                   i >> dd;
                   if ( readTillNumeric(i) ) i >> yy;
                   else valid = false;                
                 }
              else valid = false;
            }
         else valid = false;
         if ( valid ) date.set(mm,dd,yy);
         return i;
       }
};

#endif /* #ifndef _DATE_HH_ */


/*
  $Log: Date.hh,v $
  Revision 1.2  2000/07/20 00:57:27  vinod
  Added year adjustment in set, changed separator, added extraction operator

  Revision 1.1  2000/07/18 05:10:54  vinod
  Initial version

*/
