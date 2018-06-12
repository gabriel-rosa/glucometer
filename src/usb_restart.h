#ifndef USB_RESTART
#define USB_RESTART

#include <stdlib.h>
#include <string.h>
#include <setupapi.h>
#pragma comment (lib, "Setupapi.lib")

#define STR_BUFFER_SIZE 256

int usb_restart(
    uint16_t vendor_id,
    uint16_t product_id)
{
	short last_error = 0;

    HDEVINFO dev_info = SetupDiGetClassDevsA(NULL, "USB", NULL, DIGCF_PRESENT|DIGCF_ALLCLASSES);
    if (dev_info == INVALID_HANDLE_VALUE) {
		return -1;
    }

    uint32_t size, reg_type;
    char strbuf[STR_BUFFER_SIZE];

    SP_DEVINFO_DATA dev_info_data;
    dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
    for (int i=0; SetupDiEnumDeviceInfo(dev_info, i, &dev_info_data); ++i) {

        uint16_t my_vendor_id = 0;
        uint16_t my_product_id = 0;

        // Retrieve the first hardware ID
        if (SetupDiGetDeviceRegistryPropertyA(dev_info, &dev_info_data, SPDRP_HARDWAREID,
            &reg_type, (unsigned char*)strbuf, STR_BUFFER_SIZE, &size)) {
            
            if (size < 4) 
                continue;

            // skip first 4 characters ("USB/")
            for (int j=4; j < size; ++j) {
                if (strncmp(&strbuf[j], "VID_", 4) == 0) {
                    my_vendor_id = (uint16_t)strtol(&strbuf[j+4], NULL, 16);
                } else if (strncmp(&strbuf[j], "PID_", 4) == 0) {
                    my_product_id = (uint16_t)strtol(&strbuf[j+4], NULL, 16);
                }
            }
        }

        if (my_vendor_id != vendor_id || my_product_id != product_id)
            continue;

        // found the device we want
        SP_PROPCHANGE_PARAMS params;
        params.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
        params.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
        params.HwProfile = 0;
        params.Scope = DICS_FLAG_CONFIGSPECIFIC;
        params.StateChange = DICS_PROPCHANGE;

        if(!SetupDiSetClassInstallParamsA(dev_info, &dev_info_data, (SP_CLASSINSTALL_HEADER*)&params, sizeof(SP_PROPCHANGE_PARAMS))) {
			last_error = GetLastError();
			return last_error;
        }   
           
        if(!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, dev_info, &dev_info_data)) {   
			last_error = GetLastError();
			return last_error;
        }

		// device was flagged to reboot
		SP_DEVINSTALL_PARAMS install_params;
		install_params.cbSize = sizeof(install_params);
		if (SetupDiGetDeviceInstallParams(dev_info, &dev_info_data, &install_params) && (install_params.Flags & (DI_NEEDRESTART | DI_NEEDREBOOT))) {
			return -1;
		}
		

		break;
    }

    SetupDiDestroyDeviceInfoList(dev_info);

	return 0;
}

#endif /*USB_RESTART*/