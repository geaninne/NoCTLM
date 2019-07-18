#ifndef _basicnoc
#define _basicnoc

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>
#include "router.h"

template <class packetStructure, uint8_t x, uint8_t y>
SC_MODULE(basicNoC)
{
	sc_export <sc_fifo_out_if<packetStructure> > inport[x][y];
	sc_export <sc_fifo_in_if<packetStructure> >  outport[x][y];
	
	router<packetStructure> *r[x][y];
	
	void createMeshTopology(){
		// mesh topology
		sc_fifo<packetStructure> *fifo;
		for(int lx=0; lx<x; lx++)
			for(int ly=0; ly<y; ly++)
				if(lx==0){

					fifo=new sc_fifo<packetStructure>;
					r[lx][ly]->internalOutport[EAST](*fifo);
					r[lx+1][ly]->internalInport[WEST](*fifo);
					
					// ignored port
					fifo=new sc_fifo<packetStructure>;
					r[lx][ly]->internalOutport[WEST](*fifo);
					r[lx][ly]->internalInport[WEST](*fifo);
					
					if(ly==0){
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);
						r[lx][ly+1]->internalInport[SOUTH](*fifo);						
						
					// ignored port
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);						
						r[lx][ly]->internalInport[SOUTH](*fifo);						
					}
					else if(ly==y-1){
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);
						r[lx][ly-1]->internalInport[NORTH](*fifo);

					// ignored port
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);						
						r[lx][ly]->internalInport[NORTH](*fifo);						

					}
					else{
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);
						r[lx][ly+1]->internalInport[SOUTH](*fifo);

						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);
						r[lx][ly-1]->internalInport[NORTH](*fifo);
					}
				}
				else if(lx==x-1){
					fifo=new sc_fifo<packetStructure>;
					r[lx][ly]->internalOutport[WEST](*fifo);
					r[lx-1][ly]->internalInport[EAST](*fifo);

					// ignored port
					fifo=new sc_fifo<packetStructure>;
					r[lx][ly]->internalOutport[EAST](*fifo);
					r[lx][ly]->internalInport[EAST](*fifo);
					
					if(ly==0){
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);
						r[lx][ly+1]->internalInport[SOUTH](*fifo);						
						
					// ignored port
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);						
						r[lx][ly]->internalInport[SOUTH](*fifo);						
					}
					else if(ly==y-1){
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);
						r[lx][ly-1]->internalInport[NORTH](*fifo);

					// ignored port
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);						
						r[lx][ly]->internalInport[NORTH](*fifo);						

					}
					else{
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);
						r[lx][ly+1]->internalInport[SOUTH](*fifo);

						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);
						r[lx][ly-1]->internalInport[NORTH](*fifo);
					}
				}
				else{
					fifo=new sc_fifo<packetStructure>;
					r[lx][ly]->internalOutport[WEST](*fifo);
					r[lx-1][ly]->internalInport[EAST](*fifo);

					fifo=new sc_fifo<packetStructure>;
					r[lx][ly]->internalOutport[EAST](*fifo);
					r[lx+1][ly]->internalInport[WEST](*fifo);

					if(ly==0){
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);
						r[lx][ly+1]->internalInport[SOUTH](*fifo);						
						
					// ignored port
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);						
						r[lx][ly]->internalInport[SOUTH](*fifo);						
					}
					else if(ly==y-1){
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);
						r[lx][ly-1]->internalInport[NORTH](*fifo);

					// ignored port
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);						
						r[lx][ly]->internalInport[NORTH](*fifo);						

					}
					else{
						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[NORTH](*fifo);
						r[lx][ly+1]->internalInport[SOUTH](*fifo);

						fifo=new sc_fifo<packetStructure>;
						r[lx][ly]->internalOutport[SOUTH](*fifo);
						r[lx][ly-1]->internalInport[NORTH](*fifo);
					}
				}			

	}
	
	SC_CTOR(basicNoC){
		// in and out noc interface connection
		for(int lx=0; lx<x; lx++)
			for(int ly=0; ly<y; ly++){
				char name[100];
				sprintf(name,"router_%dx%d",lx,ly);
				r[lx][ly]=new router<packetStructure>(name,lx,ly);
				inport[lx][ly](r[lx][ly]->inport);
				outport[lx][ly](r[lx][ly]->outport);
			}
			
		createMeshTopology();
			
	}
	
};

#endif
