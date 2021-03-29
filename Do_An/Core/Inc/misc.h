#ifndef __MISC_H
#define __MISC_H

#include "main.h"

typedef enum reset_cause
{
    eRESET_CAUSE_UNKNOWN = 0,
    eRESET_CAUSE_LOW_POWER_RESET,            /*  */
    eRESET_CAUSE_WINDOW_WATCHDOG_RESET,      /*  */
    eRESET_CAUSE_INDEPENDENT_WATCHDOG_RESET, /* IWDG Timeout */
    eRESET_CAUSE_SOFTWARE_RESET,             /* Reset caused by NVIC_SystemReset() */
    eRESET_CAUSE_POWER_ON_POWER_DOWN_RESET,  /*  */
    eRESET_CAUSE_EXTERNAL_RESET_PIN_RESET,   /* Low signal on NRST pin | Reset pin pushed */
    eRESET_CAUSE_BROWNOUT_RESET,             /*  */
} reset_cause_t;


/* Independent watchdog constant  */
#define PRESCALER_128_UPPER_LIMIT (13107u)
#define PRESCALER_256_UPPER_LIMIT (26214u)
#define IWDG_RESOLUTION (4095u)

reset_cause_t resetCauseGet(void);
const char *resetCauseGetName(reset_cause_t reset_cause);
void iwdgInit(IWDG_HandleTypeDef *hiwdg, uint32_t millis);

#endif // !