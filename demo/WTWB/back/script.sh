#!/bin/bash
gem5_path=~/co_proj/gem5
result_path=~/co_proj/demo/WTWB/through

benchmark_mul=~/co_proj/benchmark/multiply
benchmark_qsort=~/co_proj/benchmark/quick_small

arg='--cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --mem-type=NVMainMemory --nvmain-config=/home/t1nf/co_proj/NVmain/Config/PCM_ISSCC_2012_4GB.config --l1i_size=32kB --l1d_size=32kB --l2_size=128kB --l3_size=128kB --l3_assoc=4'

${gem5_path}/build/X86/gem5.opt ${gem5_path}/configs/example/se.py -c $benchmark_mul $arg > ${result_path}/result 
