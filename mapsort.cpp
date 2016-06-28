#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <math.h>
#include <mutex>
#include <ctime>
#include <algorithm>

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
	long ** q_matrix;
	thread * workers;
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
		while (c_i != n_key){
			t_r = rand() % tam;
			if (!find_k(array[t_r])){
				keys[c_i] = array[t_r];
				c_i++;
			}
		}
		cout<<"Llaves creadas correctamente"<<endl;
		sort(keys, keys + n_key);
		workers = new thread [n_key];
		c_matrix = init_m(intervals, block,0);
		q_matrix = init_m(intervals, block,-1);
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
				cout<<a[i][j]<<" ";
			cout<<endl;
		}
		cout<<endl;
	}
	
	void print_cq (){
		print_m(c_matrix, intervals, block);
		print_m(q_matrix, intervals, block);
	}
	
	void print(){
		for (int i=0;i<tam;i++){
			cout<<array[i]<<", ";
		}
		cout<<endl;
	}
	
	void print_k (){
		for (int i=0;i<n_key;i++){
			cout<<keys[i]<<", ";
		}
		cout<<endl;
	}
	
	void t_sort (int min, int max, int i, int & count){
		int count_t = 0;
		if (min == -1){
			for(int it=0;it<tam;it++){
				if (array[it]<max) {
					c_matrix[i][int(floor(it/t_q))] +=1;
					count_t++;
				}
			}
		}else if (max == -1){
			for(int it=0;it<tam;it++){
				if (min<=array[it]) {
					c_matrix[i][int(floor(it/t_q))] +=1;
					count_t++;
				}
			}
		}else{
			for(int it=0;it<tam;it++){
				if (min<=array[it] && array[it]<max) {
					c_matrix[i][int(floor(it/t_q))] +=1;
					count_t++;
				}
			}
		}
		count=count_t;
	}
	
	void m_sort (){
		int t_count [intervals];
		t_sort(-1,keys[0],0,t_count[0]);
		for (int i=1;i<n_key;i++){
			t_sort(keys[i-1],keys[i],i,t_count[i]);
		}
		t_sort(keys[n_key-1],-1,n_key,t_count[n_key]);
		int q_count = 0;
		for(int i=0;i<intervals;i++){
			for(int j=0;j<block;j++){
				if (c_matrix[i][j]!=0)
					q_matrix[i][j] = q_count;
				q_count += c_matrix[i][j];
			}
			cout<<"T: "<<t_count[i]<<endl;
		}
	}
	
};

void t_insert (long long d, map m){
	m.insert(d);
}

int main (){
	srand(std::time(0));
	c_time t;
	long tam = 16;
	int  max = 100;
	long * array = new long [tam];
	//int  n_thread = 10;//thread::hardware_concurrency();
	int n_keys=7;
	int block =4;
	map m(n_keys);
	for (int i = 0;i<tam;i++)
		array[i] = rand()%max;

	map_sort map (array, tam, n_keys, block);
	map.m_sort();
	map.print();
	map.print_k();
	map.print_cq();

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
