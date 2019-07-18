#ifndef _sink_noc
#define _sink_noc

#include "systemc.h"
#include "basicPacket.h"

SC_MODULE(sink_noc){

	sc_fifo_in<basicPacket<int> > port;
	
	coordinate myAddress;
	
	void Consume(){
		basicPacket<int> aux;
		while(1){
			aux=port.read();
			cout << "@sink_noc ("<< this->name() <<"):" << aux << " -@-simtime:" << sc_time_stamp() << endl;
			if(aux.getDestination().address!=myAddress.address){
				cout << "=-=-=-=-=-" << endl;
				cout << "ERROR @sink_noc (MY ADDRESS: "<< myAddress.address <<", pkt Destination:" << aux.getDestination().address << endl;
				cout << aux << endl;
				cout << "=-=-=-=-=-" << endl;				
			}
			wait(SC_ZERO_TIME);
		}
	}
	
//	SC_CTOR(sink_noc){
	sink_noc(sc_module_name _name, uint8_t _x, uint8_t _y){
		myAddress.x=_x;
		myAddress.y=_y;
		SC_THREAD(Consume);
	}

    SC_HAS_PROCESS(sink_noc);
	
};

#endif
