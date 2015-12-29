#include "stdio.h"
//by cgm@meitu.com
///Communication between xxx Station and SFC is through HTTP POST requests.
///All command, parameter and data set names are case sensitive.
///UTF-8 is the character set to be used by both client and server.
///All responses from SFC shall include one of ?standard response?(see standard response).

//1.1.  Standard Response
typedef enum{
    PSC_OK = 0x0, //Data process OK.
    PSC_ERROR = 0x1,//Error, Gatekeeper Station should resend
    PSC_FATAL_ERROR = 0x2,//Fatal error, Gatekeeper Station stop send the data and retry after 60 seconds until get ?0 PSC_OK?.
}e_standard_response;


//3.1.  Query Routing Parameter List
/*
Symbol Definition   Required    Example Remark
cmd Y   QUERY_ROUTING   Cmd for querying SFC routing check.
wip_no  Y   BIR0000000000000    Device SN.
station_type    Y   BB1 Testing station name such as BB1, CAPT, WBT etc.
model   Y   VKY Product project code.
*/
#define MAX_WPI_NO 20
#define MAX_MODEL_LEN 10
#define MAX_SFC_ERROR_MSG_LEN 20
#define MAX_SW_VERSION_CHECK_LEN 20
#define MAX_RF_BAND_ID_LEN 100
#define MAX_IMEI_LEN 20
#define MAX_VERSION_LEN 50
#define MAX_PATH_LEN 256
#define MAX_PRODUCT_ID_LEN 20
#define MAX_STRLEN 30
typedef struct{
    int cmd;
    char wip_no[MAX_WPI_NO];
    int station_type;
    char model[MAX_MODEL_LEN];
}s_query_routing_parameter_list;

///3.2.  Response Parameter List
typedef struct{
    int cmd;
    char wip_no[MAX_WPI_NO];
    int permission;
    char sfc_error_msg[MAX_SFC_ERROR_MSG_LEN];
    char sw_version_check[MAX_SW_VERSION_CHECK_LEN];
    char rf_band_id[MAX_RF_BAND_ID_LEN];
    char imei[MAX_IMEI_LEN];
    char imei2[MAX_IMEI_LEN];
    char meid[MAX_IMEI_LEN];
    char esn[MAX_IMEI_LEN];
    char skuid[MAX_IMEI_LEN];
    char bt_addr[MAX_IMEI_LEN];
    char wifi_mac_addr[MAX_IMEI_LEN];
    char sim_lock_nkey[MAX_IMEI_LEN];
    char sim_lock_nskey[MAX_IMEI_LEN];
    char chip_version[MAX_IMEI_LEN];
    char mcp_version[MAX_VERSION_LEN];
    char emmc_version[MAX_VERSION_LEN];
    char tp_version[MAX_VERSION_LEN];
    char camera_version[MAX_VERSION_LEN];
    int  sim_category;
    char tool_version[MAX_VERSION_LEN];
    char rdl_fw_path[MAX_PATH_LEN];
    char customer_product_id[MAX_PRODUCT_ID_LEN];
    char customer_product_id2[MAX_PRODUCT_ID_LEN];
    char customer_sw_id[MAX_VERSION_LEN];
    char battery_pn[MAX_VERSION_LEN];
    char rdcode_with_skuid[MAX_VERSION_LEN];
}s_routing_respone_parameter_list;

///4.1.  Add Test Item Result Parameter List

typedef struct{
    int    cmd;
    char wip_no[MAX_WPI_NO];
    char test_station_name[MAX_PATH_LEN];
    char station_code[MAX_STRLEN];
    char test_item_name[MAX_STRLEN];
    char test_spec_name[MAX_STRLEN];
    char test_value[MAX_STRLEN];
    char upper_bound[MAX_STRLEN];
    char low_bound[MAX_STRLEN];
    char test_result[MAX_STRLEN];
    char symptom_code[MAX_STRLEN];
}s_test_item_result_parameter_list;
