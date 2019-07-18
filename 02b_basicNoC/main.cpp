#include <systemc>
#include "basicNoC.h"
#include "basicPacket.h"
#include "router.h"
#include "source_noc.h"
#include "sink_noc.h"

using namespace std;

int sc_main(int, char*[])
{


  basicNoC<basicPacket<int>,3,3> noc3x3("noc3x3");

  /*matriz para gerar estimulos*/
  source_noc<3,3> *s[3][3];


  sink_noc *sk[3][3];

  for(int x=0;x<3;x++)
    for(int y=0;y<3;y++){
		char name[100];
		/*armazena em name os enderecos x e y*/
		sprintf(name,"s_%dx%d",x,y);
		
		s[x][y]=new source_noc<3,3>(name,x,y);
		s[x][y]->port.bind(noc3x3.inport[x][y]);
	}

  for(int x=0;x<3;x++)
    for(int y=0;y<3;y++){
		char name[100];
		sprintf(name,"sk%dx%d",x,y);
		sk[x][y]=new sink_noc(name,x,y);
		sk[x][y]->port.bind(noc3x3.outport[x][y]);
	}

  sc_start(sc_time(150,SC_NS));
  
  return 0;
}
