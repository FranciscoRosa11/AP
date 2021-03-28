current_dir=$( cd "$( dirname ${BASH_SOURCE[0]} )" && pwd)
source ${current_dir}/set_env.sh

source setup.sh
mkdir $PROJECT_METRICS_DIR

cd ../..

cd TP1/bin/

echo "Results:"

for i in {100,200,300,400,500,1000,2000}
do
    for j in {1.1,1.2,1.3,1.4,1.5,1.6}
    do
        for a in {0.999,0.899,0.799,0.699,0.599}
        do
            for s in {90,100,110}
            do
                
                    echo "N = $i, T = $j, A = $a and S = $s"
                    ${PROJECT_BIN_DIR}/rooms_sa --persons $i --processes 10 --temp $j --annealing $a --stability $s --metrics $PROJECT_METRICS_DIR/report.csv --error --label test
                    ${PROJECT_BIN_DIR}/rooms_mc --persons $i --processes 10 --temp $j --annealing $a --stability $s --metrics $PROJECT_METRICS_DIR/report.csv --error --label test
                    ${PROJECT_BIN_DIR}/rooms_greedy --persons $i --processes 10 --temp $j --annealing $a --stability $s --metrics $PROJECT_METRICS_DIR/report.csv --error --label test
                    ${PROJECT_BIN_DIR}/rooms_greedy_person --persons $i --processes 10 --temp $j --annealing $a --stability $s --metrics $PROJECT_METRICS_DIR/report.csv --error --label test
            done
        done
    done
done
