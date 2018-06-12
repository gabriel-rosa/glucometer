#ifndef PHD_TYPES
#define PHD_TYPES

typedef unsigned char intu8;
typedef unsigned short intu16;
typedef unsigned int intu32;

typedef struct Any
{
	intu16 length;
	intu8 *value; /* first element of the array */
} Any;

#define NOM_PART_UNSPEC 0
#define NOM_PART_OBJ 1
#define NOM_PART_METRIC 2
#define NOM_PART_ALERT 3
#define NOM_PART_DIM 4
#define NOM_PART_VATTR 5
#define NOM_PART_PGRP 6
#define NOM_PART_SITES 7
#define NOM_PART_INFRASTRUCT 8
#define NOM_PART_FEF 9
#define NOM_PART_ECG_EXTN 10
#define NOM_PART_PHD_DM 128
#define NOM_PART_PHD_HF 129
#define NOM_PART_PHD_AI 130
#define NOM_PART_RET_CODE 255
#define NOM_PART_EXT_NOM 256
#define NOM_PART_PRIV 1024

typedef struct TYPE
{
	intu16 partition;
	intu16 code;
} TYPE;

typedef struct AVA_Type
{
	intu16 attribute_id;
	Any attribute_value;
} AVA_Type;

typedef struct AttributeList
{
	intu16 count;
	intu16 length;
	AVA_Type *value; /* first element of the array */
} AttributeList;

typedef struct AttributeIdList
{
	intu16 count;
	intu16 length;
	intu16 *value; /* first element of the array */
} AttributeIdList;

typedef struct HighResRelativeTime
{
	intu8 value[8];
} HighResRelativeTime;

typedef struct AbsoluteTimeAdjust
{
	intu8 value[6];
} AbsoluteTimeAdjust;

typedef struct AbsoluteTime
{
	intu8 century;
	intu8 year;
	intu8 month;
	intu8 day;
	intu8 hour;
	intu8 minute;
	intu8 second;
	intu8 sec_fractions;
} AbsoluteTime;

#define OS_DISABLED 0
#define OS_ENABLED 1
#define OS_NOT_AVAILABLE 2

typedef struct octet_string
{
	intu16 length;
	intu8 *value; /* first element of the array */
} octet_string;

typedef struct SystemModel
{
	octet_string manufacturer;
	octet_string model_number;
} SystemModel;

typedef struct ProdSpecEntry
{
	intu16 spec_type;
	#define UNSPECIFIED 0
	#define SERIAL_NUMBER 1
	#define PART_NUMBER 2
	#define HW_REVISION 3
	#define SW_REVISION 4
	#define FW_REVISION 5
	#define PROTOCOL_REVISION 6
	#define PROD_SPEC_GMDN 7
	intu16 component_id;
	octet_string prod_spec;
} ProdSpecEntry;

typedef struct ProductionSpec
{
	intu16 count;
	intu16 length;
	ProdSpecEntry *value; /* first element of the array */
} ProductionSpec;

#define ON_MAINS 0x8000
#define ON_BATTERY 0x4000
#define CHARGING_FULL 0x0080
#define CHARGING_TRICKLE 0x0040
#define CHARGING_OFF 0x0020

typedef struct BatMeasure
{
	intu32 value;
	intu16 unit;
} BatMeasure;

#define MS_INVALID 0x8000
#define MS_QUESTIONABLE 0x4000
#define MS_NOT_AVAILABLE 0x2000
#define MS_CALIBRATION_ONGOING 0x1000
#define MS_TEST_DATA 0x0800
#define MS_DEMO_DATA 0x0400
#define MS_VALIDATED_DATA 0x0080
#define MS_EARLY_INDICATION 0x0040
#define MS_MSMT_ONGOING 0x0020

typedef struct NuObsValue
{
	intu16 metric_id;
	intu16 state;
	intu16 unit_code;
	intu32 value;
} NuObsValue;

