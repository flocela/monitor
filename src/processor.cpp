#include <thread>
#include <chrono>
#include "processor_data.h"
#include "processor.h"
#include "linux_parser.h"

static std::string::size_type sz;

Processor::Processor(int delta_time__sec) {
  ProcessorData processor_data_one = LinuxParser::createProcessorData();
  std::this_thread::sleep_for(std::chrono::seconds(delta_time__sec));
  ProcessorData processor_data_two = LinuxParser::createProcessorData();

  float idle_one = processor_data_one._idle__ct + 
                   processor_data_one._iowait__ct;
  float non_idle_one = processor_data_one._user__ct +
                       processor_data_one._nice__ct +
                       processor_data_one._system__ct + 
                       processor_data_one._irq__ct +
                       processor_data_one._softirq__ct + 
                       processor_data_one._steal__ct;
  float total_one = idle_one + non_idle_one;

  float idle_two = processor_data_two._idle__ct + 
                   processor_data_two._iowait__ct;
  float non_idle_two = processor_data_two._user__ct +
                       processor_data_two._nice__ct +
                       processor_data_two._system__ct + 
                       processor_data_two._irq__ct +
                       processor_data_two._softirq__ct + 
                       processor_data_two._steal__ct;
  float total_two = idle_two + non_idle_two;

  float idle_delta  = idle_two - idle_one;
  float total_delta = total_two - total_one;
  
  if (total_delta != 0.0)
    _cpu_util__percent = 100 *((total_delta - idle_delta)/total_delta);
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  return _cpu_util__percent;
}
