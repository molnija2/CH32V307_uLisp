#include "ch32v30x_rtc.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_bkp.h"
#include "ch32v30x_exti.h"


#include "rtc.h"



const u8 table_week[12] = {0,3,3,6,1,4,6,2,5,0,3,5} ;
const u8 mon_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31 } ;

static volatile u16 iSecondClock=0;
//static u16 iAlarmStatus ;

u8 RTC_Alarm_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min) ;
void RTC_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


u16 GetSecondChangeStatus(void)
{
	if(iSecondClock == 0)
		return 0  ;

	iSecondClock = 0 ;

	return 1 ;
}





//initializes rtc
void RTC_Init()
{
	u8 temp = 0 ;

    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );

	PWR_BackupAccessCmd(ENABLE) ;

	/* Enable LSE oscillator */
    RCC_LSEConfig(RCC_LSE_ON) ;

	Delay_Ms(20);
    /* Select rtc clock source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE) ;
    /* Enable rtc clock */
    RCC_RTCCLKCmd(ENABLE) ;
    RTC_WaitForLastTask() ;
	RTC_WaitForSynchro() ;
	Delay_Ms(10);


	if(BKP_ReadBackupRegister(BKP_DR1)!=0x5a5a)
	{

		BKP_DeInit() ;
		/* Enable LSE oscillator */
	    RCC_LSEConfig(RCC_LSE_ON) ;
	    while((RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET)&&(temp<250))
	    {
	    	temp ++ ;
	    	Delay_Ms(10) ;
	    }

	    if(temp>=250) return ;

	    /* Select rtc clock source */
	    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE) ;
	    /* Enable rtc clock */
	    RCC_RTCCLKCmd(ENABLE) ;
	    RTC_WaitForLastTask() ;
	    RTC_WaitForSynchro() ;

	    /* Enable SECOND rtc interrupt */
	    RTC_ITConfig( RTC_IT_SEC|RTC_IT_ALR, ENABLE );
	    RTC_WaitForLastTask() ;
	    RTC_WaitForSynchro() ;
	    /* Enable Alarm rtc interrupt */

	    BKP_WriteBackupRegister(BKP_DR1, 0x5a5a) ;
 	}
	else
	{
		Delay_Ms(10);
	    /* Select rtc clock source */
	    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE) ;
	    /* Enable rtc clock */
	    RCC_RTCCLKCmd(ENABLE) ;
	    RTC_WaitForLastTask() ;
		RTC_WaitForSynchro() ;
		Delay_Ms(10);
	    // Enable SECOND rtc interrupt
	    RTC_ITConfig( RTC_IT_SEC|RTC_IT_ALR, ENABLE );
	    RTC_WaitForLastTask() ;
	    RTC_WaitForSynchro() ;

		Delay_Ms(10);
	}


	/*RTC_ClearITPendingBit(RTC_FLAG_SEC );
	RTC_ClearITPendingBit(RTC_FLAG_ALR );

    // Configure rtc interrupt
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

	Delay_Ms(200);

    // Enable rtc interrupt
    NVIC_EnableIRQ( RTC_IRQn );
	Delay_Ms(200);*/

}





void RTC_AlarmIrqInit(u8 on)
{
	//  ENABLE or DISABLE

	RTC_ITConfig( RTC_IT_ALR, ENABLE );

    RTC_WaitForLastTask() ;

    EXTI_InitTypeDef EXTI_InitStruct = {0};

    EXTI_InitStruct.EXTI_Line = EXTI_Line17;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;

    EXTI_Init(&EXTI_InitStruct) ;

}


void RTC_Reset(void)
{
	RCC_BackupResetCmd(ENABLE) ;
	RTC_WaitForLastTask() ;

	RCC_BackupResetCmd(DISABLE) ;
	RTC_WaitForLastTask() ;

	RTC_Init() ;

	ResetAlarmStatus() ;
}





