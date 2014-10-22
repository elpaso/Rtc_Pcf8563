/*****
 *  NAME
 *    Pcf8563 Real Time Clock support routines
 *  AUTHOR
 *    Joe Robertson, jmr
 *    orbitalair@bellsouth.net
 *    http://orbitalair.wikispaces.com/Arduino
 *  CREATION DATE
 *    9/24/06,  init - built off of usart demo.  using mikroC
 *  NOTES
 *  HISTORY
 *    10/14/06 ported to CCS compiler, jmr
 *    2/21/09  changed all return values to hex val and not bcd, jmr
 *    1/10/10  ported to arduino, jmr
 *    2/14/10  added 3 world date formats, jmr
 *    28/02/2012 A. Pasotti
 *             fixed a bug in RTCC_ALARM_AF,
 *             added a few (not really useful) methods
 *    22/10/2014 Fix whitespace, tabs, and newlines, cevich
 *    22/10/2014 add voltLow get/set, cevich
 *    22/10/2014 add century get, cevich
 *    22/10/2014 Fix get/set date/time race condition, cevich
 *
 *  TODO
 *    x Add Euro date format
 *    Add short time (hh:mm) format
 *    Add 24h/12h format
 *    Add timer support
 ******
 *  Robodoc embedded documentation.
 *  http://www.xs4all.nl/~rfsber/Robo/robodoc.html
 */

#ifndef Rtc_Pcf8563_H
#define Rtc_Pcf8563_H

#include "Arduino.h"
#include "Wire.h"

/* the read and write values for pcf8563 rtcc */
/* these are adjusted for arduino */
#define RTCC_R      0xa3
#define RTCC_W      0xa2

#define RTCC_SEC        1
#define RTCC_MIN        2
#define RTCC_HR         3
#define RTCC_DAY        4
#define RTCC_WEEKDAY    5
#define RTCC_MONTH      6
#define RTCC_YEAR       7
#define RTCC_CENTURY    8

/* register addresses in the rtc */
#define RTCC_STAT1_ADDR     0x0
#define RTCC_STAT2_ADDR     0x01
#define RTCC_SEC_ADDR       0x02
#define RTCC_MIN_ADDR       0x03
#define RTCC_HR_ADDR        0x04
#define RTCC_DAY_ADDR       0x05
#define RTCC_WEEKDAY_ADDR   0x06
#define RTCC_MONTH_ADDR     0x07
#define RTCC_YEAR_ADDR      0x08
#define RTCC_ALRM_MIN_ADDR  0x09
#define RTCC_SQW_ADDR       0x0D
#define RTCC_TIMER1_ADDR    0x0E
#define RTCC_TIMER2_ADDR    0x0F

/* setting the alarm flag to 0 enables the alarm.
 * set it to 1 to disable the alarm for that value.
 */
#define RTCC_ALARM      0x80
#define RTCC_ALARM_AIE  0x02
#define RTCC_ALARM_AF   0x08 // 0x08 : not 0x04!!!!
/* optional val for no alarm setting */
#define RTCC_NO_ALARM   99

#define RTCC_CENTURY_MASK   0x80
#define RTCC_VLSEC_MASK     0x80

/* date format flags */
#define RTCC_DATE_WORLD     0x01
#define RTCC_DATE_ASIA      0x02
#define RTCC_DATE_US        0x04
/* time format flags */
#define RTCC_TIME_HMS       0x01
#define RTCC_TIME_HM        0x02

/* square wave contants */
#define SQW_DISABLE     B00000000
#define SQW_32KHZ       B10000000
#define SQW_1024HZ      B10000001
#define SQW_32HZ        B10000010
#define SQW_1HZ         B10000011


/* arduino class */
class Rtc_Pcf8563 {
    public:
    Rtc_Pcf8563();

    void initClock();  /* zero out all values, disable all alarms */
    void clearStatus(); /* set both status bytes to zero */
    void clearVoltLow(void); /* Only clearing is possible */

    void getDateTime();     /* get date and time vals to local vars */
    void setDateTime(byte day, byte weekday, byte month, bool century, byte year,
                     byte hour, byte minute, byte sec);
    void getDate();   /* get date vals to local vars */
    void setDate(byte day, byte weekday, byte month, bool century, byte year);
    void getTime();    /* get time vars + 2 status bytes to local vars */
    void setTime(byte hour, byte minute, byte sec);  /* Also clear Volt-Low */
    void getAlarm();
    byte readStatus2();
    boolean alarmEnabled();
    boolean alarmActive();

    void enableAlarm(); /* activate alarm flag and interrupt */
    /* set alarm vals, 99=ignore */
    void setAlarm(byte min, byte hour, byte day, byte weekday);
    void clearAlarm(); /* clear alarm flag and interrupt */
    void resetAlarm();  /* clear alarm flag but leave interrupt unchanged */
    void setSquareWave(byte frequency);
    void clearSquareWave();

    bool getVoltLow();
    byte getSecond();
    byte getMinute();
    byte getHour();
    byte getDay();
    byte getMonth();
    byte getYear();
    bool getCentury();
    byte getWeekday();
    byte getStatus1();
    byte getStatus2();

    byte getAlarmMinute();
    byte getAlarmHour();
    byte getAlarmDay();
    byte getAlarmWeekday();

    /*get a output string, these call getTime/getDate for latest vals */
    char *formatTime(byte style=RTCC_TIME_HMS);
    /* date supports 3 styles as listed in the wikipedia page about world date/time. */
    char *formatDate(byte style=RTCC_DATE_US);

    private:
    /* methods */
    byte decToBcd(byte value);
    byte bcdToDec(byte value);
    /* time variables */
    byte hour;
    byte minute;
    bool volt_low;
    byte sec;
    byte day;
    byte weekday;
    byte month;
    byte year;
    /* alarm */
    byte alarm_hour;
    byte alarm_minute;
    byte alarm_weekday;
    byte alarm_day;
    /* CLKOUT */
    byte squareWave;
    /* timer */
    byte timer_control;
    byte timer_current;
    /* support */
    byte status1;
    byte status2;
    bool century;

    char strOut[9];
    char strDate[11];

    int Rtcc_Addr;
};

#endif
