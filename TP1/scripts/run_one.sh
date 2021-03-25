#!/usr/bin/env bash
# Run the matrix program multiple times and collect results
current_dir=$( cd "$( dirname ${BASH_SOURCE[0]} )" && pwd )
source ${current_dir}/set_env.sh

prog=${ROOMS_PROG:-noop}
processes=${ROOMS_PROCESSES:-3}
debug_level=${ROOMS_DEBUG:-debug}
persons=${ROOMS_PERSONS:-5}
label=${ROOMS_LABEL:-test}
program=rooms_${prog}

echo "Running ${prog} with $processes processes and $persons persons with log level ${debug_level}"
command="${PROJECT_BIN_DIR}/${program} -n ${persons} -p ${processes} -l ${label} --${debug_level}"
echo "running: $command"
${command}
echo "finished: $command"
