#!/usr/bin/env bash
# Run the matrix program multiple times and collect results
current_dir=$( cd "$( dirname ${BASH_SOURCE[0]} )" && pwd )
source ${current_dir}/set_env.sh

multirun() {
  echo "LOOPING over programs"
  for progname in "${programs[@]}"; do
    echo "- LOOP prog # $progname "
    program=${PROJECT_BIN_DIR}/${progname}
    local thread_label=""
    echo "- LOOPING over persons from $min_persons up to $max_persons step = $persons_step"
    for ((n=${min_persons}; n<=${max_persons}; n+=${persons_step})) do
      echo "- - LOOP person # $n "
      # always start at 1 then jump evenly
      if [ "$n" -eq "0" ];then
        persons=1
      else
        person_label="__p=${n}"
        persons=$n
      fi
      echo "- - LOOPING over processes from $min_processes up to $max_processes step = $processes_step"
      for ((p=${min_processes}; p<=${max_processes}; p+=${processes_step})) do
        echo "- - - LOOP process # $p "
        # always start at 1 then jump evenly
        if [ "$p" -eq "0" ];then
          processes=1
        else
          process_label="__p=${p}"
          processes=$p
        fi

        echo "- - - - LOOP matrix-size # $c "
#       full_label="${omp_label}; schedule=$order;$chunk_size"
        full_label="${person_label};${process_label}"
        singlerun
      done
    done
  done
}

singlerun() {
    to_run="${program} --persons ${persons} --processes ${processes} ${debug_arg} -m ${metrics_file} -l ${full_label}"
    echo "About to: ${to_run}"
    if $interactive; then
      askcontinue
    fi
    if [ -z ${BATCH_RUN_NOOP+x} ]; then
      ${to_run}
    else
      echo "DRY RUN. Not doing anything"
    fi
}

askcontinue() {
  local question=${1:-"Do you want to continue?"}
	read -p "$question (waiting 5s then defaulting to YES)? " -n 1 -r -n 5
	echo    # (optional) move to a new line
	# if no reply (timeout) or yY then yes, else leave it false
	if [[ -z "$REPLY" || $REPLY =~ ^[Yy]$ ]];then
		return 0
	fi
	return 1
}


if [ -z ${BATCH_RUN_INTERACTIVE+x} ]; then
  interactive=false
else
  interactive=true
fi

debug_level=${BATCH_DEBUG:-debug}
report_name=${BATCH_RUN_REPORT:-1}
mkdir -p ${PROJECT_METRICS_DIR}
metrics_file=${PROJECT_METRICS_DIR}/"rooms_metrics${report_name}.csv"

if [ -f "${metrics_file}" ]; then
  if [ -z ${BATCH_RUN_NOOP+x} ]; then
    echo "Found an existing metrics file at ${metrics_file}"
    askcontinue "Do you want to delete the report at ${metrics_file} and start clean?" && rm -f $metrics_file
  fi
fi

programs=( "rooms_noop" "rooms_montecarlo" )
min_processes=${BATCH_RUN_PROCESSES_MIN:-1}
max_processes=${BATCH_RUN_PROCESSES_MAX:-1}
processes_step=${BATCH_RUN_PROCESSES_STEP:-1} # must be non-zero to be non infinite
min_persons=${BATCH_RUN_PERSONS_MIN:-10}
max_persons=${BATCH_RUN_PERSONS_MAX:-20}
persons_step=${BATCH_RUN_PERSONS_STEP:-10} # must be non-zero to be non infinite
label=rooms
multirun

askcontinue "Want to see the results?"
cat ${metrics_file}