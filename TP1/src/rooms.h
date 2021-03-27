
/**
 * Run the room assignment algorithm starting with a number of persons, a number of rooms (persons/2)
 * an initial assignment of persons to rooms, and a a cost matrix that determines the compatibility
 * between any two persons.
 *
 * @param cost_matrix square symmetrical matrix giving the compatibility between pairs of persons
 *        where 1 is most compatible, and 10 is least compatible
 * @param num_persons the number of persons paired in rooms
 * @param rooms_array the array of rooms with initial (random) assignments of persons
 * @param num_rooms the number of rooms: always half the number of persons
 * @param metrics pointer to a structure to hold the cost and number of steps used (to return)
 * @param stability_indicator number of times to iterate without a swap before determining that we are done
 */
extern void run_rooms(int *cost_matrix, int num_persons, int *rooms_array, int num_rooms, struct metrics *metrics,
                      int stability_indicator);

extern void initialize_algorithm(struct config *config, struct metrics *metrics);

extern void start_iteration();

extern void end_iteration();

extern bool should_swap(int delta);
