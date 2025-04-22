/******************************************************************************
 * CELLCOM
 * Common Business Logic
 *
 * $Date: 2019/03/10 11:44:05 $
 * Copyright (c) Nokia Software 2019
 ******************************************************************************/
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#include "nodebase.h"
#include "lookup_library.h"

/***********************************************************************
 *
 * Constants
 *
 ***********************************************************************/


#define DIAG_LOW 10
#define DIAG_MED 50
#define DIAG_HIGH 90

#define FSIZE 1023 + 1 // Maximum field size
#define BSIZE 1023 + 1 // Maximum buffer variables size
#define SMFSIZE 31 + 1 // SMALL field size



/***********************************************************************
 *
 * Macros
 *
 ***********************************************************************/
#ifndef ADD_FIELD
#define ADD_FIELD(name, value) if(*value != '\0') { o_add_field(name, value); DIAG(DIAG_HIGH, "ADD_FIELD(): '%s' = [%s]", name, value); }
#endif
#ifndef ADD_FIELD_FROM_INPUT
#define ADD_FIELD_FROM_INPUT(name)  o_add_field(name, i_get(name)); DIAG(DIAG_HIGH, "ADD_FIELD(): '%s' = [%s]", name, i_get(name));
#endif
#ifndef GET_FIELD
#define GET_FIELD(value, name, f_size) memset(value,'\0',f_size); strncpy(value,i_get(name),f_size); DIAG(DIAG_HIGH, "GET_FIELD(): '%s' = [%s]", name, value);
#endif


/***********************************************************************
 *
 * Rejection storages
 *
 ***********************************************************************/
const char* INV_PGW_IMSI = "INV_PGW_IMSI";
const char* INV_PGW_RECORD_OPEN_TIME = "INV_PGW_RECOPENTIME";
const char* INV_PGW_LIST_OF_SERVICE_DATA = "INV_PGW_LISTSERVDATA";

const char* INV_SGW_IMSI = "INV_SGW_IMSI";
const char* INV_SGW_RECORD_OPENING_TIME = "INV_SGW_RECOPENTIME";
const char* INV_SGW_CHANGE_TIME = "INV_SGW_CHANGETIME";
const char* INV_SGW_APN = "INV_SGW_APN";
const char* LOCAL_SGW = "LOCAL_SGW";

const char* INV_SGSN_SGSN_ADDRESS = "INV_SGSN_SGSNADDRESS";
const char* INV_SGSN_GGSN_ADDRESS = "INV_SGSN_GGSNADDRESS";
const char* INV_SGSN_IMSI = "INV_SGSN_IMSI";
const char* INV_SGSN_RECORD_OPENING_TIME = "INV_SGSN_RECOPENTIME";
const char* INV_SGSN_CHANGE_TIME = "INV_SGSN_CHANGETIME";
const char* INV_SGSN_CHARGING_ID = "INV_SGSN_CHARGINGID";


// Node parameters

char NP_DurationNoAggregation[SMFSIZE];
char NP_DiscardLocalSGW[SMFSIZE];
char NP_ProcessRG0Records[SMFSIZE];
char NP_LocalIMSIPrefixes[FSIZE];
char NP_AlarmThreshold[SMFSIZE];

int number_of_rejections;
int send_alarm;
int NP_AlarmThreshold_int;
int record_number;

/* global variables*/
// Lookup tables descriptors
int LS_SGSN_IP_RANGE = -1;
int LS_FLUSH_TRIGGERS = -1;
int LS_CAUSE_FOR_CLOSING = -1;
int LS_APN_FILTER = -1;
char csLocalIMSIPrefixes[FSIZE];

//FIELD variables
char EL_RECORD_TYPE [FSIZE];
char EL_GGSN_ADDRESS [FSIZE];
char EL_SGSN_ADDRESS [FSIZE];
char EL_SERVED_IMEI [FSIZE];
char EL_PLMNID [FSIZE];
char EL_SERVED_PDP_TYPE_ORG[FSIZE];
char EL_SERVED_PDP_TYPE_VALUE[FSIZE];
char EL_pdpPDNType[FSIZE];
char EL_NAPI_FOR_MSISDN [FSIZE];
char EL_SERVED_MSISDN [FSIZE];
char EL_SERVED_PDP_ADDRESS [FSIZE];
char EL_SERVED_PDP_ADDR_EXT [FSIZE];
char EL_FCI [FSIZE];
char EL_NODE_ID [FSIZE];
char EL_DYNAMIC_ADDRESS_FLAG [FSIZE];
char EL_DURATION [FSIZE];
char EL_USER_LOCATION [FSIZE];
char EL_CHANGE_CONDITION [FSIZE];
char EL_FIRST_ACTIVITY [FSIZE];
char EL_LAST_ACTIVITY [FSIZE];
char EL_NO_OCS_CREDIT_CONTROL [FSIZE];
char EL_RECORD_CLOSING_TIME [FSIZE];
char EL_CLOSING_TIMEZONE [FSIZE];
char EL_RECORD_OPENING_TIME [FSIZE];
char EL_OPENING_TIMEZONE [FSIZE];
char EL_ACCESS_POINT_NAME [FSIZE];
char EL_CONTAINER_NUMBER [FSIZE];
char EL_FIRST_SEQUENCE_NUMBER [FSIZE];
char EL_LAST_SEQUENCE_NUMBER [FSIZE];
char EL_SERVICE_ID [FSIZE];
char EL_DATA_VOLUME_UPLINK [FSIZE];
char EL_DATA_VOLUME_DOWNLINK [FSIZE];
char EL_RATING_GROUP [FSIZE];
char EL_DIAGNOSTICS_VALUE [FSIZE];
char EL_CHARGING_CHARACTERISTICS [FSIZE];
char EL_CAUSE_FOR_CLOSING [FSIZE];
char EL_S_RECORD_OPENING_TIME [FSIZE];
char EL_S_RECORD_CLOSING_TIME [FSIZE];
char EL_S_CHARGING_CHARACTERISTICS [FSIZE];
char EL_S_UPLINK [FSIZE];
char EL_S_DOWNLINK [FSIZE];
char EL_S_FIRST_SEQUENCE_NUMBER [FSIZE];
char EL_S_LAST_SEQUENCE_NUMBER [FSIZE];
char EL_S_QOS_REQUESTED [FSIZE];
char EL_S_QOS_NEGOTIATED [FSIZE];
char EL_S_COMPLETE [FSIZE];
char EL_S_CAUSE_FOR_CLOSING [FSIZE];
char EL_MS_NW_CAPABILITY [FSIZE];
char EL_SGSN_CHANGE [FSIZE];
// Aggregation fields
char EL_START_TIME [FSIZE];
char EL_END_TIME [FSIZE];
char EL_REC_SEQ_NBR [FSIZE];
char EL_PARTIAL_TYPE [FSIZE];
char EL_FLUSH_TRIGGER [FSIZE];
char EL_TOTAL_VOLUME [FSIZE];
char EL_FAILURE_HANDLING [FSIZE];
char EL_REJECTION_STORAGE[FSIZE];
// 5G Volume CR new fields
char EL_5G_FLAG [FSIZE];
char EL_5G_VOLUME_UPLINK [FSIZE];
char EL_5G_VOLUME_DOWNLINK [FSIZE];


typedef struct pgwcontainerfields {
    int reject_flag;
    int count;
    int container_number;

    int missing_timeOfReport;
    int missing_ratingGroup;
    int missing_resultCode;
    int missing_failureHandlingContinue;

    char pSFreeFormatData [FSIZE];
    char timeUsage [FSIZE];
    char serviceConditionChange [FSIZE];
    char timeOfFirstUsage [FSIZE];
    char timeOfLastUsage [FSIZE];
    char resultCode [FSIZE];
    char failureHandlingContinue [FSIZE];
    char timeOfReport [FSIZE];
    char chargingRuleBaseName [FSIZE];
    char datavolumeFBCUplink [FSIZE];
    char datavolumeFBCDownlink [FSIZE];
    char ratingGroup [FSIZE];
    struct pgwcontainerfields *next;
} Pcontainerfields;

/***********************************************************************
 *
 * Application-specific functions
 *
 ***********************************************************************/


void init_pgwcontainerfields(Pcontainerfields *p) {
    DIAG(DIAG_HIGH, "init_pgwcontainerfields()");
    p->reject_flag = 0;
    p->count = 1;
    p->container_number = -1;

    p->missing_timeOfReport = 0;
    p->missing_ratingGroup = 0;
    p->missing_resultCode = 0;
    p->missing_failureHandlingContinue = 0;

    memset(p->pSFreeFormatData, '\0', FSIZE);
    memset(p->timeUsage, '\0', FSIZE);
    memset(p->serviceConditionChange, '\0', FSIZE);
    memset(p->timeOfFirstUsage, '\0', FSIZE);
    memset(p->timeOfLastUsage, '\0', FSIZE);
    memset(p->resultCode, '\0', FSIZE);
    memset(p->failureHandlingContinue, '\0', FSIZE);
    memset(p->timeOfReport, '\0', FSIZE);
    memset(p->chargingRuleBaseName, '\0', FSIZE);
    memset(p->datavolumeFBCUplink, '\0', FSIZE);
    memset(p->datavolumeFBCDownlink, '\0', FSIZE);
    memset(p->ratingGroup, '\0', FSIZE);
    memset(p->failureHandlingContinue, '\0', FSIZE);
    p->next = NULL;
}

void drop_pgwcontainer_list(Pcontainerfields *p) {
    Pcontainerfields *q = NULL;
    DIAG(DIAG_HIGH, "drop_pgwcontainer_list()");
    while (p != NULL) {
        q = p;
        p = p->next;
        free(q);
        q = NULL;
    }
    DIAG(DIAG_HIGH, "drop_pgwcontainer_list(): pgwcontainer_list was dropped ");
}

