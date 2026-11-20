#pragma once

#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

class StateWriter {
public:
  StateWriter(const std::string &filename, char type)
      : binary(filename.size() >= 4 &&
               filename.compare(filename.size() - 4, 4, ".bin") == 0),
        out(filename, binary ? std::ios::binary : std::ios::out) {
    if (!out)
      throw std::runtime_error("не удалось открыть файл для записи");
    if (binary) {
      out.write("LR3B", 4);
      out.put(type);
    }
  }

  void number(int value) {
    if (binary)
      unsignedNumber(static_cast<std::uint32_t>(value));
    else
      out << value << '\n';
  }

  void text(const std::string &value) {
    if (!binary) {
      out << value << '\n';
      return;
    }
    if (value.size() > std::numeric_limits<std::uint32_t>::max())
      throw std::runtime_error("слишком длинная строка");
    unsignedNumber(static_cast<std::uint32_t>(value.size()));
    out.write(value.data(), static_cast<std::streamsize>(value.size()));
  }

  void finish() {
    out.close();
    if (!out)
      throw std::runtime_error("не удалось записать файл состояния");
  }

private:
  bool binary;
  std::ofstream out;

  void unsignedNumber(std::uint32_t value) {
    for (int shift = 0; shift < 32; shift += 8)
      out.put(static_cast<char>((value >> shift) & 0xFF));
  }
};

class StateReader {
public:
  StateReader(const std::string &filename, char type)
      : in(filename, std::ios::binary) {
    if (!in)
      return;
    char header[4]{};
    if (in.read(header, 4) && std::string(header, 4) == "LR3B") {
      binary = true;
      if (in.get() != type)
        throw std::runtime_error("тип структуры в файле не совпадает");
      const auto start = in.tellg();
      in.seekg(0, std::ios::end);
      remaining = in.tellg() - start;
      in.seekg(start);
    } else {
      in.close();
      in.clear();
      in.open(filename);
    }
  }

  bool count(int &value) {
    if (!in.is_open())
      return false;
    if (!binary && in.peek() == std::char_traits<char>::eof())
      return false;
    value = number();
    if (value < 0 || (binary && value > remaining / 4))
      fail();
    return true;
  }

  int number() {
    if (binary) {
      auto value = static_cast<std::int64_t>(unsignedNumber());
      if (value > 0x7FFFFFFF)
        value -= 0x100000000LL;
      return static_cast<int>(value);
    }
    int value;
    if (!(in >> value))
      fail();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
  }

  std::string text() {
    std::string value;
    if (binary) {
      const auto length = unsignedNumber();
      if (length > remaining)
        fail();
      value.resize(length);
      if (!in.read(value.data(), static_cast<std::streamsize>(length)))
        fail();
      remaining -= length;
    } else if (!std::getline(in, value)) {
      fail();
    }
    return value;
  }

private:
  std::ifstream in;
  bool binary = false;
  std::streamoff remaining = 0;

  [[noreturn]] static void fail() {
    throw std::runtime_error("повреждён файл состояния");
  }

  std::uint32_t unsignedNumber() {
    if (remaining < 4)
      fail();
    std::uint32_t value = 0;
    for (int shift = 0; shift < 32; shift += 8)
      value |= static_cast<std::uint32_t>(in.get()) << shift;
    if (!in)
      fail();
    remaining -= 4;
    return value;
  }
};
