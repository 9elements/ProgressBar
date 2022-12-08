#pragma once

#include <stdbool.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_log.h>

#include "pn7160.h"

#define MAX_PAYLOAD_SIZE 255
#define MSG_HEADER_SIZE  3

#define MSG_TYPE_DATA         0x00
#define MSG_TYPE_COMMAND      0x20
#define MSG_TYPE_RESPONSE     0x40
#define MSG_TYPE_NOTIFICATION 0x60

/* Packet Boundary Flag */
#define PACKET_BOUNDARY_FLAG_LAST_SEGMENT     0x00
#define PACKET_BOUNDARY_FLAG_NOT_LAST_SEGMENT 0x10

/* Group Identifier (GID) */
#define GROUP_ID_CORE             0x00
#define GROUP_ID_RF_MANAGEMENT    0x01
#define GROUP_ID_NFCEE_MANAGEMENT 0x02
#define GROUP_ID_PROPRIETARY      0x0F

/* OpCode Identifier (OID) */
#define CORE_RESET_CMD           0x00
#define CORE_RESET_RSP           0x00
#define CORE_RESET_NTF           0x00
#define CORE_INIT_CMD            0x01
#define CORE_INIT_RSP            0x01
#define CORE_SET_CONFIG_CMD      0x02
#define CORE_SET_CONFIG_RSP      0x02
#define CORE_GET_CONFIG_CMD      0x03
#define CORE_GET_CONFIG_RSP      0x03
#define CORE_CONN_CREATE_CMD     0x04
#define CORE_CONN_CREATE_RSP     0x04
#define CORE_CONN_CLOSE_CMD      0x05
#define CORE_CONN_CLOSE_RSP      0x05
#define CORE_CONN_CREDITS_NTF    0x06
#define CORE_GENERIC_ERROR_NTF   0x07
#define CORE_INTERFACE_ERROR_NTF 0x08

/* 1001b - 1111b RFU */

#define RF_DISCOVER_MAP_CMD            0x00
#define RF_DISCOVER_MAP_RSP            0x00
#define RF_SET_LISTEN_MODE_ROUTING_CMD 0x01
#define RF_SET_LISTEN_MODE_ROUTING_RSP 0x01
#define RF_GET_LISTEN_MODE_ROUTING_CMD 0x02
#define RF_GET_LISTEN_MODE_ROUTING_RSP 0x02
#define RF_GET_LISTEN_MODE_ROUTING_NTF 0x02
#define RF_DISCOVER_CMD                0x03
#define RF_DISCOVER_RSP                0x03
#define RF_DISCOVER_NTF                0x03
#define RF_DISCOVER_SELECT_CMD         0x04
#define RF_DISCOVER_SELECT_RSP         0x04
#define RF_INTF_ACTIVATED_NTF          0x05
#define RF_DEACTIVATE_CMD              0x06
#define RF_DEACTIVATE_RSP              0x06
#define RF_DEACTIVATE_NTF              0x06
#define RF_FIELD_INFO_NTF              0x07
#define RF_T3T_POLLING_CMD             0x08
#define RF_T3T_POLLING_RSP             0x08
#define RF_T3T_POLLING_NTF             0x08
#define RF_NFCEE_ACTION_NTF            0x09
#define RF_NFCEE_DISCOVERY_REQ_NTF     0x0A
#define RF_PARAMETER_UPDATE_CMD        0x0B
#define RF_PARAMETER_UPDATE_RSP        0x0B

/* 1100b - 1111b RFU */

#define NFCEE_DISCOVER_CMD 0x00
#define NFCEE_DISCOVER_RSP 0x00
#define NFCEE_DISCOVER_NTF 0x00
#define NFCEE_MODE_SET_CMD 0x01
#define NFCEE_MODE_SET_RSP 0x01

/* 0010b - 1111b RFU */

