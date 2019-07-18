#ifndef _source_noc
#define _source_noc
#include <iostream>
#include <inttypes.h>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

#include "systemc.h"
#include "basicPacket.h"

static int id=0;

template<int maxx, int maxy>
SC_MODULE(source_noc){

	sc_fifo_out<basicPacket<int> > port;
	coordinate myAddress;
	coordinate addressDest;


	void Stimuli(){
		basicPacket<int> *aux;
		basicPacket<string> *aux2;	
		int sizePacket;	
		uint16_t dest;
		sc_time timeEntry;
		int timeEnt;
		uint16_t addressSource = myAddress.address;
		string url = "in/";
		std::stringstream url2;
		url2 << addressSource;
		string urlFinal = url + url2.str() + ".txt";
		cout<<urlFinal<<endl;
	  string line;
	  	int i=0;

	istringstream is(line);
	string part[4];


		ifstream myfile (urlFinal.c_str());
		if (myfile.is_open()){
		while (! myfile.eof() ){
			while (getline(myfile, part[i], ' ')) i++;
			std::istringstream iss (part[0]);
			iss >> timeEnt;
	
			std::istringstream iss2 (part[1]);
			iss2 >> dest;

			std::istringstream iss3 (part[2]);
			iss3 >> sizePacket;
			//printf("tamanho do pacote eh %d\n",sizePacket);

			int lengthStr=(part[3].length()-1);
			
			//printf("%d\n",timeEnt);
			if((lengthStr!=sizePacket) && (part[0].length()!=0)){
   				 perror ("The packet size not match");		
   			}
			
			addressDest.address = dest;

   			aux2=new basicPacket<string>(part[3]);
			aux2->setDestination(addressDest.x,addressDest.y);
			cout<<dest<<endl;
			printf("DESTINATION X ");
			cout <<addressDest.x<<endl;
			printf("DESTINATION Y ");
			cout <<addressDest.y<<endl;
			aux2->setID(id);


			int imagDur; //< Assuming you are getting a value from somewhere.

			sc_time sc_timeEnt = sc_time(timeEnt,SC_NS);
			if(sc_time_stamp()>sc_timeEnt){
				//cout<<sc_time_stamp()<<endl;
				//cout<<sc_timeEnt<<endl;
				perror("Entry time of the packet must be higher");
			}else{
				printf("caiu no else\n");
				//sc_time waitTime = timeEnt - sc_time_stamp();
				//wait(timeEnt-sc_time_stamp(),SC_NS);
				while(sc_time_stamp()<sc_timeEnt){
					wait(1,SC_NS);
				//	printf("caiu no while\n");*/
				//sc_time t1(sc_time_stamp(),SC_NS);
				//}
				}




			}
			/*				aux->setID(id);
				timeEntry=sc_time_stamp();//.toString();
				aux->setTimeEntry(timeEntry);
			}while(aux->getDestination().address==myAddress.address);
			cout << "src("<< this->name() <<"): injecting packet at " << sc_time_stamp() << " pkt: " << aux << endl;

			cout << "sizePacket1=" << sizePacket << endl; 

//			printf("size packet = %d\n", sizePacket );
			port.write(*aux);*/
		//}
		//id++;
		//wait();	
    	}
    	myfile.close();
  		}
		/*for(i=0;i<4;i++){
			cout<<part[i]<<endl;
		}*/


		

		for(int i=0; i<1; i++){
			wait(rand()%30,SC_NS);

			sizePacket = rand()%256;
  			
			int message[sizePacket];

			for(int j=0;j<sizePacket;j++){
				message[j] = rand()%100; 
			}
			aux=new basicPacket<int>(*message);
			do{
				aux->setDestination(rand()%maxx, rand()%maxy);

				aux->setSizePacket(sizePacket);
				aux->setID(id);
				timeEntry=sc_time_stamp();//.toString();
				aux->setTimeEntry(timeEntry);
			}while(aux->getDestination().address==myAddress.address);
			cout << "src("<< this->name() <<"): injecting packet at " << sc_time_stamp() << " pkt: " << aux << endl;

			cout << "sizePacket1=" << sizePacket << endl; 

//			printf("size packet = %d\n", sizePacket );
			port.write(*aux);
		}
		id++;
		wait();
	}
	
	//SC_CTOR(source_noc){
	source_noc(sc_module_name _name, uint8_t _x, uint8_t _y): sc_module(_name){
		myAddress.x=_x;
		cout<<hex<<unsigned(myAddress.x)<<endl;
		myAddress.y=_y;
		cout<<hex<<unsigned(myAddress.y)<<endl;
		SC_THREAD(Stimuli);
		cout << hex<< myAddress.address << endl;
	}
	SC_HAS_PROCESS(source_noc);
};

#endif