void add_pgwcontainer_element(Pcontainerfields **p, Pcontainerfields *el) {
    Pcontainerfields *h = NULL, *n = NULL;
    int sum = 0;
    DIAG(DIAG_HIGH, "add_pgwcontainer_element()");
    if (el == NULL) {
        DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Can not add empty element");
        return;
    }
    DIAG(DIAG_HIGH, "add_pgwcontainer_element() : checking for empty list");
    if (*p == NULL) {
        DIAG(DIAG_HIGH, "add_pgwcontainer_element() : empty list");
        *p = el;
        DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Empty list, first element added %s", el->ratingGroup);
        el->next = NULL;
        return;
    }

    h = *p;
    while (h != NULL) {
        if (el->reject_flag == 0) {
            if (strcmp(h->ratingGroup, el->ratingGroup) == 0) {
                h->count++;
                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Found element %s", h->ratingGroup);
                if (h->container_number == -1) {
                    DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element container_number %s to %s", h->container_number, el->container_number);
                    h->container_number = el->container_number;
                }
                if (h->count == 1) {
                    DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element timeOfFirstUsage %s to %s", h->timeOfFirstUsage, el->timeOfFirstUsage);
                    strncpy(h->timeOfFirstUsage, el->timeOfFirstUsage, FSIZE);
                }

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element pSFreeFormatData %s to %s", h->pSFreeFormatData, el->pSFreeFormatData);
                strncpy(h->pSFreeFormatData, el->pSFreeFormatData, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element chargingRuleBaseName %s to %s", h->chargingRuleBaseName, el->chargingRuleBaseName);
                strncpy(h->chargingRuleBaseName, el->chargingRuleBaseName, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element timeOfLastUsage %s to %s", h->timeOfLastUsage, el->timeOfLastUsage);
                strncpy(h->timeOfLastUsage, el->timeOfLastUsage, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element resultCode %s to %s", h->resultCode, el->resultCode);
                strncpy(h->resultCode, el->resultCode, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element failureHandlingContinue %s to %s", h->failureHandlingContinue, el->failureHandlingContinue);
                strncpy(h->failureHandlingContinue, el->failureHandlingContinue, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element timeOfReport %s to %s", h->timeOfReport, el->timeOfReport);
                strncpy(h->timeOfReport, el->timeOfReport, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element chargingRuleBaseName %s to %s", h->chargingRuleBaseName, el->chargingRuleBaseName);
                strncpy(h->chargingRuleBaseName, el->chargingRuleBaseName, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element serviceConditionChange %s to %s", h->serviceConditionChange, el->serviceConditionChange);
                strncpy(h->serviceConditionChange, el->serviceConditionChange, FSIZE);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element ratingGroup %s to %s", h->ratingGroup, el->ratingGroup);
                strncpy(h->ratingGroup, el->ratingGroup, FSIZE);

                sum = atoi(h->timeUsage) + atoi(el->timeUsage);
                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element timeUsage %s to %d", h->timeUsage, sum);
                snprintf(h->timeUsage, FSIZE, "%d", sum);

                sum = atoi(h->datavolumeFBCDownlink) + atoi(el->datavolumeFBCDownlink);
                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element datavolumeFBCDownlink %s to %d", h->datavolumeFBCDownlink, sum);
                snprintf(h->datavolumeFBCDownlink, FSIZE, "%d", sum);

                sum = atoi(h->datavolumeFBCUplink) + atoi(el->datavolumeFBCUplink);
                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Updated element datavolumeFBCUplink %s to %d", h->datavolumeFBCUplink, sum);
                snprintf(h->datavolumeFBCUplink, FSIZE, "%d", sum);

                DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Added element %s count=%d", h->ratingGroup, h->count);
                free(el);
                break;
            }
        }
        n = h;
        h = h->next;
    }

    if (h == NULL) {
        n->next = el;
        el->next = NULL;
        DIAG(DIAG_HIGH, "add_pgwcontainer_element(): Added new element %s %d", el->ratingGroup, el->count);
    }
    DIAG(DIAG_HIGH, "add_pgwcontainer_element() : return");
}

void init_fields(void) {
    DIAG(DIAG_HIGH, "init_fields()");
    memset(EL_RECORD_TYPE, '\0', FSIZE);
    memset(EL_GGSN_ADDRESS, '\0', FSIZE);
    memset(EL_SGSN_ADDRESS, '\0', FSIZE);
    memset(EL_SERVED_IMEI, '\0', FSIZE);
    memset(EL_PLMNID, '\0', FSIZE);
    memset(EL_SERVED_PDP_TYPE_ORG, '\0', FSIZE);
    memset(EL_SERVED_PDP_TYPE_VALUE, '\0', FSIZE);
    memset(EL_pdpPDNType, '\0', FSIZE);
    memset(EL_NAPI_FOR_MSISDN, '\0', FSIZE);
    memset(EL_SERVED_MSISDN, '\0', FSIZE);
    memset(EL_SERVED_PDP_ADDRESS, '\0', FSIZE);
    memset(EL_SERVED_PDP_ADDR_EXT, '\0', FSIZE);
    memset(EL_FCI, '\0', FSIZE);
    memset(EL_NODE_ID, '\0', FSIZE);
    memset(EL_DYNAMIC_ADDRESS_FLAG, '\0', FSIZE);
    memset(EL_DURATION, '\0', FSIZE);
    memset(EL_USER_LOCATION, '\0', FSIZE);
    memset(EL_CHANGE_CONDITION, '\0', FSIZE);
    memset(EL_FIRST_ACTIVITY, '\0', FSIZE);
    memset(EL_LAST_ACTIVITY, '\0', FSIZE);
    memset(EL_NO_OCS_CREDIT_CONTROL, '\0', FSIZE);
    memset(EL_RECORD_CLOSING_TIME, '\0', FSIZE);
    memset(EL_CLOSING_TIMEZONE, '\0', FSIZE);
    memset(EL_RECORD_OPENING_TIME, '\0', FSIZE);
    memset(EL_OPENING_TIMEZONE, '\0', FSIZE);
    memset(EL_ACCESS_POINT_NAME, '\0', FSIZE);
    memset(EL_CONTAINER_NUMBER, '\0', FSIZE);
    memset(EL_FIRST_SEQUENCE_NUMBER, '\0', FSIZE);
    memset(EL_LAST_SEQUENCE_NUMBER, '\0', FSIZE);
    memset(EL_SERVICE_ID, '\0', FSIZE);
    memset(EL_DATA_VOLUME_UPLINK, '\0', FSIZE);
    memset(EL_DATA_VOLUME_DOWNLINK, '\0', FSIZE);
    memset(EL_RATING_GROUP, '\0', FSIZE);
    memset(EL_DIAGNOSTICS_VALUE, '\0', FSIZE);
    memset(EL_CHARGING_CHARACTERISTICS, '\0', FSIZE);
    memset(EL_CAUSE_FOR_CLOSING, '\0', FSIZE);
    memset(EL_S_RECORD_OPENING_TIME, '\0', FSIZE);
    memset(EL_S_RECORD_CLOSING_TIME, '\0', FSIZE);
    memset(EL_S_CHARGING_CHARACTERISTICS, '\0', FSIZE);
    memset(EL_S_UPLINK, '\0', FSIZE);
    memset(EL_S_DOWNLINK, '\0', FSIZE);
    memset(EL_S_FIRST_SEQUENCE_NUMBER, '\0', FSIZE);
    memset(EL_S_LAST_SEQUENCE_NUMBER, '\0', FSIZE);
    memset(EL_S_QOS_REQUESTED, '\0', FSIZE);
    memset(EL_S_QOS_NEGOTIATED, '\0', FSIZE);
    memset(EL_S_COMPLETE, '\0', FSIZE);
    memset(EL_S_CAUSE_FOR_CLOSING, '\0', FSIZE);
    memset(EL_MS_NW_CAPABILITY, '\0', FSIZE);
    memset(EL_SGSN_CHANGE, '\0', FSIZE);
    memset(EL_REC_SEQ_NBR, '\0', FSIZE);
	
    memset(EL_5G_FLAG, '\0', FSIZE);
    memset(EL_5G_VOLUME_UPLINK, '\0', FSIZE);
    memset(EL_5G_VOLUME_DOWNLINK, '\0', FSIZE);
}

void init_pgw_container_vars(void) {
    DIAG(DIAG_HIGH, "init_pgw_container_vars()");
    memset(EL_FCI, '\0', FSIZE);
    memset(EL_DURATION, '\0', FSIZE);
    memset(EL_CHANGE_CONDITION, '\0', FSIZE);
    memset(EL_FIRST_ACTIVITY, '\0', FSIZE);
    memset(EL_LAST_ACTIVITY, '\0', FSIZE);
    memset(EL_NO_OCS_CREDIT_CONTROL, '\0', FSIZE);
    memset(EL_RECORD_CLOSING_TIME, '\0', FSIZE);
    memset(EL_CLOSING_TIMEZONE, '\0', FSIZE);
    memset(EL_CONTAINER_NUMBER, '\0', FSIZE);
    memset(EL_SERVICE_ID, '\0', FSIZE);
    memset(EL_DATA_VOLUME_UPLINK, '\0', FSIZE);
    memset(EL_DATA_VOLUME_DOWNLINK, '\0', FSIZE);
    memset(EL_RATING_GROUP, '\0', FSIZE);
    memset(EL_FAILURE_HANDLING, '\0', FSIZE);
}

void init_aggr_fields(void) {
    DIAG(DIAG_HIGH, "init_aggr_fields()");
    memset(EL_START_TIME, '\0', FSIZE);
    memset(EL_END_TIME, '\0', FSIZE);

    memset(EL_PARTIAL_TYPE, '\0', FSIZE);
    memset(EL_FLUSH_TRIGGER, '\0', FSIZE);
    memset(EL_TOTAL_VOLUME, '\0', FSIZE);
    //memset(EL_FAILURE_HANDLING, '\0', FSIZE);
}

void write_pgwRecord(const char* output, int rflag, char* reason) {
    char buff[SMFSIZE];
    if (output[0] == '\0' && rflag == 0) {
        DIAG(DIAG_HIGH, "write_pgwRecord(): No defined output link, filtering record");
        //DIAG(DIAG_LOW,"Filtering pgwRecord number %d", record_number);
        return;
    }
    DIAG(DIAG_HIGH, "write_pgwRecord(): enter");
    nb_new_record();
    ADD_FIELD("EL_RECORD_TYPE", EL_RECORD_TYPE);
    ADD_FIELD("EL_GGSN_ADDRESS", EL_GGSN_ADDRESS);
    ADD_FIELD("EL_SGSN_ADDRESS", EL_SGSN_ADDRESS);
    ADD_FIELD("EL_SERVED_IMEI", EL_SERVED_IMEI);
    ADD_FIELD("EL_PLMNID", EL_PLMNID);
    ADD_FIELD("EL_SERVED_PDP_TYPE_ORG", EL_SERVED_PDP_TYPE_ORG);
    ADD_FIELD("EL_SERVED_PDP_TYPE_VALUE", EL_SERVED_PDP_TYPE_VALUE);
    ADD_FIELD("EL_NAPI_FOR_MSISDN", EL_NAPI_FOR_MSISDN);
    ADD_FIELD("EL_SERVED_MSISDN", EL_SERVED_MSISDN);
    ADD_FIELD("EL_SERVED_PDP_ADDRESS", EL_SERVED_PDP_ADDRESS);
    ADD_FIELD("EL_SERVED_PDP_ADDR_EXT", EL_SERVED_PDP_ADDR_EXT);
    ADD_FIELD("EL_FCI", EL_FCI);
    ADD_FIELD("EL_NODE_ID", EL_NODE_ID);
    ADD_FIELD("EL_DYNAMIC_ADDRESS_FLAG", EL_DYNAMIC_ADDRESS_FLAG);
    ADD_FIELD("EL_DURATION", EL_DURATION);
    ADD_FIELD("EL_USER_LOCATION", EL_USER_LOCATION);
    ADD_FIELD("EL_CHANGE_CONDITION", EL_CHANGE_CONDITION);
    ADD_FIELD("EL_FIRST_ACTIVITY", EL_FIRST_ACTIVITY);
    ADD_FIELD("EL_LAST_ACTIVITY", EL_LAST_ACTIVITY);
    ADD_FIELD("EL_NO_OCS_CREDIT_CONTROL", EL_NO_OCS_CREDIT_CONTROL);
    ADD_FIELD("EL_RECORD_CLOSING_TIME", EL_RECORD_CLOSING_TIME);
    ADD_FIELD("EL_CLOSING_TIMEZONE", EL_CLOSING_TIMEZONE);
    ADD_FIELD("EL_RECORD_OPENING_TIME", EL_RECORD_OPENING_TIME);
    ADD_FIELD("EL_OPENING_TIMEZONE", EL_OPENING_TIMEZONE);
    ADD_FIELD("EL_ACCESS_POINT_NAME", EL_ACCESS_POINT_NAME);
    ADD_FIELD("EL_CONTAINER_NUMBER", EL_CONTAINER_NUMBER);
    ADD_FIELD("EL_FIRST_SEQUENCE_NUMBER", EL_FIRST_SEQUENCE_NUMBER);
    ADD_FIELD("EL_LAST_SEQUENCE_NUMBER", EL_LAST_SEQUENCE_NUMBER);
    ADD_FIELD("EL_SERVICE_ID", EL_SERVICE_ID);
    ADD_FIELD("EL_DATA_VOLUME_UPLINK", EL_DATA_VOLUME_UPLINK);
    ADD_FIELD("EL_DATA_VOLUME_DOWNLINK", EL_DATA_VOLUME_DOWNLINK);
    ADD_FIELD("EL_RATING_GROUP", EL_RATING_GROUP);
    ADD_FIELD("EL_DIAGNOSTICS_VALUE", EL_DIAGNOSTICS_VALUE);
    ADD_FIELD("EL_CHARGING_CHARACTERISTICS", EL_CHARGING_CHARACTERISTICS);
    ADD_FIELD("EL_CAUSE_FOR_CLOSING", EL_CAUSE_FOR_CLOSING);
    //Fields related to aggregation
    ADD_FIELD("EL_START_TIME", EL_START_TIME);
    ADD_FIELD("EL_END_TIME", EL_END_TIME);
    ADD_FIELD("EL_REC_SEQ_NBR", EL_REC_SEQ_NBR);
    ADD_FIELD("EL_PARTIAL_TYPE", EL_PARTIAL_TYPE);
    ADD_FIELD("EL_FLUSH_TRIGGER", EL_FLUSH_TRIGGER);
    ADD_FIELD("EL_TOTAL_VOLUME", EL_TOTAL_VOLUME);
    ADD_FIELD("EL_FAILURE_HANDLING", EL_FAILURE_HANDLING);
    // Direct field mapping
    ADD_FIELD("chargingID", i_get(".chargingID"));
    ADD_FIELD("servedIMSI", i_get(".servedIMSI"));
    ADD_FIELD("chChSelectionMode", i_get(".chChSelectionMode"));
    ADD_FIELD("rATType", i_get(".rATType"));
    ADD_FIELD("apnSelectionMode", i_get(".apnSelectionMode"));
    ADD_FIELD("localSequenceNumber", i_get(".localSequenceNumber"));
    ADD_FIELD("causeForRecClosing", i_get(".causeForRecClosing"));

    // New 5G CR fields
    ADD_FIELD("EL_5G_FLAG", EL_5G_FLAG);
    ADD_FIELD("EL_5G_VOLUME_UPLINK", EL_5G_VOLUME_UPLINK);
    ADD_FIELD("EL_5G_VOLUME_DOWNLINK", EL_5G_VOLUME_DOWNLINK);

    audit_out_type("PGW");

    if (rflag) {
        ADD_FIELD("OUTPUT", output);
        ADD_FIELD("REASON", reason);
        ADD_FIELD("ProcessRG0Records", NP_ProcessRG0Records);
        ADD_FIELD("timeOfReport", i_get("timeOfReport"));
        ADD_FIELD("ratingGroup", i_get("ratingGroup"));
        snprintf(buff, SMFSIZE, "%d", rflag);
        ADD_FIELD("REJECT_FLAG", buff);
        nb_write_record("INVALID");
        DIAG(DIAG_HIGH, "write_pgwRecord(): writing record to INVALID");
        //DIAG(DIAG_LOW,"Writing pgwRecord number %d to INVALID", record_number);
    } else {
        nb_write_record(output);
        DIAG(DIAG_HIGH, "write_pgwRecord(): writing record to %s", output);
        //DIAG(DIAG_LOW,"Writing pgwRecord number %d to %s", record_number,output);
    }

}

void write_sGWRecord(const char* output) {
    if (output[0] == '\0') {
        DIAG(DIAG_HIGH, "write_sGWRecord(): No defined output link, filtering record");
        //DIAG(DIAG_LOW,"Filtering sGWRecord number %d", record_number);

        return;
    }
    DIAG(DIAG_HIGH, "write_sGWRecord(): enter");
    nb_new_record();
    ADD_FIELD("EL_RECORD_TYPE", EL_RECORD_TYPE);
    ADD_FIELD("EL_RECORD_OPENING_TIME", EL_RECORD_OPENING_TIME);
    ADD_FIELD("EL_S_RECORD_OPENING_TIME", EL_S_RECORD_OPENING_TIME);
    ADD_FIELD("EL_S_RECORD_CLOSING_TIME", EL_S_RECORD_CLOSING_TIME);
    ADD_FIELD("EL_SERVED_IMEI", EL_SERVED_IMEI);
    ADD_FIELD("EL_NAPI_FOR_MSISDN", EL_NAPI_FOR_MSISDN);
    ADD_FIELD("EL_SERVED_MSISDN", EL_SERVED_MSISDN);
    ADD_FIELD("EL_SGSN_ADDRESS", EL_SGSN_ADDRESS);
    ADD_FIELD("EL_GGSN_ADDRESS", EL_GGSN_ADDRESS);
    ADD_FIELD("EL_ACCESS_POINT_NAME", EL_ACCESS_POINT_NAME);
    ADD_FIELD("EL_SERVED_PDP_TYPE_ORG", EL_SERVED_PDP_TYPE_ORG);
    ADD_FIELD("EL_SERVED_PDP_TYPE_VALUE", EL_SERVED_PDP_TYPE_VALUE);
    ADD_FIELD("EL_SERVED_PDP_ADDRESS", EL_SERVED_PDP_ADDRESS);
    ADD_FIELD("EL_DIAGNOSTICS_VALUE", EL_DIAGNOSTICS_VALUE);
    ADD_FIELD("EL_S_CHARGING_CHARACTERISTICS", EL_S_CHARGING_CHARACTERISTICS);
    ADD_FIELD("EL_S_UPLINK", EL_S_UPLINK);
    ADD_FIELD("EL_S_DOWNLINK", EL_S_DOWNLINK);
    ADD_FIELD("EL_S_FIRST_SEQUENCE_NUMBER", EL_S_FIRST_SEQUENCE_NUMBER);
    ADD_FIELD("EL_S_LAST_SEQUENCE_NUMBER", EL_S_LAST_SEQUENCE_NUMBER);
    ADD_FIELD("EL_NODE_ID", EL_NODE_ID);
    ADD_FIELD("EL_CHANGE_CONDITION", EL_CHANGE_CONDITION);
    ADD_FIELD("EL_S_CAUSE_FOR_CLOSING", EL_S_CAUSE_FOR_CLOSING);
    //Fields related to aggregation
    ADD_FIELD("EL_START_TIME", EL_START_TIME);
    ADD_FIELD("EL_END_TIME", EL_END_TIME);
    ADD_FIELD("EL_REC_SEQ_NBR", EL_REC_SEQ_NBR);
    ADD_FIELD("EL_PARTIAL_TYPE", EL_PARTIAL_TYPE);
    ADD_FIELD("EL_FLUSH_TRIGGER", EL_FLUSH_TRIGGER);
    ADD_FIELD("EL_TOTAL_VOLUME", EL_TOTAL_VOLUME);
    ADD_FIELD("EL_DURATION", EL_DURATION);
    // Direct field mapping
    ADD_FIELD("chargingID", i_get(".chargingID"));
    ADD_FIELD("servedIMSI", i_get(".servedIMSI"));
    ADD_FIELD("duration", i_get(".duration"));
    audit_out_type("SGW");
    nb_write_record(output);
    DIAG(DIAG_HIGH, "write_sGWRecord(): writing record to %s", output);
    //DIAG(DIAG_LOW,"Writing sGWRecord number %d to %s", record_number,output);
}

void write_sgsnPDPRecord(const char* output) {
    if (output[0] == '\0') {
        DIAG(DIAG_HIGH, "write_sgsnPDPRecord(): No defined output link, filtering record");
        //DIAG(DIAG_LOW,"Filtering sgsnPDPRecord number %d", record_number);

        return;
    }
    DIAG(DIAG_HIGH, "write_sgsnPDPRecord(): enter");
    nb_new_record();
    ADD_FIELD("EL_RECORD_TYPE", EL_RECORD_TYPE);
    ADD_FIELD("EL_RECORD_OPENING_TIME", EL_RECORD_OPENING_TIME);
    ADD_FIELD("EL_S_RECORD_OPENING_TIME", EL_S_RECORD_OPENING_TIME);
    ADD_FIELD("EL_S_RECORD_CLOSING_TIME", EL_S_RECORD_CLOSING_TIME);
    ADD_FIELD("EL_SERVED_IMEI", EL_SERVED_IMEI);
    ADD_FIELD("EL_NAPI_FOR_MSISDN", EL_NAPI_FOR_MSISDN);
    ADD_FIELD("EL_SERVED_MSISDN", EL_SERVED_MSISDN);
    ADD_FIELD("EL_SGSN_ADDRESS", EL_SGSN_ADDRESS);
    ADD_FIELD("EL_GGSN_ADDRESS", EL_GGSN_ADDRESS);
    ADD_FIELD("EL_ACCESS_POINT_NAME", EL_ACCESS_POINT_NAME);
    ADD_FIELD("EL_SERVED_PDP_TYPE_ORG", EL_SERVED_PDP_TYPE_ORG);
    ADD_FIELD("EL_SERVED_PDP_TYPE_VALUE", EL_SERVED_PDP_TYPE_VALUE);
    ADD_FIELD("EL_SERVED_PDP_ADDRESS", EL_SERVED_PDP_ADDRESS);
    ADD_FIELD("EL_MS_NW_CAPABILITY", EL_MS_NW_CAPABILITY);
    ADD_FIELD("EL_S_CHARGING_CHARACTERISTICS", EL_S_CHARGING_CHARACTERISTICS);
    ADD_FIELD("EL_S_UPLINK", EL_S_UPLINK);
    ADD_FIELD("EL_S_DOWNLINK", EL_S_DOWNLINK);
    ADD_FIELD("EL_S_QOS_REQUESTED", EL_S_QOS_REQUESTED);
    ADD_FIELD("EL_S_QOS_NEGOTIATED", EL_S_QOS_NEGOTIATED);
    ADD_FIELD("EL_CHANGE_CONDITION", EL_CHANGE_CONDITION);
    ADD_FIELD("EL_S_CAUSE_FOR_CLOSING", EL_S_CAUSE_FOR_CLOSING);
    ADD_FIELD("EL_S_COMPLETE", EL_S_COMPLETE);
    ADD_FIELD("EL_S_FIRST_SEQUENCE_NUMBER", EL_S_FIRST_SEQUENCE_NUMBER);
    ADD_FIELD("EL_S_LAST_SEQUENCE_NUMBER", EL_S_LAST_SEQUENCE_NUMBER);
    ADD_FIELD("EL_SGSN_CHANGE", EL_SGSN_CHANGE);
    //Fields related to aggregation
    ADD_FIELD("EL_START_TIME", EL_START_TIME);
    ADD_FIELD("EL_END_TIME", EL_END_TIME);
    ADD_FIELD("EL_REC_SEQ_NBR", EL_REC_SEQ_NBR);
    ADD_FIELD("EL_PARTIAL_TYPE", EL_PARTIAL_TYPE);
    ADD_FIELD("EL_FLUSH_TRIGGER", EL_FLUSH_TRIGGER);
    ADD_FIELD("EL_TOTAL_VOLUME", EL_TOTAL_VOLUME);
    ADD_FIELD("EL_DURATION", EL_DURATION);
    // Direct field mapping
    ADD_FIELD("chargingID", i_get(".chargingID"));
    ADD_FIELD("servedIMSI", i_get(".servedIMSI"));
    ADD_FIELD("routingArea", i_get(".routingArea"));
    ADD_FIELD("locationAreaCode", i_get(".locationAreaCode"));
    ADD_FIELD("cellIdentifier", i_get(".cellIdentifier"));
    ADD_FIELD("duration", i_get(".duration"));
    audit_out_type("SGSN");
    nb_write_record(output);
    DIAG(DIAG_HIGH, "write_sgsnPDPRecord(): writing record to %s", output);
    //DIAG(DIAG_LOW,"Writing sgsnPDPRecord number %d to %s", record_number,output);
}

unsigned int convert_datetime_to_epoch(const char* datetime) {
    long unix_days; /* Number of days since 01.01.70*/
    long test_days; /* Used for day check like the  */
    /* invalid 29.02.70             */
    long unix_seconds; /* Number of seconds since      */
    /* 01.01.70 00:00:00.           */
    long correction; /* Correction in day calculation*/
    long offset; /* Offset in day calculation.   */

    long year; /* Variable for the year.       */
    long month; /* Variable for the month.      */
    long day; /* Variable for the day.        */
    long hours; /* Variable for the hours.      */
    long minutes; /* Variable for the minutes.    */
    long seconds; /* Variable for the seconds.    */

    int i; /* Running number.              */

    int ret; /* Variable for the return code.*/
    int ret_date; /* Variable for the date return */
    /* code.                        */
    int ret_time; /* Variable for the time return */
    /* code.                        */
    char * date; /* pointer to the date part     */
    char * time; /* pointer to the time part     */

    DIAG(DIAG_HIGH, "convert_datetime_to_epoch() datetime='%s'", datetime);

    /*************************************************************/
    /* Initialization.                                           */
    /*************************************************************/

    ret_date = 0;
    ret_time = 0;
    if (strlen(datetime) == 14) {
        date = ((char *) datetime) + 2;
        time = ((char *) datetime) + 8;
    } else {
        date = ((char *) datetime);
        time = ((char *) datetime) + 6;
    }

    /*************************************************************/
    /* Test if date only consists of digits.                     */
    /*************************************************************/

    for (i = 0; i < 6; i++) {
        if (date[ i ] < '0' || date [ i ] > '9') {
            ret_date = -1;
            DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong date %s, err code '%d'", date, ret_date);
        }
    }
    /*************************************************************/
    /* Test if time only consists of digits.                     */
    /*************************************************************/

    for (i = 0; i < 6; i++) {
        if (time[ i ] < '0' || time [ i ] > '9') {
            ret_time = -2;
            DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong time %s, err code '%d'", time, ret_time);
        }
    }

    /*************************************************************/
    /* Convert date to long.                                     */
    /*************************************************************/

    year = 10 * date[ 0 ] + date [ 1 ] - 11 * '0';

    if (year < 70) {
        year += 2000;
    } else {
        year += 1900;
    }

    month = 10 * date[ 2 ] + date [ 3 ] - 11 * '0';
    day = 10 * date[ 4 ] + date [ 5 ] - 11 * '0';

    /*************************************************************/
    /* Convert time to long.                                     */
    /*************************************************************/

    hours = 10 * time[ 0 ] + time [ 1 ] - 11 * '0';
    minutes = 10 * time[ 2 ] + time [ 3 ] - 11 * '0';
    seconds = 10 * time[ 4 ] + time [ 5 ] - 11 * '0';

    /*************************************************************/
    /* Test if date is valid.                                   */
    /*************************************************************/

    if ((year < 1970) || (year > 2037)) {
        ret_date = -1;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong year '%d'", year);
    }

    if ((month < 1) || (month > 12)) {
        ret_date = -1;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong month '%d'", month);
    }

    if (day < 1) {
        ret_date = -1;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong day '%d'", day);
    }
    if (((month == 1) || (month == 12)) && (day > 31)) {
        /* The other months follow later.                            */
        ret_date = -1;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong month or day '%d', '%d'", month, day);

    }

    /*************************************************************/
    /* Test if time is valid.                                   */
    /*************************************************************/

    if ((hours < 0) ||
            ((hours == 24) && (minutes != 0) && (seconds != 0)) ||
            (hours > 24)) {
        ret_time = -2;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong hours '%d'", hours);
    }

    if ((minutes < 0) || (minutes > 59)) {
        ret_time = -2;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong minutes '%d'", minutes);
    }

    if ((seconds < 0) || (seconds > 59)) {
        ret_time = -2;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong seconds '%d'", seconds);
    }

    /*************************************************************/
    /* Convert date and time.                                    */
    /*************************************************************/

    unix_days = 365 * (year - 1970) + 31 * (month - 1) + day - 1;

    test_days = 365 * (year - 1970) + 31 * month + year / 4
            - (4 * month + 27) / 10 - 492;

    if (month <= 2) {
        year--;
        correction = 0;
    } else {
        correction = (4 * month + 23) / 10;
    }

    /*************************************************************/
    /*  offset = floor(year/4) - floor(3*(floor(year/100)+1)/4)  */
    /*  This is the right offset for all times. But for the      */
    /*  interval from 1901 till 2100 the second summand has the  */
    /*  constant value 15. Therefore we use the simple one.      */
    /*************************************************************/
    offset = year / 4;

    unix_days = unix_days - correction + offset - 492;

    /* 492 = floor ( 1969 / 4 )  is the offset for 01.01.1970.   */
    /* Test if day is valid.                                    */

    if ((month > 1) && (month < 12) && (unix_days >= test_days)) {
        ret_date = -1;
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch() Wrong conversion, code '%d'", ret_date);
    }

    ret = ret_date + ret_time;

    unix_seconds = 86400 * unix_days + 3600 * hours + 60 * minutes + seconds;

    /*************************************************************/
    /* RETURN.                                                   */
    /*************************************************************/

    if (ret == 0) {
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch(): unix_seconds='%ld'", unix_seconds);
        return unix_seconds;
    } else {
        DIAG(DIAG_HIGH, "convert_datetime_to_epoch(): Wrong date, code = '%d'", ret);

        return ret;
    }

} /* END OF FUNCTION unix_time */

void lowercase(char *sPtr) {
    while (*sPtr != '\0') {
        *sPtr = tolower((unsigned char) *sPtr);
        ++sPtr;
    }
}

void uppercase(char *sPtr) {
    while (*sPtr != '\0') {
        *sPtr = toupper((unsigned char) *sPtr);
        ++sPtr;
    }
}

void convert_ipv4(const char* source, char* dest) {

    unsigned int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;

    sscanf(source, "%2x%2x%2x%2x", &ip1, &ip2, &ip3, &ip4);
    sprintf(dest, "%03u%03u%03u%03u", ip1, ip2, ip3, ip4);
    DIAG(DIAG_HIGH, "convert_ipv4() : ip in hex : %s, converted ip in dec : %s", source, dest);
}

void split_time(const char* tt, char* datetime, char* tz) {
    char dt[FSIZE];
    char czTZH[3] = "", czTZMI[3] = "";
    int czs = 0;
    unsigned int czTZS = 0;
    int ttlen = strlen(tt);
    DIAG(DIAG_HIGH, "split_time() : input date : %s, size = %d", tt, ttlen);
    memset(dt, '\0', FSIZE);
    if (ttlen == 12) {
        //sscanf(tt, "%12s%2x%2s%2s", dt, &czTZS, czTZH, czTZMI);
        strncpy(dt, tt, 13);
        snprintf(datetime, 14, "20%s", dt);
        czs = 0;
    } else if (ttlen == 18) {
        sscanf(tt, "%12s%2x%2s%2s", dt, &czTZS, czTZH, czTZMI);
        snprintf(datetime, 15, "20%s", dt);
        czs = atoi(czTZH)*3600 + atoi(czTZMI)*60;
    } else if (ttlen == 14) {

        strncpy(datetime, tt, 15);
        czs = 0;
    }

    snprintf(tz, FSIZE, "%d", czs);
    DIAG(DIAG_HIGH, "split_time() : input date : %s, converted date : %s, time zone %s", tt, datetime, tz);

}

void ReloadLookupTables(void) {
    DIAG(DIAG_LOW, "ReloadLookupTables(): entering the function");

    LS_APN_FILTER = ls_reload_table(LS_APN_FILTER);
    if (LS_APN_FILTER < 0) {
        DIAG(DIAG_HIGH, "LS_APN_FILTER Table reloading failed....");
    }

    LS_CAUSE_FOR_CLOSING = ls_reload_table(LS_CAUSE_FOR_CLOSING);
    if (LS_CAUSE_FOR_CLOSING < 0) {
        DIAG(DIAG_HIGH, "LS_CAUSE_FOR_CLOSING Table reloading failed....");
    }

    LS_FLUSH_TRIGGERS = ls_reload_table(LS_FLUSH_TRIGGERS);
    if (LS_FLUSH_TRIGGERS < 0) {
        DIAG(DIAG_HIGH, "LS_FLUSH_TRIGGERS Table reloading failed....");
    }

    LS_SGSN_IP_RANGE = ls_reload_table(LS_SGSN_IP_RANGE);
    if (LS_SGSN_IP_RANGE < 0) {

        DIAG(DIAG_HIGH, "LS_SGSN_IP_RANGE Table reloading failed....");
    }
    DIAG(DIAG_LOW, "ReloadLookupTables(): returning...");
}

int lookup_LS_APN_FILTER(const char* rec_type, const char* apn) {
    const char* ret_val = NULL;
    int res = -1;
    char keys[FSIZE];

    snprintf(keys, FSIZE, "%s,%s", rec_type, apn);
    DIAG(DIAG_HIGH, "lookup_LS_APN_FILTER(): Searching with keys rec_type,apn  = '%s'", keys);
    ret_val = ls_lookup(LS_APN_FILTER, keys);
    res = get_ls_match_length();
    if (res > 0) {

        DIAG(DIAG_HIGH, "lookup_LS_APN_FILTER(): Match found");
    }
    return res;
}

int lookup_LS_CAUSE_FOR_CLOSING(const char* rec_type, const char* SEARCH_KEY, const char* SEARCH_VALUE, char* CAUSE_FOR_CLOSING) {
    const char* ret_val = NULL;
    int res = -1;
    char keys[FSIZE];

    snprintf(keys, FSIZE, "%s,%s,%s", rec_type, SEARCH_KEY, SEARCH_VALUE);
    DIAG(DIAG_HIGH, "lookup_LS_CAUSE_FOR_CLOSING(): Searching with keys rec_type,search_key,search_value  = '%s'", keys);
    ret_val = ls_lookup(LS_CAUSE_FOR_CLOSING, keys);
    res = get_ls_match_length();
    if (res > 0) {

        if (CAUSE_FOR_CLOSING != NULL) strncpy(CAUSE_FOR_CLOSING, ls_lookup_value(1), FSIZE);
        DIAG(DIAG_HIGH, "lookup_LS_CAUSE_FOR_CLOSING(): Match found, return value : CAUSE_FOR_CLOSING = '%s'", ls_lookup_value(1));
    }
    return res;
}

int lookup_LS_FLUSH_TRIGGERS(const char* rec_type, const char* SEARCH_KEY, const char* SEARCH_VALUE, char* FLUSH_TRIGGER) {
    const char* ret_val = NULL;
    int res = -1;
    char keys[FSIZE];

    snprintf(keys, FSIZE, "%s,%s,%s", rec_type, SEARCH_KEY, SEARCH_VALUE);
    DIAG(DIAG_HIGH, "lookup_LS_FLUSH_TRIGGERS(): Searching with keys rec_type,search_key,search_value  = '%s'", keys);
    ret_val = ls_lookup(LS_FLUSH_TRIGGERS, keys);
    res = get_ls_match_length();
    if (res > 0) {

        if (FLUSH_TRIGGER != NULL) strncpy(FLUSH_TRIGGER, ls_lookup_value(1), FSIZE);
        DIAG(DIAG_HIGH, "lookup_LS_FLUSH_TRIGGERS(): Match found, return value : FLUSH_TRIGGER = '%s'", ls_lookup_value(1));
    }
    return res;
}

int lookup_LS_SGSN_IP_RANGE(const char* SGSN_IP) {
    const char* ret_val = NULL;
    int res = -1;
    //snprintf(keys, FSIZE, "%s,%s", rec_type, apn);
    DIAG(DIAG_HIGH, "lookup_LS_SGSN_IP_RANGE(): Searching with key SGSN_IP  = '%s'", SGSN_IP);
    ret_val = ls_lookup(LS_SGSN_IP_RANGE, SGSN_IP);
    res = get_ls_match_length();
    if (res > 0) {

        DIAG(DIAG_HIGH, "lookup_LS_SGSN_IP_RANGE(): Match found");
    }
    return res;
}
/*
char * left_trim(char *pointerToString, char charToRemove) {
    //char buff[FSIZE];
    char * p = pointerToString;
    int l = strlen(p);

    while (p[l - 1] == charToRemove) p[--l] = 0;
    while (*p && *p == charToRemove) ++p, --l;

    memmove(pointerToString, p, l + 1);
    return pointerToString;
}
 */
/*
char *replace_str(char *s, const char *oldW, const char *newW, char* result) {
    //char result[FSIZE];
    //char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            // Jumping to index after the old word. 
            i += oldWlen - 1;
        }
    }

    // Making new string with enough length 
    //	result = (char *) malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        // compare the substring with the result 
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        } else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}
 */
// function trim_character - removes character from string s and stores result in result parameter
// parameters : s - source string, char_to_trim - character that will be removed
// direction - 'L' - removes from left, 'R' - removes from right, 'A' - removes all, else returns original value

void trim_character(const char *s, char char_to_trim, char direction, char* result) {
    int i = 0, j = 0, l = 0;

    if (direction != 'L' && direction != 'R' && direction != 'A') {
        strncpy(result, s, FSIZE);
        DIAG(DIAG_MED, "trim_character() : Source '%s', wrong direction , returning source", s);
        return;
    }

    if (direction == 'L') {
        while (s[i] == char_to_trim && s[i] != '\0') i++;
        strncpy(result, &s[i], FSIZE);
    } else if (direction == 'A') {
        while (s[i] != '\0') {
            if (s[i] != char_to_trim) {
                result[j] = s[i];
                j++;
            }
            i++;
        }
        result[j] = '\0';
    } else if (direction == 'R') {
        l = strlen(s) - 1;
        while (l >= 0 && s[l] == char_to_trim) l--;
        while (i <= l) {

            result[i] = s[i];
            i++;
        }
        result[i] = '\0';
    }
    DIAG(DIAG_MED, "trim_character() : Source '%s', char to trim '%c', direction '%c', result '%s'", s, char_to_trim, direction, result);
}

char *Hex2Dec(char *value) {
    static char hbuff[100];
    int ival = 0;
    int iValue = 0;

    memset(hbuff, '\0', 100);
    if (strlen(value) > 0) {
        ival = sscanf(value, "%x", &iValue);
        snprintf(hbuff, FSIZE, "%u", iValue);
    }
    DIAG(DIAG_HIGH, "Hex2Dec() : Source '%s', result '%s'", value, hbuff);

    return hbuff;
}

int check_prefix(char *szNumber, char *szNumPart) {
    if (strncmp(szNumber, szNumPart, strlen(szNumPart)) == 0)
        return 1;

    else
        return 0;
}

/*
int search_for_substring(char *s1, char *s2) {
    int M = strlen(s1);
    int N = strlen(s2);

   // A loop to slide pat[] one by one 
    int i;
    for (i = 0; i <= (N - M); i++) {
        int j;

// For current index i, check for pattern match 
        for (j = 0; j < M; j++)
            if (s2[i + j] != s1[j])
                break;

        if (j == M)
            return i;
    }

    return -1;
}
 */
void remove_before_9725(char *szNumber, char* result) {
    char* cptr = NULL;
    cptr = strstr(szNumber, "9725");
    if (cptr == NULL)
        strncpy(result, szNumber, FSIZE);

    else
        strncpy(result, cptr, FSIZE);

    DIAG(DIAG_HIGH, "remove_before_9725() : Checking number : %s,Converted number : %s", szNumber, result);
    //int iPos = search_for_substring("9725", szNumber);
    //if (iPos == -1)
    //    strncpy(result, szNumber, FSIZE - 1);
    //else {
    //    strncpy(result, &szNumber[iPos], FSIZE - 1);
    //}
}

int is_local_number(char *szNumber) {
    //int iPref_Index = 0;
    char buff[FSIZE];
    char prefixes[FSIZE];
    char *pt;

    if (strlen(NP_LocalIMSIPrefixes) == 0) {
        DIAG(DIAG_HIGH, "is_local_number() : No prefixes given in node parameter");
        return 0; // No prefixes given in node parameter
    } else {
        DIAG(DIAG_HIGH, "is_local_number() : Checking number : %s,Prefixes : %s", szNumber, NP_LocalIMSIPrefixes);
    }
    memset(prefixes, '\0', FSIZE);
    strncpy(prefixes, NP_LocalIMSIPrefixes, FSIZE);
    pt = strtok(prefixes, ",");
    while (pt != NULL) {
        int a = atoi(pt);
        memset(buff, '\0', FSIZE);
        snprintf(buff, FSIZE, "%d", a);
        DIAG(DIAG_HIGH, "is_local_number() : Checking for prefix %s", buff);
        if (check_prefix(szNumber, buff) > 0) {
            DIAG(DIAG_HIGH, "is_local_number() : Found prefix : %s", buff);
            return 1;
        }
        pt = strtok(NULL, ",");
    }
    DIAG(DIAG_HIGH, "is_local_number() : No matching prefixes");

    return 0;
}

time_t convert_time_to_epoch(const char* tt, int substract_tz) {
    char buff[FSIZE];
    char date[FSIZE];
    char datetime[FSIZE];
    char tz[FSIZE];
    char czTZH[3] = "", czTZMI[3] = "";
    int i = 0, czs = 0, l = 0;
    unsigned int czTZS = 0;
    time_t t_of_day = 0;


    memset(buff, '\0', FSIZE);
    memset(date, '\0', FSIZE);
    memset(tz, '\0', FSIZE);
    memset(datetime, '\0', FSIZE);

    strncpy(buff, tt, FSIZE);
    l = strlen(buff);
    DIAG(DIAG_HIGH, "convert_time_to_epoch(): input field : %s, size : %d", buff, l);
    if (l == 18) {
        sscanf(buff, "%12s%2x%2s%2s", datetime, &czTZS, czTZH, czTZMI);
    } else if ((l == 14) || (l == 12)) {
        strncpy(datetime, tt, FSIZE);
    } else if (l == 0) {
        DIAG(DIAG_HIGH, "convert_time_to_epoch(): Empty date : %s, returning 0", buff);
        return 0;
    } else {
        DIAG(DIAG_HIGH, "convert_time_to_epoch(): Wrong date : %s, returning 0", buff);
        return 0;
    }
    DIAG(DIAG_HIGH, "convert_time_to_epoch(): datetime : %s", datetime);
    //split_time(tt, date, tz);

    t_of_day = convert_datetime_to_epoch(datetime);

    if (t_of_day < 0) {
        DIAG(DIAG_HIGH, "convert_time_to_epoch() : Error in conversion, code =  %lu, returning 0", t_of_day);
        return 0;
    } else {
        DIAG(DIAG_HIGH, "convert_time_to_epoch() : epoch = %lu", t_of_day);
        if (substract_tz == 1) {
            czs = atoi(czTZH)*3600 + atoi(czTZMI)*60;
            if (i == 45) czs = 0 - czs;
            t_of_day = t_of_day - czs;
            DIAG(DIAG_HIGH, "convert_time_to_epoch() : Subtracted tz seconds %d from epoch = %lu", czs, t_of_day);
        }
        DIAG(DIAG_HIGH, "convert_time_to_epoch() : returned epoch = %lu", t_of_day);

        return t_of_day;
    }
}

int validate_datetime_field(const char *field) {
    DIAG(DIAG_HIGH, "validate_datetime_field(): entered...");

    int year, mon, day, hour, min, sec, tzsign, tzhour, tzmin;
    int leap_year = 0;

    if (strlen(field) == 0) return 0;

    sscanf(field, "%02u%02u%02u%02u%02u%02u%02x%02u%02u", &year, &mon, &day, &hour, &min, &sec, &tzsign, &tzhour, &tzmin);

    year = year + 2000;

    DIAG(DIAG_HIGH, "validate_datetime_field() : year = %d, mpn = %d, day = %d, hour = %d, min = %d, sec = %d", year, mon, day, hour, min, sec);

    if (mon > 12 || mon < 1) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : mon > 12 || mon < 1");
        return 0;
    }
    if (day < 1) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : day < 1");
        return 0;
    }

    if (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0) leap_year = 1;

    DIAG(DIAG_HIGH, "validate_datetime_field() : checking year %d to be leap -> %d", year, leap_year);

    if (((mon == 4) || (mon == 6) || (mon == 9) || (mon == 11)) && (day > 30)) {
        DIAG(DIAG_HIGH, "validate_datetime_field() :((mon == 4) || (mon == 6) || (mon == 9) || (mon == 11)) && (day > 30)");
        return 0;
    } else if (mon == 2) {
        if ((leap_year == 1) && (day > 29)) {
            DIAG(DIAG_HIGH, "validate_datetime_field() : mon == 2 && (leap_year == 1) && (day > 29)");
            return 0;
        } else if ((leap_year == 0) && (day > 28)) {
            DIAG(DIAG_HIGH, "validate_datetime_field() : mon == 2 && (leap_year == 0) && (day > 28)");
            return 0;
        }
    } else if (day > 31) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : day > 31");
        return 0;
    }

    if (hour > 23) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : hour > 23");
        return 0;
    }
    if (min > 59) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : min > 59");
        return 0;
    }
    if (min == 59) {
        if (sec > 60) {
            DIAG(DIAG_HIGH, "validate_datetime_field() : min == 59 && sec > 60");
            return 0;
        }
    } else if (sec > 59) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : sec > 59");
        return 0;
    }
    if (tzhour > 12) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : tzhour > 12");
        return 0;
    }
    if (tzmin > 60) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : tzmin > 60");
        return 0;
    }
    if (tzsign != 43 && tzsign != 45) {
        DIAG(DIAG_HIGH, "validate_datetime_field() : tzsign != 43 && tzsign != 45");
        return 0;
    }
    DIAG(DIAG_HIGH, "validate_datetime_field(): validated");

    return 1;
}