void RTC_AlarmsCheck(_calendar_obj *calendar)
{
	u8 i ;
	int i_nearest ;
	u16 a_time, c_time, min_diff = 10000;
	_day_alarm_obj alarm ;

	i_nearest = -1 ;

	for(i=0;i<ALARMS_NUMBER;i++)
	{
		*(u16*)(&alarm) = BKP_ReadBackupRegister(BKP_DR2+0x0004*i) ;

		a_time = (alarm.hour&ALARM_HOUR_MASK)*60+(alarm.min&ALARM_MIN_MASK) ;
		c_time = calendar->hour*60+calendar->min ;
		if((a_time>c_time) && (min_diff>(a_time-c_time)))
		{
			if((alarm.min&ALARM_ON_MASK)!=0)
			{
				i_nearest = i ;
				min_diff = a_time - c_time ;
			}
		}

	}
	if(i_nearest == -1)
	{
		i=0;
		while((i<ALARMS_NUMBER)&&(i_nearest == -1))
		{
			*(u16*)(&alarm) = BKP_ReadBackupRegister(BKP_DR2+0x0004*i) ;

			a_time = (alarm.hour&ALARM_HOUR_MASK)*60+(alarm.min&ALARM_MIN_MASK) ;
			c_time = calendar->hour*60+calendar->min ;
			if(a_time<c_time)
			{
				if((alarm.min&ALARM_ON_MASK)!=0)
				{
					i_nearest = i ;
				}
			}
			i++ ;
		}
	}

	if(i_nearest != -1)
	{
		*(u16*)(&alarm) = BKP_ReadBackupRegister(BKP_DR2+0x0004*i_nearest) ;

		RTC_Alarm_Set(calendar->w_year, calendar->w_month,
				calendar->w_date, alarm.hour&ALARM_HOUR_MASK, alarm.min&ALARM_MIN_MASK) ;


		//Beep_sound(500,100) ;
	}
}




// this function will be executed every 1ms
void RTC_IRQHandler( void )
{
    if( RTC_GetITStatus( RTC_FLAG_SEC ) != RESET )
    {
        /* Clear interrupt flag */
        RTC_ClearITPendingBit(RTC_FLAG_SEC );

        iSecondClock = 1 ;

        u16 iAlarmStatus = GetAlarmStatus() ;
        if(iAlarmStatus>0) SetAlarmStatus(iAlarmStatus-1) ;
    }

    if( RTC_GetITStatus( RTC_FLAG_ALR ) != RESET )
    {
    	RTC_ClearITPendingBit(RTC_FLAG_ALR );
    	SetAlarmStatus( 60 ) ;

        // Clear interrupt flag
        RTC_ClearITPendingBit(RTC_FLAG_ALR );

    }

    RTC_WaitForLastTask() ;
}

u16 GetAlarmStatus()
{
	return BKP_ReadBackupRegister(BKP_DR2+0x0004*ALARMS_NUMBER) ;
}

void ResetAlarmStatus()
{
	BKP_WriteBackupRegister(BKP_DR2+0x0004*ALARMS_NUMBER, 0) ;
}

void SetAlarmStatus(u16 stat)
{
	BKP_WriteBackupRegister(BKP_DR2+0x0004*ALARMS_NUMBER, stat) ;
}


u8 Is_Leap_Year(u16 year)
{
	if(year%4==0)
	{
		if(year%100==0)
		{
			if(year%400==0) return 1 ;
			else return 0 ;
		}
		else return 1 ;
	}
	else return 0 ;
}



u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
	u16 t ;
	u32 seccount = 0 ;
	if((syear<1970)||(syear>2099)) return 1 ;

	for(t=1970;t<syear;t++)
	{
		if(Is_Leap_Year(t)) seccount+=31622400 ;
		else seccount += 31536000 ;
	}

	smon -= 1 ;
	for(t=0;t<smon;t++)
	{
		seccount += (u32)mon_table[t]*86400 ;
		if((Is_Leap_Year(syear))&&(t==1)) seccount += 86400 ;
	}
	seccount += (u32)(sday-1)*86400 ;
	seccount += (u32)hour*3600 ;
	seccount += (u32)min*60 ;
	seccount += sec ;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE) ;
	PWR_BackupAccessCmd(ENABLE) ;

	RTC_SetCounter(seccount) ;

	RTC_WaitForLastTask() ;
	return 0 ;
}