typedef struct NuObsValueCmp
{
	intu16 count;
	intu16 length;
	NuObsValue *value; /* first element of the array */
} NuObsValueCmp;

typedef struct SampleType
{
	intu8 sample_size;
	intu8 significant_bits;
} SampleType;

#define SAMPLE_TYPE_SIGNIFICANT_BITS_SIGNED_SAMPLES 255

#define SMOOTH_CURVE 0x8000
#define DELAYED_CURVE 0x4000
#define STATIC_SCALE 0x2000
#define SA_EXT_VAL_RANGE 0x1000

typedef struct SaSpec
{
	intu16 array_size;
	SampleType sample_type;
	intu16 flags;
} SaSpec;

typedef struct ScaleRangeSpec8
{
	intu32 lower_absolute_value;
	intu32 upper_absolute_value;
	intu8 lower_scaled_value;
	intu8 upper_scaled_value;
} ScaleRangeSpec8;

typedef struct ScaleRangeSpec16
{
	intu32 lower_absolute_value;
	intu32 upper_absolute_value;
	intu16 lower_scaled_value;
	intu16 upper_scaled_value;
} ScaleRangeSpec16;

typedef struct ScaleRangeSpec32
{
	intu32 lower_absolute_value;
	intu32 upper_absolute_value;
	intu32 lower_scaled_value;
	intu32 upper_scaled_value;
} ScaleRangeSpec32;

typedef struct EnumVal
{
	intu16 choice;
	intu16 length;
	#define OBJ_ID_CHOSEN 0x0001
	#define TEXT_STRING_CHOSEN 0x0002
	#define BIT_STR_CHOSEN 0x0010
	union
	{
		intu16 enum_obj_id;
		octet_string enum_text_string;
		intu32 enum_bit_str; // BITS-32
	} u;
} EnumVal;

typedef struct EnumObsValue
{
	intu16 metric_id;
	intu16 state;
	EnumVal value;
} EnumObsValue;

typedef struct AttrValMapEntry
{
	intu16 attribute_id;
	intu16 attribute_len;
} AttrValMapEntry;

typedef struct AttrValMap
{
	intu16 count;
	intu16 length;
	AttrValMapEntry *value; /* first element of the array */
} AttrValMap;

typedef struct HandleAttrValMapEntry
{
	intu16 obj_handle;
	AttrValMap attr_val_map;
} HandleAttrValMapEntry;

#define UNCONFIRMED 0x0000
#define CONFIRMED 0x0001

typedef struct HandleAttrValMap
{
	intu16 count;
	intu16 length;
	HandleAttrValMapEntry *value; /* first element of the array */
} HandleAttrValMap;

#define ST_ALG_NOS 0x0000
#define ST_ALG_MOVING_AVERAGE 0x0001
#define ST_ALG_RECURSIVE_ 0x0002
#define ST_ALG_MIN_PICK 0x0003
#define ST_ALG_MAX_PICK 0x0004
#define ST_ALG_MEDIAN 0x0005
#define ST_ALG_TRENDED 0x0200
#define ST_ALG_NO_DOWNSAMPLING 0x0400

typedef struct SetTimeInvoke
{
	AbsoluteTime date_time;
	intu32 accuracy;
} SetTimeInvoke;

typedef struct SegmIdList
{
	intu16 count;
	intu16 length;
	intu16 *value; /* first element of the array */
} SegmIdList;

typedef struct AbsTimeRange
{
	AbsoluteTime from_time;
	AbsoluteTime to_time;
} AbsTimeRange;

typedef struct SegmentInfo
{
	intu16 seg_inst_no;
	AttributeList seg_info;
} SegmentInfo;

typedef struct SegmentInfoList
{
	intu16 count;
	intu16 length;
	SegmentInfo *value; /* first element of the array */
} SegmentInfoList;

