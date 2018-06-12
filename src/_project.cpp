#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ieee11073.h"
#include "manager.h"
#include "communication/plugin/usb/plugin_usb.h"
#include "communication/service.h"


static ContextId CONTEXT_ID;

void new_data_received(Context *ctx, DataList *list)
{
	fprintf(stderr, "Medical Device Data Updated:\n");

	char *data = json_encode_data_list(list);

	if (data != NULL) {
		fprintf(stderr, "%s", data);
		fprintf(stderr, "\n");

		fflush(stderr);
		free(data);
	}

	// uncomment for manager-initiated disassociation testing
	// manager_request_association_release(CONTEXT_ID);
}

void print_device_attributes(Context *ctx, Request *r, DATA_apdu *response_apdu)
{
	DataList *list = manager_get_mds_attributes(CONTEXT_ID);
	char *data = json_encode_data_list(list);

	fprintf(stderr, "print_device_attributes\n");

	if (data != NULL) {
		fprintf(stderr, "%s", data);
		fprintf(stderr, "\n");

		fflush(stderr);
	}

	data_list_del(list);
	free(data);
}

void device_reqmdsattr()
{
	fprintf(stderr, "device_reqmdsattr\n");
	manager_request_get_all_mds_attributes(CONTEXT_ID, print_device_attributes);
}

void device_associated(Context *ctx, DataList *list)
{
	fprintf(stderr, " Medical Device System Associated:\n");

	char *data = json_encode_data_list(list);

	if (data != NULL) {
		fprintf(stderr, "%s", data);
		fprintf(stderr, "\n");

		fflush(stderr);
		free(data);
	}

	device_reqmdsattr();
}

static void print_help()
{
	printf(
		"Sample IEEE 11073 manager for testing\n\n"
		"Usage: ieee_manager [OPTION]\n"
		"Options:\n"
		"        --help                Print this help\n"
		"        --tcp                 Run TCP mode on default port\n");
}

int main(int argc, char **argv)
{
	if (argc == 2) {
		if (strcmp(argv[1], "--help") == 0) {
			print_help();
			exit(0);
		}
		else {
			fprintf(stderr, "ERROR: invalid option: %s\n", argv[1]);
			fprintf(stderr, "Try `glucometer_probe --help'"
				" for more information.\n");
			exit(1);
		}

	}
	else if (argc > 2) {
		fprintf(stderr, "ERROR: Invalid number of options\n");
		fprintf(stderr, "Try `ieee_manager --help'"
			" for more information.\n");
		exit(1);
	}

	fprintf(stderr, "\nIEEE 11073 Sample application\n");

	CommunicationPlugin comm_plugin;
	plugin_usb_setup(&comm_plugin);
	CommunicationPlugin *comm_plugins[1] = { &comm_plugin };

	ManagerListener listener;
	listener.measurement_data_updated = &new_data_received;
	listener.device_available = &device_associated;

	unsigned int size = 100;
	char *input = (char *)malloc(size);

	manager_init(comm_plugins);

	manager_add_listener(listener);

	manager_start();

	while (1) {
		fprintf(stderr, "--> ");
		//unsigned int read_bytes = getline(&input, &size, stdin);
		fgets(input, size, stdin);

		if (strcmp(input, "exit") == 0) {
			//print_device_attributes();
			break;
		}
		else if (strcmp(input, "start") == 0) {
			manager_start();
		}
		else if (strcmp(input, "stop") == 0) {
			manager_stop();
		}
		else if (strcmp(input, "state") == 0) {
			//char *state = communication_get_state_name();
			//fprintf(stderr, "Current State: %s\n", state);
		}
		else if (strcmp(input, "print_mds") == 0) {
			//print_device_attributes();
		}
		else {
			fprintf(stderr, "Command not recognized\n");
		}
	}

	manager_finalize();
	free(input);
	return 0;

	return 0;
}