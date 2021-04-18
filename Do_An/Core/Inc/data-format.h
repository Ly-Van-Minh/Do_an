#ifndef __DATA_FORMAT_H
#define __DATA_FORMAT_H

#include "main.h"
#include "lora.h"

#define THIS_NODE_ADDRESS NODE1_ADDRESS
#define NODE1_ADDRESS   0x11u   /* Address node 1 */
#define NODE1_ADDRESS   0x11u   /* Address node 1 */
#define NODE2_ADDRESS   0x22u   /* Address node 2 */
#define NODE3_ADDRESS   0x33u   /* Address node 3 */
#define GATEWAY_ADDRESS 0xFFu   /* Adress Gateway */

enum MSG_INDEX
{
    INDEX_SOURCE_ID = 0,     /* Header field, Tranceiver ID */
    INDEX_DEST_ID,           /* Header field, Receiver ID */
    INDEX_MSG_TYPE,          /* Header field, Type of msg, refer @MSG_TYPE enumeration */
    INDEX_MSG_STATUS,        /* Header field, Status of msg, refer @MSG_STATUS enumeration */
    INDEX_SEQUENCE_ID,       /* Header field, Message's sequence ID */
    INDEX_DATA_LOCATION,     /* Data field, refer to @DATA_LOCATION enumeration */
    INDEX_DATA_LIGHT_STATUS, /* Data field, either On or Off */
    INDEX_DATA_ERR_CODE,     /* Data field, refer to @DATA_ERR_CODE */
    INDEX_DATA_TIME_ALIVE,   /* Data field, time elapsed since light is controlled to On state */
    UNDEFINED_2,             /*  */
    INDEX_MAX = 10,
};

typedef enum MSG_TYPE
{
    MSG_TYPE_UNKNOWN = 0,
    MSG_TYPE_REQUEST,
    MSG_TYPE_RESPONSE,
    MSG_TYPE_NOTIF,
    MSG_TYPE_MAX,
} MsgTypeDef_t;

typedef enum MSG_STATUS
{
    MSG_STS_UNKNOWN = 0,
    MSG_STS_OK,
    MSG_STS_FAILED,
    MSG_STS_MAX,
} MsgStsTypeDef_t;

typedef enum DATA_LIGHT_STATUS
{
    LIGHT_STATUS_OFF = 0,
    LIGHT_STATUS_ON = 1,
    LIGHT_STATUS_MAX,
} NodeStsTypedef_t;

typedef enum DATA_ERR_CODE
{
    ERR_CODE_NONE = 0,
    ERR_CODE_LIGHT_ON_FAILED,
    ERR_CODE_LIGHT_OFF_FAILED,
    ERR_CODE_MAX,
} NodeErrCodeTypeDef_t;

typedef enum DATA_LOCATION
{
    LOCATION_YEN_LANG = 0,
    LOCATION_DAI_CO_VIET,
    LOCATION_GIAI_PHONG,
    LOCATION_MAX,
} NodeLocationTypeDef_t;

u8 ucSendData[PAYLOAD_LENGTH] =
    {
        [INDEX_SOURCE_ID]         = THIS_NODE_ADDRESS,
        [INDEX_DEST_ID]           = GATEWAY_ADDRESS,
        [INDEX_MSG_TYPE]          = MSG_TYPE_UNKNOWN,
        [INDEX_MSG_STATUS]        = MSG_STS_UNKNOWN,
        [INDEX_SEQUENCE_ID]       = 0,
        [INDEX_DATA_LOCATION]     = LOCATION_YEN_LANG,
        [INDEX_DATA_LIGHT_STATUS] = LIGHT_STATUS_OFF,
        [INDEX_DATA_ERR_CODE]     = ERR_CODE_NONE,
        [INDEX_DATA_TIME_ALIVE]   = 0,
        [UNDEFINED_2]             = 0,
};
u8 ucReceivedData[PAYLOAD_LENGTH] = {0};

#endif /* !__DATA_FORMAT_H */