typedef struct SegmSelection
{
	intu16 choice;
	intu16 length;
	#define ALL_SEGMENTS_CHOSEN 0x0001
	#define SEGM_ID_LIST_CHOSEN 0x0002
	#define ABS_TIME_RANGE_CHOSEN 0x0003
	union
	{
		intu16 all_segments;
		SegmIdList segm_id_list;
		AbsTimeRange abs_time_range;
	} u;
} SegmSelection;

#define PMSC_VAR_NO_OF_SEGM 0x8000
#define PMSC_EPI_SEG_ENTRIES 0x0800
#define PMSC_PERI_SEG_ENTRIES 0x0400
#define PMSC_ABS_TIME_SELECT 0x0200
#define PMSC_CLEAR_SEGM_BY_LIST_SUP 0x0100
#define PMSC_CLEAR_SEGM_BY_TIME_SUP 0x0080
#define PMSC_CLEAR_SEGM_REMOVE 0x0040
#define PMSC_MULTI_PERSON 0x0008

#define SEG_ELEM_HDR_ABSOLUTE_TIME 0x8000
#define SEG_ELEM_HDR_RELATIVE_TIME 0x4000
#define SEG_ELEM_HDR_HIRES_RELATIVE_TIME 0x2000

typedef struct SegmEntryElem
{
	intu16 class_id;
	TYPE metric_type;
	intu16 handle;
	AttrValMap attr_val_map;
} SegmEntryElem;

typedef struct SegmEntryElemList
{
	intu16 count;
	intu16 length;
	SegmEntryElem *value; /* first element of the array */
} SegmEntryElemList;

typedef struct PmSegmentEntryMap
{
	intu16 segm_entry_header;
	SegmEntryElemList segm_entry_elem_list;
} PmSegmentEntryMap;

typedef struct SegmElemStaticAttrEntry
{
	intu16 class_id;
	TYPE metric_type;
	AttributeList attribute_list;
} SegmElemStaticAttrEntry;

typedef struct PmSegmElemStaticAttrList
{
	intu16 count;
	intu16 length;
	SegmElemStaticAttrEntry *value; /* first element of the array */
} PmSegmElemStaticAttrList;

typedef struct TrigSegmDataXferReq
{
	intu16 seg_inst_no;
} TrigSegmDataXferReq;

#define TSXR_SUCCESSFUL 0
#define TSXR_FAIL_NO_SUCH_SEGMENT 1
#define TSXR_FAIL_SEGM_TRY_LATER 2
#define TSXR_FAIL_SEGM_EMPTY 3
#define TSXR_FAIL_OTHER 512

typedef struct TrigSegmDataXferRsp
{
	intu16 seg_inst_no;
	intu16 trig_segm_xfer_rsp;
} TrigSegmDataXferRsp;

#define SEVTSTA_FIRST_ENTRY 0x8000
#define SEVTSTA_LAST_ENTRY 0x4000
#define SEVTSTA_AGENT_ABORT 0x0800
#define SEVTSTA_MANAGER_CONFIRM 0x0080
#define SEVTSTA_MANAGER_ABORT 0x0008

typedef struct SegmDataEventDescr
{
	intu16 segm_instance;
	intu32 segm_evt_entry_index;
	intu32 segm_evt_entry_count;
	intu16 segm_evt_status;
} SegmDataEventDescr;

typedef struct SegmentDataEvent
{
	SegmDataEventDescr segm_data_event_descr;
	octet_string segm_data_event_entries;
} SegmentDataEvent;

typedef struct SegmentDataResult
{
	SegmDataEventDescr segm_data_event_descr;
} SegmentDataResult;

#define SEGM_STAT_TYPE_MINIMUM 1
#define SEGM_STAT_TYPE_MAXIMUM 2
#define SEGM_STAT_TYPE_AVERAGE 3

typedef struct SegmentStatisticEntry
{
	intu16 segm_stat_type;
	octet_string segm_stat_entry;
} SegmentStatisticEntry;

typedef struct SegmentStatistics
{
	intu16 count;
	intu16 length;
	SegmentStatisticEntry *value; /* first element of the array */
} SegmentStatistics;

