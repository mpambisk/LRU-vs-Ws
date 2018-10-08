#include <iostream>
#include "Inv_ptable.h"

using namespace std;

Tdata::Tdata(long unsigned int to_frame,int to_pid,bool to_dirty){
	frame=to_frame;
	pid=to_pid;
	dirty=to_dirty;
	in_ws=0;
	//cout<<"constructor Tdata"<<endl;
}
int Tdata::check_key(long unsigned int cframe,int cpid){
	if((cframe==frame)&&(cpid==pid)){
		return 1;
	}
	return 0;
}

Inv_ptable::Inv_ptable(long unsigned int size){
	//cout<<"constructor Inv_ptable"<<endl;
	save_disk=0;
	tsize=size;
	page_replace=0;
	table=new Tdata*[size];
	full=0;
}

Inv_ptable::~Inv_ptable(){
	for(int i=0;i<full;i++){
		if(table[i]!=NULL){
			delete table[i];
		}
	}
	delete [] table;
	//cout<<"destructor Inv_ptable"<<endl;
}

long int Inv_ptable::search_first_notinws(){
	for(int i = 0; i<full;i++){
		if(table[i]!=NULL){
			if(table[i]->get_in_ws()==0){
				return i;
			}
		}
	}
	return -1;
}

long int Inv_ptable::search(long unsigned int frame, int pid){
	if(is_empty())
		return 0;
	for(int i=0;i<full;i++){
		if(table[i]!=NULL){
			if(table[i]->check_key(frame,pid)==1){
				return i;
			}
		}
	}
	return -1;
}

void Inv_ptable::change(long unsigned int pos,long unsigned int to_frame, int to_pid,bool to_dirty){
	if(table[pos]->get_dirty()==1){
		save_disk++;
		//save data to disk
	}
	page_replace++;													//replace page
	table[pos]->set_frame(to_frame);
	table[pos]->set_pid(to_pid);
	table[pos]->set_dirty(to_dirty);

}

int Inv_ptable::add_data_LRU(long unsigned int frame, int pid , bool dirty,long unsigned int & pf,LRU * algor){

	if(is_empty()){																								//case empty table
		table[0]= new Tdata(frame,pid,dirty);
		full++;
		pf++;
		algor->update(0);
		//page fault !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return 0;
	}
	long int pos=search(frame,pid);									//search for same data
	if(pos!=-1){																//found
		if(dirty==1){															//write case 
			table[pos]->set_dirty(1);
		}
		algor->update(pos);														//update
		return pos;
	}

	else {																		//not found
		if(is_full()){															//and table is full
			pf++;
			long unsigned int old=algor->search_old();							//find non recently used page
			change(old,frame,pid,dirty);
			algor->update(old);													//update
			return old;
			//page fault !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//page replace !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			
		}
		else{																	//not found and table isnt full
			table[full]=new Tdata(frame,pid,dirty);
			full++;
			pf++;
			algor->update(full-1);												//update
			//page fault !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return full-1;
		}
	}
}

