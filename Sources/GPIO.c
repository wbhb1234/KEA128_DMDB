/*
 * GPIO.c
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#include "GPIO.h"

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
#define LED0_TOGGLE		OUTPUT_TOGGLE(PTC,PTC0)
#define LED1_TOGGLE		OUTPUT_TOGGLE(PTC,PTC1)
#define LED2_TOGGLE		OUTPUT_TOGGLE(PTC,PTC2)
#define LED3_TOGGLE		OUTPUT_TOGGLE(PTC,PTC3)

#define LED0_OFF		OUTPUT_CLEAR(PTC,PTC0);
#define LED1_OFF		OUTPUT_CLEAR(PTC,PTC1);
#define LED2_OFF		OUTPUT_CLEAR(PTC,PTC2);
#define LED3_OFF		OUTPUT_CLEAR(PTC,PTC3);

#define LED0_ON			OUTPUT_SET(PTC,PTC0);
#define LED1_ON			OUTPUT_SET(PTC,PTC1);
#define LED2_ON			OUTPUT_SET(PTC,PTC2);
#define LED3_ON			OUTPUT_SET(PTC,PTC3);

/**********************************************************************************************
* Local functions
**********************************************************************************************/

/**********************************************************************************************
* Global functions
**********************************************************************************************/
/***********************************************************************************************
*
* @brief    GPIO_Init - Initialize the LEDs as outputs
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
	CONFIG_PIN_AS_GPIO(PTC,PTC0,OUTPUT); /* Configure LED 0 (PTC0) as an output */
	CONFIG_PIN_AS_GPIO(PTC,PTC1,OUTPUT); /* Configure LED 1 (PTC1) as an output */
	CONFIG_PIN_AS_GPIO(PTC,PTC2,OUTPUT); /* Configure LED 2 (PTC2) as an output */
	CONFIG_PIN_AS_GPIO(PTC,PTC3,OUTPUT); /* Configure LED 3 (PTC3) as an output */
	
	CONFIG_PIN_AS_GPIO(PTD,PTD0,INPUT); /* Configure SW1 (PTD0) as an input */	
	CONFIG_PIN_AS_GPIO(PTD,PTD1,INPUT); /* Configure SW2 (PTD1) as an input */
	ENABLE_INPUT(PTD,PTD0);			 /* Enable input SW1*/	
	ENABLE_INPUT(PTD,PTD1);			/*  Enable input SW2*/
	
	LED0_OFF;							/* Turn off LED0 */
	LED1_OFF;							/* Turn off LED1 */
	LED2_OFF;							/* Turn off LED2 */
	LED3_OFF;							/* Turn off LED3 */

}
