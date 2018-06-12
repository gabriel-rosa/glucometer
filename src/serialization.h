#ifndef SERIALIZATION
#define SERIALIZATION

#include "phd_types.h"
#include "phd_types_meta.h"

#ifndef WIN32
    #include <arpa/inet.h>
#else
    #include <WinSock2.h>
#endif

void* get_struct_format(
	char *struct_name)
{
	struct struct_fmt *new_fmt = NULL;
	
	char *string = struct_name;	
	
	#include "str_to_struct.h" // ugly hack :(
	
	return new_fmt;
}

void* get_union_format(
	unsigned char union_type,
	intu16 choice)
{
	struct struct_fmt *new_fmt = NULL;
	
	#include "choice_to_struct.h" // ugly hack :(
	
	return new_fmt;
}

void* get_struct_param(
	void *structure, 
	struct struct_fmt *fmt,
	const char *param)
{
	void *param_ptr = NULL;
	
	for (size_t i = 0; i < fmt->num_members; i++) {
		if (strcmp(param, fmt->names[i]) == 0) {
			param_ptr = ((unsigned char*)structure) + fmt->offsets[i];
			break;
		}
	}

	return param_ptr;
}

intu16 get_struct_iu16(
	void *structure, 
	struct struct_fmt *fmt,
	const char *param)
{
	void *param_ptr = get_struct_param(structure, fmt, param);

	intu16 param_value = 0;

	if (!(param_ptr == NULL)) {
		memcpy(&param_value, param_ptr, sizeof(intu16));
	}

	return param_value;
}

intu16 get_struct_ptr_count(
	void *structure,
	struct struct_fmt *fmt)
{
	return get_struct_iu16(structure, fmt, "count");
}

size_t get_struct_ptr_size(
	void *structure,
	struct struct_fmt *fmt)
{
	intu16 count = get_struct_iu16(structure, fmt, "count");
	intu16 length = get_struct_iu16(structure, fmt, "length");
	
	return (size_t) (count == 0 ? 0 : length / count);
}

size_t struct_pack(
	void *structure,
	char *structure_type,
	unsigned char *buffer) 
{
	size_t pos = 0;

	struct struct_fmt *fmt = get_struct_format(structure_type);
	if (fmt == NULL) {
		// ERROR
	}

	for (size_t i = 0; i < fmt->num_members; i++) {
		unsigned char *base_ptr = ((unsigned char*)structure) + fmt->offsets[i];

		switch (fmt->types[i])
		{
			case MEMBER_TYPE:
			{
				if (fmt->sizes[i] == sizeof(intu32)) {
					intu32 tmp;
					memcpy(&tmp, base_ptr, sizeof(intu32));
					tmp = htonl(tmp);
					memcpy(buffer + pos, &tmp, sizeof(intu32));
				} else if (fmt->sizes[i] == sizeof(intu16)) {
					intu16 tmp;
					memcpy(&tmp, base_ptr, sizeof(intu16));
					tmp = htons(tmp);
					memcpy(buffer + pos, &tmp, sizeof(intu16));
				} else {
					memcpy(buffer + pos, base_ptr, sizeof(intu8));
				}
				pos += fmt->sizes[i];
			} break;

			case IU8_PTR_TYPE:
			{
				size_t length = get_struct_iu16(structure, fmt, "length");

				intu8 *data_ptr;
				memcpy(&data_ptr, base_ptr, sizeof(intu8*));

				memcpy(buffer + pos, data_ptr, length);
				pos += length;
			} break;


			case IU16_PTR_TYPE:
			case IU32_PTR_TYPE:
			{
				intu16 count = get_struct_ptr_count(structure, fmt);
				size_t size = get_struct_ptr_size(structure, fmt);

				unsigned char *data_ptr;
				memcpy(&data_ptr, base_ptr, sizeof(unsigned char*));

				for (size_t j = 0; j < count; j++) {
					if (size == sizeof(intu32)) {
						intu32 tmp;
						memcpy(&tmp, data_ptr + j*size, size);
						tmp = htonl(tmp);
						memcpy(buffer + pos, &tmp, size);
					} else if (size == sizeof(intu16)) {
						intu16 tmp;
						memcpy(&tmp, data_ptr + j*size, size);
						tmp = htons(tmp);
						memcpy(buffer + pos, &tmp, size);
					}
					pos += size;
				}
			} break;
			
			case STRUCT_TYPE:
			{
				pos += struct_pack((void*)base_ptr, fmt->type_names[i], buffer + pos);
			} break;

			case STRUCT_PTR_TYPE:
			{
				intu16 count = get_struct_ptr_count(structure, fmt);
				size_t size = get_struct_ptr_size(structure, fmt);

				unsigned char *data_ptr;
				memcpy(&data_ptr, base_ptr, sizeof(unsigned char*));

				for (size_t j = 0; j < count; j++) {
					void *struct_ptr;
					memcpy(&struct_ptr, data_ptr + j * sizeof(void*), sizeof(void*));
					pos += struct_pack(struct_ptr, fmt->type_names[i], buffer + pos);
				}
			} break;

			case ENUM_UNION:
			case SEGM_UNION:
			case MESG_UNION:
			case APDU_UNION:
			{
				intu16 choice = get_struct_iu16(structure, fmt, "choice");

				if (choice == OBJ_ID_CHOSEN || choice == ALL_SEGMENTS_CHOSEN) {
					intu16 tmp;
					memcpy(&tmp, base_ptr, sizeof(intu16));
					tmp = htons(tmp);
					memcpy(buffer + pos, &tmp, sizeof(intu16));

					pos += sizeof(intu16);
				} else if (choice == BIT_STR_CHOSEN) {
					intu32 tmp;
					memcpy(&tmp, base_ptr, sizeof(intu32));
					tmp = htonl(tmp);
					memcpy(buffer + pos, &tmp, sizeof(intu32));

					pos += sizeof(intu32);
				} else {
					struct struct_fmt *new_fmt = get_union_format(fmt->types[i], choice);

					if (new_fmt == NULL) {
						// ERROR
					}

					pos += struct_pack((void*)base_ptr, new_fmt->struct_name, buffer + pos);
				}
			} break;

			default:
			{
			}
		}
	}

	return pos;
}

