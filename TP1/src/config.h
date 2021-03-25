#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <omp.h>
#include "log.h"

struct config {
    int num_persons;
    int num_processes;
    char *in_file;
    char *out_file;
    char *test_file;
    char *metrics_file;
    char *label;
};

struct metrics {
    char *label; // label for metrics row from -l command line arg
    double total_seconds;         // total time in seconds for the run
    int used_iterations; // number of actual iterations needed to complete clustering
    int num_persons;
    int num_processes; // number of processors that mpi is running on
    int num_rooms;
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

#endif