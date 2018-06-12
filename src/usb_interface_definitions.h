#ifndef USB_INTERFACE_DEFINITIONS
#define USB_INTERFACE_DEFINITIONS

#define PHD_CLASSFUNCTION_DESCRIPTOR_TYPE 0x20
#define PHD_11073_FUNCTION_DESCRIPTOR_TYPE 0x30

#define PHD_VENDOR 0x01
#define PHD_11073_20601 0x02

// Class Function Descriptor
typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bPHDCDataCode;
	uint8_t bmCapability;
} phd_classfunction_descriptor;

// Function Extension Descriptor with single specification
typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bReserved;
	uint8_t bNumDevSpecs;
} phd_11073_function_descriptor;

#endif /* USB_INTERFACE_DEFINITIONS */