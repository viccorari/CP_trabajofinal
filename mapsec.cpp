#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <math.h>
#include <mutex>
#include <ctime>
#include <algorithm>
#include <vector>
#include <climits>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>


using namespace std;

typedef long* lp;

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
	lp dato;
	node * sig;
	node (lp i){
		dato = i;
		sig = NULL;
	}
	~node(){
		delete sig;
	}
};

class lista{
	public:
	node * raiz;
	mutex mu;
	lista(){
		raiz = NULL;
	}
	~lista(){
		delete raiz;
	}
	void insert(lp d){
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
			cout<<"|"<<*(temp->dato);
			if(temp->sig) cout<<"->";
			temp = temp->sig;
		}
		return;
	}
	long size (){
		long c = 0;
		node * temp = raiz;
		while(temp){
			temp = temp->sig;
			c++;
		}
		return c;
	}
};

/*
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
	long size (){
		long c = 0;
		for(int i = 0;i<n_key;i++){
			c += keys[i]->size();
		}
		return c;
	}
	int get_n_keys (){
		return n_key;
	}
};
*/

typedef pair<long,lista> llis;

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

	llis ** init_m_p (int n_i, int n_j, int def){
		llis ** temp = new llis * [n_i];
		for (int i=0;i<n_i;i++){
			temp [i] = new llis [n_j];
			for(int j=0;j<n_j;j++)
				temp[i][j].first = def;
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
	
	void print_m_p (pair<long,lista> ** a, int n_i, int n_j){
		for (int i=0;i<n_i;i++){
			for(int j=0;j<n_j;j++){
				cout<<a[i][j].first<<", ";
				a[i][j].second.print();
			}
			cout<<endl;
		}
		cout<<endl;
	}
	
	void print_cq (){
		print_m(c_matrix, intervals, block);
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
	
	void t_sort (long min, long max, int i){
		for(int it=0;it<tam;it++){
			if (min<=array[it] && array[it]<max) {
				c_matrix[i][int(floor(it/t_q))] +=1;
				t_count[i]++;
			}
		}
	}
	
	void t_quick(long min, long max, int i){
		for(int b=0; b<block ;b++){
			if(c_matrix[i][b] != -1){
				long pos = 0;
				for(int k=0;k<t_q;k++){
					long p_t = (b*t_q)+k;
					if (min<=array[p_t] && array[p_t]<max) {
						o_array[c_matrix[i][b]+pos] = array[p_t];
						pos++;
					}
				}
			}
		}
		long max_i = (i==intervals-1)? tam:a_t_count[i+1];
		sort (o_array+a_t_count[i],o_array+max_i);
	}

	
	void m_sort (){
		long temp,min,max;
		
		for (int i=0;i<intervals;i++){
			min = (i==0)? LONG_MIN: keys[i-1];
			max = (i==n_key)? LONG_MAX: keys[i];
			t_sort(min,max,i);
		}
		//for (int i=0;i<intervals;i++)
		//	workers[i].join();
		
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
		for(int i=1;i<intervals;i++)
			a_t_count[i] = a_t_count[i-1]+t_count[i-1];

		for (int i=0;i<intervals;i++){
			min = (i==0)? LONG_MIN: keys[i-1];
			max = (i==n_key)? LONG_MAX: keys[i];
			t_quick(min,max,i);
		}
		//for (int i=0;i<intervals;i++)
		//	workers[i].join();
		printf("Se Ordenaron %d Numeros\n",q_count);
	}
	
};

/*
void t_insert (long long d, map m){
	m.insert(d);
}*/

int main (){
	struct timespec begin, end;
	double elapsed;
	
	srand(std::time(0));
	c_time t;
	long tam = 10000;
	int  max = 10000000;
	long * array = new long [tam];
	//long array [] = {10,6,14,1,13,9,5,11,3,7,16,2,12,8,15,4};
	//int  n_thread = 4;//thread::hardware_concurrency();
	int n_keys=7;
	int block =100;
	//map m(n_keys);
	for (int i = 0;i<tam;i++)
		array[i] = rand()%max;

	map_sort map (array, tam, n_keys, block);
	t.init();
	
	clock_gettime(CLOCK_MONOTONIC, &begin);

	map.m_sort();

	clock_gettime(CLOCK_MONOTONIC, &end);

	elapsed = end.tv_sec - begin.tv_sec;
	elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
	
	cout<<"Tiempo hebras: "<<elapsed<<endl;
	
	t.stop();
	t.print();
	//map.print();
	//map.print_k();
	//map.print_cq();
	//map.print_o();
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
