#ifndef TESTS_H
#define TESTS_H

#include "../../include/rng_interface.h"
#include <stdint.h>

double benchmark_rng(uint32_t (*rng)(void));
void run_test(const char* rng_name, const char* suite);
void run_testu01_battery(rng_t *rng, const char *battery_name);

#endif