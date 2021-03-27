source set_env.sh
mkdir $PROJECT_METRICS_DIR

cd ../..

make clean
make

cd TP1/bin/

echo "Results:"

for i in {100,200,300}
do
    for j in {1.1,1.2,1.3,1.4,1.5}
    do
        for a in {0.999, 0.899, 0.799, 0.699, 0.599}
        do
            echo "N = $i and P = $j"
            ./rooms_sa --persons $i --processes 1 --temp $j --annealing $a --metrics $PROJECT_METRICS_DIR/report.csv --error --label test --debug
            ./rooms_mc --persons $i --processes 1 --temp $j --annealing $a --metrics $PROJECT_METRICS_DIR/report.csv --error --label test --debug
        done
    done
done