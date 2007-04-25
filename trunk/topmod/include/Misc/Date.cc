/* $Id: Date.cc,v 1.1 2000/07/18 05:10:53 vinod Exp $ */

#include "Date.hh"
 
const int Date :: DaysIn[13] = {
  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

const int Date :: TotalDays[2][13] = {
  {
    0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
  },
  {
    0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335
  }
};
  
const char * Date :: DayName[8] = {
  "Sunday", "Monday", "Tuesday", "Wednesday",
  "Thursday", "Friday", "Saturday"
};

const char * Date :: MonthName[13] = {
  "January", "February", "March", "April", "May", "June", "July",
  "August", "September", "October", "November", "December"
};


/*
  $Log: Date.cc,v $
  Revision 1.1  2000/07/18 05:10:53  vinod
  Initial version

*/