void check_raise_alarm(void) {
    char reason[BSIZE];
    number_of_rejections++;
    if (send_alarm && (number_of_rejections > NP_AlarmThreshold_int) && (NP_AlarmThreshold_int > 0)) {

        memset(reason, '\0', BSIZE);
        snprintf(reason, BSIZE, "Alarm threshold reached for rejected records <%d> ", number_of_rejections);
        DIAG(DIAG_HIGH, "check_raise_alarm(): Alarm threshold reached for rejected records <%d> ", number_of_rejections);
        send_alarm = 0;
        nb_msg_custom(MSG_WARNING, reason);
    }
}

int validate_pgwRecord(void) {
    int b_number;
    char reason[BSIZE];
    char buff[FSIZE];

    DIAG(DIAG_HIGH, "validate_pgwRecord() : enter");
    memset(reason, '\0', BSIZE);
    memset(buff, '\0', FSIZE);

    if (i_field_exists("servedIMSI")) {
        GET_FIELD(buff, "servedIMSI", FSIZE);
        if (buff[0] == '\0') {
            i_reject(INV_PGW_IMSI, "Invalid pGWRecord: servedIMSI missing");
            DIAG(DIAG_HIGH, "validate_pgwRecord() : Invalid pGWRecord: servedIMSI missing");
            check_raise_alarm();
            return 0;
        }
    } else {
        i_reject(INV_PGW_IMSI, "Invalid pGWRecord: servedIMSI missing");
        DIAG(DIAG_HIGH, "validate_pgwRecord() : Invalid pGWRecord: servedIMSI missing");
        check_raise_alarm();
        return 0;
    }
    if (i_field_exists("recordOpeningTime")) {
        GET_FIELD(buff, "recordOpeningTime", FSIZE);
        if (buff[0] == '\0') {
            i_reject(INV_PGW_RECORD_OPEN_TIME, "Invalid pGWRecord: recordOpeningTime missing");
            DIAG(DIAG_HIGH, "Invalid pGWRecord: recordOpeningTime missing");
            return 0;
        } else if (validate_datetime_field(buff) == 0) {
            snprintf(reason, BSIZE, "Invalid pGWRecord: recordOpeningTime %s", buff);
            i_reject(INV_PGW_RECORD_OPEN_TIME, reason);
            DIAG(DIAG_HIGH, "validate_pgwRecord() : %s", reason);
            check_raise_alarm();
            return 0;
        }
    } else {
        i_reject(INV_PGW_RECORD_OPEN_TIME, "Invalid pGWRecord: recordOpeningTime missing");
        DIAG(DIAG_HIGH, "Invalid pGWRecord: recordOpeningTime missing");
        return 0;
    }

    if (i_address(".listOfServiceData")) {
        b_number = 0;
        i_enter();
        while (i_next("PChangeOfServiceCondition")) {
            b_number++;
        }
        i_exit();
        if (b_number == 0) {
            snprintf(reason, BSIZE, "Invalid pGWRecord: Block 'PChangeOfServiceCondition' missing");
            i_reject(INV_PGW_LIST_OF_SERVICE_DATA, reason);
            DIAG(DIAG_HIGH, "process_pgwRecord() : validate_pgwRecord : %s", reason);
            check_raise_alarm();
            return 0;
        }
    } else {
        snprintf(reason, BSIZE, "Invalid pGWRecord: Block 'listOfServiceData' missing");
        i_reject(INV_PGW_LIST_OF_SERVICE_DATA, reason);
        DIAG(DIAG_HIGH, "process_pgwRecord() : validate_pgwRecord : %s", reason);
        check_raise_alarm();
        return 0;
    }
    DIAG(DIAG_HIGH, "validate_pgwRecord() : exit");

    return 1;
}

