#export SYSTEMC=/home/geaninne/Downloads/systemc-2.3.1a
#export SYSTEMC=${SYSTEMC_HOME}
export SYSTEMC_HOME=/usr/local/systemc-2.3.2/
g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 -o noc main.cpp -lsystemc -lm
