#include "nci.h"
#include <stdint.h>

/* This tag will be prefixed to the log messages */
static const char *TAG = "nci";

static tags_present_status_t TAG_STATUS = NO_TAGS_PRESENT;
static uint8_t NUM_TAGS = 0;
static uint8_t TAGS[10][32] = {0};

static esp_err_t send_message(uint8_t type, uint8_t group, uint8_t opcode, uint8_t *payload, size_t len, TickType_t timeout) {
    uint8_t buf[3 + len];
    memset(buf, 0, (3 + len) * sizeof(uint8_t));
    buf[0] = (type | group) & 0xEF; /* type and group in first byte, Packet Boundary Flag is always 0 */
    buf[1] = opcode & 0x3F;         /* opcode in second byte, clear RFU bits */
    buf[2] = len;                   /* payload len */

    for (uint32_t i = 0; i < len; i++) {
        buf[i + 3] = payload[i];
    }

    return write_data(buf, sizeof(buf), timeout);
}

static bool is_type(uint8_t type, uint8_t group, uint8_t opcode, uint8_t *data) { return (((type | group) & 0xEF) == data[0]) && ((opcode & 0x3F) == data[1]); }

static void save_tag(uint8_t msgType, uint8_t *data) {
    uint8_t offset; /* offset in the NCI message of the unique ID */
    switch (msgType) {
    case RF_INTF_ACTIVATED_NTF:
        offset = 12;
        break;

    case RF_DISCOVER_NTF:
        offset = 9;
        break;

    default:
        return;
    }

    uint8_t nfc_id_len = data[offset];
    if (nfc_id_len > 32) {
        nfc_id_len = 32;
    }
    uint8_t new_tag_index = NUM_TAGS;

    for (uint8_t i = 0; i < nfc_id_len; i++) {
        TAGS[new_tag_index][i] = data[offset + 1 + i];
    }

    NUM_TAGS++;
}