int Inv_ptable::add_data_WS(long unsigned int frame, int pid, bool dirty, long unsigned int &pf, WS *algor ,WS * algor2,long unsigned int &count){

	if(is_empty()){																								//case empty table
		table[0]= new Tdata(frame,pid,dirty);
		full++;
		pf++;
		long unsigned int ws_pos=algor->add_data(table[0]);
		table[0]->set_in_ws(1);
		algor->update(ws_pos,count);
		//page fault !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return 0;
	}
	else{																								//not empty
		long int ws_pos=algor->search(frame);
		if(ws_pos!=-1){																						//found in ws
			Tdata ** temp=algor->get_data();																				
			if(dirty==1){
				algor->get_data();
				temp[ws_pos]->set_dirty(1);
			}
			algor->update(ws_pos,count);
			return 1;		
		}
		else{																							//not found in ws
			long int pos= search(frame,pid);
			if(pos>=0){																					//found in ipt but not in ws
				if(algor->is_full()){																	//ws is full
					long unsigned int pos2=algor->search_old();
					algor->replace(pos2,table[pos]);
					table[pos]->set_in_ws(1);																//in ws
					algor->update(pos2,count);																
					return 1;
				}
				else{																					//ws not full
					long unsigned int wspos=algor->add_data(table[pos]);
					table[pos]->set_in_ws(1);
					algor->update(wspos,count);
				}
			}
			else{																						//not in ws not in ipt table is full
				if(is_full()){										
					long int tmp=search_first_notinws();
					if(algor->is_full()){																//ws is full
						if(tmp==-1){																	//special: all ipt data in ws
							long unsigned int pos3=algor->search_old();
							long unsigned int pos4=algor2->search_old();
							if(pos3>=pos4){																//in current ws
								Tdata ** tdata = algor->get_data();
								tdata[pos3]->set_frame(frame);
								tdata[pos3]->set_dirty(dirty);
								pf++;																	//page fault
								algor->update(pos3,count);
							}
							else{																		// na grafei se auto to ws na svistei apo to allo
								Tdata ** tdata2 = algor2->get_data();
								tdata2[pos4]->set_frame(frame);
								tdata2[pos4]->set_dirty(dirty);
								tdata2[pos4]->set_pid(pid);
								Tdata * to_ws1 =tdata2[pos4];
								tdata2[pos4]=NULL;
								long int * occ = algor2->get_occur();
								occ[pos4]=-1;
								algor2->de_full();														//decrement
								algor->replace(pos3,to_ws1);
								algor->update(pos3,count);
								pf++;																	//page fault
							}
						}
						else{																			//found
							table[tmp]->set_frame(frame);
							table[tmp]->set_dirty(dirty);
							pf++;																		//page fault
							long unsigned int pos3=algor->search_old();
							algor->replace(pos3,table[tmp]);
							algor->update(pos3,count);
						}
					}
					else{																				//ws not full
						pf++;																			//in both cases page fault
						if(tmp==-1){																	//special all ipt data in ws
							long unsigned int pos3=algor->search_old();
							long unsigned int pos4=algor2->search_old();
							if(pos3>=pos4){																//in current ws
								Tdata ** tdata = algor->get_data();
								tdata[pos3]->set_frame(frame);
								tdata[pos3]->set_dirty(dirty);
								tdata[pos3]->set_in_ws(1);
								algor->update(pos3,count);								
							}
							else{
								Tdata ** tdata2 = algor2->get_data();
								tdata2[pos4]->set_frame(frame);
								tdata2[pos4]->set_dirty(dirty);
								tdata2[pos4]->set_pid(pid);
								Tdata * to_ws1 =tdata2[pos4];
								tdata2[pos4]=NULL;
								long int * occ = algor2->get_occur();
								occ[pos4]=-1;
								algor2->de_full();														//decrement
								long unsigned int pos5=algor->add_data(to_ws1);
								to_ws1->set_in_ws(1);
								algor->update(pos5,count);								
							}					

						}
						else{
							table[tmp]->set_frame(frame);
							table[tmp]->set_dirty(dirty);
							table[tmp]->set_in_ws(1);
							long unsigned int pos5=algor->add_data(table[tmp]);
							algor->update(pos5,count);
						}
					}
				}
				else{
					pf++; 					                           //page fault
					//case not in ws not in ipt and ipt not full
					if(algor->is_full()){															
						//case not in ws not in ipt ipt not full ws full
						table[full]= new Tdata(frame,pid,dirty);
						table[full]->set_in_ws(1);
						full++;
						long unsigned int pos3=algor->search_old();
						algor->replace(pos3,table[full-1]);
						algor->update(pos3,count);

					}
					else{
						//case not in ws not in ipt ipt not full ws not full
						if(algor->is_full()){															
							//case not in ws not in ipt ipt not full ws full
							table[full]= new Tdata(frame,pid,dirty);
							table[full]->set_in_ws(1);
							full++;
							long unsigned int pos3= algor->add_data(table[full-1]);
							algor->update(pos3,count);
						}
					}
				}


			}

		}						

	}
}