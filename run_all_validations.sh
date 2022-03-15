make

echo "======================TEST RUN======================"
./sim_cache 32 8192 4 262144 8 0 0 gcc_trace.txt

echo "====================VALIDATION 0===================="
./sim_cache 16 1024 2 0 0 0 0 gcc_trace.txt

echo "====================VALIDATION 1===================="
./sim_cache 16 1024 1 0 0 0 0 perl_trace.txt

echo "====================VALIDATION 2===================="
./sim_cache 16 1024 2 0 0 1 0 gcc_trace.txt

echo "====================VALIDATION 3===================="
./sim_cache 16 1024 2 0 0 2 0 vortex_trace.txt

echo "====================VALIDATION 4===================="
./sim_cache 16 1024 2 8192 4 0 0 gcc_trace.txt

echo "====================VALIDATION 5===================="
./sim_cache 16 1024 1 8192 4 0 0 go_trace.txt

echo "====================VALIDATION 6===================="
./sim_cache 16 1024 2 8192 4 0 1 gcc_trace.txt

echo "====================VALIDATION 7===================="
./sim_cache 16 1024 1 8192 4 0 1 compress_trace.txt