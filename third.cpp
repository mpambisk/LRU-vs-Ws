#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "Inv_ptable.h"

using namespace std;

long unsigned int hextodec(char * );

int main(int argc, char * argv[]){

//----------ARGUMENTS---------------------
	//cout<<"argc "<<argc<<endl;
	if((argc<4)||(argc>6)){
		cout<<"arg error"<<endl;
		return 1;
	}
	int algor =atoi(argv[1]);
	long unsigned int frames = atoi(argv[2]);
	int q = atoi(argv[3]);
	int max=0;
	int ws_size=0;
	if(argc >= 5){
		ws_size = atoi( argv[4]);
		if(argc == 6){
			max = atoi(argv[5]);
		}
	}
	//cout<<"First arg "<<algor<<" second arg "<<frames
	//<<" third arg "<<q<<" forth arg "<<ws_size<<" last arg "<<max<<endl;


//------------FILE----------------------------
	FILE * f1;
	FILE * f2;
	f1=fopen("bzip.trace","r");
	f2=fopen("gcc.trace","r");
	int i;
	int flagf1=1;
	int flagf2=1;
	long unsigned int f1counter=0;
	long unsigned int f2counter=0;
	long unsigned int rcounter=0;
	long unsigned int wcounter=0;
	long unsigned int num=0;
	char buffer[30];
	char tempbuf[8];
	bool dirty;
	long unsigned int page_faults=0;
	long unsigned int wscounter=0;
//------------Page table----------------------------
	Inv_ptable ipt(frames);
//--------------------------------------------------
	LRU * algori;
	WS * ws1;
	WS * ws2;
	if(algor==0){
		cout<<"USING LRU ALGORITHM"<<endl;
		algori=new LRU(frames);
	}
	else if(algor==1){
		cout<<"USING WS ALGORITHM"<<endl;
		ws1 = new WS(ws_size,0);
		ws2 = new WS(ws_size,0);
	}
	else{
		cout<<"error use 0 for lru , 1 for ws "<<endl;
		return 1;
	}
//--------------------------------------------------

	while(1){
		i=0;
		while((i<q) && (flagf1==1)){
			if((f1counter>=max) && (max>0)){
				flagf1=0;
				break;
			}
			if(fgets(buffer,29,f1)!=NULL){
				//cout<<"buffer f1 "<<buffer;
				if(strstr(buffer,"R")){
					dirty=0;
					rcounter++;
				}
				else if(strstr(buffer,"W")){
					wcounter++;
					dirty=1;
				}
				else{
					//cout<<"wrong line in f1"<<endl;
					continue;
				}
				strncpy(tempbuf,buffer,8);
				num=hextodec(tempbuf);
				if(algor==0)
					ipt.add_data_LRU(num/4096,0,dirty,page_faults,algori);
				else
					ipt.add_data_WS(num/4096,0,dirty,page_faults,ws1,ws2,wscounter);
				f1counter++;
			}
			else{
				cout<<"end of file f1"<<endl;
				flagf1=0;
			}
			i++;
			
		}
		i=0;
		while((i<q) && (flagf2==1)){
			if((f2counter>=max) && (max>0)){
				flagf2=0;
				break;
			}
			if(fgets(buffer,29,f2)!=NULL){
				//cout<<"buffer f2 "<<buffer;
				if(strstr(buffer,"R")){
					rcounter++;
					dirty=0;
				}
				else if(strstr(buffer,"W")){
					wcounter++;
					dirty=1;
				}
				else{
					//cout<<"wrong line in f1"<<endl;
					continue;
				}
				strncpy(tempbuf,buffer,8);
				num=hextodec(tempbuf);
				if(algor==0)
					ipt.add_data_LRU(num/4096,1,dirty,page_faults,algori);
				else
					ipt.add_data_WS(num/4096,1,dirty,page_faults,ws1,ws2,wscounter);
				f2counter++;
			}
			else{
				cout<<"end of file f2"<<endl;
				flagf2=0;
			}
			i++;
		}
		if ((flagf1==0)&&(flagf2==0))
			break;
	}
	cout<<"legal lines from f1 : "<<f1counter<<endl;
	cout<<"legal lines from f2 : "<<f2counter<<endl;
	cout<<"total legal lines : "<<f1counter+f2counter<<endl;
	cout<<"number of reads : "<<rcounter<<endl;
	cout<<"number of writes : "<<wcounter<<endl;
	cout<<"number of page faults : "<<page_faults<<endl;
	cout<<"percentage of page faults : "<<((double)page_faults/(double)(f1counter+f2counter))*100<<"%"<<endl;
	cout<<"number of page replaces : "<<ipt.get_page_replace()<<endl;
	cout<<"number of reads from disk same with page faults "<<endl;
	cout<<"number of disk writes : "<<ipt.get_save_disk()<<endl;

//---------------------------------------------------------------------
	if(algor==0){
		delete algori;
	}
	else if(algor==1){
		delete ws1;
		delete ws2;
	}
	
	fclose(f1);
	fclose(f2);
}



long unsigned int hextodec(char * buffer){
	long unsigned int mult=1;
	long unsigned int temp;
	long unsigned int to_return=0;
	for(int i=7;i>=0;i--){
		if(buffer[i]=='a')
			temp=10;
		else if(buffer[i]=='b')
			temp=11;
		else if(buffer[i]=='c')
			temp=12;
		else if(buffer[i]=='d')
			temp=13;
		else if(buffer[i]=='e')
			temp=14;
		else if(buffer[i]=='f')
			temp=15;
		else
			temp=(int)(buffer[i])-int('0');
		temp=temp*mult;
		mult=mult*16;
		to_return=to_return+temp;
	}
	return to_return;
}