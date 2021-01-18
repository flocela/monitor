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
    float Utilization();
    void update();

  // TODO: Declare any necessary private members
  private:
    float _cpu_util__percent;
    int _delta_time__sec = -1;
};

#endif