#define NCI_PROPRIETARY_ACT_CMD 0x02
#define NCI_PROPRIETARY_ACT_RSP 0x02

#define RESET_KEEP_CONFIG  0x00
#define RESET_CLEAR_CONFIG 0x01

/* Generic Status Codes */
#define STATUS_OK                 0x00
#define STATUS_REJECTED           0x01
#define STATUS_RF_FRAME_CORRUPTED 0x02
#define STATUS_FAILED             0x03
#define STATUS_NOT_INITIALIZED    0x04
#define STATUS_SYNTAX_ERROR       0x05
#define STATUS_SEMANTIC_ERROR     0x06
/* 0x07 - 0x08 RFU */
#define STATUS_INVALID_PARAM         0x09
#define STATUS_MESSAGE_SIZE_EXCEEDED 0x0A
/* 0x0B - 0x9F RFU */

/* RF Discovery Specific Status Codes */
#define DISCOVERY_ALREADY_STARTED          0xA0
#define DISCOVERY_TARGET_ACTIVATION_FAILED 0xA1
#define DISCOVERY_TEAR_DOWN                0xA2
/* 0xA3 - 0xAF RFU */

/* RF Interface Specific Status Codes */
#define RF_TRANSMISSION_ERROR 0xB0
#define RF_PROTOCOL_ERROR     0xB1
#define RF_TIMEOUT_ERROR      0xB2
/* 0xB3 - 0xBF RFU */

/* NFCEE Interface Specific Status Codes */
#define NFCEE_INTERFACE_ACTIVATION_FAILED 0xC0
#define NFCEE_TRANSMISSION_ERROR          0xC1
#define NFCEE_PROTOCOL_ERROR              0xC2
#define NFCEE_TIMEOUT_ERROR               0xC3
/* 0xC4 - 0xDF RFU */
/* 0xE0 - 0xFF For proprietary use */

#define NFC_RF_TECHNOLOGY_A     0x00
#define NFC_RF_TECHNOLOGY_B     0x01
#define NFC_RF_TECHNOLOGY_F     0x02
#define NFC_RF_TECHNOLOGY_15693 0x03
/* 0x04 - 0x7F RFU */
/* 0x80 - 0xFE For proprietary use */
/* 0xFF RFU */

#define NFC_A_PASSIVE_POLL_MODE 0x00
#define NFC_B_PASSIVE_POLL_MODE 0x01
#define NFC_F_PASSIVE_POLL_MODE 0x02
#define NFC_A_ACTIVE_POLL_MODE  0x03
/* RFU 0x04 */
#define NFC_F_ACTIVE_POLL_MODE      0x05
#define NFC_15693_PASSIVE_POLL_MODE 0x06
/* 0x07 - 0x6F RFU */
/* 0x70 - 0x7F Reserved for Proprietary Technologies in Poll Mode */
#define NFC_A_PASSIVE_LISTEN_MODE 0x80
#define NFC_B_PASSIVE_LISTEN_MODE 0x81
#define NFC_F_PASSIVE_LISTEN_MODE 0x82
#define NFC_A_ACTIVE_LISTEN_MODE  0x83
/* RFU - 0x84 */
#define NFC_F_ACTIVE_LISTEN_MODE      0x85
#define NFC_15693_PASSIVE_LISTEN_MODE 0x86
/* 0x87 - 0xEF RFU */
/* 0xF0 - 0xFF Reserved for Proprietary Technologies in Listen Mode */

#define PROTOCOL_UNDETERMINED 0x00
#define PROTOCOL_T1T          0x01
#define PROTOCOL_T2T          0x02
#define PROTOCOL_T3T          0x03
#define PROTOCOL_ISO_DEP      0x04
#define PROTOCOL_NFC_DEP      0x05
/* 0x06 - 0x7F RFU */
/* 0x80 - 0xFE For proprietary use */
/* 0xFF RFU */

