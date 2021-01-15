#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "processor_data.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class Processor {
 public:
  Processor(int delta_time__sec);
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    float _cpu_util__percent;
};

#endif