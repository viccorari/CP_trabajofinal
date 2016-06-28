#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <math.h>
#include <mutex>
#include <ctime>
#include <algorithm>
#include <vector>

using namespace std;

class c_time{
	public:
	clock_t start;
	clock_t end;
	c_time(){};
	void init (){start = clock();}
	void stop (){end = clock();}
	void print(){cout<<"Tiempo: "<<(end-start)/(double) CLOCKS_PER_SEC<<" seg."<<endl;}
};

class node{
	public:
	long long dato;
	node * sig;
	node (long long i){
		dato = i;
		sig = NULL;
	}
};

class lista{
	public:
	node * raiz;
	mutex mu;
	lista(){
		raiz = NULL;
	}
	void insert(long long d){
		mu.lock();
		node ** temp = &raiz;
		while(*temp){
			temp = &((*temp)->sig);
		}
		*temp = new node(d);
		mu.unlock();
	}
	void print (){
		node * temp = raiz;
		while (temp){
			cout<<temp->dato<<", ";
			temp = temp->sig;
		}
		return;
	}
	long count (){
		long c = 0;
		node * temp = raiz;
		while(temp){
			temp = temp->sig;
			c++;
		}
		return c;
	}
};

class map{
	public:

	int n_key;
	lista ** keys;
	
	map (int _n_key){
		n_key = _n_key;
		keys = new lista * [n_key];
		for(int i = 0;i<n_key;i++){
			keys[i] = new lista();
		}
	}
	int f_hash (long dato){
		return abs(dato) % n_key;
	}
	void insert(long dato){
		int key = f_hash(dato);
		keys[key]->insert(dato);
	}
	void print (){
		for(int i = 0;i<n_key;i++){
			keys[i]->print();
			cout<<endl;
		}
	}
	void print_keys (){
		for (int i=0;i<n_key;i++){
			if (keys[i]->raiz) cout<<(keys[i]->raiz)->dato<<", ";
			else cout<<"NULL, ";
		}
		cout<<endl;
	}
	long * get_keys (){
		long * temp = new long [n_key];
		for (int i=0;i<n_key;i++){
			if (keys[i]->raiz) temp[i] = (keys[i]->raiz)->dato;
			else temp[i] = -1;
		}
		return temp;
	}
	long count (){
		long c = 0;
		for(int i = 0;i<n_key;i++){
			c += keys[i]->count();
		}
		return c;
	}
	int get_n_keys (){
		return n_key;
	}
};

class map_sort{
	public:
	long * array;
	long * o_array;
	long * keys;
	int tam;
	int n_key;
	int intervals;
	int block;
	int t_q;
	long ** c_matrix;
	//long ** q_matrix;
	thread * workers;
	int * t_count;
	int * a_t_count;

	map_sort (long * _array, int _tam, int _n_key, int _block){
		array = _array;
		tam = _tam;
		n_key = _n_key;
		t_q = _block;
		block = tam/t_q;
		intervals = n_key+1;
		o_array = new long [tam];
		keys = new long [n_key];
		int c_i = 0;
		int t_r = 0;
		t_count = new int [intervals];
		a_t_count = new int [intervals];
		for(int i=0;i<intervals;i++){t_count[i] = 0;}
		while (c_i != n_key){
			t_r = rand() % tam;
			if (!find_k(array[t_r])){
				keys[c_i] = array[t_r];
				c_i++;
			}
		}
		cout<<"Llaves creadas correctamente"<<endl;
		sort(keys, keys + n_key);
		workers = new thread [intervals];
		c_matrix = init_m(intervals, block,0);
		//q_matrix = init_m(intervals, block,-1);
	}
	
	bool find_k (long d){
		for(int i=0;i<n_key;i++){
			if (keys[i] == d) return true;
		}
		return false;
	}
	
	long ** init_m (int n_i, int n_j, int def){
		long ** temp = new long * [n_i];
		for (int i=0;i<n_i;i++){
			temp [i] = new long [n_j];
			for(int j=0;j<n_j;j++)
				temp[i][j] = def;
		}
		return temp;
	}
	
	void print_m (long ** a, int n_i, int n_j){
		for (int i=0;i<n_i;i++){
			for(int j=0;j<n_j;j++)
				cout<<a[i][j]<<"\t";
			cout<<endl;
		}
		cout<<endl;
	}
	
	void print_cq (){
		print_m(c_matrix, intervals, block);
		//print_m(q_matrix, intervals, block);
	}
	
	void print(){
		for (int i=0;i<tam;i++){
			cout<<array[i]<<", ";
		}
		cout<<endl;
	}

	void print_o(){
		for (int i=0;i<tam;i++){
			cout<<o_array[i]<<", ";
		}
		cout<<endl;
	}
	
	void print_k (){
		for (int i=0;i<n_key;i++){
			cout<<keys[i]<<", ";
		}
		cout<<endl;
	}
	
