
//#define AXIS_C


#define ENDSW_PLUS_MASK         (1<<0)
#define ENDSW_MINUS_MASK        (1<<1)
#define ENDSW_BRAKE_MASK        (1<<2)
#define ENDSW_REPER_MASK        (1<<3)

#define X_plus_switch_on	(iEndSwitchesStatus & ENDSW_PLUS_MASK ) !=0
#define X_plus_switch_off	(iEndSwitchesStatus & ENDSW_PLUS_MASK ) == 0

#define X_minus_switch_on	(iEndSwitchesStatus & ENDSW_MINUS_MASK ) !=0
#define X_minus_switch_off	(iEndSwitchesStatus & ENDSW_MINUS_MASK ) == 0

#define X_reper_switch_on	(iEndSwitchesStatus & ENDSW_REPER_MASK ) != 0
#define X_reper_switch_off	(iEndSwitchesStatus & ENDSW_REPER_MASK ) == 0

#define X_brake_switch_on	(iEndSwitchesStatus & ENDSW_BRAKE_MASK ) != 0
#define X_brake_switch_off	(iEndSwitchesStatus & ENDSW_BRAKE_MASK ) == 0


#define STATUS_BUSY			1
#define STATUS_READY			0
//#define STATUS_HOME			2
//#define STATUS_HOME			3

#define SET_STATUS_READY  		work_status=STATUS_READY ;
#define SET_STATUS_BUSY  		work_status=STATUS_BUSY ;




#define INITIAL_ACURASY		0

#define SOUND_BUF_SIZE	5000  //EXAMPLE



/*#define X_plus_switch_on	1
#define X_plus_switch_0ff	0
#define X_minus_switch_on	1
#define X_minus_switch_off	0
*/

//#define X_minus_half		0//bit_is_clear(PINB,4)
//#define X_plus_half			0//bit_is_set(PINB,4)

#define DIR_MINUS	0x00
#define DIR_PLUS	0x01

#define HOME_MINUS	0x02
#define HOME_PLUS	0x00




#define US0_BUFFER_SIZE		64
#define DBGU_BUFFER_SIZE		64


#define TARGET_SPEED_COEF	1
#define CURRENT_SPEED_COEFF	20000  //0x10000 //0x9C40 //20*PWM_MAXDUTY;

#define PROPORTIONAL_SPEED_COEF	2
#define DIFFERENTIAL_SPEED_COEF	3
#define INTEGRAL_SPEED_COEF	2



#define  PWM_MAXPERIOD		0x0FFF
#define  PWM_MAXDUTY		0x0F80
#define  PWM_MINDUTY		0x0001
#define  PWM_ZERO			0x0
#define  POWER_MAX_AMPLUTUDE	255

#define PWM_DEF_CH0		2
#define PWM_DEF_CH1		3



#define CURRENT_SPEED_COEFF_Y	510*PWM_MAXDUTY;


#define SPEED_MAX_PERIOD	0x3ffff


#define  PWM_CH_VCMD			2
#define  PWM_CH_VCMD_INV		3
#define  PWM_CH_VREAL			1
#define  PWM_CH_VREAL_INV		0



#define DEF_REPEATER_NORMAL	0
#define DEF_REPEATER_SLAVE	1
#define DEF_REPEATER_MASTER	2



#define	DEF_LCD_LINE_LENGHT	16
#define	DEF_LCD_LINE_QUANTITY	2


typedef  signed char int8_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef short int int16_t;
//typedef unsigned int uint32_t;
//typedef int int32_t;








#define MAX_DIGITAL (4095)
#define MAX_DIGITAL_DIV (4096)



#define DEF_USART_RX_SIZE	2048
#define DEF_TIMEOUT		3000






#ifndef EXTERN


char cRxBuffer[DEF_USART_RX_SIZE] ;
char *RxRecvPtr, *CmdBufferPtr, cRecv ;
char *RxRecvReadPtr ;
volatile uint16_t uiRecvIndex, uiRecvReadIndex ;
volatile uint8_t iFlagPackageEnd ;
volatile uint8_t iPackageLenght ;
//volatile uint32_t uiTimeOutCounter ;


#else

extern char cRxBuffer[DEF_USART_RX_SIZE] ;
extern char *RxRecvPtr, *CmdBufferPtr, cRecv ;
extern char *RxRecvReadPtr ;
extern volatile uint16_t uiRecvIndex, uiRecvReadIndex ;
extern uint8_t volatile iFlagPackageEnd ;
extern uint8_t volatile iPackageLenght ;
//extern uint32_t volatile uiTimeOutCounter ;

#endif










/********************************************************

   FROM AVR-robot2-head

********************************************************/



#ifndef EXTERN



/*char cOperand1[20],cOperand2[20], cOperand3[20],cOperand4[20] ;
char  iCmdSymbolCount;
int32_t iOperand1,iOperand2, iOperand13,iOperand4;
uint32_t uOperand1,uOperand2, uOperand13,uOperand4;
*/


volatile uint16_t work_status;

volatile uint32_t KeyPushButton ;

uint32_t  iFlashData_address ;
unsigned short iFlashData_page ;



#else


extern char cOperand1[20],cOperand2[20], cOperand3[20],cOperand4[20] ;
extern char  iCmdSymbolCount;
extern int32_t iOperand1,iOperand2, iOperand13,iOperand4;
extern uint32_t uOperand1,uOperand2, uOperand13,uOperand4;


extern uint16_t iUARTTimeOutCount, iTimeOut ;




extern volatile uint16_t work_status ;


extern volatile uint32_t KeyPushButton ;

extern uint32_t  iFlashData_address ;
extern unsigned short iFlashData_page ;


#endif
