#pragma once
#include <fcntl.h>
#include <vector>
class Serializable {
  public:
    virtual bool Serialize(std::vector<char> &buf) = 0;
    virtual bool Deserialize(std::vector<char> &buf) = 0;
    virtual int GetType() = 0;

  public:
    Serializable() {
    }

    virtual ~Serializable() {
    }
};