typedef struct ObservationScan
{
	intu16 obj_handle;
	AttributeList attributes;
} ObservationScan;


#define ASSOC_VERSION1 0x80000000

#define PROTOCOL_VERSION1 0x80000000

#define MDER 0x8000
#define XER 0x4000
#define PER 0x2000

typedef struct UUID_Ident
{
	intu8 value[16];
} UUID_Ident;

#define DATA_PROTO_ID_20601 20601
#define DATA_PROTO_ID_EXTERNAL 65535

typedef struct DataProto
{
	intu16 data_proto_id;
	Any data_proto_info;
} DataProto;

typedef struct DataProtoList
{
	intu16 count;
	intu16 length;
	DataProto *value; /* first element of the array */
} DataProtoList;

typedef struct AARQ_apdu
{
	intu32 assoc_version;
	DataProtoList data_proto_list;
} AARQ_apdu;

#define ACCEPTED 0
#define REJECTED_PERMANENT 1
#define REJECTED_TRANSIENT 2
#define ACCEPTED_UNKNOWN_CONFIG 3
#define REJECTED_NO_COMMON_PROTOCOL 4
#define REJECTED_NO_COMMON_PARAMETER 5
#define REJECTED_UNKNOWN 6
#define REJECTED_UNAUTHORIZED 7
#define REJECTED_UNSUPPORTED_ASSOC_VERSION 8

typedef struct AARE_apdu
{
	intu16 result;
	DataProto selected_data_proto;
} AARE_apdu;

#define RELEASE_REQUEST_REASON_NORMAL 0

typedef struct RLRQ_apdu
{
	intu16 reason;
} RLRQ_apdu;

#define RELEASE_RESPONSE_REASON_NORMAL 0

typedef struct RLRE_apdu
{
	intu16 reason;
} RLRE_apdu;

#define ABORT_REASON_UNDEFINED 0
#define ABORT_REASON_BUFFER_OVERFLOW 1
#define ABORT_REASON_RESPONSE_TIMEOUT 2
#define ABORT_REASON_CONFIGURATION_TIMEOUT 3

typedef struct ABRT_apdu
{
	intu16 reason;
} ABRT_apdu;

typedef octet_string PRST_apdu;


typedef struct EventReportArgumentSimple
{
	intu16 obj_handle;
	intu32 event_time;
	intu16 event_type;
	Any event_info;
} EventReportArgumentSimple;

typedef struct GetArgumentSimple
{
	intu16 obj_handle;
	AttributeIdList attribute_id_list;
} GetArgumentSimple;

#define REPLACE 0
#define ADD_VALUES 1
#define REMOVE_VALUES 2
#define SET_TO_DEFAULT 3

typedef struct AttributeModEntry
{
	intu16 modify_operator;
	AVA_Type attribute;
} AttributeModEntry;

typedef struct ModificationList
{
	intu16 count;
	intu16 length;
	AttributeModEntry *value; /* first element of the array */
} ModificationList;

typedef struct SetArgumentSimple
{
	intu16 obj_handle;
	ModificationList modification_list;
} SetArgumentSimple;

typedef struct ActionArgumentSimple
{
	intu16 obj_handle;
	intu16 action_type;
	Any action_info_args;
} ActionArgumentSimple;

typedef struct EventReportResultSimple
{
	intu16 obj_handle;
	intu32 currentTime;
	intu16 event_type;
	Any event_reply_info;
} EventReportResultSimple;

typedef struct GetResultSimple
{
	intu16 obj_handle;
	AttributeList attribute_list;
} GetResultSimple;

typedef struct TypeVer
{
	intu16 type;
	intu16 version;
} TypeVer;

typedef struct TypeVerList
{
	intu16 count;
	intu16 length;
	TypeVer *value; /* first element of the array */
} TypeVerList;

