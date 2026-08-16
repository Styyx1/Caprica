#pragma once

#include "ByteSwap.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace caprica {

struct CapricaBinaryReader {
  Endianness endianness { Endianness::Little };
  explicit CapricaBinaryReader(const std::string& file) : strm(file, std::ifstream::binary) {
    strm.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  }
  CapricaBinaryReader(const CapricaBinaryReader&) = delete;
  ~CapricaBinaryReader() = default;

  bool eof() {
    strm.peek();
    return strm.eof();
  }

  template <typename T>
  T read();

protected:
  std::ifstream strm;
};

template <>
inline int8_t CapricaBinaryReader::read() {
  int8_t val;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap(val);
}

template <>
inline uint8_t CapricaBinaryReader::read() {
  uint8_t val = 0;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap(val);
}

template <>
inline int16_t CapricaBinaryReader::read() {
  int16_t val;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap(val);
}

template <>
inline uint16_t CapricaBinaryReader::read() {
  uint16_t val;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap(val);
}

template <>
inline int32_t CapricaBinaryReader::read() {
  int32_t val;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap(val);
}

template <>
inline uint32_t CapricaBinaryReader::read() {
  uint32_t val;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap(val);
}

template <>
inline float CapricaBinaryReader::read() {
  float val;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap_float(val);
}

template <>
inline time_t CapricaBinaryReader::read() {
  static_assert(sizeof(time_t) == 8, "time_t is not 64 bits");
  time_t val;
  strm.read((char*)&val, sizeof(val));
  return endianness == Endianness::Little ? val : byteswap(val);
}

template <>
inline std::string CapricaBinaryReader::read() {
  auto len = read<uint16_t>();
  std::unique_ptr<char[]> buf(new char[len]);
  strm.read(buf.get(), len);
  return std::string(buf.get(), buf.get() + len);
}

}