size_t struct_unpack(
	void **structure_ptr,
	char *structure_type,
	unsigned char *buffer)
{
	size_t pos = 0;

	struct struct_fmt *fmt = get_struct_format(structure_type);
	if (fmt == NULL) {
		// ERROR
	}

	void *structure = malloc(fmt->struct_size);
	memcpy(structure_ptr, &structure, sizeof(void*));

	for (size_t i = 0; i < fmt->num_members; i++) {
		unsigned char *base_ptr = ((unsigned char*)structure) + fmt->offsets[i];

		switch (fmt->types[i])
		{
			case MEMBER_TYPE:
			{
				if (fmt->sizes[i] == sizeof(intu32)) {
					intu32 tmp;
					memcpy(&tmp, buffer + pos, sizeof(intu32));					
					tmp = ntohl(tmp);
					memcpy(base_ptr, &tmp, sizeof(intu32));
				} else if (fmt->sizes[i] == sizeof(intu16)) {
					intu16 tmp;
					memcpy(&tmp, buffer + pos, sizeof(intu16));					
					tmp = ntohs(tmp);
					memcpy(base_ptr, &tmp, sizeof(intu16));
				} else {
					memcpy(base_ptr, buffer + pos, sizeof(intu8));
				}
				pos += fmt->sizes[i];
			} break;

			case IU8_PTR_TYPE:
			{
				size_t length = get_struct_iu16(structure, fmt, "length");

				intu8 *data_ptr;

				if (length == 0) {
					data_ptr = NULL;
				} else {
					data_ptr = malloc(length);
					memcpy(data_ptr, buffer + pos, length);
				}

				memcpy(base_ptr, &data_ptr, sizeof(intu8*));
				pos += length;
			} break;


			case IU16_PTR_TYPE:
			case IU32_PTR_TYPE:
			{
				intu16 count = get_struct_ptr_count(structure, fmt);
				size_t size = get_struct_ptr_size(structure, fmt);

				unsigned char *data_ptr;

				if (count == 0 || size == 0) {
					data_ptr = NULL;
				} else {
					data_ptr = malloc(count * size);

					size_t ptr_pos = 0;

					for (size_t j = 0; j < count; j++) {
						if (size == sizeof(intu32)) {
							intu32 tmp;
							memcpy(&tmp, buffer + ptr_pos, size);
							tmp = htonl(tmp);
							memcpy(data_ptr + j*size, &tmp, size);
						} else if (size == sizeof(intu16)) {
							intu16 tmp;
							memcpy(&tmp, buffer + ptr_pos, size);
							tmp = htons(tmp);
							memcpy(data_ptr + j*size, &tmp, size);
						}
						ptr_pos += size;
					}
				}

				memcpy(base_ptr, &data_ptr, sizeof(unsigned char*));
				pos += count * size;
			} break;
			
			case STRUCT_TYPE:
			{
				struct struct_fmt *new_fmt = get_struct_format(fmt->type_names[i]);

				if (new_fmt == NULL) {
					// ERROR
				}
				
				void *temp_struct;
				size_t size = struct_unpack(&temp_struct, fmt->type_names[i], buffer + pos);
				memcpy((void*)base_ptr, temp_struct, new_fmt->struct_size);
				free(temp_struct);
				pos += size;
			} break;

			case STRUCT_PTR_TYPE:
			{
				intu16 count = get_struct_ptr_count(structure, fmt);
				//size_t size = get_struct_ptr_size(structure, fmt);

				unsigned char *data_ptr;

				//if (count == 0 || size == 0) {
				if (count == 0) {
					data_ptr = NULL;
				} else {
					struct struct_fmt *new_fmt = get_struct_format(fmt->type_names[i]);

					if (new_fmt == NULL) {
						// ERROR
					}

					//data_ptr = malloc(count * size);
					data_ptr = malloc(count * new_fmt->struct_size);

					for (size_t j = 0; j < count; j++) {	
						void *temp_struct;
						//unsigned char *src_ptr = buffer + pos + j*size;
						size_t size = struct_unpack(&temp_struct, fmt->type_names[i], buffer + pos);
						//unsigned char *offset_ptr = data_ptr + j*size;
						unsigned char *offset_ptr = data_ptr + j*new_fmt->struct_size;
						memcpy(offset_ptr, temp_struct, new_fmt->struct_size);
						free(temp_struct);

						pos += size;
					}
				}

				memcpy(base_ptr, &data_ptr, sizeof(unsigned char*));
				//pos += count * size;
			} break;
			
			case ENUM_UNION:
			case SEGM_UNION:
			case MESG_UNION:
			case APDU_UNION:
			{
				intu16 choice = get_struct_iu16(structure, fmt, "choice");

				if (choice == OBJ_ID_CHOSEN || choice == ALL_SEGMENTS_CHOSEN) {
					intu16 tmp;
					memcpy(&tmp, base_ptr, sizeof(intu16));
					tmp = htons(tmp);
					memcpy(buffer + pos, &tmp, sizeof(intu16));

					pos += sizeof(intu16);
				} else if (choice == BIT_STR_CHOSEN) {
					intu32 tmp;
					memcpy(&tmp, base_ptr, sizeof(intu32));
					tmp = htonl(tmp);
					memcpy(buffer + pos, &tmp, sizeof(intu32));

					pos += sizeof(intu32);
				} else {
					struct struct_fmt *new_fmt = get_union_format(fmt->types[i], choice);

					if (new_fmt == NULL) {
						// ERROR
					}

					void *temp_struct;
					size_t size = struct_unpack(&temp_struct, new_fmt->struct_name, buffer + pos);
					memcpy((void*)base_ptr, temp_struct, new_fmt->struct_size);
					free(temp_struct);
					pos += size;
				}
			} break;

			default:
			{
			}
		}
	}

	return pos;
}

