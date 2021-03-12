#ifndef PROCESSOR_H
#define PROCESSOR_H


class Processor {
  public:
    virtual float Utilization() const = 0;
    virtual void update() = 0;
};

#endif