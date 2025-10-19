#include "HIDHelper.h"
#include <iostream>

HIDHelper::HIDHelper(unsigned short vid, unsigned short pid, int interfaceNumber)
    : vendor_id(vid), product_id(pid), interface_number(interfaceNumber), handle(nullptr)
{
    hid_init();
}

HIDHelper::~HIDHelper()
{
    Close();
}

bool HIDHelper::Open()
{
    if (handle)
    {
        // already open
        return true;
    }

    struct hid_device_info *devs = hid_enumerate(vendor_id, 0);
    struct hid_device_info *cur_dev = devs;

    bool found = false;

    while (cur_dev)
    {
        if (cur_dev->vendor_id == vendor_id && cur_dev->product_id == product_id)
        {
            if (interface_number == -1 || cur_dev->interface_number == interface_number)
            {
                handle = hid_open_path(cur_dev->path);

                if (handle)
                {
                    found = true;
                }

                break;
            }
        }

        cur_dev = cur_dev->next;
    }

    hid_free_enumeration(devs);
    return found;
}

int HIDHelper::Write(const unsigned char *data, size_t length)
{
    if (handle)
    {
        return hid_write(handle, data, length);
    }

    return -1;
}

void HIDHelper::Close()
{
    if (handle)
    {
        hid_close(handle);
        handle = nullptr;
    }
}
