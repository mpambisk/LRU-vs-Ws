
class LRU{
	long unsigned int * positions;
	long unsigned int size;
	long unsigned int counter;
public:
	LRU(long unsigned int);
	~LRU();

	long unsigned int * get_positions(){return positions;}
	long unsigned int get_size(){return size;}
	long unsigned int get_counter(){return counter;}

	void set_positions(long unsigned int * to_positions){positions=to_positions;}
	void set_size(long unsigned int to_size){size=to_size;}
	void set_counter(long unsigned int to_counter){counter=to_counter;}
	
	void update(long unsigned int);														//informs lrus table about changesn
	long unsigned int search_old();
};