	void t_sort (int min, int max, int i){
		if (min == -1){
			for(int it=0;it<tam;it++){
				if (array[it]<max) {
					c_matrix[i][int(floor(it/t_q))] +=1;
					t_count[i]++;
				}
			}
		}else if (max == -1){
			for(int it=0;it<tam;it++){
				if (min<=array[it]) {
					c_matrix[i][int(floor(it/t_q))] +=1;
					t_count[i]++;
				}
			}
		}else{
			for(int it=0;it<tam;it++){
				if (min<=array[it] && array[it]<max) {
					c_matrix[i][int(floor(it/t_q))] +=1;
					t_count[i]++;
				}
			}
		}
	}
	
	void t_quick(int min, int max, int i){
		for(int b=0; b<block ;b++){
			if(c_matrix[i][b] != -1){
				vector<long> temp;
				for(int k=0;k<t_q;k++){
					printf("C: %i, %d\n",b, (b*t_q)+k);
					if (min == -1){
						if (array[(b*t_q)+k]<max)
							//printf("Oooooooooooooooooooooooooooooooook menor\n");
							temp.push_back(array[(b*t_q)+k]);
					}else if (max == -1){
						if (min<=array[(b*t_q)+k])
							//printf("Oooooooooooooooooooooooooooooooook mayor\n");
							temp.push_back(array[(b*t_q)+k]);
					}else{
						if (min<=array[(b*t_q)+k] && array[(b*t_q)+k]<max)
							//printf("Oooooooooooooooooooooooooooooooook\n");
							temp.push_back(array[(b*t_q)+k]);
					}
				}
				//printf("Aqui %ld\n", c_matrix[i][b]);
				long pos = c_matrix[i][b];
				for (int v=0;v<temp.size();v++){
					//printf("Soy: %i, Vector pos:%d, dato: %ld\n",i, v, temp[v]);
					
					o_array[pos+v] = temp[v];
				}
				temp.clear();
			}
		}

		if(i==intervals-1){
			sort (array+a_t_count[i],array+tam-1);
		}else{
			sort (array+a_t_count[i],array+a_t_count[i+1]);
		}
		

	}

	
	void m_sort (){
		int temp;
		workers[0] = thread(&map_sort::t_sort, this, -1,keys[0],0);
		workers[0].join();
		for (int i=1;i<n_key;i++)
			workers[i] = thread(&map_sort::t_sort, this, keys[i-1],keys[i],i);
		for (int i=1;i<n_key;i++)
			workers[i].join();
		workers[intervals-1] = thread(&map_sort::t_sort, this, keys[n_key-1],-1,n_key);
		workers[intervals-1].join();
		int q_count = 0;
		int c_t = 0;
		for(int i=0;i<intervals;i++){
			for(int j=0;j<block;j++){
				temp = c_matrix[i][j];
				if (c_matrix[i][j]!=0)
					c_matrix[i][j] = q_count;
				else c_matrix[i][j] = -1;
				q_count += temp;
			}
			c_t += t_count[i];
		}
		a_t_count [0] = 0;
		cout<<"Acumulado"<<endl;
		for(int i=1;i<intervals;i++)
			a_t_count[i] = a_t_count[i-1]+t_count[i-1];
		for(int i=0;i<intervals;i++)
			cout<<a_t_count[i]<<", ";
		cout<<endl;
		cout<<"Suma: "<<c_t<<endl;
		
		cout<<"t_q: "<<t_q<<endl;
		cout<<"block: "<<block<<endl;
		
		t_quick(-1,keys[0],0);
		for (int i=1;i<n_key;i++)
			t_quick(keys[i-1],keys[i],i);
		t_quick(keys[n_key-1],-1,n_key);
		/*
		workers[0] = thread(&map_sort::t_quick, this, -1,keys[0],0);
		workers[0].join();
		for (int i=1;i<n_key;i++)
			workers[i] = thread(&map_sort::t_quick, this, keys[i-1],keys[i],i);
		for (int i=1;i<n_key;i++)
			workers[i].join();
		workers[intervals-1] = thread(&map_sort::t_quick, this, keys[n_key-1],-1,n_key);
		workers[intervals-1].join();
		*/
	}
	
};

void t_insert (long long d, map m){
	m.insert(d);
}

int main (){
	srand(std::time(0));
	c_time t;
	long tam = 12;
	int  max = 20;
	long * array = new long [tam];
	//long array [] = {10,6,14,1,13,9,5,11,3,7,16,2,12,8,15,4};
	//int  n_thread = 10;//thread::hardware_concurrency();
	int n_keys=7;
	int block =4	;
	map m(n_keys);
	for (int i = 0;i<tam;i++)
		array[i] = rand()%max;

	map_sort map (array, tam, n_keys, block);
	map.m_sort();
	map.print();
	map.print_k();
	map.print_cq();
	map.print_o();
/*
	t.init();//Init Time
	thread workers [n_thread];
	for (int i = 0;i<tam/n_thread ;i++){
		for (int j = 0; j<n_thread;j++)
			workers[j] = thread(t_insert, ref(array[j*i]),ref(m));
		for (int j = 0; j<n_thread;j++)
			workers[j].join();
	}
	t.stop();//Stop Time
	//m.print();
	long * keys = m.get_keys();
	for(int i = 0;i<m.get_n_keys();i++){
		cout<<keys[i]<<", ";
	}
	cout<<endl;
	//m.print_keys();
	t.print();
	cout<<"Total: "<<m.count()<<endl;
*/
}
