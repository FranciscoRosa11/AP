#include <stdio.h>
#include "config.h"
#include "rooms.h"
#include "matrix_support.h"
#include "rooms_support.h"
#include "math.h"

/**
 * Simulated Annealing Implementation:
 * Same base algo as Monte Carlo but adds a chance of swapping rooms even when the swap
 * does not necessarily reduce the overall cost.
 * This is done by adding a "temperature" T that cools at a certain rate per iteration
 * (annealing). At higher temperatures and unexpected swap is more probably.
 * So as time (iterations) goes by swaps are more likely to be driven only by actual
 * improvements.
 */

// T is the temperature - the probability of swapping even if the swap does not reduce cost
float T;
// annealing_rate is the rate at which T (temperature) reduces each iteration
float annealing_rate;

void initialize_algorithm(struct config *config, struct metrics *metrics)
{
    metrics->algo_name = "Simulated_Annealing";
    T = config->initial_temperature;
    annealing_rate = config->annealing_rate;
}

void start_iteration()
{
    // NOOP
}

void end_iteration()
{
    T = annealing_rate * T; // reduce the temperature (annealing)
}

/**
 * Simulated Annealing:
 * if the swap is an improvement ( delta < 0 ) or if the delta is small enough
 * such that e^-(delta/T) > some random number between 0 and 1, then swap
 */
bool should_swap(int delta)
{
    // if the swap is an improvement ( delta < 0 ) or if the delta
    // is small enough such that e^-(delta/T) > some random number between 0 and 1,
    // then swap
    float delta_on_T = (float) delta / T;
    float exp_minus_delta_on_T = expf(-1.f * delta_on_T);
    float threshold = random_float();
    DEBUG("delta=%d   T=%f  delta/T=%f  exp=%f, threshold=%f",
          delta, T, delta_on_T, exp_minus_delta_on_T, threshold);
    return (delta < 0 || exp_minus_delta_on_T > threshold);
}

