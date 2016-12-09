/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

#include "Platform.h"
#if PL_CONFIG_HAS_RTOS
#include "RTOS.h"
#include "FRTOS1.h"
#include "LED.h"
#include "Event.h"
#include "Keys.h"
#include "Application.h"

static void AppTask(void* param) {
  const int *whichLED = (int*)param;

  (void)param; /* avoid compiler warning */
  for(;;) {
    if (*whichLED==1) {
      LED1_Neg();
    } else if (*whichLED==2) {
      //LED2_Neg();
    }
    /* \todo handle your application code here */
    FRTOS1_vTaskDelay(pdMS_TO_TICKS(500));
  }
}

static void MainTask(void* param) {

  (void)param; /* avoid compiler warning */
  for(;;) {
	#if PL_CONFIG_HAS_KEYS
		KEY_Scan();
		EVNT_HandleEvent(APP_EventHandler, TRUE);
	#endif
	  /*Task Delay*/
	  FRTOS1_vTaskDelay(pdMS_TO_TICKS(50));
  }
}
/*
static void vSlaveTask(void *pvParameters) {
  xSemaphoreHandle sem = (xSemaphoreHandle)pvParameters;

  if (sem==NULL) {
    for(;;) {
      // should not be NULL?
    	vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  }
  for(;;) {
    //! \todo Implement functionality
	  if(xSemaphoreTake(sem, 100)){
		  LED2_Neg();
	  }
	  vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

static void vMasterTask(void *pvParameters) {
  //! \todo Understand functionality
  xSemaphoreHandle sem = NULL;

  (void)pvParameters; // parameter not used
  sem = xSemaphoreCreateBinary();
  if (sem==NULL) { // semaphore creation failed
    for(;;){} // error
  }
  vQueueAddToRegistry(sem, "IPC_Sem");

  //Main Task
  if (FRTOS1_xTaskCreate(vSlaveTask, (signed portCHAR *)"SemSlave", configMINIMAL_STACK_SIZE, sem, tskIDLE_PRIORITY+2, NULL) != pdPASS) {
	  for(;;){} // error case only, stay here!
  }

  for(;;) {
    (void)xSemaphoreGive(sem); // give control to other task
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
*/

void RTOS_Init(void) {
  static const int led1 = 1;
 // static const int led2 = 2;

  EVNT_SetEvent(EVNT_STARTUP); /* set startup event */
  /*! \todo Create tasks here */

  	  /*LED toggle Task*/
  if (FRTOS1_xTaskCreate(AppTask, (signed portCHAR *)"App1", configMINIMAL_STACK_SIZE, (void*)&led1, tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }
  	  /*Main Task*/
  if (FRTOS1_xTaskCreate(MainTask, (signed portCHAR *)"Main", configMINIMAL_STACK_SIZE, NULL , tskIDLE_PRIORITY, NULL) != pdPASS) {
    for(;;){} /* error case only, stay here! */
  }

  /*LED toggle Task*/
  /*
  if (FRTOS1_xTaskCreate(vMasterTask, (signed portCHAR *)"SemMaster", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL) != pdPASS) {
	  for(;;){} // error case only, stay here!
  }*/



}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}

#endif /* PL_CONFIG_HAS_RTOS */