void nci_state_machine(nci_state_t state) {
    esp_err_t rc = ESP_OK;
    switch (state) {
    case HW_RESET_RFC: {
        uint8_t payload[1] = {RESET_KEEP_CONFIG}; /* CORE_RESET_CMD with Keep Configuration */
        send_message(MSG_TYPE_COMMAND, GROUP_ID_CORE, CORE_RESET_CMD, payload, sizeof(payload), 20 / portTICK_PERIOD_MS);
        nci_state_machine(HW_RESET_WFR);
    } break;

    case HW_RESET_WFR: {
        uint8_t buf[6] = {0};
        rc = read_data(buf, sizeof(buf), 20 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_RESPONSE, GROUP_ID_CORE, CORE_RESET_RSP, buf) == true) {
            nci_state_machine(SW_RESET_RFC);
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case SW_RESET_RFC: {
        send_message(MSG_TYPE_COMMAND, GROUP_ID_CORE, CORE_INIT_CMD, NULL, 0, 20 / portTICK_PERIOD_MS);
        nci_state_machine(SW_RESET_WFR);
    } break;

    case SW_RESET_WFR: {
        uint8_t buf[32] = {0}; // TODO: is this size correct?
        rc = read_data(buf, sizeof(buf), 20 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_RESPONSE, GROUP_ID_CORE, CORE_INIT_RSP, buf) == true) {
            nci_state_machine(ENABLE_CUSTOM_COMMANDS_RFC);
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case ENABLE_CUSTOM_COMMANDS_RFC: {
        send_message(MSG_TYPE_COMMAND, GROUP_ID_PROPRIETARY, NCI_PROPRIETARY_ACT_CMD, NULL, 0, 10 / portTICK_PERIOD_MS);
        nci_state_machine(ENABLE_CUSTOM_COMMANDS_WFR);
    } break;

    case ENABLE_CUSTOM_COMMANDS_WFR: {
        uint8_t buf[32] = {0}; // TODO: is this size correct?
        rc = read_data(buf, sizeof(buf), 10 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_RESPONSE, GROUP_ID_PROPRIETARY, NCI_PROPRIETARY_ACT_RSP, buf) == true) {
            nci_state_machine(RF_IDLE_CMD);
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case RF_IDLE_CMD: {
        uint8_t payload[9] = {4, NFC_A_PASSIVE_POLL_MODE, 0x01, NFC_B_PASSIVE_POLL_MODE, 0x01, NFC_F_PASSIVE_POLL_MODE, 0x01, NFC_15693_PASSIVE_POLL_MODE, 0x01}; // TODO: check these values
        send_message(MSG_TYPE_COMMAND, GROUP_ID_RF_MANAGEMENT, RF_DISCOVER_CMD, payload, sizeof(payload), 10 / portTICK_PERIOD_MS);
        nci_state_machine(RF_IDLE_WFR);
    } break;

    case RF_IDLE_WFR: {
        uint8_t buf[4] = {0};
        rc = read_data(buf, sizeof(buf), 10 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_RESPONSE, GROUP_ID_RF_MANAGEMENT, RF_DISCOVER_RSP, buf) == true) {
            nci_state_machine(RF_DISCOVERY);
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case RF_DISCOVERY: {
        uint8_t buf[32] = {0}; // TODO: is this size correct?
        rc = read_data(buf, sizeof(buf), 500 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK) {
            if (is_type(MSG_TYPE_NOTIFICATION, GROUP_ID_RF_MANAGEMENT, RF_INTF_ACTIVATED_NTF, buf) == true) {
                /* single tag found */
                save_tag(RF_INTF_ACTIVATED_NTF, buf);
                TAG_STATUS = NEW_TAG_PRESENT;
                nci_state_machine(RF_POLL_ACTIVE);
            } else if (is_type(MSG_TYPE_NOTIFICATION, GROUP_ID_RF_MANAGEMENT, RF_DISCOVER_NTF, buf) == true) {
                /* multiple tags found */
                save_tag(RF_DISCOVER_NTF, buf);
                nci_state_machine(RF_WAIT_FOR_ALL_DISCOVERIES);
            }
        } else {
            TAG_STATUS = NO_TAGS_PRESENT;
            nci_state_machine(RF_DISCOVERY);
        }
    } break;

    case RF_WAIT_FOR_ALL_DISCOVERIES: {
        uint8_t buf[32] = {0}; // TODO: is this size correct?
        rc = read_data(buf, sizeof(buf), 25 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_NOTIFICATION, GROUP_ID_RF_MANAGEMENT, RF_DISCOVER_NTF, buf) == true) {
            notification_type_t type = (notification_type_t) buf[buf[6] + 7];
            switch (type) {
            case LAST_NOTIFICATION:
            case LAST_NOTIFICATION_NFCC_LIMIT:
                save_tag(RF_DISCOVER_NTF, buf);
                nci_state_machine(RF_WAIT_FOR_HOST_SELECT);
                break;
            case MORE_NOTIFICATION:
                save_tag(RF_DISCOVER_NTF, buf);
                nci_state_machine(RF_WAIT_FOR_ALL_DISCOVERIES);
                break;
            default:
                nci_state_machine(RF_WAIT_FOR_ALL_DISCOVERIES);
                break;
            }
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case RF_WAIT_FOR_HOST_SELECT: {
        NUM_TAGS = 0;
        uint8_t payload[1] = {(uint8_t) IDLE_MODE}; /* in RF_WAIT_FOR_HOST_SELECT the deactivation type is ignored by the NFCC */
        send_message(MSG_TYPE_COMMAND, GROUP_ID_RF_MANAGEMENT, RF_DEACTIVATE_CMD, payload, sizeof(payload), 25 / portTICK_PERIOD_MS);
        nci_state_machine(RF_DEACTIVATE_1_WFR);
    } break;

    case RF_POLL_ACTIVE: {
        NUM_TAGS = 0;
        uint8_t payload[1] = {(uint8_t) IDLE_MODE};
        send_message(MSG_TYPE_COMMAND, GROUP_ID_RF_MANAGEMENT, RF_DEACTIVATE_CMD, payload, sizeof(payload), 25 / portTICK_PERIOD_MS);
        nci_state_machine(RF_DEACTIVATE_2_WFR);
    } break;

    case RF_DEACTIVATE_1_WFR: {
        uint8_t buf[32] = {0}; // TODO: is this size correct?
        rc = read_data(buf, sizeof(buf), 10 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_RESPONSE, GROUP_ID_RF_MANAGEMENT, RF_DEACTIVATE_RSP, buf) == true) {
            nci_state_machine(RF_IDLE_CMD);
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case RF_DEACTIVATE_2_WFR: {
        uint8_t buf[32] = {0}; // TODO: is this size correct?
        rc = read_data(buf, sizeof(buf), 10 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_RESPONSE, GROUP_ID_RF_MANAGEMENT, RF_DEACTIVATE_RSP, buf) == true) {
            nci_state_machine(RF_DEACTIVATE_2_WFN);
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case RF_DEACTIVATE_2_WFN: {
        uint8_t buf[32] = {0}; // TODO: is this size correct?
        rc = read_data(buf, sizeof(buf), 10 / portTICK_PERIOD_MS);
        if (rc == ESP_OK && buf[3] == STATUS_OK && is_type(MSG_TYPE_RESPONSE, GROUP_ID_RF_MANAGEMENT, RF_DEACTIVATE_NTF, buf) == true) {
            nci_state_machine(RF_IDLE_CMD);
        } else {
            nci_state_machine(ERROR);
        }
    } break;

    case ERROR:
        ESP_LOGE(TAG, "Encountered an error in the NCI state machine!");
        break;

    default:
        break;
    }
}

tags_present_status_t get_tag_status(void) { return TAG_STATUS; }

uint8_t get_num_tags(void) { return NUM_TAGS; }

uint8_t *get_tag(uint8_t index) {
    TAG_STATUS = OLD_TAG_PRESENT;

    return TAGS[index];
}