void struct_free_children(
	void *structure,
	char *structure_type) 
{
	struct struct_fmt *fmt = get_struct_format(structure_type);
	if (fmt == NULL) {
		// ERROR
	}

	for (size_t i = 0; i < fmt->num_members; i++) {
		unsigned char *base_ptr = ((unsigned char*)structure) + fmt->offsets[i];

		switch (fmt->types[i])
		{
			case IU8_PTR_TYPE:
			{
				size_t length = get_struct_iu16(structure, fmt, "length");

				if (length == 0) {					
					break;
				}

				unsigned char *data_ptr;
				memcpy(&data_ptr, base_ptr, sizeof(unsigned char*));
				free(data_ptr);
			} break;

			case IU16_PTR_TYPE:
			case IU32_PTR_TYPE:
			{
				intu16 count = get_struct_ptr_count(structure, fmt);
				size_t size = get_struct_ptr_size(structure, fmt);

				if (size == 0 || count == 0) {					
					break;
				}

				unsigned char *data_ptr;
				memcpy(&data_ptr, base_ptr, sizeof(unsigned char*));
				free(data_ptr);

			} break;

			case STRUCT_TYPE:
			{
				if (base_ptr != NULL) {
					struct_free_children(base_ptr, fmt->type_names[i]);
				}
			} break;

			case STRUCT_PTR_TYPE:
			{
				intu16 count = get_struct_ptr_count(structure, fmt);
				//size_t size = get_struct_ptr_size(structure, fmt);

				//if (size == 0 || count == 0) {					
				if (count == 0) {
					break;				
				}

				struct struct_fmt *new_fmt = get_struct_format(fmt->type_names[i]);

				if (new_fmt == NULL) {
					// ERROR
				}

				unsigned char *data_ptr;
				memcpy(&data_ptr, base_ptr, sizeof(unsigned char*));

				for (size_t j = 0; j < count; j++) {
					struct_free_children(data_ptr + j * new_fmt->struct_size, fmt->type_names[i]);
				}				

				free(data_ptr);
			} break;	

			case ENUM_UNION:
			case SEGM_UNION:
			case MESG_UNION:
			case APDU_UNION:
			{
				intu16 choice = get_struct_iu16(structure, fmt, "choice");

				if (!(choice == OBJ_ID_CHOSEN || choice == ALL_SEGMENTS_CHOSEN || choice == BIT_STR_CHOSEN)) {
					struct struct_fmt *new_fmt = get_union_format(fmt->types[i], choice);

					if (base_ptr != NULL) {
						struct_free_children(base_ptr, new_fmt->struct_name);
					}
				}
			} break;

			default:
			{
			}
		}
	}
}

void struct_free(
	void *structure,
	char *structure_type)
{
	struct_free_children(structure, structure_type);
	free(structure);
}

#endif /* SERIALIZATION */