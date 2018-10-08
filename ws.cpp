#include <iostream>
#include "ws.h"

using namespace std;

WS::WS(long unsigned int to_size,int to_pid){
	full=0;
	pid=to_pid;
	size=to_size;
	data= new Tdata*[size];
	occur=new long int[size];								//latest occurance
	for(int i=0;i<size;i++){								//initialisation
		occur[i]=-1;
		data[i]=NULL;
	}
}


WS::~WS(){
	delete [] occur;
	delete [] data;
}

void WS::replace(long unsigned int pos,Tdata * newd){
	data[pos]->set_in_ws(0);								//out of ws
	data[pos]=newd;
}

long  int WS::search(long unsigned int to_frame){
	for(int i=0;i<full;i++){
		if(data[i]!=NULL){
			if(data[i]->get_frame()==to_frame){
				return i;
			}
		}
	}
	return -1;
}

long unsigned int WS::search_old(){
	long unsigned int min = occur[0];
	long unsigned int pos=0;
	for(int i=1;i<full;i++){
		if(occur[i]<min){
			min=occur[i];
			pos=i;
		}
	}
	return pos;
}

long unsigned int WS::add_data(Tdata * to_add){
	if(full<size){
		data[full]=to_add;
		full++;
		return full-1;
	}
	else{
		long unsigned int to_rep=search_old();																//check!!!!!!!!!!!!!!
	}
}


void WS::update(long unsigned int pos, long unsigned int & count ){
	count++;
	occur[pos]=count;
}