int validate_sGWRecord(void) {

    char buff[FSIZE];
    char buff2[FSIZE];
    char reason[BSIZE];

    DIAG(DIAG_HIGH, "validate_sGWRecord() : enter");

    memset(reason, '\0', BSIZE);
    memset(buff, '\0', FSIZE);
    memset(buff2, '\0', FSIZE);

    if (i_field_exists("servedIMSI")) {
        GET_FIELD(buff, "servedIMSI", FSIZE);
        if (buff[0] == '\0') {
            i_reject(INV_SGW_IMSI, "Invalid sGWRecord: servedIMSI missing");
            check_raise_alarm();
            DIAG(DIAG_HIGH, "validate_sGWRecord(): Invalid sGWRecord: servedIMSI missing");
            return 0;
        }
    } else {
        i_reject(INV_SGW_IMSI, "Invalid sGWRecord: servedIMSI missing");
        check_raise_alarm();
        DIAG(DIAG_HIGH, "validate_sGWRecord(): Invalid sGWRecord: servedIMSI missing");
        return 0;
    }

    if (i_field_exists(".recordOpeningTime")) {
        GET_FIELD(buff, ".recordOpeningTime", FSIZE);
        if (buff[0] == '\0') {
            i_reject(INV_SGW_RECORD_OPENING_TIME, "Invalid sGWRecord: recordOpeningTime missing");
            DIAG(DIAG_HIGH, "validate_sGWRecord() : Invalid sGWRecord: recordOpeningTime missing");
            return 0;
        } else if (validate_datetime_field(buff) == 0) {
            snprintf(reason, BSIZE, "Invalid sGWRecord: recordOpeningTime %s", buff);
            i_reject(INV_SGW_RECORD_OPENING_TIME, reason);
            DIAG(DIAG_HIGH, "validate_sGWRecord() : %s", reason);
            check_raise_alarm();
            return 0;
        }
    } else {
        i_reject(INV_SGW_RECORD_OPENING_TIME, "Invalid sGWRecord: recordOpeningTime missing");
        DIAG(DIAG_HIGH, "validate_sGWRecord() : Invalid sGWRecord: recordOpeningTime missing");
        return 0;
    }

    memset(buff, '\0', FSIZE);
    if (i_address(".listOfTrafficVolumes")) {
        i_enter();
        while (i_next("ChangeOfCharCondition")) {
            i_enter();
            memset(buff, '\0', FSIZE);
            if (i_field_exists("changeTime"))
                GET_FIELD(buff, "changeTime", FSIZE);
            i_exit();
        }
        i_exit();
    }

    if (buff[0] == '\0') {
        i_reject(INV_SGW_CHANGE_TIME, "Invalid sGWRecord: changeTime missing");
        DIAG(DIAG_HIGH, "validate_sGWRecord() : Invalid sGWRecord: changeTime missing");
        return 0;
    } else if (validate_datetime_field(buff) == 0) {
        snprintf(reason, BSIZE, "Invalid sGWRecord: changeTime %s", buff);
        i_reject(INV_SGW_CHANGE_TIME, reason);
        DIAG(DIAG_HIGH, "validate_sGWRecord() : %s", reason);
        check_raise_alarm();
        return 0;
    }

    GET_FIELD(buff, ".servedIMSI", FSIZE);
    if (is_local_number(buff)) {
        if (strcmp(NP_DiscardLocalSGW, "1") == 0) {
            snprintf(reason, BSIZE, "Local sGWRecord: servedIMSI %s", buff);
            i_reject("LOCAL_SGW", reason);
            check_raise_alarm();
            return 0;
        } else {
            GET_FIELD(buff2, ".accessPointNameNI", FSIZE);
            lowercase(buff2);
            if (lookup_LS_APN_FILTER("SGW", buff2) > 0) {
                snprintf(reason, BSIZE, "Invalid sGWRecord: APN is %s and servedIMSI %s", buff2, buff);
                i_reject(INV_SGW_APN, reason);
                DIAG(DIAG_HIGH, "validate_sGWRecord() : %s", reason);
                check_raise_alarm();
                return 0;
            }
        }
    }
    DIAG(DIAG_HIGH, "validate_sGWRecord() : exit");

    return 1;
}

