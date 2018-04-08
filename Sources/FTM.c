/*
 * FTM.c
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#include "FTM.h"
#define PWM_PERIOD 10000u  /*设置周期  4khz = 125us ; MOD =(125us /0.2us)-1 = 624
			                  * 2khz = 250us ; MOD = (250us/0.2us)-1 = 1249
	                      * 1khz = 500us ; MOD = (500us/0.2us)-1 =  2400
	                      * 500hz = 1000us; MOD = (1000us/0.2us)-1 = 4999
                 	      * 8khz = 62.5us; MOD = (62.5us/0.2us)-1 = 311
	                                                                     */
/**********************************************************************************************
* External objects
**********************************************************************************************/


/**********************************************************************************************
* Global variables
**********************************************************************************************/


/**********************************************************************************************
* Constants and macros
**********************************************************************************************/


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/


/**********************************************************************************************
* Local variables
**********************************************************************************************/
UINT16 period[4] = {0, 0, 0, 0};
pt2FuncU8 FTM_Callback;


/**********************************************************************************************
* Local functions
**********************************************************************************************/
void FTM2_IRQHandler(void);

/**********************************************************************************************
* Global functions
**********************************************************************************************/
/***********************************************************************************************
*
* @brief    FTM_Init - Initialize the FTM counter and enable 4 interrupts (CH0 to CH3)
* @param    none
* @return   none
*
************************************************************************************************/
void FTM_Init()
{

	
	SIM_SCGC |= SIM_SCGC_FTM2_MASK; /* Enable Clock for FTM2 为FTM2启用时钟 */
	
	
	FTM2_SC |= FTM_SC_PS(7);	/* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */ 
														/* Counter increase by one every 6.4 us 计数器每6.4微秒个增加一个。*/
	
	/* Enable Channle 0*/
	FTM2_C0SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 0 interrupt */
	FTM2_C0SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */
	
	/*Enable Channel 1*/
	FTM2_C1SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 1 interrupt */
	FTM2_C1SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */
	

	/*Enable Channel 2*/
	FTM2_C2SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 2 interrupt */
	FTM2_C2SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */

	/*Enable Channel 3*/
	FTM2_C3SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 3 interrupt */
	FTM2_C3SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */	
	
	/*Select interrupt frequency*/
	FTM2_C0V = FTM_CnV_VAL(period[0]) ;	 
	FTM2_C1V = FTM_CnV_VAL(period[1]) ;	 
	FTM2_C2V = FTM_CnV_VAL(period[2]) ;	 
	FTM2_C3V = FTM_CnV_VAL(period[3]);  	

	FTM2_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/		
	 
	 
	

	 
}


/*电机右*/
void PWM8khz_Init_2(uint8_t dutycycle)
{
	uint32_t period;
	if(dutycycle>=10)dutycycle=10;
	if(dutycycle<=0)dutycycle = 0;
	SIM_PINSEL0 |= SIM_PINSEL_FTM1PS0_MASK | SIM_PINSEL_FTM1PS1_MASK;
  	
	
	SIM_SCGC |= SIM_SCGC_FTM1_MASK; /* Enable Clock for FTM1  （MASK：掩码，就是置一）*/

	FTM1_SC  |= FTM_SC_PS(2); /* Select Preescaler in this case 4. 20 Mhz /4 =5000 Khz. */ 
														/* Counter increase by one every 0.2 us */
  period  = PWM_PERIOD; /*设置周期  8khz = 125us ; MOD =(125us /0.2us)-1 = 624
			            * 4khz = 250us ; MOD = (250us/0.2us)-1 = 1249
	                * 2khz = 500us ; MOD = (500us/0.2us)-1 =  2400
	                * 1khz = 1000us; MOD = (1000us/0.2us)-1 = 4999
                 	* 16khz = 62.5us; MOD = (62.5us/0.2us)-1 = 311
	                                                                     */
	
		/*
	 *
	 * 左电机 PWM0
	 *
	*/
  //FTM0_C0SC   |= FTM_CnSC_CHIE_MASK; /*通道中断使能*/
  FTM1_C0SC  |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; /* 边沿对齐模式 高电平有效*/
	/*Select PWM frequency dutycycle*/
	FTM1_MOD  |=	FTM_MOD_MOD(period);							
  FTM1_C0V 	|=  FTM_CnV_VAL(dutycycle * (period/100));                                
	/*
	 *
	 * 左电机 PWM1
	 *
	*/
 //FTM0_C1SC  |= 	FTM_CnSC_CHIE_MASK;
	FTM1_C1SC  |=  FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; /* 边沿对齐模式 高电平有效*/
		/*Select PWM frequency dutycycle*/
	FTM1_MOD  |=	FTM_MOD_MOD(period);							
  FTM1_C1V 	|=  FTM_CnV_VAL(dutycycle * (period/100)); 
	
	FTM1_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/
}


