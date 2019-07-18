export SYSTEMC=/home/geaninne/Downloads/systemc-2.3.1a
g++ -I$SYSTEMC/include -I. -L. -L$SYSTEMC/lib-linux -o run.x *.cpp -lsystemc -lm