typedef struct SetResultSimple
{
	intu16 obj_handle;
	AttributeList attribute_list;
} SetResultSimple;

typedef struct ActionResultSimple
{
	intu16 obj_handle;
	intu16 action_type;
	Any action_info_args;
} ActionResultSimple;

#define NO_SUCH_OBJECT_INSTANCE 1
#define ACCESS_DENIED 2
#define NO_SUCH_ACTION 9
#define INVALID_OBJECT_INSTANCE 17
#define PROTOCOL_VIOLATION 23
#define NOT_ALLOWED_BY_OBJECT 24
#define ACTION_TIMED_OUT 25
#define ACTION_ABORTED 26

typedef struct ErrorResult
{
	intu16 error_value;
	Any parameter;
} ErrorResult;

#define UNRECOGNIZED_APDU 0
#define BADLY_STRUCTURED_APDU 2
#define UNRECOGNIZED_OPERATION 101
#define RESOURCE_LIMITATION 103
#define UNEXPECTED_ERROR 303

typedef struct RejectResult
{
	intu16 problem;
} RejectResult;

typedef union data_apdu_message_union
{
	EventReportArgumentSimple roiv_cmipEventReport;
	EventReportArgumentSimple roiv_cmipConfirmedEventReport;
	GetArgumentSimple roiv_cmipGet;
	SetArgumentSimple roiv_cmipSet;
	SetArgumentSimple roiv_cmipConfirmedSet;
	ActionArgumentSimple roiv_cmipAction;
	ActionArgumentSimple roiv_cmipConfirmedAction;
	EventReportResultSimple rors_cmipConfirmedEventReport;
	GetResultSimple rors_cmipGet;
	SetResultSimple rors_cmipConfirmedSet;
	ActionResultSimple rors_cmipConfirmedAction;
	ErrorResult roer;
	RejectResult rorj;
} data_apdu_message_union;

typedef struct DATA_apdu_message
{
	intu16 choice;
	intu16 length;
	#define ROIV_CMIP_EVENT_REPORT_CHOSEN 0x0100
	#define ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN 0x0101
	#define ROIV_CMIP_GET_CHOSEN 0x0103
	#define ROIV_CMIP_SET_CHOSEN 0x0104
	#define ROIV_CMIP_CONFIRMED_SET_CHOSEN 0x0105
	#define ROIV_CMIP_ACTION_CHOSEN 0x0106
	#define ROIV_CMIP_CONFIRMED_ACTION_CHOSEN 0x0107
	#define RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN 0x0201
	#define RORS_CMIP_GET_CHOSEN 0x0203
	#define RORS_CMIP_CONFIRMED_SET_CHOSEN 0x0205
	#define RORS_CMIP_CONFIRMED_ACTION_CHOSEN 0x0207
	#define ROER_CHOSEN 0x0300
	#define RORJ_CHOSEN 0x0400
	data_apdu_message_union u;
} DATA_apdu_message;

typedef struct DATA_apdu
{
	intu16 invoke_id;
	DATA_apdu_message message;
} DATA_apdu;

typedef struct APDU
{
	intu16 choice;
	intu16 length;
	#define AARQ_CHOSEN 0xE200
	#define AARE_CHOSEN 0xE300
	#define RLRQ_CHOSEN 0xE400
	#define RLRE_CHOSEN 0xE500
	#define ABRT_CHOSEN 0xE600
	#define PRST_CHOSEN 0xE700
	union
	{
		AARQ_apdu aarq;
		AARE_apdu aare;
		RLRQ_apdu rlrq;
		RLRE_apdu rlre;
		ABRT_apdu abrt;
		PRST_apdu prst;
	} u;
} APDU;

#define NOM_VERSION1 0x80000000

#define FUN_UNITS_UNIDIRECTIONAL 0x80000000
#define FUN_UNITS_HAVETESTCAP 0x40000000
#define FUN_UNITS_CREATETESTASSOC 0x20000000