/*电机左*/
void PWM8khz_Init(uint8_t dutycycle)
{
	uint32_t period;
	if(dutycycle>=10)dutycycle=10;
	if(dutycycle<=0)dutycycle = 0;
	
	SIM_SCGC |= SIM_SCGC_FTM0_MASK; /* Enable Clock for FTM0 */
//	SIM_SCGC |= SIM_SCGC_FTM1_MASK; /* Enable Clock for FTM1 */
  FTM0_SC  |= FTM_SC_PS(2); /* Select Preescaler in this case 4. 20 Mhz /4 =5000 Khz. */ 
//	FTM1_SC  |= FTM_SC_PS(4); /* Select Preescaler in this case 4. 20 Mhz /4 =5000 Khz. */ 
														/* Counter increase by one every 0.2 us */
  period  = PWM_PERIOD; /*设置周期  8khz = 125us ; MOD =(125us /0.2us)-1 = 624*/
	
	
	
	
	/*
	 *
	 * 左电机 PWM0
	 *
	*/
  //FTM0_C0SC   |= FTM_CnSC_CHIE_MASK; /*通道中断使能*/
  FTM0_C0SC  |= FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; /* 边沿对齐模式 高电平有效*/
	/*Select PWM frequency dutycycle*/
	FTM0_MOD  |=	FTM_MOD_MOD(period);							
  FTM0_C0V 	|=  FTM_CnV_VAL(dutycycle * (period/100));                                
	/*
	 *
	 * 左电机 PWM1
	 *
	*/
 //FTM0_C1SC  |= 	FTM_CnSC_CHIE_MASK;
	FTM0_C1SC  |=  FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK; /* 边沿对齐模式 高电平有效*/
		/*Select PWM frequency dutycycle*/
	FTM0_MOD  |=	FTM_MOD_MOD(period);							
  FTM0_C1V 	|=  FTM_CnV_VAL(dutycycle * (period/100)); 





	FTM0_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/
	
	

}
/***********************************************************************************************
*
* @brief    FTM_SetPeriod - Set the period of the periodic interrupt
* @param    ch - channel to configure, per - period to set
* @return   none
*
************************************************************************************************/
void FTM_SetPeriod(UINT8 ch, UINT16 per)
{
	period[ch] = per;
	
	FTM2_C0V = FTM_CnV_VAL(period[0]) ;
	FTM2_C1V = FTM_CnV_VAL(period[1]) ;	
	FTM2_C2V = FTM_CnV_VAL(period[2]) ;
	FTM2_C3V = FTM_CnV_VAL(period[3]) ;
}