int validate_sgsnPDPRecord(void) {
    char buff[FSIZE];
    char buff2[FSIZE];
    char reason[BSIZE];

    memset(reason, '\0', BSIZE);
    memset(buff, '\0', FSIZE);
    memset(buff2, '\0', FSIZE);

    DIAG(DIAG_HIGH, "validate_sgsnPDPRecord() : enter");
    if (i_address(".sgsnAddress")) {
        i_enter();
        GET_FIELD(buff, "iPBinV4Address", FSIZE);
        GET_FIELD(buff2, "iPBinV6Address", FSIZE);

        if (buff[0] == '\0' && buff2[0] == '\0') {
            DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: sgsnAddress missing");
            i_reject(INV_SGSN_SGSN_ADDRESS, "Invalid sgsnPDPRecord: sgsnAddress missing");
            check_raise_alarm();
            return 0;
        }
        i_exit();
    } else {
        DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: sgsnAddress missing");
        i_reject(INV_SGSN_SGSN_ADDRESS, "Invalid sgsnPDPRecord: sgsnAddress missing");
        check_raise_alarm();
        return 0;
    }
    if (i_address(".ggsnAddressUsed")) {
        i_enter();
        GET_FIELD(buff, "iPBinV4Address", FSIZE);
        GET_FIELD(buff2, "iPBinV6Address", FSIZE);

        if (buff[0] == '\0' && buff2[0] == '\0') {
            DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: ggsnAddressUsed missing");
            i_reject(INV_SGSN_GGSN_ADDRESS, "Invalid sgsnPDPRecord: ggsnAddressUsed missing");
            check_raise_alarm();
            return 0;
        }
        i_exit();
    } else {
        DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: ggsnAddressUsed missing");
        i_reject(INV_SGSN_GGSN_ADDRESS, "Invalid sgsnPDPRecord: ggsnAddressUsed missing");
        check_raise_alarm();
        return 0;
    }

    if (i_field_exists(".servedIMSI")) {
        GET_FIELD(buff, ".servedIMSI", FSIZE);
        if (buff[0] == '\0') {
            DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: servedIMSI missing");
            i_reject(INV_SGSN_IMSI, "Invalid sgsnPDPRecord: servedIMSI missing");
            check_raise_alarm();
            return 0;
        }
    } else {
        DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: servedIMSI missing");
        i_reject(INV_SGSN_IMSI, "Invalid sgsnPDPRecord: servedIMSI missing");
        check_raise_alarm();
        return 0;
    }

    if (i_field_exists(".recordOpeningTime")) {
        GET_FIELD(buff, ".recordOpeningTime", FSIZE);
        if (buff[0] == '\0') {
            i_reject(INV_SGSN_RECORD_OPENING_TIME, "Invalid sgsnPDPRecord: recordOpeningTime missing");
            DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: recordOpeningTime missing");
            check_raise_alarm();
            return 0;
        } else if (validate_datetime_field(buff) == 0) {
            DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: recordOpeningTime %s", buff);
            snprintf(reason, BSIZE, "Invalid sgsnPDPRecord: recordOpeningTime %s", buff);
            i_reject(INV_SGSN_RECORD_OPENING_TIME, reason);
            check_raise_alarm();
            return 0;
        }
    } else {
        i_reject(INV_SGSN_RECORD_OPENING_TIME, "Invalid sgsnPDPRecord: recordOpeningTime missing");
        DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord : recordOpeningTime missing");
        check_raise_alarm();
        return 0;
    }

    if (i_field_exists(".listOfTrafficVolumes.ChangeOfCharConditionS.changeTime")) {
        GET_FIELD(buff, ".listOfTrafficVolumes.ChangeOfCharConditionS.changeTime", FSIZE);
        if (buff[0] == '\0') {
            DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: changeTime missing.");
            i_reject(INV_SGSN_CHANGE_TIME, "Invalid sgsnPDPRecord: changeTime missing");
            check_raise_alarm();
            return 0;
        } else if (validate_datetime_field(buff) == 0) {
            DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: changeTime <%s>", buff);
            snprintf(reason, BSIZE, "Invalid sgsnPDPRecord: changeTime %s", buff);
            i_reject(INV_SGSN_CHANGE_TIME, reason);
            check_raise_alarm();
            return 0;
        }
    } else {
        DIAG(DIAG_HIGH, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: changeTime missing.");
        i_reject(INV_SGSN_CHANGE_TIME, "Invalid sgsnPDPRecord: changeTime missing");
        check_raise_alarm();
        return 0;
    }

    if (i_field_exists(".chargingID")) {
        GET_FIELD(buff, ".chargingID", FSIZE);
        if (buff[0] == '\0') {
            DIAG(DIAG_LOW, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: chargingID missing");
            i_reject(INV_SGSN_CHARGING_ID, "Invalid sgsnPDPRecord: chargingID missing");
            check_raise_alarm();
            return 0;
        }
    } else {
        DIAG(DIAG_LOW, "validate_sgsnPDPRecord(): REJECTED - Invalid sgsnPDPRecord: chargingID missing");
        i_reject(INV_SGSN_CHARGING_ID, "Invalid sgsnPDPRecord: chargingID missing");
        check_raise_alarm();
        return 0;
    }
    DIAG(DIAG_HIGH, "validate_sgsnPDPRecord() : exit");

    return 1;
}

void process_pgwRecord(void) {
    char buff[FSIZE];
    char buff2[FSIZE];
    Pcontainerfields *head, *el = NULL;
    //char current_address[FSIZE], restore_address[FSIZE];
    char reason[BSIZE];
    //char buff3[FSIZE * 2 + 1];
    char OUTPUT1[FSIZE], OUTPUT2[FSIZE], OUTPUT3[FSIZE], lastc;

    int container_number = 0, i = 0, reject_flag = 0, pdpPDNType_exists = 0, served_pdp_type = 0;
    unsigned int flags = 0;
    int volume_element_nr_5g = 0; 
    int64_t total_ul_volume_5g = INT64_C(0); 
    int64_t total_dl_volume_5g = INT64_C(0);


    //char* buff_ptr;
    DIAG(DIAG_HIGH, "process_pgwRecord(): enter");

    if (i_address(".") == NB_OK) {
        memset(OUTPUT1, '\0', FSIZE);
        memset(OUTPUT2, '\0', FSIZE);
        memset(OUTPUT3, '\0', FSIZE);


        memset(buff, '\0', FSIZE);
        GET_FIELD(buff, ".chargingID", FSIZE);
        lastc = buff[strlen(buff) - 1];
        switch (lastc) {
            case '0':
            case '1': strncpy(OUTPUT3, "1", 1);
                break;
            case '2':
            case '3': strncpy(OUTPUT3, "2", 1);
                break;
            case '4':
            case '5': strncpy(OUTPUT3, "3", 1);
                break;
            case '6':
            case '7': strncpy(OUTPUT3, "4", 1);
                break;
            case '8':
            case '9': strncpy(OUTPUT3, "5", 1);
        }

        strncpy(EL_RECORD_TYPE, "19", FSIZE);
        GET_FIELD(EL_SERVED_IMEI, ".servedIMEISV", FSIZE);

        //EL_PLMNID
        memset(buff, '\0', FSIZE);
        memset(buff2, '\0', FSIZE);
        GET_FIELD(buff, ".servingNodePLMNIdentifier", FSIZE);
        trim_character(buff, 'F', 'A', buff2);
        strncpy(EL_PLMNID, buff2, FSIZE);
        DIAG(DIAG_HIGH, "process_pgwRecord() : EL_PLMNID <%s>", EL_PLMNID);

        //EL_SERVED_PDP_TYPE_ORG and EL_SERVED_PDP_TYPE_VALUE
        memset(buff2, '\0', FSIZE);
        if (i_field_exists(".pdpPDNType")) {
            GET_FIELD(EL_pdpPDNType, ".pdpPDNType", FSIZE);
            pdpPDNType_exists = 1;
        } else {
            if ((i_is_block(".servedPDPPDNAddress") == 1) && (i_is_block("servedPDPPDNAddressExt") == 1)) {
                served_pdp_type = 3;
            } else if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV4Address")) {
                served_pdp_type = 1;
            } else if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV6Address")) {
                served_pdp_type = 2;
            }
        }

        //EL_NAPI_FOR_MSISDN and EL_SERVED_MSISDN
        memset(buff2, '\0', FSIZE);
        GET_FIELD(buff, ".servedMSISDN", FSIZE);
        strncpy(buff2, buff, FSIZE);
        char c = buff[0];
        buff[0] = buff[1];
        buff[1] = c;
        buff[2] = '\0';
        strncpy(EL_NAPI_FOR_MSISDN, buff, FSIZE);
        DIAG(DIAG_HIGH, "process_pgwRecord() : EL_NAPI_FOR_MSISDN <%s>", EL_NAPI_FOR_MSISDN);

        GET_FIELD(buff, ".servedIMSI", FSIZE);

        if (is_local_number(buff) == 1) {
            memset(buff, '\0', FSIZE);
            remove_before_9725(buff2, buff);
            strncpy(EL_SERVED_MSISDN, buff, FSIZE);
        } else {
            strncpy(EL_SERVED_MSISDN, buff2, FSIZE);
        }
        DIAG(DIAG_HIGH, "process_pgwRecord() : EL_SERVED_MSISDN <%s>", EL_SERVED_MSISDN);

        //EL_NODE_ID

        GET_FIELD(EL_NODE_ID, ".nodeID", FSIZE);
        uppercase(EL_NODE_ID);
        DIAG(DIAG_HIGH, "process_pgwRecord() : EL_NODE_ID <%s>", EL_NODE_ID);

        // EL_DYNAMIC_ADDRESS_FLAG -------------------------------------
        if (i_field_exists(".dynamicAddressFlag"))
            strncpy(EL_DYNAMIC_ADDRESS_FLAG, "1", FSIZE);
        else
            strncpy(EL_DYNAMIC_ADDRESS_FLAG, "0", FSIZE);

        // EL_ACCESS_POINT_NAME
        GET_FIELD(EL_ACCESS_POINT_NAME, ".accessPointNameNI", FSIZE);

        // EL_FIRST_SEQUENCE_NUMBER, EL_LAST_SEQUENCE_NUMBER, EL_REC_SEQ_NBR
        memset(buff, '\0', FSIZE);
        if (i_field_exists(".recordSequenceNumber")) {
            GET_FIELD(buff, ".recordSequenceNumber", FSIZE);
            strncpy(EL_FIRST_SEQUENCE_NUMBER, buff, FSIZE);
            strncpy(EL_LAST_SEQUENCE_NUMBER, buff, FSIZE);
            strncpy(EL_REC_SEQ_NBR, buff, FSIZE);
        } else {
            strncpy(EL_FIRST_SEQUENCE_NUMBER, "0", FSIZE);
            strncpy(EL_LAST_SEQUENCE_NUMBER, "0", FSIZE);
            strncpy(EL_REC_SEQ_NBR, "0", FSIZE);
        }

        //EL_CHARGING_CHARACTERISTICS
        GET_FIELD(buff, ".chargingCharacteristics", FSIZE);
        buff[2] = '\0';
        strncpy(EL_CHARGING_CHARACTERISTICS, Hex2Dec(buff), FSIZE);
        DIAG(DIAG_HIGH, "process_pgwRecord() : EL_CHARGING_CHARACTERISTICS <%s>", EL_CHARGING_CHARACTERISTICS);

        //EL_CAUSE_FOR_CLOSING
        GET_FIELD(buff, ".causeForRecClosing", FSIZE);
        if (lookup_LS_CAUSE_FOR_CLOSING("PGW", "causeForRecClosing", buff, EL_CAUSE_FOR_CLOSING) <= 0)
            strncpy(EL_CAUSE_FOR_CLOSING, "0", FSIZE);
        DIAG(DIAG_HIGH, "process_pgwRecord() : EL_CHARGING_CHARACTERISTICS <%s>", EL_CAUSE_FOR_CLOSING);

        // EL_USER_LOCATION -------------------------------------
        memset(buff2, '\0', FSIZE);
        GET_FIELD(buff, ".userLocationInformation", FSIZE);
        strncpy(buff2, buff, FSIZE); // make a copy of value
        if (strlen(buff) == 16) {
            strncpy(EL_USER_LOCATION, buff, FSIZE);
            DIAG(DIAG_HIGH, "process_pgwRecord() : strlen(userLocationInformation <%s>) = 16 ,EL_USER_LOCATION <%s>", buff, EL_USER_LOCATION);
        } else if (strlen(buff) > 16) {
            //buff[2] = '\0'; // only first two chars
            unsigned int iFlags;
            int offset = 0;
            sscanf(buff, "%2x", &iFlags);
            DIAG(DIAG_HIGH, "process_pgwRecord() : Flags <%s>, flags in hex <%04x>", buff, iFlags);
            if (iFlags & 0x1) // CGI
            {
                strncpy(buff, &buff2[11 + 2], 4); // chars 11 to 14
                strncpy(EL_USER_LOCATION, Hex2Dec(buff), FSIZE); // hex to dec
                offset = offset + 14;
                DIAG(DIAG_HIGH, "process_pgwRecord() : Flags <%04x> - > CGI, chars 11 to 14 <%s>, current offset %d", iFlags, buff, offset);
            }
            if (iFlags & (0x1 << 1)) // SAI
            {
                offset = offset + 14;
                DIAG(DIAG_HIGH, "process_pgwRecord() : Flags <%04x> - > SAI, skip next 14 + 2 chars bit flags starting from 0 (zero) <%s>, current offset %d", iFlags, buff, offset);
            }
            if (iFlags & (0x1 << 2)) // RAI
            {
                offset = offset + 14;
                DIAG(DIAG_HIGH, "process_pgwRecord() : Flags <%04x> - > RAI, skip next 14 + 2 chars bit flags starting from 0 (zero) <%s>, current offset %d", iFlags, buff, offset);
            }
            if (iFlags & (0x1 << 3)) // TAI
            {
                offset = offset + 10;
                DIAG(DIAG_HIGH, "process_pgwRecord() : Flags <%04x> - > TAI, skip next 10 + 2 chars bit flags starting from 0 (zero) <%s>, current offset %d", iFlags, buff, offset);
            }
            if (iFlags & (0x1 << 4)) // ECGI
            {
                int iLen = strlen(buff2);
                DIAG(DIAG_HIGH, "process_pgwRecord() : strlen(userLocationInformation <%s>) = %d, current offset %d", buff2, iLen, offset);
                snprintf(buff, FSIZE, "01%8.8s%2.2s%4.4s", &buff2[offset + 2], &buff2[iLen - 2], &buff2[offset + 8 + 2]);
                strncpy(EL_USER_LOCATION, buff, FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Flags <%04x> - > ECGI, concatenate 01 + first 8 characters of ECGI + last two characters of ECGI + characters 9-12 of ECGI <%s>", iFlags, buff);
            }
            DIAG(DIAG_HIGH, "process_pgwRecord() : strlen(userLocationInformation <%s>) > 16 , flags <%02x>, EL_USER_LOCATION <%s>", buff2, iFlags, EL_USER_LOCATION);
        }

        //EL_RECORD_OPENING_TIME and EL_OPENING_TIMEZONE
        GET_FIELD(buff, ".recordOpeningTime", FSIZE);
        split_time(buff, EL_RECORD_OPENING_TIME, EL_OPENING_TIMEZONE);
        DIAG(DIAG_HIGH, "process_pgwRecord() : EL_RECORD_OPENING_TIME <%s>, EL_OPENING_TIMEZONE <%s>", EL_RECORD_OPENING_TIME, EL_OPENING_TIMEZONE);

        //EL_GGSN_ADDRESS
        GET_FIELD(EL_GGSN_ADDRESS, ".pGWAddress.iPBinV4Address", FSIZE);

        //EL_SGSN_ADDRESS
        GET_FIELD(EL_SGSN_ADDRESS, ".servingNodeAddress.iPBinV4Address", FSIZE);

        //EL_SERVED_PDP_ADDRESS
        if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV4Address")) {
            GET_FIELD(EL_SERVED_PDP_ADDRESS, ".servedPDPPDNAddress.iPAddress.iPBinV4Address", FSIZE);
        } else if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV6Address")) {
            GET_FIELD(EL_SERVED_PDP_ADDRESS, ".servedPDPPDNAddress.iPAddress.iPBinV6Address", FSIZE);
        }

        //EL_SERVED_PDP_ADDR_EXT 
        if (i_field_exists(".servedPDPPDNAddressExt.iPAddress.iPBinV4Address")) {
            GET_FIELD(EL_SERVED_PDP_ADDR_EXT, ".servedPDPPDNAddressExt.iPAddress.iPBinV4Address", FSIZE);
        } else if (i_field_exists(".servedPDPPDNAddressExt.iPAddress.iPBinV6Address")) {
            GET_FIELD(EL_SERVED_PDP_ADDR_EXT, ".servedPDPPDNAddressExt.iPAddress.iPBinV6Address", FSIZE);
        }

        //EL_DIAGNOSTICS_VALUE
        if (i_field_exists(".diagnostics.gsm0408Cause")) {
            GET_FIELD(EL_DIAGNOSTICS_VALUE, ".diagnostics.gsm0408Cause", FSIZE);
        }

        // setting output to local or roaming
        memset(buff2, '\0', FSIZE);
        GET_FIELD(buff, ".servingNodeAddress.iPBinV4Address", FSIZE);
        convert_ipv4(buff, buff2);
        if (lookup_LS_SGSN_IP_RANGE(buff2))
            strncpy(OUTPUT2, "N", FSIZE);
        else
            strncpy(OUTPUT2, "R", FSIZE);

	// Processing RANSecondaryRATUsageReport elements from listOfRANSecondaryRATUsageReports
        volume_element_nr_5g = 0; 
        total_ul_volume_5g = INT64_C(0); 
        total_dl_volume_5g = INT64_C(0); 
		
        if (i_address(".listOfRANSecondaryRATUsageReports")) {
            strncpy(EL_5G_FLAG, "1", FSIZE);
			
            i_enter(); // enter listOfRANSecondaryRATUsageReports
            while (i_next("RANSecondaryRATUsageReport") == NB_OK) {
                i_enter();
                volume_element_nr_5g++; 
				
                if (i_field_exists("dataVolumeUplink")) {
                    GET_FIELD(buff, "dataVolumeUplink", FSIZE);
                    DIAG(DIAG_HIGH, "process RANSecondaryRATUsageReport %d : read dataVolumeUplink : %s", volume_element_nr_5g, buff);
                    // add to total 5g uplink volume
                    total_ul_volume_5g = total_ul_volume_5g + strtoull (buff,0,0);
                }
				
                if (i_field_exists("dataVolumeDownlink")) {
                    GET_FIELD(buff2, "dataVolumeDownlink", FSIZE);
                    DIAG(DIAG_HIGH, "process RANSecondaryRATUsageReport %d : read dataVolumeDownlink : %s", volume_element_nr_5g, buff2);
                    // add to total 5g downlink volume
                    total_dl_volume_5g = total_dl_volume_5g + strtoull (buff2,0,0); 
                }
				
                i_exit(); // i_exit listOfRANSecondaryRATUsageReports
					
            } // end of while i_next RANSecondaryRATUsageReport
			
            snprintf(EL_5G_VOLUME_UPLINK, FSIZE, "%" PRId64, total_ul_volume_5g);
            snprintf(EL_5G_VOLUME_DOWNLINK, FSIZE, "%" PRId64, total_dl_volume_5g);
			
            i_exit(); // i_exit listOfRANSecondaryRATUsageReports
        }
        else {
            strncpy(EL_5G_FLAG, "0", FSIZE);
            strncpy(EL_5G_VOLUME_UPLINK, "0", FSIZE);
            strncpy(EL_5G_VOLUME_DOWNLINK, "0", FSIZE);
        }

        DIAG(DIAG_HIGH, "process_pgwRecord() : NORMAL/ROUMING RECORD OUTPUT : %s", OUTPUT2);

        // Processing PChangeOfServiceCondition containers from listOfServiceData - loading
        container_number = 0;

        if (i_address(".listOfServiceData")) {
            i_enter();
            head = NULL;
            el = NULL;
            while (i_next("PChangeOfServiceCondition") == NB_OK) {
                i_enter();
                container_number++;
                el = (Pcontainerfields *) malloc(sizeof (Pcontainerfields));
                if (el != NULL) {
                    init_pgwcontainerfields(el);
                    el->container_number = container_number;
                    if (i_field_exists("pSFurnishChargingInformation.pSFreeFormatData")) {
                        GET_FIELD(el->pSFreeFormatData, "pSFurnishChargingInformation.pSFreeFormatData", FSIZE);
                    }
                    GET_FIELD(el->timeUsage, "timeUsage", FSIZE);
                    GET_FIELD(el->serviceConditionChange, "serviceConditionChange", FSIZE);
                    GET_FIELD(el->timeOfFirstUsage, "timeOfFirstUsage", FSIZE);
                    GET_FIELD(el->timeOfLastUsage, "timeOfLastUsage", FSIZE);
                    GET_FIELD(el->chargingRuleBaseName, "chargingRuleBaseName", FSIZE);
                    GET_FIELD(el->datavolumeFBCUplink, "datavolumeFBCUplink", FSIZE);
                    GET_FIELD(el->datavolumeFBCDownlink, "datavolumeFBCDownlink", FSIZE);

                    if (i_field_exists("timeOfReport")) {
                        GET_FIELD(el->timeOfReport, "timeOfReport", FSIZE);
                    } else {
                        el->missing_timeOfReport = 1;
                        el->reject_flag = 1;
                        DIAG(DIAG_HIGH, "process_pgwRecord() : Missing timeOfReport");
                    }
                    if (i_field_exists("ratingGroup")) {
                        GET_FIELD(el->ratingGroup, "ratingGroup", FSIZE);
                    } else {
                        el->missing_ratingGroup = 1;
                        el->reject_flag = 1;
                        DIAG(DIAG_HIGH, "process_pgwRecord() : Missing ratingGroup");
                    }
                    if (i_field_exists("resultCode")) {
                        GET_FIELD(el->resultCode, "resultCode", FSIZE);
                    } else {
                        el->missing_resultCode = 1;
                        DIAG(DIAG_HIGH, "process_pgwRecord() : Missing resultCode");
                    }
                    if (i_field_exists("failureHandlingContinue")) {
                        GET_FIELD(el->failureHandlingContinue, "failureHandlingContinue", FSIZE);
                    } else {
                        el->missing_failureHandlingContinue = 1;
                        DIAG(DIAG_HIGH, "process_pgwRecord() : Missing failureHandlingContinue");
                    }
                    el->next = NULL;

                    add_pgwcontainer_element(&head, el);
                } //if el != NULL

                i_exit();
            } //while i_next
            i_exit();
        } //if i_address

        // Processing PChangeOfServiceCondition containers from listOfServiceData - loading

        el = head;
        while (el != NULL) {
            reject_flag = 0;
            init_pgw_container_vars();
            DIAG(DIAG_HIGH, "process_pgwRecord() : Processing container %d", el->container_number);
            // EL_CONTAINER_NUMBER
            snprintf(EL_CONTAINER_NUMBER, FSIZE, "%d", el->container_number);
            DIAG(DIAG_HIGH, "process_pgwRecord() : EL_CONTAINER_NUMBER <%s>", EL_CONTAINER_NUMBER);

            memset(reason, '\0', BSIZE);
            DIAG(DIAG_HIGH, "timeOfReport = %s", el->timeOfReport);
            if (el->timeOfReport[0] == '\0') {
                snprintf(reason, BSIZE, "Invalid pGWRecord: listOfServiceData.PChangeOfServiceCondition[%d].timeOfReport missing", el->container_number);
                check_raise_alarm();
                reject_flag = 1;
                DIAG(DIAG_HIGH, "process_pgwRecord() :  <%s>", reason);
            } else if (validate_datetime_field(el->timeOfReport) == 0) {
                snprintf(reason, BSIZE, "Invalid pGWRecord: listOfServiceData.PChangeOfServiceCondition[%d].timeOfReport %s", el->container_number, el->timeOfReport);
                check_raise_alarm();
                reject_flag = 1;
                DIAG(DIAG_HIGH, "process_pgwRecord() :  <%s>", reason);
            } else {
                split_time(el->timeOfReport, EL_RECORD_CLOSING_TIME, EL_CLOSING_TIMEZONE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : EL_RECORD_CLOSING_TIME <%s>, EL_CLOSING_TIMEZONE <%s>", EL_RECORD_CLOSING_TIME, EL_CLOSING_TIMEZONE);
            }

            //EL_FAILURE_HANDLING
            if (strcmp(el->failureHandlingContinue, "1") == 0) {
                strncpy(EL_FAILURE_HANDLING, "1", FSIZE);
            } else {
                strncpy(EL_FAILURE_HANDLING, "0", FSIZE);
            }
            DIAG(DIAG_HIGH, "process_pgwRecord() : failureHandlingContinue <%s>, EL_FAILURE_HANDLING <%s>", el->failureHandlingContinue, EL_FAILURE_HANDLING);

            //EL_RATING_GROUP
            DIAG(DIAG_HIGH, "ratingGroup = %s", el->ratingGroup);
            if (el->ratingGroup[0] == '\0') {
                snprintf(reason, BSIZE, "Invalid pGWRecord: listOfServiceData.PChangeOfServiceCondition[%d].ratingGroup missing", el->container_number);
                check_raise_alarm();
                reject_flag = 2;
                DIAG(DIAG_HIGH, "process_pgwRecord() :  <%s>", reason);
            } else {
                strncpy(EL_RATING_GROUP, el->ratingGroup, FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : EL_RATING_GROUP <%s>", EL_RATING_GROUP);
            }

            //EL_FCI
            memset(EL_FCI, '\0', FSIZE);
            memset(buff, '\0', FSIZE);
            snprintf(EL_FCI, FSIZE, "%s;%s", EL_SERVED_PDP_ADDR_EXT, el->pSFreeFormatData);
            DIAG(DIAG_HIGH, "process_pgwRecord() : EL_SERVED_PDP_ADDR_EXT <%s>,pSFreeFormatData <%s> - EL_FCI <%s> ", EL_SERVED_PDP_ADDR_EXT, el->pSFreeFormatData, EL_FCI);

            strncpy(EL_DURATION, el->timeUsage, FSIZE);
            strncpy(EL_SERVICE_ID, el->chargingRuleBaseName, FSIZE);
            strncpy(EL_DATA_VOLUME_UPLINK, el->datavolumeFBCUplink, FSIZE);
            strncpy(EL_DATA_VOLUME_DOWNLINK, el->datavolumeFBCDownlink, FSIZE);

            // EL_FIRST_ACTIVITY
            DIAG(DIAG_HIGH, "timeOfFirstUsage = %s", el->timeOfFirstUsage);
            snprintf(EL_FIRST_ACTIVITY, FSIZE, "%lu", convert_time_to_epoch(el->timeOfFirstUsage, 1));

            // EL_LAST_ACTIVITY
            DIAG(DIAG_HIGH, "timeOfLastUsage = %s", el->timeOfLastUsage);
            snprintf(EL_LAST_ACTIVITY, FSIZE, "%lu", convert_time_to_epoch(el->timeOfLastUsage, 1));

            //EL_NO_OCS_CREDIT_CONTROL
            memset(buff, '\0', FSIZE);
            memset(buff2, '\0', FSIZE);
            memset(EL_NO_OCS_CREDIT_CONTROL, '\0', FSIZE);

            if (strcmp(el->failureHandlingContinue, "1") == 0) {
                strncpy(EL_NO_OCS_CREDIT_CONTROL, "1", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : resultCode <%s> ,failureHandlingContinue <%s>, EL_NO_OCS_CREDIT_CONTROL = %s", el->resultCode, el->failureHandlingContinue, EL_NO_OCS_CREDIT_CONTROL);
            } else if (strcmp(el->resultCode, "2001") == 0) {
                strncpy(EL_NO_OCS_CREDIT_CONTROL, "2", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : resultCode <%s> ,failureHandlingContinue <%s>, EL_NO_OCS_CREDIT_CONTROL = %s", el->resultCode, el->failureHandlingContinue, EL_NO_OCS_CREDIT_CONTROL);
            } else if (strcmp(el->resultCode, "4012") == 0) {
                strncpy(EL_NO_OCS_CREDIT_CONTROL, "4", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : resultCode <%s> ,failureHandlingContinue <%s>, EL_NO_OCS_CREDIT_CONTROL = %s", el->resultCode, el->failureHandlingContinue, EL_NO_OCS_CREDIT_CONTROL);
            } else if (strcmp(el->resultCode, "5003") == 0) {
                strncpy(EL_NO_OCS_CREDIT_CONTROL, "5", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : resultCode <%s> ,failureHandlingContinue <%s>, EL_NO_OCS_CREDIT_CONTROL = %s", el->resultCode, el->failureHandlingContinue, EL_NO_OCS_CREDIT_CONTROL);
            } else if ((el->missing_resultCode == 1) && (el->missing_failureHandlingContinue == 1)) {
                DIAG(DIAG_HIGH, "process_pgwRecord() : missing fields 'resultCode' and 'failureHandlingContinue', flags, EL_NO_OCS_CREDIT_CONTROL = 0");
                strncpy(EL_NO_OCS_CREDIT_CONTROL, "0", FSIZE);
            }

            // EL_CHANGE_CONDITION
            memset(buff2, '\0', FSIZE);
            memset(EL_CHANGE_CONDITION, '\0', FSIZE);

            strncpy(buff2, &el->serviceConditionChange[2], 8);
            sscanf(buff2, "%8x", &flags);
            DIAG(DIAG_HIGH, "process_pgwRecord() : serviceConditionChange <%s>, flags <%08x>", el->serviceConditionChange, flags);
            if (flags & (0x1 << (31 - 4))) {// pDPContextChange
                strncpy(EL_CHANGE_CONDITION, "0", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 4 -> pDPContextChange");
            } else if (flags & (0x1 << (31 - 0))) { // qoSChange
                strncpy(EL_CHANGE_CONDITION, "10", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 10 -> qoSChange");
            } else if (flags & (0x1 << (31 - 26))) { // volumeLimit
                strncpy(EL_CHANGE_CONDITION, "1", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 26 -> volumeLimit");
            } else if (flags & (0x1 << (31 - 25))) {// timeLimit
                strncpy(EL_CHANGE_CONDITION, "2", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 25 -> timeLimit");
            } else if (flags & (0x1 << (31 - 5))) {// rATChange
                strncpy(EL_CHANGE_CONDITION, "22", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 5 -> rATChange");
            } else if (flags & (0x1 << (31 - 1))) {// sGSNChange
                strncpy(EL_CHANGE_CONDITION, "3", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 1 -> sGSNChange");
            } else if (flags & (0x1 << (31 - 24))) { // recordClosure
                strncpy(EL_CHANGE_CONDITION, "21", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 24 -> recordClosure");
            } else if (flags & (0x1 << (31 - 3))) { // tariffTimeSwitch
                strncpy(EL_CHANGE_CONDITION, "11", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 3 -> tariffTimeSwitch");
            } else if (flags & (0x1 << (31 - 6))) { // serviceIdledOut
                strncpy(EL_CHANGE_CONDITION, "31", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : Bit 6 -> serviceIdledOut");
            }
            DIAG(DIAG_HIGH, "process_pgwRecord() : serviceConditionChange <%s>, flags <%08x>,  EL_CHANGE_CONDITION <%s>", el->serviceConditionChange, flags, EL_CHANGE_CONDITION);

            // EL_SERVED_PDP_TYPE_ORG, EL_SERVED_PDP_TYPE_VALUE, FULL OUTPUT NAME 
            memset(buff, '\0', FSIZE);
            memset(buff2, '\0', FSIZE);
            char *last_two = NULL;
            if (pdpPDNType_exists == 1) {
                strncpy(buff, EL_pdpPDNType, FSIZE);
                strncpy(buff2, buff, FSIZE);
                buff[2] = '\0';
                int len = strlen(buff2);
                last_two = &buff2[len - 2];
            }

            memset(OUTPUT1, '\0', FSIZE);
            if (strcmp(EL_RATING_GROUP, "0") == 0 && strcmp(NP_ProcessRG0Records, "1") == 0) {
                snprintf(OUTPUT1, FSIZE, "%s%s", "G_OUT_", OUTPUT2);
                if (pdpPDNType_exists == 1) {
                    strncpy(EL_SERVED_PDP_TYPE_ORG, &buff[1], 1);
                    strncpy(EL_SERVED_PDP_TYPE_VALUE, last_two, FSIZE);
                } else {
                    strncpy(EL_SERVED_PDP_TYPE_ORG, "1", 1);
                    switch (served_pdp_type) {
                        case 1: strncpy(EL_SERVED_PDP_TYPE_VALUE, "21", FSIZE);
                            break;
                        case 2: strncpy(EL_SERVED_PDP_TYPE_VALUE, "57", FSIZE);
                            break;
                        case 3: strncpy(EL_SERVED_PDP_TYPE_VALUE, "8D", FSIZE);
                            break;
                    }
                    //strncpy(EL_SERVED_PDP_TYPE_VALUE, "1", FSIZE);
                }
            } else if (atoi(EL_RATING_GROUP) > 0) {
                if (strcmp(OUTPUT2, "N") == 0)
                    snprintf(OUTPUT1, FSIZE, "%s%s%s", "SA_OUT_", OUTPUT2, OUTPUT3);
                else
                    snprintf(OUTPUT1, FSIZE, "%s%s", "SA_OUT_", OUTPUT2);
                if (pdpPDNType_exists == 1) {
                    strncpy(EL_SERVED_PDP_TYPE_ORG, Hex2Dec(buff), FSIZE);
                    strncpy(EL_SERVED_PDP_TYPE_VALUE, Hex2Dec(last_two), FSIZE);
                } else {
                    strncpy(EL_SERVED_PDP_TYPE_ORG, "1", 1);
                    switch (served_pdp_type) {
                        case 1: strncpy(EL_SERVED_PDP_TYPE_VALUE, "33", FSIZE);
                            break;
                        case 2: strncpy(EL_SERVED_PDP_TYPE_VALUE, "87", FSIZE);
                            break;
                        case 3: strncpy(EL_SERVED_PDP_TYPE_VALUE, "141", FSIZE);
                            break;
                    }
                }
            } else {
                memset(OUTPUT1, '\0', FSIZE); // container will be filtered
                DIAG(DIAG_HIGH, "EL_RATING_GROUP <%s>, NP_ProcessRG0Records <%s> -> will be filtered", EL_RATING_GROUP, NP_ProcessRG0Records);
            }
            DIAG(DIAG_HIGH, "process_pgwRecord() : OUTPUT <%s>", OUTPUT1);
            DIAG(DIAG_HIGH, "process_pgwRecord() : EL_RATING_GROUP = %s, NP_ProcessRG0Records = %s -> RECORD OUTPUT : %s", EL_RATING_GROUP, NP_ProcessRG0Records, OUTPUT1);
            DIAG(DIAG_HIGH, "process_pgwRecord() : pdpPDNType_exists <%d>, served_pdp_type <%d>, pdpPDNType <%s>, EL_SERVED_PDP_TYPE_ORG <%s>", pdpPDNType_exists,served_pdp_type, EL_pdpPDNType, EL_SERVED_PDP_TYPE_ORG);
            DIAG(DIAG_HIGH, "process_pgwRecord() : pdpPDNType_exists <%d>, served_pdp_type <%d>, pdpPDNType <%s>, EL_SERVED_PDP_TYPE_VALUE <%s>", pdpPDNType_exists, served_pdp_type, EL_pdpPDNType, EL_SERVED_PDP_TYPE_VALUE);

            // Aggregation fields
            init_aggr_fields();

            memset(buff, '\0', FSIZE);
            //strncpy(buff, i_get)
            snprintf(EL_START_TIME, FSIZE, "%lu", convert_time_to_epoch(EL_RECORD_OPENING_TIME, 0));
            snprintf(EL_END_TIME, FSIZE, "%lu", convert_time_to_epoch(EL_RECORD_CLOSING_TIME, 0));

            if (atoi(EL_DURATION) > atoi(NP_DurationNoAggregation)) {
                strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
                DIAG(DIAG_HIGH, "process_pgwRecord() : EL_DURATION <%s> > NP_DurationNoAggregation <%s>, EL_PARTIAL_TYPE <%s>", EL_DURATION, NP_DurationNoAggregation, EL_PARTIAL_TYPE);
            } else {
                i = atoi(EL_REC_SEQ_NBR);
                if (i < 1)
                    strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
                else if (i == 1)
                    strncpy(EL_PARTIAL_TYPE, "F", FSIZE);
                else if (i > 1) {
                    i = atoi(EL_CAUSE_FOR_CLOSING);
                    if (i == 0 || i == 7)
                        strncpy(EL_PARTIAL_TYPE, "L", FSIZE);
                    else
                        strncpy(EL_PARTIAL_TYPE, "P", FSIZE);
                }
                DIAG(DIAG_HIGH, "process_pgwRecord() : EL_REC_SEQ_NBR <%s>,  EL_CAUSE_FOR_CLOSING <%s>, EL_PARTIAL_TYPE <%s>", EL_REC_SEQ_NBR, EL_CAUSE_FOR_CLOSING, EL_PARTIAL_TYPE);
            }

            if (lookup_LS_FLUSH_TRIGGERS("PGW", "EL_CHANGE_CONDITION", EL_CHANGE_CONDITION, buff) > 0) {
                strncpy(EL_FLUSH_TRIGGER, "1", FSIZE);
            } else if (lookup_LS_FLUSH_TRIGGERS("PGW", "EL_CAUSE_FOR_CLOSING", EL_CAUSE_FOR_CLOSING, buff) > 0) {
                strncpy(EL_FLUSH_TRIGGER, "1", FSIZE);
            } else {
                strncpy(EL_FLUSH_TRIGGER, "0", FSIZE);
            }

            snprintf(EL_TOTAL_VOLUME, FSIZE, "%d", atoi(EL_DATA_VOLUME_UPLINK) + atoi(EL_DATA_VOLUME_DOWNLINK));
            if (reject_flag == 0)
                write_pgwRecord(OUTPUT1, reject_flag, reason);
            else
                write_pgwRecord(OUTPUT2, reject_flag, reason);
            el = el->next;
        } // while (el != NULL)
        drop_pgwcontainer_list(head);
        head = NULL;
        el = NULL;
    }
    DIAG(DIAG_HIGH, "process_pgwRecord(): exit");
}

void process_sGWRecord(void) {
    char buff[FSIZE], buff2[FSIZE], OUTPUT[FSIZE];
    int i;
    long lDownLink = 0;
    long lUpLink = 0;


    DIAG(DIAG_HIGH, "process_sGWRecord(): enter");
    if (i_address(".") == NB_OK) {
        // setting output to SG-CDR local
        memset(OUTPUT, '\0', FSIZE);
        snprintf(OUTPUT, FSIZE, "%s%s", "G_OUT_", "N");

        strncpy(EL_RECORD_TYPE, "18", FSIZE);

        // EL_RECORD_OPENING_TIME and EL_S_RECORD_OPENING_TIME
        memset(buff2, '\0', FSIZE);
        memset(buff, '\0', FSIZE);
        GET_FIELD(buff, "recordOpeningTime", 12); // datetime is 12 characters - YYMMDDHHmmss
        snprintf(EL_RECORD_OPENING_TIME, FSIZE, "20%s", buff);
        snprintf(EL_S_RECORD_OPENING_TIME, FSIZE, "20%s", buff);


        // EL_NAPI_FOR_MSISDN
        GET_FIELD(buff, "servedMSISDN", FSIZE);
        memset(buff2, '\0', FSIZE);
        strncpy(buff2, buff, FSIZE);
        char tmpChar = buff[0];
        buff[0] = buff[1];
        buff[1] = tmpChar;
        buff[2] = '\0'; // get only first 2 chars and swap it
        strncpy(EL_NAPI_FOR_MSISDN, buff, FSIZE);
        DIAG(DIAG_HIGH, "process_sGWRecord() : EL_NAPI_FOR_MSISDN <%s>", EL_NAPI_FOR_MSISDN);

        // EL_SERVED_MSISDN
        memset(buff, '\0', FSIZE);
        trim_character(buff2, 'F', 'R', buff);
        GET_FIELD(buff2, "servedIMSI", FSIZE);

        if (is_local_number(buff2) == 1) {
            memset(buff2, '\0', FSIZE);
            remove_before_9725(buff, buff2);
            strncpy(EL_SERVED_MSISDN, buff2, FSIZE);
        } else {
            strncpy(EL_SERVED_MSISDN, buff, FSIZE);
        }
        DIAG(DIAG_HIGH, "process_sGWRecord() : EL_SERVED_MSISDN <%s>", EL_SERVED_MSISDN);

        // EL_SERVED_PDP_TYPE_ORG and EL_SERVED_PDP_TYPE_VALUE
        if (i_field_exists("pdpPDNType")) {
            GET_FIELD(buff, "pdpPDNType", FSIZE);
            int l = strlen(buff);
            if (l >= 2) {
                strncpy(EL_SERVED_PDP_TYPE_ORG, &buff[l - 3], 1);
                strncpy(EL_SERVED_PDP_TYPE_VALUE, &buff[l - 2], 2);
            } else if (l == 1) {
                strncpy(EL_SERVED_PDP_TYPE_ORG, buff, l);
                strncpy(EL_SERVED_PDP_TYPE_VALUE, buff, 1);
            }
            DIAG(DIAG_HIGH, "process_sGWRecord() :pdpPDNType <%s>,  EL_SERVED_PDP_TYPE_ORG <%s>, EL_SERVED_PDP_TYPE_VALUE <%s>", buff, EL_SERVED_PDP_TYPE_ORG, EL_SERVED_PDP_TYPE_VALUE);
        } else {
            strncpy(EL_SERVED_PDP_TYPE_ORG, "1", FSIZE);
            if ((i_is_block("servedPDPPDNAddress") == 1) && (i_is_block("servedPDPPDNAddressExt") == 1)) {
                strncpy(EL_SERVED_PDP_TYPE_VALUE, "8D", FSIZE);
            } else if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV4Address")) {
                strncpy(EL_SERVED_PDP_TYPE_VALUE, "21", FSIZE);
            } else if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV6Address")) {
                strncpy(EL_SERVED_PDP_TYPE_VALUE, "57", FSIZE);
            }

            DIAG(DIAG_HIGH, "process_sGWRecord() :pdpPDNType <%s>,  EL_SERVED_PDP_TYPE_ORG <%s>, EL_SERVED_PDP_TYPE_VALUE <%s>", buff, EL_SERVED_PDP_TYPE_ORG, EL_SERVED_PDP_TYPE_VALUE);
        }
        GET_FIELD(EL_ACCESS_POINT_NAME, "accessPointNameNI", FSIZE);

        GET_FIELD(EL_SGSN_ADDRESS, ".servingNodeAddress.iPBinV4Address", FSIZE);
        GET_FIELD(EL_GGSN_ADDRESS, ".sGWAddress.iPBinV4Address", FSIZE);

        // EL_S_RECORD_CLOSING_TIME, EL_S_DOWNLINK and EL_S_UPLINK
        if (i_address(".listOfTrafficVolumes")) {
            i_enter();
            while (i_next("ChangeOfCharCondition")) {
                i_enter();
                memset(buff, '\0', FSIZE);
                if (i_field_exists("changeTime"))
                    GET_FIELD(buff, "changeTime", 12);
                lDownLink = lDownLink + atoi(i_get("dataVolumeGPRSDownlink"));
                lUpLink = lUpLink + atoi(i_get("dataVolumeGPRSUplink"));
                i_exit();
            }
            i_exit();
        }

        if (buff[0] != '\0')
            snprintf(EL_S_RECORD_CLOSING_TIME, FSIZE, "20%s", buff);
        snprintf(EL_S_DOWNLINK, FSIZE, "%lu", lDownLink);
        snprintf(EL_S_UPLINK, FSIZE, "%lu", lUpLink);
        DIAG(DIAG_HIGH, "process_sGWRecord() : EL_S_RECORD_CLOSING_TIME <%s>", EL_S_RECORD_CLOSING_TIME);
        DIAG(DIAG_HIGH, "process_sGWRecord() : EL_S_DOWNLINK <%s>, EL_S_UPLINK <%s>", EL_S_DOWNLINK, EL_S_UPLINK);

        GET_FIELD(EL_SERVED_IMEI, "servedIMEISV", FSIZE);

        //EL_S_FIRST_SEQUENCE_NUMBER and EL_S_LAST_SEQUENCE_NUMBER
        if (i_field_exists("recordSequenceNumber")) {
            GET_FIELD(buff, "recordSequenceNumber", FSIZE);
            strncpy(EL_S_FIRST_SEQUENCE_NUMBER, buff, FSIZE);
            strncpy(EL_S_LAST_SEQUENCE_NUMBER, buff, FSIZE);
        } else {
            strncpy(EL_S_FIRST_SEQUENCE_NUMBER, "0", FSIZE);
            strncpy(EL_S_LAST_SEQUENCE_NUMBER, "0", FSIZE);
        }

        // EL_NODE_ID
        GET_FIELD(buff, "rATType", FSIZE);
        DIAG(DIAG_HIGH, "process_sGWRecord() : rATType <%s>", buff);
        if (buff[0] == '6') {
            DIAG(DIAG_HIGH, "process_sGWRecord() : rATType <%s>", buff);
            strncpy(EL_NODE_ID, "LTE", FSIZE);
        } else {
            GET_FIELD(EL_NODE_ID, "nodeID", FSIZE);
        }

        DIAG(DIAG_HIGH, "process_sGWRecord() : EL_NODE_ID <%s>", EL_NODE_ID);

        GET_FIELD(EL_S_CHARGING_CHARACTERISTICS, "chargingCharacteristics", 2);

        GET_FIELD(EL_CHANGE_CONDITION, ".listOfTrafficVolumes.ChangeOfCharCondition.changeCondition", FSIZE);

        //EL_CAUSE_FOR_CLOSING
        if (lookup_LS_CAUSE_FOR_CLOSING("SGW", "EL_CHANGE_CONDITION", EL_CHANGE_CONDITION, EL_S_CAUSE_FOR_CLOSING) <= 0) {
            GET_FIELD(buff, ".causeForRecClosing", FSIZE);
            strncpy(EL_S_CAUSE_FOR_CLOSING, buff, FSIZE);
        }
        DIAG(DIAG_HIGH, "process_sGWRecord() :EL_S_CAUSE_FOR_CLOSING <%s>", EL_S_CAUSE_FOR_CLOSING);

        //EL_SERVED_PDP_ADDRESS
        if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV4Address")) {
            GET_FIELD(EL_SERVED_PDP_ADDRESS, ".servedPDPPDNAddress.iPAddress.iPBinV4Address", FSIZE);
        } else if (i_field_exists(".servedPDPPDNAddress.iPAddress.iPBinV6Address")) {
            GET_FIELD(EL_SERVED_PDP_ADDRESS, ".servedPDPPDNAddress.iPAddress.iPBinV6Address", FSIZE);
        }

        //EL_DIAGNOSTICS_VALUE
        if (i_field_exists(".diagnostics.gsm0408Cause")) {
            GET_FIELD(EL_DIAGNOSTICS_VALUE, ".diagnostics.gsm0408Cause", FSIZE);
        }

        GET_FIELD(EL_DURATION, ".duration", FSIZE);

        // Aggregation fields
        init_aggr_fields();

        snprintf(EL_START_TIME, FSIZE, "%lu", convert_time_to_epoch(EL_RECORD_OPENING_TIME, 0));
        snprintf(EL_END_TIME, FSIZE, "%lu", convert_time_to_epoch(EL_S_RECORD_CLOSING_TIME, 0));

        memset(buff, '\0', FSIZE);
        memset(buff2, '\0', FSIZE);
        if (atoi(EL_DURATION) > atoi(NP_DurationNoAggregation)) {
            strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
            DIAG(DIAG_HIGH, "process_sGWRecord() :EL_DURATION <%s> > NP_DurationNoAggregation <%s>, EL_PARTIAL_TYPE <%s>", EL_DURATION, NP_DurationNoAggregation, EL_PARTIAL_TYPE);
        } else {
            if (i_field_exists(".recordSequenceNumber")) {
                GET_FIELD(EL_REC_SEQ_NBR, ".recordSequenceNumber", FSIZE);
                i = atoi(EL_REC_SEQ_NBR);
                if (i < 1)
                    strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
                else if (i == 1)
                    strncpy(EL_PARTIAL_TYPE, "F", FSIZE);
                else if (i > 1) {
                    i = atoi(EL_S_CAUSE_FOR_CLOSING);
                    if (i == 0 || i == 4)
                        strncpy(EL_PARTIAL_TYPE, "L", FSIZE);
                    else
                        strncpy(EL_PARTIAL_TYPE, "P", FSIZE);
                }
            } else {
                strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
            }
            DIAG(DIAG_HIGH, "process_sGWRecord() :EL_REC_SEQ_NBR <%s>,  EL_CAUSE_FOR_CLOSING <%s>, EL_PARTIAL_TYPE <%s>", EL_REC_SEQ_NBR, EL_CAUSE_FOR_CLOSING, EL_PARTIAL_TYPE);
        }
        if (lookup_LS_FLUSH_TRIGGERS("SGW", "EL_CHANGE_CONDITION", EL_CHANGE_CONDITION, buff) > 0) {
            strncpy(EL_FLUSH_TRIGGER, "1", FSIZE);
        } else if (lookup_LS_FLUSH_TRIGGERS("SGW", "EL_S_CAUSE_FOR_CLOSING", EL_S_CAUSE_FOR_CLOSING, buff) > 0) {
            strncpy(EL_FLUSH_TRIGGER, "1", FSIZE);
        } else {

            strncpy(EL_FLUSH_TRIGGER, "0", FSIZE);
        }

        snprintf(EL_TOTAL_VOLUME, FSIZE, "%d", atoi(EL_S_UPLINK) + atoi(EL_S_DOWNLINK));

        write_sGWRecord(OUTPUT);
    }
    DIAG(DIAG_HIGH, "process_sGWRecord(): exit");
}

void process_sgsnPDPRecord(void) {
    char buff[FSIZE], buff2[FSIZE], OUTPUT[FSIZE];
    char c;
    int l, i;
    DIAG(DIAG_HIGH, "process_sgsnPDPRecord(): enter");
    if (i_address(".") == NB_OK) {
        // setting output to SG-CDR local
        memset(OUTPUT, '\0', FSIZE);
        snprintf(OUTPUT, FSIZE, "%s%s", "G_OUT_", "N");

        memset(buff2, '\0', FSIZE);

        strncpy(EL_RECORD_TYPE, "18", FSIZE);

        // EL_RECORD_OPENING_TIME and EL_S_RECORD_OPENING_TIME
        GET_FIELD(buff, "recordOpeningTime", 12); // datetime is 12 characters - YYMMDDHHmmss
        snprintf(EL_RECORD_OPENING_TIME, FSIZE, "20%s", buff);
        snprintf(EL_S_RECORD_OPENING_TIME, FSIZE, "20%s", buff);

        //EL_NAPI_FOR_MSISDN
        memset(buff2, '\0', FSIZE);
        GET_FIELD(buff, "servedMSISDN", FSIZE);
        strncpy(buff2, buff, FSIZE);
        buff[2] = '\0';
        c = buff[0];
        buff[0] = buff[1];
        buff[1] = c;
        strncpy(EL_NAPI_FOR_MSISDN, buff, FSIZE);
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_NAPI_FOR_MSISDN <%s>", EL_NAPI_FOR_MSISDN);

        //EL_SERVED_MSISDN !!!!!

        GET_FIELD(buff, "servedIMSI", FSIZE);
        if (is_local_number(buff) == 1) {
            memset(buff, '\0', FSIZE);
            remove_before_9725(buff2, buff);
            strncpy(EL_SERVED_MSISDN, buff, FSIZE);
        } else {
            strncpy(EL_SERVED_MSISDN, buff2, FSIZE);
        }
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_SERVED_MSISDN <%s>", EL_SERVED_MSISDN);

        GET_FIELD(EL_SERVED_IMEI, "servedIMEI", FSIZE);

        //EL_ACCESS_POINT_NAME
        GET_FIELD(buff, "accessPointNameNI", FSIZE);
        GET_FIELD(buff2, "accessPointNameOI", FSIZE);
        //strncat(buff, buff2, FSIZE);
        //strncpy(EL_ACCESS_POINT_NAME, buff, FSIZE);
        snprintf(EL_ACCESS_POINT_NAME, FSIZE, "%s.%s", buff, buff2);
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_ACCESS_POINT_NAME <%s>", EL_ACCESS_POINT_NAME);

        //EL_SERVED_PDP_TYPE_ORG and EL_SERVED_PDP_TYPE_VALUE
        memset(buff, '\0', FSIZE);
        memset(buff2, '\0', FSIZE);
        if (i_field_exists("pdpType")) {
            //EL_SERVED_PDP_TYPE_ORG
            GET_FIELD(buff, "pdpType", 2);
            trim_character(buff, 'F', 'L', buff2);

            //strncpy(buff2, left_trim(buff, 'F'), FSIZE);
            strncpy(EL_SERVED_PDP_TYPE_ORG, buff2, FSIZE);
            DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_SERVED_PDP_TYPE_ORG <%s>", EL_SERVED_PDP_TYPE_ORG);

            //EL_SERVED_PDP_TYPE_VALUE
            memset(buff2, '\0', FSIZE);
            GET_FIELD(buff, "pdpType", FSIZE);
            l = strlen(buff);
            //strncpy(buff2, left_trim(&buff[l - 2], 'F'), FSIZE);
            trim_character(&buff[l - 2], 'F', 'L', buff2);
            strncpy(EL_SERVED_PDP_TYPE_VALUE, buff2, FSIZE);
            DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_SERVED_PDP_TYPE_VALUE <%s>", EL_SERVED_PDP_TYPE_VALUE);
        } else {
            strncpy(EL_SERVED_PDP_TYPE_ORG, "1", FSIZE);
            if ((i_is_block("servedPDPAddress") == 1) && (i_is_block("servedPDPPDNAddressExt") == 1)) {
                strncpy(EL_SERVED_PDP_TYPE_VALUE, "8D", FSIZE);
            } else if (i_field_exists(".servedPDPAddress.iPAddress.iPBinV4Address")) {
                strncpy(EL_SERVED_PDP_TYPE_VALUE, "21", FSIZE);
            } else if (i_field_exists(".servedPDPAddress.iPAddress.iPBinV6Addresss")) {
                strncpy(EL_SERVED_PDP_TYPE_VALUE, "57", FSIZE);
            }
        }


        //EL_MS_NW_CAPABILITY
        GET_FIELD(buff, "msNetworkCapability", FSIZE);
        lowercase(buff);
        strncpy(EL_MS_NW_CAPABILITY, buff, FSIZE);
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_MS_NW_CAPABILITY <%s>", EL_MS_NW_CAPABILITY);

        //EL_S_CHARGING_CHARACTERISTICS
        GET_FIELD(EL_S_CHARGING_CHARACTERISTICS, "chargingCharacteristics", 2); // only first 2 characters

        // EL_S_RECORD_CLOSING_TIME
        GET_FIELD(buff, ".listOfTrafficVolumes.ChangeOfCharConditionS.changeTime", 12); // datetime is 12 characters - YYMMDDHHmmss
        snprintf(EL_S_RECORD_CLOSING_TIME, FSIZE, "20%s", buff);
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_S_RECORD_CLOSING_TIME <%s>", EL_S_RECORD_CLOSING_TIME);

        //EL_SGSN_ADDRESS and EL_GGSN_ADDRESS
        GET_FIELD(EL_SGSN_ADDRESS, ".sgsnAddress.iPBinV4Address", FSIZE);
        if (i_field_exists(".ggsnAddressUsed.iPBinV4Address")) {
            GET_FIELD(EL_GGSN_ADDRESS, ".ggsnAddressUsed.iPBinV4Address", FSIZE);
        } else if (i_field_exists(".ggsnAddressUsed.iPBinV6Address")) {
            GET_FIELD(EL_GGSN_ADDRESS, ".ggsnAddressUsed.iPBinV6Address", FSIZE);
        }
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_SGSN_ADDRESS <%s>, EL_GGSN_ADDRESS <%s>", EL_SGSN_ADDRESS, EL_GGSN_ADDRESS);
        //EL_SERVED_PDP_ADDRESS
        if (i_field_exists(".servedPDPAddress.iPAddress.iPBinV4Address")) {
            GET_FIELD(EL_SERVED_PDP_ADDRESS, ".servedPDPAddress.iPAddress.iPBinV4Address", FSIZE);
        } else if (i_field_exists(".servedPDPAddress.iPAddress.iPBinV6Addresss")) {
            GET_FIELD(EL_SERVED_PDP_ADDRESS, ".servedPDPAddress.iPAddress.iPBinV6Addresss", FSIZE);
        }
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_SERVED_PDP_ADDRESS <%s>", EL_SERVED_PDP_ADDRESS);

        // EL_S_UPLINK and EL_S_DOWNLINK
        GET_FIELD(EL_S_UPLINK, ".listOfTrafficVolumes.ChangeOfCharConditionS.dataVolumeGPRSUplink", FSIZE);
        GET_FIELD(EL_S_DOWNLINK, ".listOfTrafficVolumes.ChangeOfCharConditionS.dataVolumeGPRSDownlink", FSIZE);

        //EL_S_QOS_REQUESTED
        GET_FIELD(buff, ".listOfTrafficVolumes.ChangeOfCharConditionS.qosRequested", FSIZE);
        l = strlen(buff);
        if (l > 18)
            strncpy(EL_S_QOS_REQUESTED, &buff[l - 18], 18);
        else
            strncpy(EL_S_QOS_REQUESTED, buff, 18);
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_S_QOS_REQUESTED <%s>", EL_S_QOS_REQUESTED);

        //EL_S_QOS_NEGOTIATED
        GET_FIELD(buff, ".listOfTrafficVolumes.ChangeOfCharConditionS.qosNegotiated", FSIZE);
        l = strlen(buff);
        if (l > 18)
            strncpy(EL_S_QOS_NEGOTIATED, &buff[l - 18], 18);
        else
            strncpy(EL_S_QOS_NEGOTIATED, buff, 18);
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_S_QOS_NEGOTIATED <%s>", EL_S_QOS_NEGOTIATED);

        GET_FIELD(EL_CHANGE_CONDITION, ".listOfTrafficVolumes.ChangeOfCharConditionS.changeCondition", FSIZE);

        //EL_S_CAUSE_FOR_CLOSING

        if (lookup_LS_CAUSE_FOR_CLOSING("SGSN", "EL_CHANGE_CONDITION", EL_CHANGE_CONDITION, EL_S_CAUSE_FOR_CLOSING) <= 0) {
            GET_FIELD(buff, ".causeForRecClosing", FSIZE);
            if (lookup_LS_CAUSE_FOR_CLOSING("SGSN", "causeForRecClosing", buff, EL_S_CAUSE_FOR_CLOSING) <= 0)
                strncpy(EL_S_CAUSE_FOR_CLOSING, "-1", FSIZE);
        }
        DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_S_CAUSE_FOR_CLOSING <%s>", EL_S_CAUSE_FOR_CLOSING);

        GET_FIELD(EL_S_COMPLETE, ".recordExtensions.NSManagementExtension.complete", FSIZE);

        //EL_S_FIRST_SEQUENCE_NUMBER and EL_S_LAST_SEQUENCE_NUMBER
        if (i_field_exists(".recordSequenceNumber")) {
            GET_FIELD(EL_S_FIRST_SEQUENCE_NUMBER, ".recordSequenceNumber", FSIZE);
            GET_FIELD(EL_S_LAST_SEQUENCE_NUMBER, ".recordSequenceNumber", FSIZE);
        } else {
            strncpy(EL_S_FIRST_SEQUENCE_NUMBER, "0", FSIZE);
            strncpy(EL_S_LAST_SEQUENCE_NUMBER, "0", FSIZE);
        }

        //EL_SGSN_CHANGE
        if (i_field_exists(".sgsnChange")) {
            GET_FIELD(buff, ".sgsnChange", FSIZE);
            if (strcmp(buff, "1") == 0)
                strncpy(EL_SGSN_CHANGE, "1", FSIZE);
            else
                strncpy(EL_SGSN_CHANGE, "0", FSIZE);
        } else
            strncpy(EL_SGSN_CHANGE, "0", FSIZE);

        GET_FIELD(EL_DURATION, ".duration", FSIZE);

        // Aggregation fields
        init_aggr_fields();

        snprintf(EL_START_TIME, FSIZE, "%lu", convert_time_to_epoch(EL_RECORD_OPENING_TIME, 0));
        snprintf(EL_END_TIME, FSIZE, "%lu", convert_time_to_epoch(EL_S_RECORD_CLOSING_TIME, 0));

        if (atoi(EL_DURATION) > atoi(NP_DurationNoAggregation)) {
            strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
            DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_DURATION <%s> > NP_DurationNoAggregation <%s>, EL_PARTIAL_TYPE <%s>", EL_DURATION, NP_DurationNoAggregation, EL_PARTIAL_TYPE);
        } else {
            if (i_field_exists(".recordSequenceNumber")) {
                GET_FIELD(EL_REC_SEQ_NBR, ".recordSequenceNumber", FSIZE);
                i = atoi(EL_REC_SEQ_NBR);
                if (i < 1)
                    strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
                else if (i == 1)
                    strncpy(EL_PARTIAL_TYPE, "F", FSIZE);
                else if (i > 1) {
                    i = atoi(EL_S_CAUSE_FOR_CLOSING);
                    if (i == 0 || i == 7)
                        strncpy(EL_PARTIAL_TYPE, "L", FSIZE);
                    else
                        strncpy(EL_PARTIAL_TYPE, "P", FSIZE);
                }
            } else {
                strncpy(EL_PARTIAL_TYPE, "S", FSIZE);
            }
            DIAG(DIAG_HIGH, "process_sgsnPDPRecord() :EL_REC_SEQ_NBR <%s>,  EL_CAUSE_FOR_CLOSING <%s>, EL_PARTIAL_TYPE <%s>", EL_REC_SEQ_NBR, EL_CAUSE_FOR_CLOSING, EL_PARTIAL_TYPE);
        }


        if (lookup_LS_FLUSH_TRIGGERS("SGSN", "EL_S_CAUSE_FOR_CLOSING", EL_S_CAUSE_FOR_CLOSING, buff) > 0) {
            strncpy(EL_FLUSH_TRIGGER, "1", FSIZE);
        } else if (lookup_LS_FLUSH_TRIGGERS("SGSN", "EL_CHANGE_CONDITION", EL_CHANGE_CONDITION, buff) > 0) {
            strncpy(EL_FLUSH_TRIGGER, "1", FSIZE);
        } else {

            strncpy(EL_FLUSH_TRIGGER, "0", FSIZE);
        }

        snprintf(EL_TOTAL_VOLUME, FSIZE, "%d", atoi(EL_S_UPLINK) + atoi(EL_S_DOWNLINK));

        write_sgsnPDPRecord(OUTPUT);
    }
    DIAG(DIAG_HIGH, "process_sgsnPDPRecord(): exit");
}

/***********************************************************************
 *
 * Reserved functions
 *
 ***********************************************************************/

/* This function is called in the beginning when the node is starting up.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_init(void) {
    char Lookup_Server[256];
    char buff[SMFSIZE];
    int ret = 0;

    DIAG(DIAG_LOW, "node_init(): entered the function");
    /* Get the node parameter */
    record_number = 0;
    memset(Lookup_Server, '\0', 256);

    strncpy(Lookup_Server, nb_get_parameter_string("LookupServer"), 256);
    if (Lookup_Server[0] == 0) {
        DIAG(DIAG_LOW, "node_init(): Node parameter Lookup Server is empty");
        nb_msg_custom(MSG_CRITICAL, "Node parameter Lookup Server is empty");
        nb_abort();
    }

    /* load the lookup library */
    nb_load_library("server_lookup");

    /* initialize lookup server */
    ret = ls_initialize();

    if (ret == 0) {
        DIAG(DIAG_LOW, "node_init(): Lookup client initialized successfully");
    } else {
        DIAG(DIAG_LOW, "node_init(): Lookup client initialization failed");
        nb_msg("GSMSCLBLN100");
        nb_msg_custom(MSG_CRITICAL, "Lookup client initialization failed");
        nb_abort();
    }

    LS_SGSN_IP_RANGE = ls_lookup_get_table(Lookup_Server, "LS_SGSN_IP_RANGE");
    if (LS_SGSN_IP_RANGE < 0) {
        DIAG(DIAG_LOW, "node_init(): Table LS_SGSN_IP_RANGE not found");
        nb_msg_custom(MSG_CRITICAL, "Table LS_SGSN_IP_RANGE not found");
        nb_abort();
    }

    LS_FLUSH_TRIGGERS = ls_lookup_get_table(Lookup_Server, "LS_FLUSH_TRIGGERS");
    if (LS_FLUSH_TRIGGERS < 0) {
        DIAG(DIAG_LOW, "node_init(): Table LS_FLUSH_TRIGGERS not found");
        nb_msg_custom(MSG_CRITICAL, "Table LS_FLUSH_TRIGGERS not found");
        nb_abort();
    }

    LS_CAUSE_FOR_CLOSING = ls_lookup_get_table(Lookup_Server, "LS_CAUSE_FOR_CLOSING");
    if (LS_CAUSE_FOR_CLOSING < 0) {
        DIAG(DIAG_LOW, "node_init(): Table LS_CAUSE_FOR_CLOSING not found");
        nb_msg_custom(MSG_CRITICAL, "Table LS_CAUSE_FOR_CLOSING not found");
        nb_abort();
    }

    LS_APN_FILTER = ls_lookup_get_table(Lookup_Server, "LS_APN_FILTER");
    if (LS_APN_FILTER < 0) {
        DIAG(DIAG_LOW, "node_init(): Table LS_APN_FILTER not found");
        nb_msg_custom(MSG_CRITICAL, "Table LS_APN_FILTER not found");
        nb_abort();
    }

    // Get rest of node parameters
    strncpy(NP_AlarmThreshold, nb_get_parameter_string("AlarmThreshold"), SMFSIZE);
    strncpy(NP_DiscardLocalSGW, nb_get_parameter_string("DiscardLocalSGW"), SMFSIZE);
    strncpy(NP_DurationNoAggregation, nb_get_parameter_string("DurationNoAggregation"), SMFSIZE);
    strncpy(NP_LocalIMSIPrefixes, nb_get_parameter_string("LocalIMSIPrefixes"), FSIZE);
    strncpy(NP_ProcessRG0Records, nb_get_parameter_string("ProcessRG0Records"), SMFSIZE);

    NP_AlarmThreshold_int = atoi(NP_AlarmThreshold);

    DIAG(DIAG_LOW, "node_init(): Node parameter 'AlarmThreshold' = <%d>", NP_AlarmThreshold_int);
    DIAG(DIAG_LOW, "node_init(): Node parameter 'DiscardLocalSGW' = <%s>", NP_DiscardLocalSGW);
    DIAG(DIAG_LOW, "node_init(): Node parameter 'DurationNoAggregation' = <%s>", NP_DurationNoAggregation);
    DIAG(DIAG_LOW, "node_init(): Node parameter 'LocalIMSIPrefixes' = <%s>", NP_LocalIMSIPrefixes);
    DIAG(DIAG_LOW, "node_init(): Node parameter 'ProcessRG0Records' = <%s>", NP_ProcessRG0Records);

    //retrieve stored counters and alarm status
    memset(buff, '\0', SMFSIZE);
    strncpy(buff, nb_store_get("number_of_rejections"), SMFSIZE);
    if (strlen(buff) == 0)
        number_of_rejections = 0;
    else
        number_of_rejections = atoi(buff);

    memset(buff, '\0', SMFSIZE);
    strncpy(buff, nb_store_get("send_alarm"), SMFSIZE);
    if (strlen(buff) == 0)
        send_alarm = 1;

    else
        send_alarm = atoi(buff);

    DIAG(DIAG_LOW, "node_init(): Retrieved Node counter 'number_of_rejections' = <%d>", number_of_rejections);
    DIAG(DIAG_LOW, "node_init(): Retrieved Node counter 'send_alarm' = <%d>", send_alarm);


    DIAG(DIAG_LOW, "node_init(): returning...");
}

/* This function is called for every input record or for every input
 * file, if input is binary.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_process(void) {
    DIAG(DIAG_HIGH, "node_process(): entered the function");

    char buff[FSIZE];
    strncpy(buff, i_get_input_type(), FSIZE);
    // ========================================================================
    //  Validation rules
    // ========================================================================
    record_number++;
    init_fields();
    // Rules for psgwRecord ----------------------------------------
    if (strcmp(buff, "pGWRecord") == 0) {
        if (validate_pgwRecord()) {
            process_pgwRecord();
        }
    } else if (strcmp(buff, "sGWRecord") == 0) {
        // Rules for sgwRecord -----------------------------------------
        if (validate_sGWRecord()) {
            process_sGWRecord();
        }
    } else if (strcmp(buff, "sgsnPDPRecord") == 0) {
        // Rules for sgsnPDPRecord -----------------------------------------
        if (validate_sgsnPDPRecord()) {

            process_sgsnPDPRecord();
        }
    }
    // ========================================================================

}

/* This function is called whenever a control record is received.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_control(void) {

    DIAG(DIAG_LOW, "node_control()");
}

/* This function is called when the node commits after processing an
 * input file successfully.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_commit(void) {

    char buff[SMFSIZE];
    DIAG(DIAG_LOW, "node_commit()");
    memset(buff, '\0', SMFSIZE);
    snprintf(buff, SMFSIZE, "%d", number_of_rejections);
    nb_store_set("number_of_rejections", buff);

    memset(buff, '\0', SMFSIZE);
    snprintf(buff, SMFSIZE, "%d", send_alarm);
    nb_store_set("send_alarm", buff);
    record_number = 0;

}

/* This function is called in the end when the node is shutting down.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_end(void) {

    DIAG(DIAG_LOW, "node_end()");
}

/* This function is called when the operator requested the flushing
 * of the steram. If the node stores any records in an internal
 * storage, all records should be retrieved from the storage and
 * written to the output in this function.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_flush(void) {

    DIAG(DIAG_LOW, "node_flush()");

    char buff[SMFSIZE];

    memset(buff, '\0', SMFSIZE);
    snprintf(buff, SMFSIZE, "%d", number_of_rejections);
    nb_store_set("number_of_rejections", buff);

    memset(buff, '\0', SMFSIZE);
    snprintf(buff, SMFSIZE, "%d", send_alarm);
    nb_store_set("send_alarm", buff);
}

/* This function is called in regular intervals, about every second.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_timer(void) {

    DIAG(DIAG_HIGH, "node_timer()");
    ReloadLookupTables();
}
/* This function is called for real-time nodes when they are resumed
 * after having paused.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_resume(void) {

    DIAG(DIAG_LOW, "node_resume()");
}

void node_request(void) {

    DIAG(DIAG_LOW, "node_request()");
}

/* This function is called for real-time nodes if they have to be stopped.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_pause(void) {

    DIAG(DIAG_LOW, "node_pause()");
}

/* This function is called if an error occurs during the processing of a
 * file/record, and should be used to reset the system to the point of
 * the last commit.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None.
 */

/*extern "C"*/
void node_rollback(void) {

    DIAG(DIAG_LOW, "node_rollback()");
}

/* This function is called whenever the node is scheduled to be executed.
 *
 * Arguments:
 *   None.
 * Return values:
 *   None. 
 */

/*extern "C"*/
int node_schedule(void) {
    DIAG(DIAG_LOW, "node_schedule()");
    if (nb_is_scheduled()) {
        DIAG(DIAG_LOW, "node_schedule(): reseting alarm counters");
        nb_store_set("number_of_rejections", "0");
        nb_store_set("send_alarm", "1");
    }
    return NB_OK;
}

