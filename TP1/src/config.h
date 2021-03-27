#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <omp.h>
#include "log.h"

#define PERSONS_PER_ROOM 2

struct config {
    int num_persons;
    int num_processes;
    float temp;
    char *in_file;
    char *out_file;
    char *test_file;
    char *metrics_file;
    char *label;
    bool test;
};

struct metrics {
    char *label; // label for metrics row from -l command line arg
    double total_seconds;         // total time in seconds for the run
    int num_persons;
    int num_processes; // number of times the algorithm is run
    float temp;
    int num_rooms;
    int steps; // count of total steps used during a run
    int cost; // global cost at the end of a run
};

struct timing {
    double main_start_time;
    double main_stop_time;
    double elapsed_total_seconds;
};

extern struct config *new_config();
extern void parse_cli(int argc, char *argv[], struct config *new_config, enum log_level_t *new_log_level);
extern struct metrics *new_metrics(struct config *config);
extern struct timing *new_timing();

extern void usage();
extern void print_metrics_headers(FILE *out);
extern void print_metrics(FILE *out, struct metrics *metrics);
extern void summarize_metrics(FILE *out, struct metrics *metrics);
extern void write_metrics_file(char *metrics_file_name, struct metrics *metrics);
extern char* valid_file(char opt, char *filename);
extern int valid_count(char opt, char *arg);
extern float valid_float(char opt, char *arg);

#endif