#define SYS_TYPE_MANAGER 0x80000000
#define SYS_TYPE_AGENT 0x00800000

#define MANAGER_CONFIG_RESPONSE 0x0000
#define STANDARD_CONFIG_START 0x0001
#define STANDARD_CONFIG_END 0x3FFF
#define EXTENDED_CONFIG_START 0x4000
#define EXTENDED_CONFIG_END 0x7FFF
#define RESERVED_START 0x8000
#define RESERVED_END 0xFFFF

#define DATA_REQ_SUPP_STOP 0x8000
#define DATA_REQ_SUPP_SCOPE_ALL 0x0800
#define DATA_REQ_SUPP_SCOPE_CLASS 0x0400
#define DATA_REQ_SUPP_SCOPE_ASN1_HANDLE 0x0200
#define DATA_REQ_SUPP_MODE_SINGLE_RSP 0x0080
#define DATA_REQ_SUPP_MODE_TIME_PERIOD 0x0040
#define DATA_REQ_SUPP_MODE_TIME_NO_LIMIT 0x0020
#define DATA_REQ_SUPP_PERSON_ID 0x0010
#define DATA_REQ_SUPP_INIT_AGENT 0x0001

typedef struct DataReqModeCapab
{
	intu16 data_req_mode_flags;
	intu8 data_req_init_agent_count;
	intu8 data_req_init_manager_count;
} DataReqModeCapab;

typedef struct PhdAssociationInformation
{
	intu32 protocolVersion;
	intu16 encodingRules;
	intu32 nomenclatureVersion;
	intu32 functionalUnits;
	intu32 systemType;
	octet_string system_id;
	intu16 dev_config_id;
	DataReqModeCapab data_req_mode_capab;
	AttributeList optionList;
} PhdAssociationInformation;

typedef struct ManufSpecAssociationInformation
{
	UUID_Ident data_proto_id_ext;
	Any data_proto_info_ext;
} ManufSpecAssociationInformation;

#define MDS_TIME_CAPAB_REAL_TIME_CLOCK 0x8000
#define MDS_TIME_CAPAB_SET_CLOCK 0x4000
#define MDS_TIME_CAPAB_RELATIVE_TIME 0x2000
#define MDS_TIME_CAPAB_HIGH_RES_RELATIVE_TIME 0x1000
#define MDS_TIME_CAPAB_SYNC_ABS_TIME 0x0800
#define MDS_TIME_CAPAB_SYNC_REL_TIME 0x0400
#define MDS_TIME_CAPAB_SYNC_HI_RES_RELATIVE_TIME 0x0200
#define MDS_TIME_STATE_ABS_TIME_SYNCED 0x0080
#define MDS_TIME_STATE_REL_TIME_SYNCED 0x0040
#define MDS_TIME_STATE_HI_RES_RELATIVE_TIME_SYNCED 0x0020
#define MDS_TIME_MGR_SET_TIME 0x0010

typedef struct MdsTimeInfo
{
	intu16 mds_time_cap_state;
	intu16 time_sync_protocol;
	intu32 time_sync_accuracy;
	intu16 time_resolution_abs_time;
	intu16 time_resolution_rel_time;
	intu32 time_resolution_high_res_time;
} MdsTimeInfo;

typedef octet_string EnumPrintableString;

#define UNKNOWN_PERSON_ID 0xFFFF

#define MSS_AVAIL_INTERMITTENT 0x8000
#define MSS_AVAIL_STORED_DATA 0x4000
#define MSS_UPD_APERIODIC 0x2000
#define MSS_MSMT_APERIODIC 0x1000
#define MSS_MSMT_PHYS_EV_ID 0x0800
#define MSS_MSMT_BTB_METRIC 0x0400
#define MSS_ACC_MANAGER_INITIATED 0x0080
#define MSS_ACC_AGENT_INITIATED 0x0040
#define MSS_CAT_MANUAL 0x0008
#define MSS_CAT_SETTING 0x0004
#define MSS_CAT_CALCULATION 0x0002

