#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "config.h"
#include "log.h"

extern struct config *config;

/**
 * Initialize a new config to hold the run configuration set from the command line
 */
struct config *new_config()
{
    struct config *new_config = (struct config *)malloc(sizeof(struct config));
    new_config->metrics_file = NULL;
    new_config->label = "no-label";
    new_config->num_persons = 10;
    new_config->num_processes = 1;
    new_config->temp = 1.f;
    new_config->test = false;
    return new_config;
}

/**
 * Initialize a new metrics to hold the run performance metrics and settings
 */
struct metrics *new_metrics(struct config *config)
{
    struct metrics *new_metrics = (struct metrics *)malloc(sizeof(struct metrics));
    new_metrics->label = config->label;
    new_metrics->num_processes = config->num_processes;
    new_metrics->num_persons = config->num_persons;
    new_metrics->num_processes = config->num_processes;
    new_metrics->temp = config->temp;
    new_metrics->total_seconds = 0;
    new_metrics->steps = 0;
    new_metrics->cost = 0;
    return new_metrics;
}

/**
 * Initialize a new timing object to hold iteration and total times
 */
struct timing *new_timing()
{
    struct timing *new_timing = (struct timing *)malloc(sizeof(struct timing));
    new_timing->main_start_time = 0;
    new_timing->main_stop_time = 0;
    new_timing->elapsed_total_seconds = 0;
    return new_timing;
}

void usage()
{
    fprintf(stderr, "Usage: rooms_<program> [options]\n");
    fprintf(stderr, "Options include:\n");
    fprintf(stderr, "    -n --persons NUM number of people to divide among rooms\n");
    fprintf(stderr, "    -p --processes NUM number of iterations to perform\n");
    fprintf(stderr, "    -t --temp TEMPERATURE FOR SA ALG\n");
    fprintf(stderr, "    -m METRICS.CSV append metrics to this CSV file (creates it if it does not exist)\n");
    fprintf(stderr, "    --info for info level messages\n");
    fprintf(stderr, "    --verbose for extra detail messages\n");
    fprintf(stderr, "    --warn to suppress all but warning and error messages\n");
    fprintf(stderr, "    --error to suppress all error messages\n");
    fprintf(stderr, "    --debug for debug level messages\n");
    fprintf(stderr, "    --trace for very fine grained debug messages\n");
    fprintf(stderr, "\n");
    exit(1);
}

char* valid_file(char opt, char *filename)
{
    if (access(filename, F_OK ) == -1 ) {
        fprintf(stderr, "Error: The option '%c' expects the name of an existing file (cannot find %s)\n", opt, filename);
        usage();
    }
    return filename;
}

int valid_count(char opt, char *arg)
{
    int value = atoi(arg);
    if (value <= 0) {
        fprintf(stderr, "Error: The option '%c' expects a counting number (got %s)\n", opt, arg);
        usage();
    }
    return value;
}

float valid_float(char opt, char *arg)
{
    float value = atof(arg);
    if (value <= 0.0f) {
        fprintf(stderr, "Error: The option '%c' expects a counting number (got %s)\n", opt, arg);
        usage();
    }
    return value;
}

void validate_config(struct config *config)
{
    // ensure even persons
    if (config->num_persons % 2 != 0) {
        FAIL("The number of persons must be even to divide between rooms");
    }

    if (IS_DEBUG) {
        printf("Config:\n");
        printf("Metrics file       : %-10s\n", config->metrics_file);
        printf("Num Persons (N)    : %-10d\n", config->num_persons);
        printf("Num Processes (P)  : %-10d\n", config->num_processes);
        printf("Temperature (T)    : %-10f\n", config->temp);
        printf("\n");
    }
}

/**
 * Parse command line args and construct a config object
 */
