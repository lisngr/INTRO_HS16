/**
 * \file
 * \brief Main application file
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This provides the main application entry point.
 */

#include "Platform.h"
#include "Application.h"
#include "Event.h"
#include "LED.h"
#include "WAIT1.h"
#include "CS1.h"
#include "Keys.h"
#include "KIN1.h"
#if PL_CONFIG_HAS_SHELL
  #include "CLS1.h"
#endif
#if PL_CONFIG_HAS_BUZZER
  #include "Buzzer.h"
#endif
#if PL_CONFIG_HAS_RTOS
  #include "FRTOS1.h"
  #include "RTOS.h"
#endif
#if PL_CONFIG_HAS_SHELL
  #include "Shell.h"
#endif
#if PL_CONFIG_HAS_QUADRATURE
  #include "Q4CLeft.h"
  #include "Q4CRight.h"
#endif
#if PL_CONFIG_HAS_MOTOR
  #include "Motor.h"
#endif
#if PL_CONFIG_BOARD_IS_ROBO_V2
  #include "PORT_PDD.h"
#endif
#if PL_CONFIG_HAS_LINE_FOLLOW
  #include "LineFollow.h"
#endif

#if PL_CONFIG_HAS_REMOTE
	#include "Remote.h"
#endif
#if PL_CONFIG_HAS_REMOTE_STDIO
	#include "RStdIO.h"
#endif

#if PL_CONFIG_HAS_EVENTS
void APP_EventHandler(EVNT_Handle event) {
  switch(event) {
  case EVNT_STARTUP:
    LED1_On(); /* just do something */
#if PL_CONFIG_HAS_BUZZER
    BUZ_PlayTune(BUZ_TUNE_WELCOME);
#endif
    break;
  case EVNT_LED_HEARTBEAT:
    LED1_Neg();
    break;
#if PL_CONFIG_HAS_KEYS

  case EVNT_SW1_PRESSED:
	  //SHELL_SendString("Switch 1 pressed, sent A\r\n");
#if PL_CONFIG_HAS_LINE_FOLLOW
	  LF_StartStopFollowing();
#endif
#if PL_CONFIG_HAS_BUZZER
    BUZ_PlayTune(BUZ_TUNE_BUTTON);
#endif

#if PL_CONFIG_CONTROL_SENDER
#if PL_CONFIG_HAS_REMOTE_STDIO
    /*Sending Radio Radio Payload over Shell (RStdIo)*/
    RSTDIO_SendToTxStdio(RSTDIO_QUEUE_TX_IN, "buzzer buz 800 400\r\n",sizeof("buzzer buz 800 400\r\n")-1 );
#else
    RemoteSendButtons(1);
#endif
    #endif
    break;
#if PL_CONFIG_NOF_KEYS > 1
  case EVNT_SW2_PRESSED:
	  //SHELL_SendString("Switch 2 pressed, sent B\n\r");
#if PL_CONFIG_CONTROL_SENDER
	  RemoteSendButtons(2);
#endif
	 break;
  case EVNT_SW3_PRESSED:
	  //SHELL_SendString("Switch 3 pressed, sent C\n\r");
#if PL_CONFIG_CONTROL_SENDER
	  RemoteSendButtons(3);
#endif
	  break;
  case EVNT_SW4_PRESSED:
	  //SHELL_SendString("Switch 4 pressed, sent D\n\r");
#if PL_CONFIG_CONTROL_SENDER
	  RemoteSendButtons(4);
#endif
	  break;
  case EVNT_SW5_PRESSED:
	  //SHELL_SendString("Switch 5 pressed, sent E\n\r");
#if PL_CONFIG_CONTROL_SENDER
	  RemoteSendButtons(5);
#endif
	  break;
  case EVNT_SW6_PRESSED:
	  //SHELL_SendString("Switch 6 pressed, sent F\n\r");
#if PL_CONFIG_CONTROL_SENDER
	  RemoteSendButtons(6);
#endif
	  break;
  case EVNT_SW7_PRESSED:
	  //SHELL_SendString("Switch 7 pressed, sent G\n\r");
#if PL_CONFIG_CONTROL_SENDER
	  RemoteSendButtons(7);
#endif
	  break;
#endif






#endif /* PL_CONFIG_HAS_KEYS */

  default:
	  break;
    /* \todo extend handler as needed */
   } /* switch */
}
#endif /* PL_CONFIG_HAS_EVENTS */

static const KIN1_UID RoboIDs[] = {
  /* 0: L20, V2 */ {{0x00,0x03,0x00,0x00,0x4E,0x45,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 1: L21, V2 */ {{0x00,0x05,0x00,0x00,0x4E,0x45,0xB7,0x21,0x4E,0x45,0x32,0x15,0x30,0x02,0x00,0x13}},
  /* 2: L4, V1  */ {{0x00,0x0B,0xFF,0xFF,0x4E,0x45,0xFF,0xFF,0x4E,0x45,0x27,0x99,0x10,0x02,0x00,0x24}}, /* revert right motor */
};

static void APP_AdoptToHardware(void) {
  KIN1_UID id;
  uint8_t res;

  res = KIN1_UIDGet(&id);
  if (res!=ERR_OK) {
    for(;;); /* error */
  }
#if PL_CONFIG_HAS_MOTOR
  if (KIN1_UIDSame(&id, &RoboIDs[2])) { /* L4 */
    MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* revert left motor */
    (void)Q4CLeft_SwapPins(TRUE);
    (void)Q4CRight_SwapPins(TRUE);
  }
#endif
#if PL_CONFIG_HAS_QUADRATURE && PL_CONFIG_BOARD_IS_ROBO_V2
  /* pull-ups for Quadrature Encoder Pins */
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 11, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 11, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 16, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 16, PORT_PDD_PULL_ENABLE);
  PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 17, PORT_PDD_PULL_UP);
  PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 17, PORT_PDD_PULL_ENABLE);
#endif
}

void APP_Start(void) {
#if PL_CONFIG_HAS_RTOS
#if configUSE_TRACE_HOOKS
  PTRC1_uiTraceStart();
#endif
#endif
  PL_Init();
#if PL_CONFIG_HAS_EVENTS
  EVNT_SetEvent(EVNT_STARTUP);
#endif
#if CLS1_DEFAULT_SERIAL
  CLS1_SendStr((uint8_t*)"Hello World!\r\n", CLS1_GetStdio()->stdOut);
#endif
  APP_AdoptToHardware();
#if PL_CONFIG_HAS_RTOS
  vTaskStartScheduler(); /* start the RTOS, create the IDLE task and run my tasks (if any) */
  /* does usually not return! */
#endif
}


