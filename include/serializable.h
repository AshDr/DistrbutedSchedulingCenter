#pragma once
#include <fcntl.h>
#include <vector>
#include <cstdint>
class Serializable {
  public:
    virtual bool Serialize(std::vector<uint8_t> &buf) = 0;
    virtual bool Deserialize(std::vector<uint8_t> &buf) = 0;
    virtual int GetType() = 0;

  public:
    Serializable() {
    }

    virtual ~Serializable() {
    }
};