#define NFCEE_Direct_RF_Interface 0x00
#define Frame_RF_interface        0x01
#define ISO_DEP_RF_interface      0x02
#define NFC_DEP_RF_interface      0x03
/* 0x04 - 0x7F RFU */
/* 0x80 - 0xFE For proprietary use */
/* 0xFF RFU */

#define APDU                   0x00
#define HCI_ACCESS             0x01
#define TYPE_3_TAG_COMMAND_SET 0x02
#define TRANSPARENT            0x03
/* 0x04 - 0x7F RFU */
/* 0x80 - 0xFE For proprietary use */
/* 0xFF RFU */

#define NFC_BIT_RATE_106  0x00
#define NFC_BIT_RATE_212  0x01
#define NFC_BIT_RATE_424  0x02
#define NFC_BIT_RATE_848  0x03
#define NFC_BIT_RATE_1695 0x04
#define NFC_BIT_RATE_3390 0x05
#define NFC_BIT_RATE_6780 0x06
/* 0x07 - 0x7F RFU */
/* 0x80 - 0xFE For proprietary use */
/* 0xFF RFU */

typedef enum {
    IDLE_MODE,
    SLEEP_MODE,
    SLEEP_AFMODE,
    DISCOVERY
    /* 0x04 - 0xFF RFU */
} nci_rf_deacivation_mode_t;

#define DH_REQUEST       0x00
#define ENDPOINT_REQUEST 0x01
#define RF_LINK_LOSS     0x02
#define NFC_B_BAD_AFI    0x03
/* 0x04 - 0xFF RFU */

/* Discovery Types/Detected Technology and Mode */
#define NCI_DISCOVERY_TYPE_POLL_A        0x00
#define NCI_DISCOVERY_TYPE_POLL_B        0x01
#define NCI_DISCOVERY_TYPE_POLL_F        0x02
#define NCI_DISCOVERY_TYPE_POLL_A_ACTIVE 0x03
#define NCI_DISCOVERY_TYPE_POLL_F_ACTIVE 0x05
#define NCI_DISCOVERY_TYPE_POLL_ISO15693 0x06

typedef enum {
    LAST_NOTIFICATION,
    LAST_NOTIFICATION_NFCC_LIMIT,
    MORE_NOTIFICATION
    /* 3 - 255 RFU */
} notification_type_t;

typedef enum {
    CARD_READWRITE,
    CARD_EMULATE,
    PEER_TO_PEER
} nci_application_mode_t;

typedef enum {
    T1T,
    T2T,
    T3T,
    ISO_DEP,
    NFC_DEP
} card_type_t;

typedef enum {
    HW_RESET_RFC,
    HW_RESET_WFR,
    SW_RESET_RFC,
    SW_RESET_WFR,
    ENABLE_CUSTOM_COMMANDS_RFC,
    ENABLE_CUSTOM_COMMANDS_WFR,
    RF_IDLE_CMD,
    RF_IDLE_WFR,
    RF_GO_TO_DISCOVERY_WFR,
    RF_DISCOVERY,
    RF_WAIT_FOR_ALL_DISCOVERIES,
    RF_WAIT_FOR_HOST_SELECT,
    RF_POLL_ACTIVE,
    RF_DEACTIVATE_1_WFR,
    RF_DEACTIVATE_2_WFR,
    RF_DEACTIVATE_2_WFN,
    ERROR,
    END
} nci_state_t;

typedef enum {
    RESPONSE_OK,
    RESPONSE_NOK,
    RESPONSE_TIMEOUT
} nci_error_t;

typedef enum {
    UNKNOWN,
    NO_TAGS_PRESENT,
    NEW_TAG_PRESENT,
    OLD_TAG_PRESENT,
    MULTIPLE_TAGS_PRESENT
} tags_present_status_t;

void nci_state_machine(nci_state_t state);
tags_present_status_t get_tag_status(void);
uint8_t get_num_tags(void);
uint8_t *get_tag(uint8_t index);
