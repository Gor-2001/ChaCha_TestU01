#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chacha/chacha.h"
#include "TestU01.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

static uint32_t current_rng_next32(void);


typedef enum {
    RNG_CHACHA20,
    RNG_CHACHA12,
    RNG_CHACHA8,
    RNG_CHACHA4,
    RNG_CHACHA4_FEEDBACK,
    RNG_BAD
} rng_type_t;

static rng_type_t current_rng;

static uint32_t current_rng_next32(void) {
    switch(current_rng) {
        case RNG_CHACHA20: return chacha20_next32();
        case RNG_CHACHA12: return chacha12_next32();
        case RNG_CHACHA8: return chacha8_next32();
        case RNG_CHACHA4: return chacha4_next32();
        case RNG_CHACHA4_FEEDBACK: return chacha4_feedback_next32();
        case RNG_BAD: return bad_prng_next32();
        default: return 0;
    }
}

static void seed_current_rng(uint64_t seed) {
    switch(current_rng) {
        case RNG_CHACHA20: chacha20_seed(seed); break;
        case RNG_CHACHA12: chacha12_seed(seed); break;
        case RNG_CHACHA8: chacha8_seed(seed); break;
        case RNG_CHACHA4: chacha4_seed(seed); break;
        case RNG_CHACHA4_FEEDBACK: chacha4_feedback_seed(seed); break;
        case RNG_BAD: bad_prng_seed(seed); break;
    }
}

static const char* rng_name(rng_type_t type) {
    switch(type) {
        case RNG_CHACHA20: return "ChaCha20";
        case RNG_CHACHA12: return "ChaCha12";
        case RNG_CHACHA8: return "ChaCha8";
        case RNG_CHACHA4: return "ChaCha4";
        case RNG_CHACHA4_FEEDBACK: return "ChaCha4-Feedback";
        case RNG_BAD: return "BadPRNG";
        default: return "Unknown";
    }
}

static void remove_directory_contents(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) return;
    
    struct dirent *entry;
    char filepath[512];
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
        remove(filepath);
    }
    
    closedir(dir);
}

static void prepare_results_directory(void) {
    struct stat st = {0};
    
    if (stat("results", &st) == 0) {
        remove_directory_contents("results");
    } else {
        mkdir("results", 0755);
    }
}

static void extract_and_write_summary(const char *detail_filename, const char *summary_filename) {
    FILE *fp = fopen(detail_filename, "r");
    if (!fp) return;
    
    FILE *summary_fp = fopen(summary_filename, "a");
    if (!summary_fp) {
        fclose(fp);
        return;
    }
    
    char line[256];
    int in_summary = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "========= Summary results")) {
            in_summary = 1;
        }
        if (in_summary) {
            printf("%s", line);
            fprintf(summary_fp, "%s", line);
            if (strstr(line, "All tests were passed") || 
                strstr(line, "Some tests were failed") ||
                strstr(line, "tests were failed")) {
                printf("\n");
                fprintf(summary_fp, "\n");
                break;
            }
        }
    }
    
    fclose(fp);
    fclose(summary_fp);
}

static void run_smallcrush(rng_type_t type, const char *summary_filename) {
    current_rng = type;
    uint64_t seed = (uint64_t)time(NULL) ^ (uint64_t)type;
    seed_current_rng(seed);
    
    char detail_filename[256];
    snprintf(detail_filename, sizeof(detail_filename), "results/detail_%s_smallcrush.txt", rng_name(type));
    
    printf("========================================\n");
    printf("Testing %s with SmallCrush\n", rng_name(type));
    printf("========================================\n");
    
    FILE *old_stdout = stdout;
    stdout = fopen(detail_filename, "w");
    if (!stdout) {
        stdout = old_stdout;
        fprintf(stderr, "Failed to open %s\n", detail_filename);
        return;
    }
    
    unif01_Gen *gen = unif01_CreateExternGenBits((char*)rng_name(type), current_rng_next32);
    bbattery_SmallCrush(gen);
    unif01_DeleteExternGenBits(gen);
    
    fclose(stdout);
    stdout = old_stdout;
    
    extract_and_write_summary(detail_filename, summary_filename);
}


