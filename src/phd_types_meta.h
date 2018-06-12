#ifndef PHD_TYPES_META
#define PHD_TYPES_META

#include "phd_types.h"

#define MEMBER_TYPE 1
#define IU8_PTR_TYPE 2
#define IU16_PTR_TYPE 3
#define IU32_PTR_TYPE 4
#define STRUCT_TYPE 5
#define STRUCT_PTR_TYPE 6
#define ENUM_UNION 7
#define SEGM_UNION 8
#define MESG_UNION 9
#define APDU_UNION 10

#define STRUCT_NAME Any
#define STRUCT_FIELDS \
	X(intu16, length, MEMBER_TYPE) \
	X(intu8*, value, IU8_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME TYPE
#define STRUCT_FIELDS \
	X(intu16, partition, MEMBER_TYPE) \
	X(intu16, code, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AVA_Type
#define STRUCT_FIELDS \
	X(intu16, attribute_id, MEMBER_TYPE) \
	X(Any, attribute_value, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AttributeList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(AVA_Type*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AttributeIdList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(intu16*, value, IU16_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME HighResRelativeTime
#define STRUCT_FIELDS \
	X(intu8, value, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AbsoluteTimeAdjust
#define STRUCT_FIELDS \
	X(intu8, value, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AbsoluteTime
#define STRUCT_FIELDS \
	X(intu8, century, MEMBER_TYPE) \
	X(intu8, year, MEMBER_TYPE) \
	X(intu8, month, MEMBER_TYPE) \
	X(intu8, day, MEMBER_TYPE) \
	X(intu8, hour, MEMBER_TYPE) \
	X(intu8, minute, MEMBER_TYPE) \
	X(intu8, second, MEMBER_TYPE) \
	X(intu8, sec_fractions, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME octet_string
#define STRUCT_FIELDS \
	X(intu16, length, MEMBER_TYPE) \
	X(intu8*, value, IU8_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SystemModel
#define STRUCT_FIELDS \
	X(octet_string, manufacturer, STRUCT_TYPE) \
	X(octet_string, model_number, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ProdSpecEntry
#define STRUCT_FIELDS \
	X(intu16, spec_type, MEMBER_TYPE) \
	X(intu16, component_id, MEMBER_TYPE) \
	X(octet_string, prod_spec, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ProductionSpec
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(ProdSpecEntry*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME BatMeasure
#define STRUCT_FIELDS \
	X(intu32, value, MEMBER_TYPE) \
	X(intu16, unit, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME NuObsValue
#define STRUCT_FIELDS \
	X(intu16, metric_id, MEMBER_TYPE) \
	X(intu16, state, MEMBER_TYPE) \
	X(intu16, unit_code, MEMBER_TYPE) \
	X(intu32, value, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME NuObsValueCmp
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(NuObsValue*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SampleType
#define STRUCT_FIELDS \
	X(intu8, sample_size, MEMBER_TYPE) \
	X(intu8, significant_bits, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SaSpec
#define STRUCT_FIELDS \
	X(intu16, array_size, MEMBER_TYPE) \
	X(SampleType, sample_type, STRUCT_TYPE) \
	X(intu16, flags, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScaleRangeSpec8
#define STRUCT_FIELDS \
	X(intu32, lower_absolute_value, MEMBER_TYPE) \
	X(intu32, upper_absolute_value, MEMBER_TYPE) \
	X(intu8, lower_scaled_value, MEMBER_TYPE) \
	X(intu8, upper_scaled_value, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScaleRangeSpec16
#define STRUCT_FIELDS \
	X(intu32, lower_absolute_value, MEMBER_TYPE) \
	X(intu32, upper_absolute_value, MEMBER_TYPE) \
	X(intu16, lower_scaled_value, MEMBER_TYPE) \
	X(intu16, upper_scaled_value, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScaleRangeSpec32
#define STRUCT_FIELDS \
	X(intu32, lower_absolute_value, MEMBER_TYPE) \
	X(intu32, upper_absolute_value, MEMBER_TYPE) \
	X(intu32, lower_scaled_value, MEMBER_TYPE) \
	X(intu32, upper_scaled_value, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME EnumVal
#define STRUCT_FIELDS \
	X(intu16, choice, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X( \
	union \
	{ \
		intu16 enum_obj_id; \
		octet_string enum_text_string; \
		intu32 enum_bit_str; \
	}, u, ENUM_UNION)
#include "fmtgen.h"

#define STRUCT_NAME EnumObsValue
#define STRUCT_FIELDS \
	X(intu16, metric_id, MEMBER_TYPE) \
	X(intu16, state, MEMBER_TYPE) \
	X(EnumVal, value, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AttrValMapEntry
#define STRUCT_FIELDS \
	X(intu16, attribute_id, MEMBER_TYPE) \
	X(intu16, attribute_len, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AttrValMap
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(AttrValMapEntry*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME HandleAttrValMapEntry
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(AttrValMap, attr_val_map, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME HandleAttrValMap
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(HandleAttrValMapEntry*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SetTimeInvoke
#define STRUCT_FIELDS \
	X(AbsoluteTime, date_time, STRUCT_TYPE) \
	X(intu32, accuracy, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmIdList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(intu16*, value, IU16_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AbsTimeRange
#define STRUCT_FIELDS \
	X(AbsoluteTime, from_time, STRUCT_TYPE) \
	X(AbsoluteTime, to_time, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmentInfo
#define STRUCT_FIELDS \
	X(intu16, seg_inst_no, MEMBER_TYPE) \
	X(AttributeList, seg_info, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmentInfoList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(SegmentInfo*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmSelection
#define STRUCT_FIELDS \
	X(intu16, choice, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X( \
	union \
	{ \
		intu16 all_segments; \
		SegmIdList segm_id_list; \
		AbsTimeRange abs_time_range; \
	}, u, SEGM_UNION)
#include "fmtgen.h"

#define STRUCT_NAME SegmEntryElem
#define STRUCT_FIELDS \
	X(intu16, class_id, MEMBER_TYPE) \
	X(TYPE, metric_type, STRUCT_TYPE) \
	X(intu16, handle, MEMBER_TYPE) \
	X(AttrValMap, attr_val_map, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmEntryElemList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(SegmEntryElem*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME PmSegmentEntryMap
#define STRUCT_FIELDS \
	X(intu16, segm_entry_header, MEMBER_TYPE) \
	X(SegmEntryElemList, segm_entry_elem_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmElemStaticAttrEntry
#define STRUCT_FIELDS \
	X(intu16, class_id, MEMBER_TYPE) \
	X(TYPE, metric_type, STRUCT_TYPE) \
	X(AttributeList, attribute_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME PmSegmElemStaticAttrList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(SegmElemStaticAttrEntry*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME TrigSegmDataXferReq
#define STRUCT_FIELDS \
	X(intu16, seg_inst_no, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME TrigSegmDataXferRsp
#define STRUCT_FIELDS \
	X(intu16, seg_inst_no, MEMBER_TYPE) \
	X(intu16, trig_segm_xfer_rsp, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmDataEventDescr
#define STRUCT_FIELDS \
	X(intu16, segm_instance, MEMBER_TYPE) \
	X(intu32, segm_evt_entry_index, MEMBER_TYPE) \
	X(intu32, segm_evt_entry_count, MEMBER_TYPE) \
	X(intu16, segm_evt_status, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmentDataEvent
#define STRUCT_FIELDS \
	X(SegmDataEventDescr, segm_data_event_descr, STRUCT_TYPE) \
	X(octet_string, segm_data_event_entries, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmentDataResult
#define STRUCT_FIELDS \
	X(SegmDataEventDescr, segm_data_event_descr, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmentStatisticEntry
#define STRUCT_FIELDS \
	X(intu16, segm_stat_type, MEMBER_TYPE) \
	X(octet_string, segm_stat_entry, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SegmentStatistics
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(SegmentStatisticEntry*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ObservationScan
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(AttributeList, attributes, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME UUID_Ident
#define STRUCT_FIELDS \
	X(intu8, value, MEMBER_TYPE)
#include "fmtgen.h"

#define DATA_PROTO_ID_20601 20601
#define DATA_PROTO_ID_EXTERNAL 65535

#define STRUCT_NAME DataProto
#define STRUCT_FIELDS \
	X(intu16, data_proto_id, MEMBER_TYPE) \
	X(Any, data_proto_info, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME DataProtoList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(DataProto*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AARQ_apdu
#define STRUCT_FIELDS \
	X(intu32, assoc_version, MEMBER_TYPE) \
	X(DataProtoList, data_proto_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AARE_apdu
#define STRUCT_FIELDS \
	X(intu16, result, MEMBER_TYPE) \
	X(DataProto, selected_data_proto, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME RLRQ_apdu
#define STRUCT_FIELDS \
	X(intu16, reason, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME RLRE_apdu
#define STRUCT_FIELDS \
	X(intu16, reason, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ABRT_apdu
#define STRUCT_FIELDS \
	X(intu16, reason, MEMBER_TYPE)
#include "fmtgen.h"

typedef octet_string PRST_apdu;

#define STRUCT_NAME EventReportArgumentSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(intu32, event_time, MEMBER_TYPE) \
	X(intu16, event_type, MEMBER_TYPE) \
	X(Any, event_info, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME GetArgumentSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(AttributeIdList, attribute_id_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME AttributeModEntry
#define STRUCT_FIELDS \
	X(intu16, modify_operator, MEMBER_TYPE) \
	X(AVA_Type, attribute, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ModificationList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(AttributeModEntry*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SetArgumentSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(ModificationList, modification_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ActionArgumentSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(intu16, action_type, MEMBER_TYPE) \
	X(Any, action_info_args, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME EventReportResultSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(intu32, currentTime, MEMBER_TYPE) \
	X(intu16, event_type, MEMBER_TYPE) \
	X(Any, event_reply_info, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME GetResultSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(AttributeList, attribute_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME TypeVer
#define STRUCT_FIELDS \
	X(intu16, type, MEMBER_TYPE) \
	X(intu16, version, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME TypeVerList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(TypeVer*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SetResultSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(AttributeList, attribute_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ActionResultSimple
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(intu16, action_type, MEMBER_TYPE) \
	X(Any, action_info_args, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ErrorResult
#define STRUCT_FIELDS \
	X(intu16, error_value, MEMBER_TYPE) \
	X(Any, parameter, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME RejectResult
#define STRUCT_FIELDS \
	X(intu16, problem, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME DATA_apdu_message
#define STRUCT_FIELDS \
	X(intu16, choice, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X( \
	union \
	{ \
		EventReportArgumentSimple roiv_cmipEventReport; \
		EventReportArgumentSimple roiv_cmipConfirmedEventReport; \
		GetArgumentSimple roiv_cmipGet; \
		SetArgumentSimple roiv_cmipSet; \
		SetArgumentSimple roiv_cmipConfirmedSet; \
		ActionArgumentSimple roiv_cmipAction; \
		ActionArgumentSimple roiv_cmipConfirmedAction; \
		EventReportResultSimple rors_cmipConfirmedEventReport; \
		GetResultSimple rors_cmipGet; \
		SetResultSimple rors_cmipConfirmedSet; \
		ActionResultSimple rors_cmipConfirmedAction; \
		ErrorResult roer; \
		RejectResult rorj; \
	}, u, MESG_UNION)	
#include "fmtgen.h"

#define STRUCT_NAME DATA_apdu
#define STRUCT_FIELDS \
	X(intu16, invoke_id, MEMBER_TYPE) \
	X(DATA_apdu_message, message, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME APDU
#define STRUCT_FIELDS \
	X(intu16, choice, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X( \
	union \
	{ \
		AARQ_apdu aarq; \
		AARE_apdu aare; \
		RLRQ_apdu rlrq; \
		RLRE_apdu rlre; \
		ABRT_apdu abrt; \
		PRST_apdu prst; \
	}, u, APDU_UNION)
#include "fmtgen.h"

#define STRUCT_NAME DataReqModeCapab
#define STRUCT_FIELDS \
	X(intu16, data_req_mode_flags, MEMBER_TYPE) \
	X(intu8, data_req_init_agent_count, MEMBER_TYPE) \
	X(intu8, data_req_init_manager_count, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME PhdAssociationInformation
#define STRUCT_FIELDS \
	X(intu32, protocolVersion, MEMBER_TYPE) \
	X(intu16, encodingRules, MEMBER_TYPE) \
	X(intu32, nomenclatureVersion, MEMBER_TYPE) \
	X(intu32, functionalUnits, MEMBER_TYPE) \
	X(intu32, systemType, MEMBER_TYPE) \
	X(octet_string, system_id, STRUCT_TYPE) \
	X(intu16, dev_config_id, MEMBER_TYPE) \
	X(DataReqModeCapab, data_req_mode_capab, STRUCT_TYPE) \
	X(AttributeList, optionList, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ManufSpecAssociationInformation
#define STRUCT_FIELDS \
	X(UUID_Ident, data_proto_id_ext, STRUCT_TYPE) \
	X(Any, data_proto_info_ext, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME MdsTimeInfo
#define STRUCT_FIELDS \
	X(intu16, mds_time_cap_state, MEMBER_TYPE) \
	X(intu16, time_sync_protocol, MEMBER_TYPE) \
	X(intu32, time_sync_accuracy, MEMBER_TYPE) \
	X(intu16, time_resolution_abs_time, MEMBER_TYPE) \
	X(intu16, time_resolution_rel_time, MEMBER_TYPE) \
	X(intu32, time_resolution_high_res_time, MEMBER_TYPE) 
#include "fmtgen.h"

#define STRUCT_NAME MetricStructureSmall
#define STRUCT_FIELDS \
	X(intu8, ms_struct, MEMBER_TYPE) \
	X(intu8, ms_comp_no, MEMBER_TYPE) 
#include "fmtgen.h"

#define STRUCT_NAME MetricIdList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(intu16*, value, IU16_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SupplementalTypeList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(TYPE*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ObservationScanList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(ObservationScan*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportPerVar
#define STRUCT_FIELDS \
	X(intu16, person_id, MEMBER_TYPE) \
	X(ObservationScanList, obs_scan_var, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportPerVarList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(ScanReportPerVar*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportInfoMPVar
#define STRUCT_FIELDS \
	X(intu16, data_req_id, MEMBER_TYPE) \
	X(intu16, scan_report_no, MEMBER_TYPE) \
	X(ScanReportPerVarList, scan_per_var, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ObservationScanFixed
#define STRUCT_FIELDS \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(octet_string, obs_val_data, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ObservationScanFixedList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(ObservationScanFixed*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportPerFixed
#define STRUCT_FIELDS \
	X(intu16, person_id, MEMBER_TYPE) \
	X(ObservationScanFixedList, obs_scan_fix, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportPerFixedList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(ScanReportPerFixed*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportInfoMPFixed
#define STRUCT_FIELDS \
	X(intu16, data_req_id, MEMBER_TYPE) \
	X(intu16, scan_report_no, MEMBER_TYPE) \
	X(ScanReportPerFixedList, scan_per_fixed, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportInfoVar
#define STRUCT_FIELDS \
	X(intu16, data_req_id, MEMBER_TYPE) \
	X(intu16, scan_report_no, MEMBER_TYPE) \
	X(ObservationScanList, obs_scan_var, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportInfoFixed
#define STRUCT_FIELDS \
	X(intu16, data_req_id, MEMBER_TYPE) \
	X(intu16, scan_report_no, MEMBER_TYPE) \
	X(ObservationScanFixedList, obs_scan_fixed, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportInfoGroupedList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(octet_string*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportInfoGrouped
#define STRUCT_FIELDS \
	X(intu16, data_req_id, MEMBER_TYPE) \
	X(intu16, scan_report_no, MEMBER_TYPE) \
	X(ScanReportInfoGroupedList, obs_scan_grouped, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportPerGrouped
#define STRUCT_FIELDS \
	X(intu16, person_id, MEMBER_TYPE) \
	X(ObservationScanGrouped, obs_scan_grouped, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportPerGroupedList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(ScanReportPerGrouped*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ScanReportInfoMPGrouped
#define STRUCT_FIELDS \
	X(intu16, data_req_id, MEMBER_TYPE) \
	X(intu16, scan_report_no, MEMBER_TYPE) \
	X(ScanReportPerGroupedList, scan_per_grouped, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ConfigObject
#define STRUCT_FIELDS \
	X(intu16, obj_class, MEMBER_TYPE) \
	X(intu16, obj_handle, MEMBER_TYPE) \
	X(AttributeList, attributes, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ConfigObjectList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(ConfigObject*, value, STRUCT_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ConfigReport
#define STRUCT_FIELDS \
	X(intu16, config_report_id, MEMBER_TYPE) \
	X(ConfigObjectList, config_obj_list, STRUCT_TYPE) 
#include "fmtgen.h"

#define STRUCT_NAME ConfigReportRsp
#define STRUCT_FIELDS \
	X(intu16, config_report_id, MEMBER_TYPE) \
	X(intu16, config_result, MEMBER_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME ASN1_HANDLEList
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(intu16*, value, IU16_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME DataRequest
#define STRUCT_FIELDS \
	X(intu16, data_req_id, MEMBER_TYPE) \
	X(intu16, data_req_mode, MEMBER_TYPE) \
	X(intu32, data_req_time, MEMBER_TYPE) \
	X(intu16, DataRequest_data_req_person_id, MEMBER_TYPE) \
	X(intu16, data_req_class, MEMBER_TYPE) \
	X(ASN1_HANDLEList, data_req_obj_handle_list, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME DataResponse
#define STRUCT_FIELDS \
	X(intu32, rel_time_stamp, MEMBER_TYPE) \
	X(intu16, data_req_result, MEMBER_TYPE) \
	X(intu16, event_type, MEMBER_TYPE) \
	X(Any, event_info, STRUCT_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME SimpleNuObsValueCmp
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(intu32*, value, IU32_PTR_TYPE)
#include "fmtgen.h"

#define STRUCT_NAME BasicNuObsValueCmp
#define STRUCT_FIELDS \
	X(intu16, count, MEMBER_TYPE) \
	X(intu16, length, MEMBER_TYPE) \
	X(intu16*, value, IU16_PTR_TYPE)
#include "fmtgen.h"

#endif /* PHD_TYPES_META */