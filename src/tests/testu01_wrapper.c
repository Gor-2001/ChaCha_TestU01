#include "tests.h"
#include "../../include/rng_interface.h"
#include <TestU01.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

static rng_t *current_rng = NULL;

static unsigned int testu01_wrapper(void) {
    return (unsigned int)current_rng->next32();
}

void run_testu01_battery(rng_t *rng, const char *battery_name) {
    current_rng = rng;
    
    unif01_Gen *gen = unif01_CreateExternGenBits((char *)rng->name, testu01_wrapper);
    
    printf("RNG,%s\n", rng->name);
    printf("Battery,%s\n", battery_name);
    printf("Timestamp,%ld\n", time(NULL));
    printf("\n");
    printf("TestName,Result,pValue,Notes\n");
    
    clock_t start = clock();
    
    if (strcmp(battery_name, "smallcrush") == 0) {
        bbattery_SmallCrush(gen);
    } else if (strcmp(battery_name, "crush") == 0) {
        bbattery_Crush(gen);
    } else if (strcmp(battery_name, "bigcrush") == 0) {
        bbattery_BigCrush(gen);
    } else {
        fprintf(stderr, "Unknown battery: %s\n", battery_name);
        unif01_DeleteExternGenBits(gen);
        return;
    }
    
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("\n");
    printf("ExecutionTime,%.2f\n", elapsed);
    
    unif01_DeleteExternGenBits(gen);
}