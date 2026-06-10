#pragma once
#include "SimpleIni.h"
#include <sstream>
#include <string>
#include <type_traits>

struct IniRGB {
  int r = 0, g = 0, b = 0;

  IniRGB() = default;
  IniRGB(int rr, int gg, int bb) : r(rr), g(gg), b(bb) {}

  std::string ToString() const {
    return std::to_string(r) + "," + std::to_string(g) + "," +
           std::to_string(b);
  }

  static IniRGB FromString(const std::string &str) {
    auto clamp = [](int val) { return std::max(0, std::min(255, val)); };

    IniRGB color;
    std::stringstream ss(str);
    std::string item;
    if (std::getline(ss, item, ','))
      color.r = clamp(std::stoi(item));
    if (std::getline(ss, item, ','))
      color.g = clamp(std::stoi(item));
    if (std::getline(ss, item, ','))
      color.b = clamp(std::stoi(item));

    return color;
  }
};

class IniHelper {
private:
  static inline CSimpleIniA ini;
  static inline const char *fileName = "lights_settings.ini";

public:
// --- Define all settings in one place using X Macro ---
// Format: SECTION, NAME, TYPE, DEFAULT_VALUE
#define SETTINGS_LIST                                                          \
  X(Sextet, ComPort, std::string, "COM54")                                     \
  X(Sextet, PipeName, std::string, "StepMania-Lights-SextetStream")            \
  X(SMX, FadeOnStep, float, 0.25)                                              \
  X(SMX, FadeOffStep, float, 0.25)                                             \
  X(SMX, P1_Up, IniRGB, IniRGB(255, 0, 0))                                     \
  X(SMX, P1_Down, IniRGB, IniRGB(255, 0, 0))                                   \
  X(SMX, P1_Left, IniRGB, IniRGB(0, 0, 255))                                   \
  X(SMX, P1_Right, IniRGB, IniRGB(0, 0, 255))                                  \
  X(SMX, P2_Up, IniRGB, IniRGB(255, 0, 0))                                     \
  X(SMX, P2_Down, IniRGB, IniRGB(255, 0, 0))                                   \
  X(SMX, P2_Left, IniRGB, IniRGB(0, 0, 255))                                   \
  X(SMX, P2_Right, IniRGB, IniRGB(0, 0, 255))                                  \
  X(SMX, P1_Start, IniRGB, IniRGB(0, 255, 0))                                  \
  X(SMX, P2_Start, IniRGB, IniRGB(0, 255, 0))                                  \
  X(SMX, BassOverlay, IniRGB, IniRGB(255, 255, 255))                           \
  X(SMX, MarUL, IniRGB, IniRGB(255, 0, 0))                                     \
  X(SMX, MarUR, IniRGB, IniRGB(255, 0, 0))                                     \
  X(SMX, MarLL, IniRGB, IniRGB(0, 0, 255))                                     \
  X(SMX, MarLR, IniRGB, IniRGB(0, 0, 255))                                     \
  X(Mame, IPAddress, std::string, "127.0.0.1")

  // --- Generate structs automatically ---
  struct SextetSettings {
#define X(section, name, type, default_val) type name;
    SETTINGS_LIST
#undef X

    SextetSettings() {
#define X(section, name, type, default_val) name = default_val;
      SETTINGS_LIST
#undef X
    }
  };

  struct SMXSettings {
#define X(section, name, type, default_val) type name;
    SETTINGS_LIST
#undef X

    SMXSettings() {
#define X(section, name, type, default_val) name = default_val;
      SETTINGS_LIST
#undef X
    }
  };

  struct MameSettings {
#define X(section, name, type, default_val) type name;
    SETTINGS_LIST
#undef X

    MameSettings() {
#define X(section, name, type, default_val) name = default_val;
      SETTINGS_LIST
#undef X
    }
  };

  // Actually store the data
  static inline SextetSettings Sextet;
  static inline SMXSettings SMX;
  static inline SMXSettings Mame;

  // --- Generic read/write template ---
  template <typename T>
  static T ReadValue(const char *section, const char *key,
                     const T &defaultValue) {
    if constexpr (std::is_same_v<T, bool>)
      return ini.GetBoolValue(section, key, defaultValue);
    else if constexpr (std::is_same_v<T, int>)
      return static_cast<int>(ini.GetLongValue(section, key, defaultValue));
    else if constexpr (std::is_same_v<T, float>)
      return static_cast<float>(ini.GetDoubleValue(section, key, defaultValue));
    else if constexpr (std::is_same_v<T, IniRGB>) {
      std::string str =
          ini.GetValue(section, key, defaultValue.ToString().c_str());
      return IniRGB::FromString(str);
    } else
      return ini.GetValue(section, key, defaultValue.c_str());
  }

  template <typename T>
  static void WriteValue(const char *section, const char *key, const T &value) {
    if constexpr (std::is_same_v<T, bool>)
      ini.SetBoolValue(section, key, value);
    else if constexpr (std::is_same_v<T, int>)
      ini.SetLongValue(section, key, value);
    else if constexpr (std::is_same_v<T, float>)
      ini.SetDoubleValue(section, key, value);
    else if constexpr (std::is_same_v<T, IniRGB>)
      ini.SetValue(section, key, value.ToString().c_str());
    else
      ini.SetValue(section, key, value.c_str());
  }

  // --- Load all settings automatically ---
  static bool Load() {
    ini.SetUnicode();
    int rtn = ini.LoadFile(fileName);

#define X(section, name, type, default_val)                                    \
  section.name = ReadValue(#section, #name, section.name);
    SETTINGS_LIST
#undef X

    return (rtn < 0);
  }

  // --- Save all settings automatically ---
  static void Save() {
#define X(section, name, type, default_val)                                    \
  WriteValue(#section, #name, section.name);
    SETTINGS_LIST
#undef X

    ini.SaveFile(fileName);
  }
};