typedef struct MetricStructureSmall
{
	intu8 ms_struct;
	#define MS_STRUCT_SIMPLE 0
	#define MS_STRUCT_COMPOUND 1
	#define MS_STRUCT_RESERVED 2
	#define MS_STRUCT_COMPOUND_FIX 3
	intu8 ms_comp_no;
} MetricStructureSmall;

typedef struct MetricIdList
{
	intu16 count;
	intu16 length;
	intu16 *value; /* first element of the array */
} MetricIdList;

typedef struct SupplementalTypeList
{
	intu16 count;
	intu16 length;
	TYPE *value; /* first element of the array */
} SupplementalTypeList;

typedef struct ObservationScanList
{
	intu16 count;
	intu16 length;
	ObservationScan *value; /* first element of the array */
} ObservationScanList;

typedef struct ScanReportPerVar
{
	intu16 person_id;
	ObservationScanList obs_scan_var;
} ScanReportPerVar;

typedef struct ScanReportPerVarList
{
	intu16 count;
	intu16 length;
	ScanReportPerVar *value; /* first element of the array */
} ScanReportPerVarList;

#define DATA_REQ_ID_MANAGER_INITIATED_MIN 0x0000
#define DATA_REQ_ID_MANAGER_INITIATED_MAX 0xEFFF
#define DATA_REQ_ID_AGENT_INITIATED 0xF000

typedef struct ScanReportInfoMPVar
{
	intu16 data_req_id;
	intu16 scan_report_no;
	ScanReportPerVarList scan_per_var;
} ScanReportInfoMPVar;

typedef struct ObservationScanFixed
{
	intu16 obj_handle;
	octet_string obs_val_data;
} ObservationScanFixed;

typedef struct ObservationScanFixedList
{
	intu16 count;
	intu16 length;
	ObservationScanFixed *value; /* first element of the array */
} ObservationScanFixedList;

typedef struct ScanReportPerFixed
{
	intu16 person_id;
	ObservationScanFixedList obs_scan_fix;
} ScanReportPerFixed;

typedef struct ScanReportPerFixedList
{
	intu16 count;
	intu16 length;
	ScanReportPerFixed *value; /* first element of the array */
} ScanReportPerFixedList;

typedef struct ScanReportInfoMPFixed
{
	intu16 data_req_id;
	intu16 scan_report_no;
	ScanReportPerFixedList scan_per_fixed;
} ScanReportInfoMPFixed;

typedef struct ScanReportInfoVar
{
	intu16 data_req_id;
	intu16 scan_report_no;
	ObservationScanList obs_scan_var;
} ScanReportInfoVar;

typedef struct ScanReportInfoFixed
{
	intu16 data_req_id;
	intu16 scan_report_no;
	ObservationScanFixedList obs_scan_fixed;
} ScanReportInfoFixed;

typedef octet_string ObservationScanGrouped;

typedef struct ScanReportInfoGroupedList
{
	intu16 count;
	intu16 length;
	ObservationScanGrouped *value; /* first element of the array */
} ScanReportInfoGroupedList;

typedef struct ScanReportInfoGrouped
{
	intu16 data_req_id;
	intu16 scan_report_no;
	ScanReportInfoGroupedList obs_scan_grouped;
} ScanReportInfoGrouped;

typedef struct ScanReportPerGrouped
{
	intu16 person_id;
	ObservationScanGrouped obs_scan_grouped;
} ScanReportPerGrouped;

typedef struct ScanReportPerGroupedList
{
	intu16 count;
	intu16 length;
	ScanReportPerGrouped *value; /* first element of the array */
} ScanReportPerGroupedList;

typedef struct ScanReportInfoMPGrouped
{
	intu16 data_req_id;
	intu16 scan_report_no;
	ScanReportPerGroupedList scan_per_grouped;
} ScanReportInfoMPgrouped;

