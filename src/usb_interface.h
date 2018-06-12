#ifndef USB_INTERFACE
#define USB_INTERFACE

#include "log.h"
#include "libusb-1.0\libusb.h"
#include "usb_interface_definitions.h"

#define MAX_BUFFER_SIZE 1024
#define DATA_TRANSFER_TIMEOUT 500

typedef struct phd_device {

	libusb_device *usb_device;
	libusb_device_handle *usb_device_handle;

	unsigned char *data_buffer;

	uint8_t ep_bulk_in;
	uint8_t ep_bulk_out;
	int interface;

	uint16_t vendor_id;
	uint16_t product_id;

	char *manufacturer;
	char *name;
	char *serial_number;

	char *system_id;
	int system_id_len;

	int *specs;
	int num_specs;

	int ok;

} phd_device;

typedef struct phd_device_list {
	phd_device *device_list;
	int number_of_devices;
} phd_device_list;

int has_11073_descriptor(
	phd_device *phd_device,
	libusb_device *device,
	unsigned char *extra_buffer,
	int extra_length)
{
	int read_bytes = 0;

	int has_11073 = 0;
	int num_specs = 0;

	while(read_bytes < extra_length) {
		int length = (int)extra_buffer[read_bytes];
		int descriptor_type = (int)extra_buffer[read_bytes + 1];

		if (descriptor_type == PHD_CLASSFUNCTION_DESCRIPTOR_TYPE) {
			phd_classfunction_descriptor *desc = (phd_classfunction_descriptor*)&extra_buffer[read_bytes];
			if (desc->bPHDCDataCode == PHD_11073_20601) {
				has_11073 = 1;
			}
		} else if (descriptor_type == PHD_11073_FUNCTION_DESCRIPTOR_TYPE) {
			phd_11073_function_descriptor *desc = (phd_11073_function_descriptor*)&extra_buffer[read_bytes];
			num_specs = desc->bNumDevSpecs;

			if (num_specs > 0) {
				int spec_index = read_bytes + 4;

				phd_device->num_specs = num_specs;
				phd_device->specs = calloc(num_specs, sizeof(int));

				for (int i = 0; i < num_specs; ++i)
				{
					phd_device->specs[i] = (int)extra_buffer[spec_index + 2*i] + 256 * ((int)extra_buffer[spec_index + 2*i + 1]);
				}
			}
		}

		read_bytes += length;
	}

	return (num_specs > 0 && has_11073);
}

int is_11073_device(
	phd_device *phd_device,
	libusb_device *device,
	struct libusb_device_descriptor device_descriptor)
{
	libusb_device_handle *handle;
	if (libusb_open(device, &handle) < 0) {
		return 0;
	}

	if (libusb_claim_interface(handle, 0) < 0) {
		libusb_close(handle);
		return 0;
	}

	struct libusb_config_descriptor *config_desc;
	if (libusb_get_config_descriptor(device, 0, &config_desc) < 0) {
		libusb_release_interface(handle, 0);
		libusb_close(handle);
		return 0;
	}

	int result = 0;
	for (int i = 0; i < config_desc->bNumInterfaces; i++) {
		struct libusb_interface_descriptor *interface_desc = &(config_desc->interface[i].altsetting[0]);

		if (interface_desc->bInterfaceClass == LIBUSB_CLASS_PERSONAL_HEALTHCARE) {
			result = has_11073_descriptor(phd_device, device, interface_desc->extra, interface_desc->extra_length);
			phd_device->interface = i;
			break;
		}

	}

	libusb_free_config_descriptor(config_desc);
	libusb_release_interface(handle, 0);
	libusb_close(handle);

	return result;
}

int populate_phd_attributes(
	phd_device *phd_device,
	libusb_device *device,
	struct libusb_device_descriptor device_descriptor)
{
	libusb_device_handle *handle;
	if (libusb_open(device, &handle) < 0) {
		return 0;
	}

