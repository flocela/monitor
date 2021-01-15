#include <string>

#include "format.h"

using std::string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int seconds_per_hour = 3600;
    int seconds_per_min  = 60;
    int num_hours = (int)(seconds/seconds_per_hour);
    int num_minutes = ( (int)( (seconds - (num_hours * seconds_per_hour)) / seconds_per_min ) );
    int num_seconds = seconds - (num_hours * seconds_per_hour) - (num_minutes * seconds_per_min);
    return (std::to_string(num_hours) + ":" + std::to_string(num_minutes) + ":" + std::to_string(num_seconds)); 

}