void Motor_setspeed(int16_t speed_left,int16_t speed_right)
{
	 uint32_t period;
	 period  = PWM_PERIOD; /*设置周期  8khz = 125us ; MOD =(125us /0.2us)-1 = 624*/
		if(speed_left>100)speed_left=100;
		if(speed_left<-100)speed_left=-100;
		if(speed_right>100)speed_left=100;
		if(speed_right<-100)speed_left=-100;

		/*
	 *
	 * 左电机 PWM1
	 *
	*/
		if(speed_left<0)  //倒转
		{
			FTM0_C0V 	=  FTM_CnV_VAL(0 * (period/100));
			FTM0_C1V 	=  FTM_CnV_VAL((uint16_t)(-speed_left) * (period/100)); 
		}
		else							//正转
		{
				FTM0_C0V 	=  FTM_CnV_VAL((uint16_t)speed_left * (period/100));
				FTM0_C1V 	=  FTM_CnV_VAL(0 * (period/100)); 
		}
		/*
	 *
   * 右电机
	 *
	*/
		if(speed_right<0)  //倒转
		{
			FTM1_C0V 	=  FTM_CnV_VAL(0 * (period/100));
			FTM1_C1V 	=  FTM_CnV_VAL((uint16_t)-speed_right * (period/100)); 
		}
		else							//正转
		{
				FTM1_C0V 	=  FTM_CnV_VAL((uint16_t)speed_right * (period/100));
				FTM1_C1V 	=  FTM_CnV_VAL(0 * (period/100)); 
		}
}

/***********************************************************************************************
*
* @brief    FTM_SetCallback - Set the callback function to call on successful matches from any channel 
* @param    pointer to function with an UINT8 argument
* @return   none
*
************************************************************************************************/  

void FTM_SetCallback(pt2FuncU8 ptr)
{
	FTM_Callback = ptr;	
}



/***********************************************************************************************
*
* @brief    FTM2_IRQHandler - Interrupt for FTM channels, clear flag, set new tiemout and execute callback 
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_IRQHandler()
{
	
	
	if (1==((FTM2_C0SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT) )  /* If the CHF of the channel is equal to 0  如果信道的CHF等于0*/
	{
		(void)FTM2_C0SC;  							/* Read to clear flag */
		FTM2_C0SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */
		FTM2_C0V = FTM2_C0V + FTM_CnV_VAL(period[0]) ; /* Refresh interrupt period 刷新中断时间 */
		FTM_Callback(0);
	}
	
	
	if (1==((FTM2_C1SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT))  /* If the CHF of the channel is equal to 1 */
	{
		(void)FTM2_C1SC;  				 			/* Read to clear flag */
		FTM2_C1SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */ 
		FTM2_C1V = FTM2_C1V + FTM_CnV_VAL(period[1]) ;	/* Refresh interrupt period */
		FTM_Callback(1);
	}


	if (1==((FTM2_C2SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT))  /* If the CHF of the channel is equal to 2 */
	{
		(void)FTM2_C2SC;  							/* Read to clear flag */
		FTM2_C2SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */
		FTM2_C2V = FTM2_C2V + FTM_CnV_VAL(period[2]) ; /* Refresh interrupt period */
		FTM_Callback(2);
	}
	

	if (1==((FTM2_C3SC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT))  /* If the CHF of the channel is equal to 1 */
	{
		(void)FTM2_C3SC; 				 			/* Read to clear flag */
		FTM2_C3SC ^= FTM_CnSC_CHF_MASK;  			/*Clear flag */
		FTM2_C3V = FTM2_C3V + FTM_CnV_VAL(period[3]) ; /* Refresh interrupt period */
		FTM_Callback(3);
	}
	
}
void dianji(int16_t speed_left1,int16_t speed_right1)
{
	uint32_t period;
	period  = PWM_PERIOD;
	FTM0_C0V 	=  FTM_CnV_VAL((uint16_t)speed_left1 * (period/100));
	FTM0_C1V 	=  FTM_CnV_VAL(0 * (period/100)); 
	FTM1_C0V 	=  FTM_CnV_VAL((uint16_t)speed_right1 * (period/100));
	FTM1_C1V 	=  FTM_CnV_VAL(0 * (period/100)); 
}

