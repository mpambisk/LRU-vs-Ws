
#ifndef WS_H
	#define WS_H

#include "Inv_ptable.h"

class Tdata;

class WS{
	Tdata ** data;																	//size pointer to Tdata
	long int * occur;																//used to determine oldest occurance
	long unsigned int size;															//size= k

	long unsigned int full;															//to check if ws is full	
	int pid;																		//to determine which ws
public:
	WS(long unsigned int,int);
	~WS();

	Tdata ** get_data(){return data;}
	long unsigned int get_size(){return size;}
	long unsigned int get_full(){return full;}
	long int * get_occur(){return occur;}
	int get_pid(){return pid;}

	void set_size(long unsigned int to_size){size=to_size;}
	void set_full(long unsigned int to_full){full=to_full;}
	void set_pid(int to_pid){pid=to_pid;}

	void de_full(){full--;}

	//*************************************************************************************************
	int is_full(){return full>=size;}												//returns 1 if ws is full
	void replace(long unsigned int , Tdata *);
	long unsigned int search_old();
	long int search(long unsigned int to_frame);									//returns pos in ws table or -1 if not found
	long unsigned int add_data(Tdata *);
	void update(long unsigned int,long unsigned int & );							//informs ws table about changes uses count to determine the oldest 
																					//page from 2 different ws

};

#endif