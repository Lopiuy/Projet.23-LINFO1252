#!/bin/bash

# shellcheck disable=SC2016

./scripts/performanceBaseProblems.sh
echo PHILOSOPHES :
cat measures/measure_philo.csv
echo PRODUCTEUR CONSOMATEUR :
cat measures/measure_prodcons.csv
echo LECTEUR ÉCRIVAINT :
cat measures/measure_rw.csv

./scripts/performancetasttas.sh
echo TEST AND SET :
cat measures/measure_tas.csv
echo TEST AND TEST AND SET :
cat measures/measure_ttas.csv
./scripts/performancebackoff.sh
echo BACKOFF :
cat measures/measure_backoff.csv

./scripts/performanceTasProblems.sh
echo PHILOSOPHES TAS :
cat measures/measure_philotas.csv
echo PRODUCTEUR CONSOMATEUR TAS :
cat measures/measure_prodconstas.csv
echo LECTEUR ÉCRIVAINT TAS :
cat measures/measure_rwtas.csv

./scripts/performanceTtasProblems.sh
echo PHILOSOPHES TTAS :
cat measures/measure_philottas.csv
echo PRODUCTEUR CONSOMATEUR TTAS :
cat measures/measure_prodconsttas.csv
echo LECTEUR ÉCRIVAINT TTAS :
cat measures/measure_rwttas.csv
