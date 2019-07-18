#ifndef _NOC_PACKET
#define _NOC_PACKET

#include <iostream>
#include <queue> 

using namespace std;

template <class payload> 
class basicPacket;

template <class payload> 
ostream & operator << (ostream &, const basicPacket<payload> &);

union coordinate{
  uint16_t address;
  struct{
    uint8_t y,x;
  };
};

template <class payload>
class basicPacket{
	private:
	  coordinate source, destination;
	  int nHops, sizeP, id;
	  payload message;
	  sc_time timeE;

	protected:
	  void constructorCommon(payload p);

	public:	  
	  void setSource(uint8_t x, uint8_t y);
	  void incHops();
	  basicPacket();
	  basicPacket(payload p);
	  basicPacket(uint16_t address, payload p);
	  basicPacket(uint8_t x, uint8_t y, payload p);
	  void setDestination(uint8_t x, uint8_t y);

	  coordinate getDestination();
	  coordinate getSource();
	  uint8_t getHops();
	  uint8_t getSizePacket();
	  void setSizePacket(int sizePacket);
	  void setID(int id);
	  void setPath(uint16_t address);
	  void setTimeEntry(sc_time timeEntry);
	  payload getPayload();

queue<uint16_t> path;
	  friend ostream & operator << <>(ostream &,    const basicPacket<payload> &);
};

template <class payload> 
basicPacket<payload>::basicPacket(){
	destination.address=0;
	constructorCommon(0);
}

template <class payload> 
basicPacket<payload>::basicPacket(payload p){
	destination.address=0;
	constructorCommon(p);
}

template <class payload> 
basicPacket<payload>::basicPacket(uint16_t address, payload p){
	destination.address=address;
	constructorCommon(p);
}

template <class payload> 
basicPacket<payload>::basicPacket(uint8_t x, uint8_t y, payload p){
	destination.x=x;
	destination.y=y;
	constructorCommon(p);
}

template <class payload> 
void basicPacket<payload>::constructorCommon(payload p){
	source.x=source.y=0;
	nHops=0;
	message=p;
}

template <class payload> 
void basicPacket<payload>::setDestination(uint8_t x, uint8_t y){
	destination.x=x;
	destination.y=y;
}

template <class payload> 
coordinate basicPacket<payload>::getDestination(){
	return destination;
}

template <class payload> 
void basicPacket<payload>::setSizePacket(int sizePacket){
	sizeP=sizePacket;
}

template <class payload> 
void basicPacket<payload>::setID(int packetID){
	id = packetID;
	}


template <class payload> 
void basicPacket<payload>::setTimeEntry(sc_time timeEntry){
	timeE=timeEntry;
}



template <class payload> 
coordinate basicPacket<payload>::getSource(){
	return source;
}

template <class payload> 
payload basicPacket<payload>::getPayload(){
	return message;
}

template <class payload> 
void basicPacket<payload>::setSource(uint8_t x, uint8_t y){
	source.x=x;
	source.y=y;
}
template <class payload> 
void basicPacket<payload>::setPath(uint16_t address){
	path.push(address);
}

template <class payload> 
void basicPacket<payload>::incHops(){
	nHops++;
}

template <class payload> 
ostream & operator << (ostream &out, const basicPacket<payload>& obj){

	out << "ID = " <<(unsigned int)obj.id<<endl<<
		"Source(x,y)=["<<(unsigned int)obj.source.x<<","<<(unsigned int)obj.source.y<<"]"<<endl<<
			"Dest(x,y)=["<<(unsigned int)obj.destination.x<<","<<(unsigned int)obj.destination.y<<"]"<<endl<<
			"Dest(address)=["<<(unsigned int)obj.destination.address<<"]"<<endl<<
			"nHops="<<(unsigned int)obj.nHops<<endl<<
			"payload="<<obj.message<<endl<<
			"timeEntry="<<obj.timeE<<endl<<
			"actualTime=" <<sc_time_stamp()<<endl<<
			"sizePacket=" <<(unsigned int)obj.sizeP<<endl<<
			"PATH: "<< (unsigned int)obj.path.front() <<endl;
			/*while(!obj.path.empty()){
				out<<"PATH "<<obj.path.front() <<endl;
				obj.path.pop();
			}*/
		
	return out;
};

#endif