	if (libusb_claim_interface(handle, phd_device->interface) < 0) {
		libusb_close(handle);
		return 0;
	}

	struct libusb_config_descriptor *config_desc;
	if (libusb_get_config_descriptor(device, 0, &config_desc) < 0) {
		libusb_release_interface(handle, 0);
		libusb_close(handle);
		return 0;
	}

	/* End points */
	struct libusb_interface_descriptor *interface_desc = &(config_desc->interface[phd_device->interface].altsetting[0]);

	for (int i = 0; i < interface_desc->bNumEndpoints; i++) {
		struct libusb_endpoint_descriptor ep = interface_desc->endpoint[i];
		int type = (ep.bmAttributes & 0x03);
		int direction = (ep.bEndpointAddress & 0x80);

		if (type == LIBUSB_TRANSFER_TYPE_BULK && direction == LIBUSB_ENDPOINT_IN) {
			phd_device->ep_bulk_in = ep.bEndpointAddress;
		} else if (type == LIBUSB_TRANSFER_TYPE_BULK &&	direction == LIBUSB_ENDPOINT_OUT) {
			phd_device->ep_bulk_out = ep.bEndpointAddress;
		}
	}

	libusb_free_config_descriptor(config_desc);

	/* IDs */
	phd_device->vendor_id = device_descriptor.idVendor;
	phd_device->product_id = device_descriptor.idProduct;

	/* Device name */
	unsigned char product[255];
	int len = libusb_get_string_descriptor_ascii(handle, device_descriptor.iProduct, product, sizeof(product));
	if (len < 0) {
		//DEBUG("Trouble getting USB product");
		libusb_release_interface(handle, phd_device->interface);
		libusb_close(handle);
		return 0;
	}

	phd_device->name = (char *) calloc(len + 1, sizeof(char));
	strncpy(phd_device->name, (char *) product, len);

	/* Device manufacturer */
	unsigned char manufacturer[255];
	len = libusb_get_string_descriptor_ascii(handle, device_descriptor.iManufacturer, manufacturer, sizeof(manufacturer));

	if (len < 0) {
		//DEBUG("Trouble getting USB manufacturer");
		libusb_release_interface(handle, phd_device->interface);
		libusb_close(handle);
		return 0;
	}

	phd_device->manufacturer = (char *) calloc(len + 1, sizeof(char));
	strncpy(phd_device->manufacturer, (char *) manufacturer, len);

	/* Device serial */
	unsigned char serial[255];
	len = libusb_get_string_descriptor_ascii(handle, device_descriptor.iSerialNumber, serial, sizeof(serial));

	if (len < 0) {
		//DEBUG("Trouble getting USB serial number");
		libusb_release_interface(handle, phd_device->interface);
		libusb_close(handle);
		return 0;
	}

	phd_device->serial_number = (char *) calloc(len + 1, sizeof(char));
	strncpy(phd_device->serial_number, (char *) serial, len);


	// initialize system-id to null
	phd_device->system_id = NULL;
	phd_device->system_id_len = 0;


	libusb_release_interface(handle, 0);
	libusb_close(handle);

	return 1;
}

void search_phd_devices(
	libusb_context *context,
	phd_device_list *phd_devices)
{
	libusb_device **device_list;
	libusb_device *device;
	
	libusb_get_device_list(context, &device_list);

	int i = 0;
	while ((device = device_list[i++]) != NULL) {
		phd_device new_device = {0};

		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(device, &desc);

		if (r < 0) {
			//DEBUG("Failed to get device descriptor");
			continue;
		}

		if (!(is_11073_device(&new_device, device, desc))) {
			continue;
		}

		if (!(populate_phd_attributes(&new_device, device, desc))) {
			continue;
		}

		phd_devices->number_of_devices++;
		phd_devices->device_list =
			(phd_device *) realloc(phd_devices->device_list,
			phd_devices->number_of_devices * sizeof(phd_device));

		new_device.usb_device = libusb_ref_device(device);
		phd_devices->device_list[phd_devices->number_of_devices - 1] = new_device;
	}

	libusb_free_device_list(device_list, 1);
}

