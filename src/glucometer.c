#ifdef WIN32
	// Memory leak detection
	#define _CRTDBG_MAP_ALLOC  
	#include <stdlib.h>  
	#include <crtdbg.h>  
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "phd_types.h"
#include "phd_types_meta.h"
#include "serialization.h"
#include "usb_interface.h"
#include "communication.h"
#include "glucometer.h"
#ifdef WIN32
	#include "usb_restart.h"
#endif

int main(int argc, char **argv)
{
	LOG_INFO("Glucometer probe");

	libusb_context *usb_context = NULL;

	int vendor_id = 0;
	int product_id = 0;

	int ret = libusb_init(&usb_context);
	if (ret != LIBUSB_SUCCESS) {
		LOG_ERROR("libusb_init failure %d", ret);
		return 0;
	}

	phd_device_list *phd_devices = (phd_device_list*)calloc(1, sizeof(phd_device_list));
	search_phd_devices(usb_context, phd_devices);	
	
	if (phd_devices->number_of_devices > 0) {

		phd_device *device = &(phd_devices->device_list[0]);

		LOG_INFO("Device name: %s", device->name);
		LOG_INFO("Device manufacturer: %s", device->manufacturer);
		LOG_INFO("Device serial: %s", device->serial_number);
		LOG_INFO("Device VID:PID: %x:%x", device->vendor_id, device->product_id);
		LOG_INFO("Number of specializations: %d", device->num_specs);

		vendor_id = device->vendor_id;
		product_id = device->product_id;

		for (int i = 0; i < device->num_specs; i++) {
			LOG_INFO("Specialization %d: %d", i, device->specs[i]);
		}

		if (open_phd_handle(device, usb_context) == 1) {
			LOG_DEBUG("Handle 0x%08x openned succesfully", device->usb_device_handle);

			unsigned char *data_buffer = (unsigned char*)calloc(MAX_BUFFER_SIZE, sizeof(unsigned char));
			int buffer_len = 0;

			state_type STATE = UNASSOCIATED;
			do {
				state_type NEXT_STATE = state_machine(STATE, device, data_buffer, &buffer_len);

				if (STATE == OPERATING) {
					// operate
					/*
					if (get_segment_info(device)) {
						buffer_len = request_data(device, data_buffer);
						if (buffer_len) {
							APDU *received_apdu;
							struct_unpack(&received_apdu, "APDU", data_buffer);

							unsigned char *buffer_data = received_apdu->u.prst.value;
							DATA_apdu *data_apdu;
							struct_unpack(&data_apdu, "DATA_apdu", buffer_data);

							unsigned char *buffer_args = data_apdu->message.u.rors_cmipConfirmedAction.action_info_args.value;
							SegmentInfoList *segment_info_list;
							struct_unpack(&segment_info_list , "SegmentInfoList", buffer_args);

							for (int i = 0; i < segment_info_list->count; ++i) {
								SegmentInfo *segment = segment_info_list->value + i;

								fprintf(stdout, "-----------------------------------------------\n");
								fprintf(stdout, "%i: Segment info instance = 0x%04x \n", i, segment->seg_inst_no);
								for (int j = 0; j < segment->seg_info.count; ++j) {
									AVA_Type *attribute = (AVA_Type*)segment->seg_info.value + j;
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
							
							struct_free(segment_info_list, "SegmentInfoList");
							struct_free(data_apdu, "DATA_apdu");
							struct_free(received_apdu, "APDU");

							fprintf(stdout, "\t");
							for (int i = 0; i < buffer_len; ++i)
								fprintf(stdout, "0x%02x ", data_buffer[i]);
							fprintf(stdout, "\n");
						}
					}
					*/

					if (trig_segment_data_xfer(device)) {
						buffer_len = request_data(device, data_buffer);
						if (buffer_len) {
							APDU *received_apdu;
							struct_unpack(&received_apdu, "APDU", data_buffer);

							if (received_apdu->choice == ABRT_CHOSEN) {
								struct_free(received_apdu, "APDU");
								break;
							}

							unsigned char *buffer_data = received_apdu->u.prst.value;
							DATA_apdu *data_apdu;
							struct_unpack(&data_apdu, "DATA_apdu", buffer_data);

							unsigned char *buffer_args = data_apdu->message.u.rors_cmipConfirmedAction.action_info_args.value;
							TrigSegmDataXferRsp *transfer_rsp;
							struct_unpack(&transfer_rsp, "TrigSegmDataXferRsp", buffer_args);

							measurement_list *measurements = (measurement_list*)calloc(1, sizeof(measurement_list));

							if (transfer_rsp->seg_inst_no == 0x00 && transfer_rsp->trig_segm_xfer_rsp == TSXR_SUCCESSFUL) {
								fprintf(stdout, "Transferring data from segment inst no 0:\n\n");
								while (buffer_len = request_data(device, data_buffer)) {
									APDU *received_apdu2;
									struct_unpack(&received_apdu2, "APDU", data_buffer);

									if (received_apdu2->choice == ABRT_CHOSEN) {
										struct_free(received_apdu2, "APDU");
										break;
									}

									unsigned char *buffer_data2 = received_apdu2->u.prst.value;
									DATA_apdu *data_apdu2;
									struct_unpack(&data_apdu2, "DATA_apdu", buffer_data2);

									if (data_apdu2->message.choice == ROER_CHOSEN) {
										struct_free(data_apdu2, "DATA_apdu");
										struct_free(received_apdu2, "APDU");
										break;
									}

									unsigned char *buffer_args2 = data_apdu2->message.u.roiv_cmipConfirmedEventReport.event_info.value;
									SegmentDataEvent *segment_data;
									struct_unpack(&segment_data, "SegmentDataEvent", buffer_args2);

									unsigned short invoke_id = data_apdu2->invoke_id;
									unsigned short segm_instance = segment_data->segm_data_event_descr.segm_instance;
									unsigned int segm_evt_entry_index = segment_data->segm_data_event_descr.segm_evt_entry_index;
									unsigned int segm_evt_entry_count = segment_data->segm_data_event_descr.segm_evt_entry_count;
									unsigned short segm_evt_status = segment_data->segm_data_event_descr.segm_evt_status;									

									short entries_len = segment_data->segm_data_event_entries.length;
									unsigned char *entries_buf = segment_data->segm_data_event_entries.value;

									insert_measurements(measurements, entries_len / 10, entries_buf, 10);

									if (segm_evt_status == SEVTSTA_AGENT_ABORT) {
										send_segment_data_result(device, invoke_id, segm_instance, segm_evt_entry_index, segm_evt_entry_count, segm_evt_status);
									} else {
										send_segment_data_result(device, invoke_id, segm_instance, segm_evt_entry_index, segm_evt_entry_count, segm_evt_status | SEVTSTA_MANAGER_CONFIRM);
									}

									struct_free(segment_data, "SegmentDataEvent");
									struct_free(data_apdu2, "DATA_apdu");
									struct_free(received_apdu2, "APDU");
								}
							}

							print_measurements(measurements);
							free_measurement_list(measurements);

							struct_free(transfer_rsp, "TrigSegmDataXferRsp");
							struct_free(data_apdu, "DATA_apdu");
							struct_free(received_apdu, "APDU");
						}
					}

				}

				STATE = NEXT_STATE;
			} while (STATE != UNASSOCIATED);

			LOG_DEBUG("Exiting...");
				
			free(data_buffer);
		}
	} else {
		LOG_INFO("No devices found :(");
	}

	release_phd_device_list(phd_devices);
	libusb_exit(usb_context);

#ifdef WIN32
	// needs to be down here so device wont be in use?
	// if device is in use it cant be restarted without a system reboot (error code -1)
	if (vendor_id && product_id) {
		int ret = usb_restart(vendor_id, product_id);
		if (ret != 0)
			LOG_ERROR("Device restart failed with code %i", ret);
	}
#endif

#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}