#include "config.h"
#include "rooms.h"
#include "log.h"
#include "matrix_support.h"
#include "rooms_support.h"

struct config *config;
enum log_level_t log_level;

void start_main_timing(struct timing *timing)
{
    double now = omp_get_wtime();
    timing->main_start_time = now;
}

void end_main_timing(struct timing *timing)
{
    double now = omp_get_wtime();
    timing->main_stop_time = now;
    timing->elapsed_total_seconds = now - timing->main_start_time;
}

void update_metrics(struct metrics *metrics, struct timing *timing)
{
    // update timings on metrics
    metrics->total_seconds = timing->elapsed_total_seconds;

    if (config->metrics_file) {
        // metrics file may or may not already exist
        INFO("Reporting metrics to: %s\n", config->metrics_file);
        write_metrics_file(config->metrics_file, metrics);
    }

    if (IS_INFO) {
        summarize_metrics(stdout, metrics);
        printf("\n");
    }

    if (IS_WARN) {
        print_metrics_headers(stdout);
        print_metrics(stdout, metrics);
    }
}

int main(int argc, char* argv [])
{
    config = new_config();
    parse_cli(argc, argv, config, &log_level);

    // test runs with only 4 people and 1 process, ignoring other option
    int n = config->test ? 4 : config->num_persons;
    int num_processes = config->test ? 1 : config->num_processes;

    float temp = config->temp;
    int num_rooms = n / 2; // n guaranteed even by config.c validate_config
    struct metrics *metrics = new_metrics(config);
    metrics->num_rooms = num_rooms;

    int *cost_matrix;
    if (config->test) {
        INFO("Creating test cost matrix 4x4");
        cost_matrix = setup_test_cost_matrix();
    }
    else {
        INFO("Allocating cost matrix %zu x %zu", n, n);
        int *cost_matrix = new_matrix(n, n); // square

        INFO("Generating random data for cost matrix");
        fill_matrix_random(n, n, cost_matrix, 1, 10);
    }

    if (log_level >= info) {
        print_matrix("Initial Cost Matrix", n, n, cost_matrix);
    }

    INFO("Allocating rooms array with 2 people per room %zu x %zu", num_rooms, 2);
    int *rooms_array = new_matrix(num_rooms, 2);

    // main loop
    for (int i = 0; i < num_processes; i++) {
        if (config->test) {
            rooms_array = setup_test_rooms();
        }
        else {
            INFO("Randomizing room allocation of persons");
            randperm(num_rooms, 2, rooms_array);
        }

        if (log_level >= info) {
            print_matrix("Initial Rooms Array", num_rooms, 2, rooms_array);
        }

        struct timing *timing = new_timing();

        // start the clock
        start_main_timing(timing);

        // run the main implementation
        run_rooms(cost_matrix, n, temp, rooms_array, num_rooms, metrics);

        // stop the clock
        end_main_timing(timing);

        // allow the implementation to clean up
        update_metrics(metrics, timing);

        if (log_level >= info) {
            print_matrix("Final; Rooms Array", num_rooms, 2, rooms_array);
        }

        if (config->test) {
            int *expected = setup_test_expected_rooms();
            if (matrices_equal(2, 2, expected, rooms_array)) {
                printf("Test PASSED");
            }
            else {
                fprintf(stderr, "Test FAILED! Result does not match expected:");
                print_matrix("Expected", 2, 2, expected);
                print_matrix("Result", 2, 2, rooms_array);
            }
        }
    }
//    free(config);
//    free(rooms_array);
//    free(cost_matrix);
    return 0;
}

