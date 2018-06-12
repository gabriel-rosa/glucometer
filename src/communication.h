#ifndef COMMUNICATION
#define COMMUNICATION

#include "log.h"
#include "phd_types.h"
#include "phd_types_meta.h"
#include "mdc_types.h"
#include "serialization.h"
#include "usb_interface.h"

typedef enum {
	UNASSOCIATED,
	ASSOCIATING,
	CONFIGURING,
	OPERATING, 
	DISASSOCIATING,
	ABORTING
} state_type;

#define MAX_TRANSMISSION_ATTEMPTS 3
int transmission_attempts = 0;

// default system id
static const intu8 default_mgr_system_id[] = { 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
static const intu16 default_mgr_system_id_len = 8;

int check_association_request(
	phd_device *phd_device, 
	unsigned char *data_buffer, 
	int len) 
{
	// interpret association request
	APDU *received_apdu;
	struct_unpack(&received_apdu, "APDU", data_buffer);

	if (received_apdu->choice != AARQ_CHOSEN)
		return -1;

	unsigned char *buffer_info = ((APDU*)received_apdu)->u.aarq.data_proto_list.value->data_proto_info.value;
	PhdAssociationInformation *request_info;
	struct_unpack(&request_info, "PhdAssociationInformation", buffer_info);

	if ((request_info->protocolVersion != PROTOCOL_VERSION1) ||
		(request_info->encodingRules != MDER) ||
		(request_info->nomenclatureVersion != NOM_VERSION1) ||
		(request_info->systemType != SYS_TYPE_AGENT)) 
	{
		struct_free(request_info, "PhdAssociationInformation");
		struct_free(received_apdu, "APDU");
		return REJECTED_UNKNOWN;		
	}

	// get id
	phd_device->system_id_len = request_info->system_id.length;
	phd_device->system_id = (char*) malloc(phd_device->system_id_len * sizeof(char));
	memcpy(phd_device->system_id, request_info->system_id.value, phd_device->system_id_len);

	// see if it's a known id
	// if it is, check if config id matches
	int result = ACCEPTED_UNKNOWN_CONFIG;

	// check config id
	int config_id = request_info->dev_config_id;

	// maybe read capabilities and options?

	struct_free(request_info, "PhdAssociationInformation");
	struct_free(received_apdu, "APDU");

	return result;
}

int send_association_response(
	phd_device *phd_device,
	unsigned short result) 
{
	PhdAssociationInformation response_info;
	response_info.protocolVersion = ASSOC_VERSION1;
	response_info.encodingRules = MDER;
	response_info.nomenclatureVersion = NOM_VERSION1;
	response_info.functionalUnits = 0x00000000;
	response_info.systemType = SYS_TYPE_MANAGER;
	response_info.system_id.value = default_mgr_system_id;
	response_info.system_id.length = default_mgr_system_id_len;
	response_info.dev_config_id = MANAGER_CONFIG_RESPONSE;
	response_info.data_req_mode_capab.data_req_mode_flags = 0x0000;
	response_info.data_req_mode_capab.data_req_init_agent_count = 0x00;
	response_info.data_req_mode_capab.data_req_init_manager_count = 0x00;
	response_info.optionList.count = 0;
	response_info.optionList.length = 0;

	unsigned char *response_info_buff = calloc(38, sizeof(unsigned char));
	struct_pack(&response_info, "PhdAssociationInformation", response_info_buff);

	AARE_apdu aare_apdu;
	aare_apdu.result = result;
	aare_apdu.selected_data_proto.data_proto_id = DATA_PROTO_ID_20601;
	aare_apdu.selected_data_proto.data_proto_info.length = 38;
	aare_apdu.selected_data_proto.data_proto_info.value = response_info_buff;

	APDU response_apdu;
	response_apdu.choice = AARE_CHOSEN;
	response_apdu.u.aare = aare_apdu;
	response_apdu.length = 44;

	unsigned char *response_apdu_buff = calloc(response_apdu.length + 4, sizeof(unsigned char));
	struct_pack(&response_apdu, "APDU", response_apdu_buff);

	int sent = send_data(phd_device, response_apdu_buff, (response_apdu.length + 4)*sizeof(unsigned char));

	free(response_info_buff);
	free(response_apdu_buff);

	if (sent != (response_apdu.length + 4)*sizeof(unsigned char))
		return 0;

	return 1;
}

int check_configuration(
	phd_device *phd_device, 
	unsigned char *data_buffer, 
	int len, 
	unsigned short *config_id,
	unsigned short *invoke_id) 
{
	// interpret configuration
	APDU *received_apdu;
	struct_unpack(&received_apdu, "APDU", data_buffer);

	if (received_apdu->choice != PRST_CHOSEN)
		return -1;

	unsigned char *buffer_data_apdu = received_apdu->u.prst.value;
	DATA_apdu *data_apdu;
	struct_unpack(&data_apdu, "DATA_apdu", buffer_data_apdu);

	*invoke_id = data_apdu->invoke_id;

	unsigned char *buffer_config_report = data_apdu->message.u.roiv_cmipConfirmedEventReport.event_info.value;
	ConfigReport *config_report;
	struct_unpack(&config_report, "ConfigReport", buffer_config_report);

	*config_id = config_report->config_report_id;

	unsigned char *config_buffer = config_report->config_obj_list.value;

	fprintf(stdout, "Dumping config report: 0x%04x \n", config_report->config_report_id);
	fprintf(stdout, "-----------------------------------------------\n");
	for (int i = 0; i < config_report->config_obj_list.count; ++i) {
		ConfigObject *config = (ConfigObject*)config_buffer + i;
		fprintf(stdout, "%i: Config object of class = %i - handle = 0x%04x \n", i, config->obj_class, config->obj_handle);

		for (int j = 0; j < config->attributes.count; ++j) {
			AVA_Type *attribute = (AVA_Type*)config->attributes.value + j;
			fprintf(stdout, "attribute-id = %i - ", attribute->attribute_id);
			fprintf(stdout, "attribute-val = ");
			unsigned char *attribute_val_buffer = attribute->attribute_value.value;
			for (int k = 0; k < attribute->attribute_value.length; ++k) {
				
				fprintf(stdout, "0x%02x ", attribute_val_buffer[k]);
			}
			fprintf(stdout, "\n");

			if (attribute->attribute_id == 2645) {
				int map_count = (attribute_val_buffer[0] << 8) + attribute_val_buffer[1];
				for (int m = 0; m < map_count; ++m) {
					AttrValMapEntry entry;
					entry.attribute_id = (attribute_val_buffer[4 + 4 * m + 0] << 8) + attribute_val_buffer[4 + 4 * m + 1];
					entry.attribute_len = (attribute_val_buffer[4 + 4 * m + 2] << 8) + attribute_val_buffer[4 + 4 * m + 3];
					fprintf(stdout, "\t %i: attribute_id = %i - attribute_len = %i \n", m, entry.attribute_id, entry.attribute_len);
				}				
			}
		}


	}
	fprintf(stdout, "-----------------------------------------------\n");

	struct_free(config_report, "ConfigReport");
	struct_free(data_apdu, "DATA_apdu");
	struct_free(received_apdu, "APDU");

	return ACCEPTED_CONFIG;
}

int send_configuration_response(
	phd_device *phd_device,
	unsigned short result,
	unsigned short config_id,
	unsigned short invoke_id) 
{
	ConfigReportRsp report_rsp;
	report_rsp.config_report_id = config_id;
	report_rsp.config_result = result;

	unsigned char *report_rsp_buff = calloc(sizeof(ConfigReportRsp), sizeof(unsigned char));
	size_t packed_size = struct_pack(&report_rsp, "ConfigReportRsp", report_rsp_buff);

	int ret = send_data_apdu(phd_device, 
							invoke_id,
							RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN, 								
							0,
							MDC_NOTI_CONFIG,
							packed_size,
							report_rsp_buff);

	free(report_rsp_buff);
	return ret;
}

int get_segment_info(
	phd_device *phd_device) 
{
	SegmSelection segment_selection;
	segment_selection.choice = ALL_SEGMENTS_CHOSEN;
	segment_selection.length = 0;
	segment_selection.u.all_segments = 0;

	unsigned char *segment_selection_buff = calloc(sizeof(SegmSelection), sizeof(unsigned char));
	size_t packed_size = struct_pack(&segment_selection, "SegmSelection", segment_selection_buff);

	int ret = send_data_apdu(phd_device,
							0,
							ROIV_CMIP_CONFIRMED_ACTION_CHOSEN,
							0x05,
							MDC_ACT_SEG_GET_INFO,
							packed_size,
							segment_selection_buff);

	free(segment_selection_buff);
	return ret;
}

int trig_segment_data_xfer(
	phd_device *phd_device)
{
	TrigSegmDataXferReq transfer_req;
	transfer_req.seg_inst_no = 0x00;

	unsigned char *transfer_req_buff = calloc(sizeof(TrigSegmDataXferReq), sizeof(unsigned char));
	size_t packed_size = struct_pack(&transfer_req, "TrigSegmDataXferReq", transfer_req_buff);

	int ret = send_data_apdu(phd_device,
							0,
							ROIV_CMIP_CONFIRMED_ACTION_CHOSEN,
							0x05,
							MDC_ACT_SEG_TRIG_XFER,
							packed_size,
							transfer_req_buff);

	free(transfer_req_buff);
	return ret;
}

int send_segment_data_result(
	phd_device *phd_device,
	intu16 invoke_id,
	intu16 segm_instance,
	intu32 segm_evt_entry_index,
	intu32 segm_evt_entry_count,
	intu16 segm_evt_status) 
{
	SegmDataEventDescr data_event_descr;
	data_event_descr.segm_instance = segm_instance;
	data_event_descr.segm_evt_entry_index = segm_evt_entry_index;
	data_event_descr.segm_evt_entry_count = segm_evt_entry_count;
	data_event_descr.segm_evt_status = segm_evt_status;

	SegmentDataResult segment_data_result;
	segment_data_result.segm_data_event_descr = data_event_descr;

	unsigned char *segment_data_result_buff = calloc(sizeof(SegmentDataResult), sizeof(unsigned char));
	size_t packed_size = struct_pack(&segment_data_result, "SegmentDataResult", segment_data_result_buff);

	int ret = send_data_apdu(phd_device,
							invoke_id,
							RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN, 								
							0x05,
							MDC_NOTI_SEGMENT_DATA,
							packed_size,
							segment_data_result_buff);

	free(segment_data_result_buff);
	return ret;
}

int send_data_apdu(
	phd_device *phd_device,	
	intu16 invoke_id,
	intu16 message_type,
	intu16 obj_handle,
	intu16 evt_type,
	size_t message_len,
	unsigned char *message_buffer)
{
	size_t packed_size = message_len;

	Any evt_info;
	evt_info.length = message_len;
	evt_info.value = message_buffer;

	data_apdu_message_union evt;
	memset(&evt, 0, sizeof(evt));
	size_t evt_size = 0;

	switch (message_type) {
		case RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
			evt.rors_cmipConfirmedEventReport.obj_handle = obj_handle;
			evt.rors_cmipConfirmedEventReport.currentTime = 0;
			evt.rors_cmipConfirmedEventReport.event_type = evt_type;
			evt.rors_cmipConfirmedEventReport.event_reply_info = evt_info;
			evt_size = 10;
			break;
		case ROIV_CMIP_CONFIRMED_ACTION_CHOSEN:
			evt.roiv_cmipConfirmedAction.obj_handle = obj_handle;
			evt.roiv_cmipConfirmedAction.action_type = evt_type;
			evt.roiv_cmipConfirmedAction.action_info_args = evt_info;
			evt_size = 6;
			break;
	}

	DATA_apdu_message message;
	message.choice = message_type;
	message.length = packed_size + evt_size;
	message.u = evt;

	DATA_apdu data_apdu;
	data_apdu.invoke_id = invoke_id;
	data_apdu.message = message;

	unsigned char *data_apdu_buff = calloc(256, sizeof(unsigned char));
	packed_size = struct_pack(&data_apdu, "DATA_apdu", data_apdu_buff);

	PRST_apdu prst_apdu;
	prst_apdu.length = packed_size;
	prst_apdu.value = data_apdu_buff;

	APDU response_apdu;
	response_apdu.choice = PRST_CHOSEN;
	response_apdu.u.prst = prst_apdu;
	response_apdu.length = packed_size + 2;

	unsigned char *response_apdu_buff = calloc(response_apdu.length + 4, sizeof(unsigned char));
	struct_pack(&response_apdu, "APDU", response_apdu_buff);

	int sent = send_data(phd_device, response_apdu_buff, (response_apdu.length + 4)*sizeof(unsigned char));

	free(data_apdu_buff);
	free(response_apdu_buff);

	if (sent != (response_apdu.length + 4)*sizeof(unsigned char))
		return 0;

	return 1;
}

void association_release(
	phd_device *phd_device)
{
	RLRQ_apdu rlrq_apdu;
	rlrq_apdu.reason = RELEASE_REQUEST_REASON_NORMAL;

	APDU response_apdu;
	response_apdu.choice = RLRQ_CHOSEN;
	response_apdu.u.rlrq = rlrq_apdu;
	response_apdu.length = 2;

	unsigned char *response_apdu_buff = calloc(response_apdu.length + 4, sizeof(unsigned char));
	struct_pack(&response_apdu, "APDU", response_apdu_buff);

	send_data(phd_device, response_apdu_buff, (response_apdu.length + 4)*sizeof(unsigned char));

	free(response_apdu_buff);	
}

void association_abort(
	phd_device *phd_device)
{
	ABRT_apdu abrt_apdu;
	abrt_apdu.reason = ABORT_REASON_UNDEFINED;

	APDU response_apdu;
	response_apdu.choice = ABRT_CHOSEN;
	response_apdu.u.abrt = abrt_apdu;
	response_apdu.length = 2;

	unsigned char *response_apdu_buff = calloc(response_apdu.length + 4, sizeof(unsigned char));
	struct_pack(&response_apdu, "APDU", response_apdu_buff);

	send_data(phd_device, response_apdu_buff, (response_apdu.length + 4)*sizeof(unsigned char));

	free(response_apdu_buff);	
}

state_type state_machine(
	state_type STATE, 
	phd_device *phd_device, 
	unsigned char *data_buffer, 
	int *len) 
{
	// by default loop in the same state
	state_type NEXT_STATE = STATE;

	int result;
	unsigned short config_id;
	unsigned short invoke_id;

	switch (STATE) {
		case UNASSOCIATED:
			LOG_DEBUG("Current state is: UNASSOCIATED");
			// check for association request
			*len = request_data(phd_device, data_buffer); 
			
			if (*len) {
				NEXT_STATE = ASSOCIATING;
			} else {
				NEXT_STATE = DISASSOCIATING;
				LOG_ERROR("No data received from USB device");
			}

			break;			
		case ASSOCIATING:
			LOG_DEBUG("Current state is: ASSOCIATING");
			if (*len == 0) {
				LOG_ERROR("Trying to associate when no data was received");
				NEXT_STATE = UNASSOCIATED;
				break;
			}

			result = check_association_request(phd_device, data_buffer, *len);

			if (result == -1) {
				LOG_ERROR("Trying to associate when data received wasn't a request");
				NEXT_STATE = ABORTING;
				break;
			}

			// if known device, respond with accepted, next state = OPERATING
			// else, respond with accepted-unknown-config, next state = CONFIGURING	

			if (!send_association_response(phd_device, result)) {
				LOG_ERROR("Association response transmission attempt %i failed", transmission_attempts++);

				if (transmission_attempts == MAX_TRANSMISSION_ATTEMPTS) {
					transmission_attempts = 0;
					NEXT_STATE = ABORTING;
				}

				break;
			}

			if (result == ACCEPTED) {				
				NEXT_STATE = OPERATING;
			} else if (result == ACCEPTED_UNKNOWN_CONFIG) {			
				NEXT_STATE = CONFIGURING;
			} else {
				LOG_ERROR("Association rejected with code %i", result);
				NEXT_STATE = UNASSOCIATED;
			}

			break;
		case CONFIGURING:
			LOG_DEBUG("Current state is: CONFIGURING");
			// check for configuration profile
			*len = request_data(phd_device, data_buffer); 
			
			if (*len == 0) {			
				LOG_ERROR("Trying to configure when no data was received");
				NEXT_STATE = ABORTING;
				break;
			}

			// check config
			result = check_configuration(phd_device, data_buffer, *len, &config_id, &invoke_id);

			if (result == -1) {
				LOG_ERROR("Trying to configure when data received wasn't a configuration profile");
				NEXT_STATE = ABORTING;
				break;
			}
			
			// if valid, store config, respond with accepted-config, next state = OPERATING
			// else, respond with unsupported-config, next state = CONFIGURING
			if (!send_configuration_response(phd_device, ACCEPTED_CONFIG, config_id, invoke_id)) {
				LOG_ERROR("Configuration response transmission attempt %i failed", transmission_attempts++);

				if (transmission_attempts == MAX_TRANSMISSION_ATTEMPTS) {
					transmission_attempts = 0;
					NEXT_STATE = ABORTING;
				}

				break;
			}

			NEXT_STATE = OPERATING;
			
			break;
		case OPERATING:
			LOG_DEBUG("Current state is: OPERATING");
			// assuming all data transmission was taken care of outside this function, transition to DISASSOCIATING state
			NEXT_STATE = DISASSOCIATING;

			break;
		case DISASSOCIATING:
			LOG_DEBUG("Current state is: DISASSOCIATING");
			LOG_INFO("Association with the USB device is being released");
			association_release(phd_device);
			request_data(phd_device, data_buffer); // receive release response from agent
			NEXT_STATE = UNASSOCIATED;

			break;
		case ABORTING:
		default:
			LOG_DEBUG("Current state is: ABORTING");
			association_abort(phd_device);
			LOG_WARNING("Association with the USB device is being aborted");
			NEXT_STATE = UNASSOCIATED;
	}

	return NEXT_STATE;
}

#endif /*COMMUNICATION*/