void parse_cli(int argc, char *argv[], struct config *new_config, enum log_level_t *new_log_level)
{
    int opt;
    struct option long_options[] = {
            {"metrics", required_argument, NULL,           'm'},
            {"label", required_argument, NULL,             'l'},
            {"persons", required_argument, NULL,           'n'},
            {"processes", required_argument, NULL,         'p'},
            {"test", required_argument, NULL,              'x'},
            {"help", required_argument, NULL,              'h'},
            {"temp", required_argument, NULL,              't'},
            // log options
            {"error", no_argument, (int *)new_log_level,   error},
            {"warn", no_argument, (int *)new_log_level,    warn},
            {"info", no_argument, (int *)new_log_level,    info},
            {"verbose", no_argument, (int *)new_log_level, verbose},
            {"debug", no_argument, (int *)new_log_level,   debug},
            {"trace", no_argument, (int *)new_log_level,   trace},
            {NULL, 0, NULL,                                0}
    };
    int option_index = 0;
    while((opt = getopt_long(argc, argv, "-p:n:l:m:hx" , long_options, &option_index)) != -1)
    {
//        fprintf(stderr, "FOUND OPT: [%c]\n", opt);
        switch(opt) {
            case 0:
                /* If this option set a flag, do nothing else: the flag is set */
                if (long_options[option_index].flag != 0)
                    break;
                // unexpected for now but maybe useful later
                printf("Unexpected option %s\n", long_options[option_index].name);
                usage();
            case 'h':
                usage();
                break;
            case 'p':
                new_config->num_processes = valid_count(optopt, optarg);
                break;
            case 'n':
                new_config->num_persons = valid_count(optopt, optarg);
                break;
            case 't':
                new_config->temp = valid_float(optopt, optarg);
            case 'm':
                new_config->metrics_file = optarg;
                break;
            case 'l':
                new_config->label = optarg;
                break;
            case 'x':
                new_config->test = true;
                break;
            case ':':
                fprintf(stderr, "ERROR: Option %c needs a value\n", optopt);
                usage();
                break;
            case '?':
                fprintf(stderr, "ERROR: Unknown option: %c\n", optopt);
                usage();
                break;
            default:
                fprintf(stderr, "ERROR: Unknown option: %c\n", optopt);
        }
    }

    validate_config(new_config);
}

/**
 * Print the headers for the metrics table to a file pointer.
 * Used for the first run to use a metrics file to produce the header row
 *
 * @param out file pointer
 */
void print_metrics_headers(FILE *out)
{
    fprintf(out, "label,total_seconds,num_persons,num_processes,steps_used,final_cost\n");
}

/**
 * Print the results of the run with timing numbers in a single row to go in a csv file
 * @param out output file pointer
 * @param metrics metrics object
 */
void print_metrics(FILE *out, struct metrics *metrics)
{
    fprintf(out, "%s,%f,%d,%d,%d,%d\n",
            metrics->label, metrics->total_seconds,
            metrics->num_persons, metrics->num_processes,
            metrics->steps, metrics->cost);
}

/**
 * Summarize the results of the run with timing numbers in a single row to go in a csv file
 * @param out output file pointer
 * @param metrics metrics object
 */
void summarize_metrics(FILE *out, struct metrics *metrics)
{
    fprintf(out, "Run Label       : %s\n"
                 "Num persons   N : %d\n"
                 "Num rooms   N/2 : %d\n"
                 "Num processes P : %d\n"
                 "Steps used      : %d\n"
                 "Final cost      : %d\n"
                 "Total seconds   : %f\n",
            metrics->label, metrics->num_persons, metrics->num_rooms,
            metrics->num_processes, metrics->steps, metrics->cost,
            metrics->total_seconds);
}

void write_metrics_file(char *metrics_file_name, struct metrics *metrics)
{
    char *mode = "a"; // default to append to the metrics file
    bool first_time = false;
    if (access(metrics_file_name, F_OK ) == -1 ) {
        // first time - lets change the mode to "w" and append
        fprintf(stdout, "Creating metrics file and adding headers: %s", metrics_file_name);
        first_time = true;
        mode = "w";
    }
    FILE *metrics_file = fopen(metrics_file_name, mode);
    if (first_time) {
        print_metrics_headers(metrics_file);
    }

    print_metrics(metrics_file, metrics);
}

