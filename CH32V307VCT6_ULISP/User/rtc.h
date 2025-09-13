#ifndef __RCT_H
#define __RTC_H

#include <ch32v30x_conf.h>




typedef struct
{
	vu8 hour ;
	vu8 min ;
	vu8 sec ;

	vu16 w_year ;
	vu8 w_month ;
	vu8 w_date ;
	vu8 week ;
} _calendar_obj ;



typedef struct
{
	vu8 hour ;
	vu8 min ;
} _day_alarm_obj ;


typedef struct
{
	u8 volume_snd ;
	u8 volume_led ;
} _volume_obj ;


#define ALARM_HOUR_MASK	0x1F
#define ALARM_MIN_MASK	0X3F

#define ALARM_ON_MASK	0x80
#define ALARM_SIG_MASK	0XC0

#define ALARMS_NUMBER	5
#define BKP_VOLUME_POS	(ALARMS_NUMBER+1)

u8 RTC_Get_Week(u16 year, u8 month, u8 day) ;
u8 Is_Leap_Year(u16 year) ;
void RTC_Init() ;
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec) ;
u8 RTC_Alarm_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min) ;
u8 RTC_Get(_calendar_obj *calendar, u8 start) ;
u16 GetSecondChangeStatus(void) ;

void RTC_Reset(void) ;


void SetBkpAlarmTime(u8 hour,u8 min, u8 nom, u8 on) ;
u16 GetBkpAlarmTime(u8 nom) ;
void RTC_AlarmIrqInit(u8 on) ;

void RTC_AlarmsCheck(_calendar_obj *calendar) ;
u16 RTC_IsAlarmsTime(_calendar_obj *calendar) ;

u16 GetAlarmStatus() ;
void ResetAlarmStatus() ;
void SetAlarmStatus(u16 stat) ;

#endif
