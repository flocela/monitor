#ifndef BASIC_PROCESSOR_H
#define BASIC_PROCESSOR_H

#include "processor_data.h"
#include "processor.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class BasicProcessor:virtual public Processor {
  public:
    BasicProcessor(int delta_time__sec);
    float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
  private:
    float _cpu_util__percent;
};

#endif