u8 RTC_Alarm_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min)
{
	u16 t ;
	u32 seccount = 0 ;

	if((syear<1970)||(syear>2099)) return 1 ;

	for(t=1970;t<syear;t++)
	{
		if(Is_Leap_Year(t)) seccount+=31622400 ;
		else seccount += 31536000 ;
	}

	smon -= 1 ;
	for(t=0;t<smon;t++)
	{
		seccount += (u32)mon_table[t]*86400 ;
		if((Is_Leap_Year(syear))&&(t==1)) seccount += 86400 ;
	}

	seccount += (u32)(sday-1)*86400 ;
	seccount += (u32)hour*3600 ;
	seccount += (u32)min*60 ;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE) ;
	PWR_BackupAccessCmd(ENABLE) ;

	RTC_SetAlarm(seccount) ;

	RTC_WaitForLastTask() ;

	return 0 ;
}




u8 RTC_Get(_calendar_obj *calendar, u8 start)
{
	static u16 daycnt = 0 ;
	u32 timecount = 0 ;
	u32 temp = 0 ;
	u32 temp1 = 0 ;

	//if(start!=0) daycnt += 1 ;

	timecount = RTC_GetCounter() ;
	temp = timecount/86400 ;

	if((daycnt!=temp)||(start!=0))
	{
		daycnt = temp ;
		temp1 = 1970 ;

		while(temp>=365)
		{
			if(Is_Leap_Year(temp1))
			{
				if(temp>=366) temp -= 366 ;
				else
				{
					temp1++ ;   break ;
				}
			}
			else
				temp -= 365 ;
			temp1 ++ ;
		}
		calendar->w_year = temp1 ;
		//printf("temp1 = %d\n", temp1) ;
		temp1 = 0 ;
		while(temp>=28)
		{
			if((Is_Leap_Year(calendar->w_year))&&(temp1==1))
			{
				if(temp>=29) temp -=29 ;
				else break ;
			}
			else
			{
				if(temp>=mon_table[temp1]) temp -= mon_table[temp1] ;
				else break ;
			}
			temp1 ++ ;
		}

		calendar->w_month = temp1 +1 ;
		calendar->w_date = temp + 1 ;
	}

	temp = timecount%86400 ;
	calendar->hour = temp/3600 ;
	calendar->min = (temp%3600)/60 ;
	calendar->sec = (temp%3600)%60 ;
	calendar->week = RTC_Get_Week(calendar->w_year, calendar->w_month, calendar->w_date) ;

	return 0 ;
}


u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
	u16 temp2 ;
	u8 yearH, yearL ;

	yearH = year/100 ;
	yearL = year%100 ;

	if(yearH>19) yearL += 100 ;

	temp2 = yearL + yearL/4 ;
	temp2 = temp2%7 ;
	temp2 = temp2 + day + table_week[month-1] ;
	if((yearL%4 == 0)&&(month<3))
		temp2 -- ;
	return (temp2%7) ;
}




void SetBkpAlarmTime(u8 hour,u8 min, u8 alarm_index, u8 on)
{
	_day_alarm_obj alarm = {0} ;
	u16 time ;

	alarm.hour = hour ;
	alarm.min = min | (on<<7) ;

	time = *(u16*)(&alarm) ;
	BKP_WriteBackupRegister(BKP_DR2 + 0x0004*alarm_index, time ) ;
}


u16 GetBkpAlarmTime(u8 alarm_index)
{
	return BKP_ReadBackupRegister(BKP_DR2 + 0x0004*alarm_index) ;
}


u16 RTC_IsAlarmsTime(_calendar_obj *calendar)
{
	u8 i ;
	u16 a_time, c_time;
	_day_alarm_obj alarm ;



	for(i=0;i<ALARMS_NUMBER;i++)
	{
		*(u16*)(&alarm) = BKP_ReadBackupRegister(BKP_DR2+0x0004*i) ;

		a_time = (alarm.hour&ALARM_HOUR_MASK)*60+(alarm.min&ALARM_MIN_MASK) ;
		c_time = calendar->hour*60+calendar->min ;
		if((a_time==c_time)&&((alarm.min&ALARM_ON_MASK)!=0))
			return 1 ;
	}

	return 0 ;
}

