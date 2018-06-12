#ifndef GLUCOMETER
#define GLUCOMETER

#include "phd_types.h"

typedef struct measurement {
	int year;
	int month; 
	int day;
	int hour;
	int minute;
	int second;
	int value;
} measurement;

typedef struct measurement_list {
	int length;
	measurement *list;
} measurement_list;

void free_measurement_list(measurement_list *list) {
	free(list->list);
	free(list);
}

void insert_measurements(measurement_list *measurement_list, int entries, unsigned char *entries_buf, int entry_len) {
	int list_start = measurement_list->length;
	measurement_list->length += entries;
	measurement_list->list = realloc(measurement_list->list, measurement_list->length * sizeof(measurement));

	int entry_index = 0;

	for (int i = list_start; i < measurement_list->length; ++i) {
		int year = 100*(10*(entries_buf[entry_index] >> 4) + (entries_buf[entry_index] & 0x0f)) + (10*(entries_buf[entry_index + 1] >> 4) + (entries_buf[entry_index + 1] & 0x0f));
		int month = 10*(entries_buf[entry_index + 2] >> 4) + (entries_buf[entry_index + 2] & 0x0f);
		int day = 10*((entries_buf[entry_index + 3] >> 4)) + (entries_buf[entry_index + 3] & 0x0f);
		int hour = 10*((entries_buf[entry_index + 4] >> 4)) + (entries_buf[entry_index + 4] & 0x0f);
		int minute = 10*(entries_buf[entry_index + 5] >> 4) + (entries_buf[entry_index + 5] & 0x0f);
		int second = 10*(entries_buf[entry_index + 6] >> 4) + (entries_buf[entry_index + 6] & 0x0f);
		int value = (((int)entries_buf[entry_index + 8]) << 8) + entries_buf[entry_index + 9];

		entry_index += entry_len;

		measurement new_measurement = {year, month, day, hour, minute, second, value};
		measurement_list->list[i] = new_measurement;
	}
}

void print_measurements(measurement_list *measurement_list) {
	fprintf(stdout, "\t Printing %i measurements:\n", measurement_list->length);
	for (int i = 0; i < measurement_list->length; ++i) {
		measurement measurement = measurement_list->list[i];
		fprintf(stdout, "\t %02i/%02i/%i %02i:%02i:%02i - %i\n", 
			measurement.day, measurement.month, measurement.year, measurement.hour, measurement.minute, measurement.second, measurement.value);
	}
}

// save

// load

// merge


#endif /*GLUCOMETER*/