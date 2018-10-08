#include <iostream>
#include "LRU.h"

using namespace std;

LRU::LRU(long unsigned int to_size ){
	size=to_size;
	counter=0;
	positions=new long unsigned int[size];
	for(int i=0;i<size;i++){							//initialisation
		positions[i]=0;
	}

}

LRU::~LRU(){
	delete [] positions;
}

void LRU::update(long unsigned int pos){
	counter++;
	positions[pos]=counter;
}

long unsigned int LRU::search_old(){
	long unsigned int pos=0;
	long unsigned int min=positions[0];
	for(int i=1;i<size;i++){
		if(positions[i]<min){
			min=positions[i];
			pos=i;
		}
	}
	return pos;
}