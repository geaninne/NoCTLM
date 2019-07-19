#ifndef _source_noc
#define _source_noc
#include <iostream>
#include <inttypes.h>
#include <fstream>
#include<sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

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
		
					int lengthStr=(part[3].length()-1);
			
					if((lengthStr!=sizePacket) && (part[0].length()!=0)){
   						 perror ("The packet size not match");		
   					}	
			
					char destxy[3];
					sprintf(destxy,"%d",dest);
		
					int testX = (int)destxy[0] - 48;
					int testY = (int)destxy[2] - 48;
					addressDest.x = testX;
					addressDest.y = testY;
			
					/*cria novo pacote passando o payload */
   					aux2 = new basicPacket<string>(part[3]);

   					aux2->setDestination(addressDest.x,addressDest.y);
   					aux2->setSizePacket(sizePacket);
					//aux2->setTimeEntry(timeEnt);
   					cout<< "destino = " << aux2->getDestination().address <<endl;

					aux2->setID(id);

					/*convertendo timeEntry para sc_time*/
					sc_time sc_timeEnt = sc_time(timeEnt,SC_NS);
					
					sc_time time_now =getTimeNow();// sc_time(sc_time_stamp(),SC_NS);
					if(time_now>=sc_timeEnt){
						perror("Entry time of the packet must be higher");
					}else{
						cout << sc_time_stamp() << " menor que " << sc_timeEnt <<endl;
						while(getTimeNow()<sc_timeEnt){
							wait(1,SC_NS);
						}
						cout <<"timeStamp = " << sc_time_stamp()<<endl; 
					}

				//}
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
				//aux->setDestination(rand()%maxx, rand()%maxy);

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
		/*endereco x*/
		myAddress.x=_x;
		cout<< "x = " << hex <<unsigned(myAddress.x)<<endl;
		
		/*endereco y*/
		myAddress.y=_y;
		cout<<"y = " << hex<<unsigned(myAddress.y)<<endl;
		
		SC_THREAD(Stimuli);
		
		/*endereco id*/
		cout << "id = " << hex<< myAddress.address << endl;
	}
	SC_HAS_PROCESS(source_noc);
};

#endif
