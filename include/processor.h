#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "processor_data.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class Processor {
  public:
    virtual float Utilization();
    virtual void update();
};

#endif