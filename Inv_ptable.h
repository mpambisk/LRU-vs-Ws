
#ifndef INV_PTABLE_H
	#define INV_PTABLE_H

#include "LRU.h"
#include "ws.h"

class Tdata{
	long unsigned int frame;
	int pid;
	bool dirty;
	//offset maybe not used
	bool in_ws;													//used to search fast if in a ws
public:
	Tdata(long unsigned int,int,bool);
	~Tdata(){};

	long unsigned int get_frame(){return frame;}
	int get_pid(){return pid;}
	bool get_dirty(){return dirty;}
	bool get_in_ws(){return in_ws;}

	void set_frame(long unsigned int to_frame){frame=to_frame;}
	void set_pid(int to_pid){pid=to_pid;}
	void set_dirty(bool to_dirty){dirty=to_dirty;}
	void set_in_ws(bool to_in_ws){in_ws=to_in_ws;}

	int check_key(long unsigned int , int)	;								//returns 1 if tables[i] data are the same with arguments

};


class WS;
class Inv_ptable{
	long unsigned int tsize;
	long unsigned int full;
	long unsigned int page_replace;
	long unsigned int save_disk;

	Tdata ** table;
public:
	Inv_ptable(long unsigned int);
	~Inv_ptable();

	long unsigned int get_save_disk(){return save_disk;}
	long unsigned int get_tsize(){return tsize;}
	long unsigned int get_page_replace(){return page_replace;}
	Tdata ** get_table(){return table;}

	void set_tsize(long unsigned int to_tsize){tsize=to_tsize;}
	void set_table(Tdata ** to_table){table=to_table;}
	void set_save_disk(long unsigned int to_save){save_disk=to_save;}

	int is_empty(){return full==0;}											//returns 1 if inverted page table is completely empty
	int is_full(){return full>=tsize;}										//returns 1 if inverted page table is full
	long int search_first_notinws();
	long int search(long unsigned int , int);									//returns position in table or -1 if not found
	void change(long unsigned int, long unsigned int , int , bool);
	int add_data_LRU(long unsigned int , int , bool,long unsigned int &,LRU * );
	int add_data_WS(long unsigned int,int, bool, long unsigned int &, WS * ,WS *,long unsigned int &);

};

#endif