static void run_crush(rng_type_t type, const char *summary_filename) {
    current_rng = type;
    uint64_t seed = (uint64_t)time(NULL) ^ (uint64_t)type;
    seed_current_rng(seed);
    
    char detail_filename[256];
    snprintf(detail_filename, sizeof(detail_filename), "results/detail_%s_crush.txt", rng_name(type));
    
    printf("========================================\n");
    printf("Testing %s with Crush\n", rng_name(type));
    printf("========================================\n");
    
    FILE *old_stdout = stdout;
    stdout = fopen(detail_filename, "w");
    if (!stdout) {
        stdout = old_stdout;
        fprintf(stderr, "Failed to open %s\n", detail_filename);
        return;
    }
    
    unif01_Gen *gen = unif01_CreateExternGenBits((char*)rng_name(type), current_rng_next32);
    bbattery_Crush(gen);
    unif01_DeleteExternGenBits(gen);
    
    fclose(stdout);
    stdout = old_stdout;
    
    extract_and_write_summary(detail_filename, summary_filename);
}

static void run_bigcrush(rng_type_t type, const char *summary_filename) {
    current_rng = type;
    uint64_t seed = (uint64_t)time(NULL) ^ (uint64_t)type;
    seed_current_rng(seed);
    
    char detail_filename[256];
    snprintf(detail_filename, sizeof(detail_filename), "results/detail_%s_bigcrush.txt", rng_name(type));
    
    printf("========================================\n");
    printf("Testing %s with BigCrush\n", rng_name(type));
    printf("========================================\n");
    
    FILE *old_stdout = stdout;
    stdout = fopen(detail_filename, "w");
    if (!stdout) {
        stdout = old_stdout;
        fprintf(stderr, "Failed to open %s\n", detail_filename);
        return;
    }
    
    unif01_Gen *gen = unif01_CreateExternGenBits((char*)rng_name(type), current_rng_next32);
    bbattery_BigCrush(gen);
    unif01_DeleteExternGenBits(gen);
    
    fclose(stdout);
    stdout = old_stdout;
    
    extract_and_write_summary(detail_filename, summary_filename);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <test_level>\n", argv[0]);
        printf("  test_level: small, crush, big\n");
        return 1;
    }
    
    prepare_results_directory();
    
    void (*test_func)(rng_type_t, const char*);
    char summary_filename[256];
    
    if (strcmp(argv[1], "small") == 0) {
        test_func = run_smallcrush;
        snprintf(summary_filename, sizeof(summary_filename), "results/summary_smallcrush.txt");
    } else if (strcmp(argv[1], "crush") == 0) {
        test_func = run_crush;
        snprintf(summary_filename, sizeof(summary_filename), "results/summary_crush.txt");
    } else if (strcmp(argv[1], "big") == 0) {
        test_func = run_bigcrush;
        snprintf(summary_filename, sizeof(summary_filename), "results/summary_bigcrush.txt");
    } else {
        printf("Unknown test level: %s\n", argv[1]);
        return 1;
    }
    
    printf("\nResults directory prepared\n");
    printf("Summaries will be written to: %s\n\n", summary_filename);
    
    test_func(RNG_BAD, summary_filename);
    //test_func(RNG_CHACHA20, summary_filename);
    //test_func(RNG_CHACHA12, summary_filename);
    //test_func(RNG_CHACHA8, summary_filename);
    //test_func(RNG_CHACHA4, summary_filename);
    //test_func(RNG_CHACHA4_FEEDBACK, summary_filename);
    
    printf("========================================\n");
    printf("All tests completed\n");
    printf("All results saved in: results/\n");
    printf("========================================\n");
    
    return 0;
}