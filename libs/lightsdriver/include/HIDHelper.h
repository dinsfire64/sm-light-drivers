#pragma once
#include <hidapi.h>
#include <string>

class HIDHelper {
public:
  HIDHelper(unsigned short vid, unsigned short pid, int interfaceNumber);
  ~HIDHelper();

  bool Open();
  void Close();

  int Write(const unsigned char *data, size_t length);

  bool IsOpen() const { return handle != nullptr; }
  hid_device *GetHandle() const { return handle; }

private:
  unsigned short vendor_id;
  unsigned short product_id;
  int interface_number;
  hid_device *handle;
};