int open_phd_handle(
	phd_device *phd_device, 
	libusb_context *usb_context)
{
	libusb_device_handle *handle = libusb_open_device_with_vid_pid(usb_context, phd_device->vendor_id, phd_device->product_id);

	if (handle == NULL) {
		//DEBUG("libusb_open_device failed");
		libusb_unref_device(phd_device->usb_device);
		return 0;
	}

	int result = libusb_claim_interface(handle, phd_device->interface);
	if (result != LIBUSB_SUCCESS) {
		//DEBUG("libusb_claim_interface failure %d", result);
		libusb_close(handle);
		libusb_unref_device(phd_device->usb_device);
		return 0;
	}

	int ret = libusb_reset_device(handle);

	phd_device->usb_device_handle = handle;

	//query_phdc(phdc_device);

	return 1;
}

void release_phd_device(
	phd_device *phd_device)
{
	if (phd_device->interface != -1) {
		libusb_release_interface(phd_device->usb_device_handle, phd_device->interface);
		phd_device->interface = -1;
	}

	if (phd_device->usb_device_handle != NULL) {
		libusb_close(phd_device->usb_device_handle);
		phd_device->usb_device_handle = NULL;
	}

	if (phd_device->usb_device != NULL) {
		libusb_unref_device(phd_device->usb_device);
		phd_device->usb_device = NULL;
	}

	if (phd_device->manufacturer != NULL) {
		free(phd_device->manufacturer);
		phd_device->manufacturer = NULL;
	}

	if (phd_device->name != NULL) {
		free(phd_device->name);
		phd_device->name = NULL;
	}

	if (phd_device->serial_number != NULL) {
		free(phd_device->serial_number);
		phd_device->serial_number = NULL;
	}

	if (phd_device->system_id != NULL) {
		free(phd_device->system_id);
		phd_device->system_id = NULL;
	}

	if (phd_device->specs != NULL) {
		free(phd_device->specs);
		phd_device->specs = NULL;
	}

	if (phd_device->data_buffer != NULL) {
		free(phd_device->data_buffer);
		phd_device->data_buffer = NULL;
	}
}

void release_phd_device_list(
	phd_device_list *phd_devices)
{
	phd_device *phd_device;

	for (int k = 0; k < phd_devices->number_of_devices; k++) {
		phd_device = &(phd_devices->device_list[k]);
		release_phd_device(phd_device);
		free(phd_device);
	}

	free(phd_devices);
}

int send_data(
	phd_device *phd_device, 
	unsigned char *buffer, 
	int len)
{
	int transferred_bytes;

	int ret = libusb_bulk_transfer(phd_device->usb_device_handle, 
								   phd_device->ep_bulk_out,
								   buffer,
								   len,
								   &transferred_bytes,
								   DATA_TRANSFER_TIMEOUT);	

	LOG_DEBUG("Data transmission (0x%08x at %i) sent %i / %i bytes", phd_device->usb_device_handle, phd_device->ep_bulk_out, transferred_bytes, len);

	if (ret == LIBUSB_SUCCESS || ret == LIBUSB_ERROR_TIMEOUT)
		return transferred_bytes;
	else
		return 0;
}

int request_data(
	phd_device *phd_device, 
	unsigned char *buffer)
{
	int transferred_bytes;	
	int ret = libusb_bulk_transfer(phd_device->usb_device_handle, 
								   phd_device->ep_bulk_in,
								   buffer,
								   MAX_BUFFER_SIZE,
								   &transferred_bytes,
								   DATA_TRANSFER_TIMEOUT);

	LOG_DEBUG("Data request (0x%08x at %i) received %i bytes", phd_device->usb_device_handle, phd_device->ep_bulk_in, transferred_bytes);

	if (ret == LIBUSB_SUCCESS)
		return transferred_bytes;
	else
		return 0;
}

#endif /*USB_INTERFACE*/
