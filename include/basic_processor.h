#ifndef BASIC_PROCESSOR_H
#define BASIC_PROCESSOR_H

#include "processor.h"

class BasicProcessor:virtual public Processor {
  public: 
    BasicProcessor(int delta_time__sec);
    float Utilization() const;
    void update();

  private:
    float _cpu_util__fraction;
    int _delta_time__sec = -1;
};

#endif