typedef struct ConfigObject
{
	intu16 obj_class;
	intu16 obj_handle;
	AttributeList attributes;
} ConfigObject;

typedef struct ConfigObjectList
{
	intu16 count;
	intu16 length;
	ConfigObject *value; /* first element of the array */
} ConfigObjectList;

typedef struct ConfigReport
{
	intu16 config_report_id;
	ConfigObjectList config_obj_list;
} ConfigReport;

#define ACCEPTED_CONFIG 0x0000
#define UNSUPPORTED_CONFIG 0x0001
#define STANDARD_CONFIG_UNKNOWN 0x0002

typedef struct ConfigReportRsp
{
	intu16 config_report_id;
	intu16 config_result;
} ConfigReportRsp;

#define DATA_REQ_START_STOP 0x8000
#define DATA_REQ_CONTINUATION 0x4000
#define DATA_REQ_SCOPE_ALL 0x0800
#define DATA_REQ_SCOPE_TYPE 0x0400
#define DATA_REQ_SCOPE_ASN1_HANDLE 0x0200
#define DATA_REQ_MODE_SINGLE_RSP 0x0080
#define DATA_REQ_MODE_TIME_PERIOD 0x0040
#define DATA_REQ_MODE_TIME_NO_LIMIT 0x0020
#define DATA_REQ_MODE_DATA_REQ_PERSON_ID 0x0008

typedef struct ASN1_HANDLEList
{
	intu16 count;
	intu16 length;
	intu16 *value; /* first element of the array */
} ASN1_HANDLEList;

typedef struct DataRequest
{
	intu16 data_req_id;
	intu16 data_req_mode;
	intu32 data_req_time;
	intu16 DataRequest_data_req_person_id;
	intu16 data_req_class;
	ASN1_HANDLEList data_req_obj_handle_list;
} DataRequest;

#define DATA_REQ_RESULT_NO_ERROR 0
#define DATA_REQ_RESULT_UNSPECIFIC_ERROR 1
#define DATA_REQ_RESULT_NO_STOP_SUPPORT 2
#define DATA_REQ_RESULT_NO_SCOPE_ALL_SUPPORT 3
#define DATA_REQ_RESULT_NO_SCOPE_CLASS_SUPPORT 4
#define DATA_REQ_RESULT_NO_SCOPE_ASN1_HANDLE_SUPPORT 5
#define DATA_REQ_RESULT_NO_MODE_SINGLE_RSP_SUPPORT 6
#define DATA_REQ_RESULT_NO_MODE_TIME_PERIOD_SUPPORT 7
#define DATA_REQ_RESULT_NO_MODE_TIME_NO_LIMIT_SUPPORT 8
#define DATA_REQ_RESULT_NO_PERSON_ID_SUPPORT 9
#define DATA_REQ_RESULT_UNKNOWN_PERSON_ID 11
#define DATA_REQ_RESULT_UNKNOWN_CLASS 12
#define DATA_REQ_RESULT_UNKNOWN_ASN1_HANDLE 13
#define DATA_REQ_RESULT_UNSUPP_SCOPE 14
#define DATA_REQ_RESULT_UNSUPP_MODE 15
#define DATA_REQ_RESULT_INIT_MANAGER_OVERFLOW 16
#define DATA_REQ_RESULT_CONTINUATION_NOT_SUPPORTED 17
#define DATA_REQ_RESULT_INVALID_REQ_ID 18

typedef struct DataResponse
{
	intu32 rel_time_stamp;
	intu16 data_req_result;
	intu16 event_type;
	Any event_info;
} DataResponse;


typedef struct SimpleNuObsValueCmp
{
	intu16 count;
	intu16 length;
	intu32 *value; /* first element of the array */
} SimpleNuObsValueCmp;


typedef struct BasicNuObsValueCmp
{
	intu16 count;
	intu16 length;
	intu16 *value; /* first element of the array */
} BasicNuObsValueCmp;

#endif /* PHD_TYPES */