#ifndef _router
#define _router

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>
#include "basicPacket.h"
using namespace sc_core;

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3
#define LOCAL 4

coordinate myAddress;

template <class packetStructure>
SC_MODULE(router)
{
	sc_export<sc_fifo_out_if<packetStructure> > inport;
	sc_export<sc_fifo_in_if<packetStructure> > outport;
	
	sc_fifo_in<packetStructure> internalInport[4];
	sc_fifo_out<packetStructure>  internalOutport[4];
	
	sc_fifo<packetStructure> inputbuffer;
	sc_fifo<packetStructure> taggedbuffer[5];
	sc_fifo<packetStructure> outputbuffer;
	
	uint8_t x, y;
	void inputPacket(int portID){
		packetStructure aux;
		while(1){
			/* consume from packet */
			aux=(portID==LOCAL)?inputbuffer.read():internalInport[portID].read();

			/* include details at the packet */
			aux.incHops();
			if(portID=LOCAL) aux.setSource(x, y);
			
			/* show details */
			cout << aux  << endl;
			cout << "current time: " << sc_time_stamp() << endl;
			cout << "@router(x,y):[" << (unsigned int)x << "," << (unsigned int)y <<"]" << endl;
			
			cout << "@portID:[" << portID <<"]" << endl;
			myAddress.x = x;
			myAddress.y= y;

			aux.setPath(myAddress.address);

			/* Transfer to a buffer that will be consumed by the routing algoritm */
			taggedbuffer[portID].write(aux);

			wait(SC_ZERO_TIME);
		}
	}
	
	void routingUnit(){
		packetStructure aux;
		while(1){
			for(int i=0;i<5;i++)
			  	if(taggedbuffer[i].nb_read(aux)){
					
					if(aux.getDestination().x>x)
						internalOutport[EAST].write(aux);
					else if(aux.getDestination().x<x)
						internalOutport[WEST].write(aux);
					else if(aux.getDestination().y<y)
						internalOutport[SOUTH].write(aux);
					else if(aux.getDestination().y>y)
						internalOutport[NORTH].write(aux);
					else
						outputbuffer.write(aux);

					wait(6, SC_NS);
				}
				else wait(1, SC_NS);
		}
	}
	
	void SpawnConsumption(){
		for(int portID=0; portID<5; portID++){
			sc_spawn( sc_bind(&router::inputPacket, this, portID));
		}
	}

//	SC_CTOR(router){
	router(sc_module_name _name, uint8_t _x, uint8_t _y): sc_module(_name){
		x=_x;
		y=_y;
		
		inport.bind(inputbuffer);
		outport.bind(outputbuffer);
			
		SC_THREAD(SpawnConsumption);
		SC_THREAD(routingUnit);
		
	} 

    SC_HAS_PROCESS(router);
	  
};

#endif
