export BATCH_RUN_NOOP=true
export BATCH_RUN_INTERACTIVE=true
unset BATCH_RUN_NOOP
unset BATCH_RUN_INTERACTIVE

export BATCH_PROCESSES_MIN=10
export BATCH_PROCESSES_MAX=20
export BATCH_PROCESSES_STEP=10
export BATCH_PERSONS_MIN=10
export BATCH_PERSONS_MAX=20
export BATCH_PERSONS_STEP=10
export BATCH_DEBUG=warn
export BATCH_PROG_NUM=1
export BATCH_REPORT=metrics_main.csv

printenv | grep BATCH_
