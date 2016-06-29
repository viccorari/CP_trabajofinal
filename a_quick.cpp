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

void order (long * & array,int inicio, int fin, int & pivot){

	long temp [fin-inicio];
	for (int k=0;k<fin-inicio;k++){
		temp[k]=array[inicio+k];
	}
	int pos = 0;
	int p_f = 0;
	for (int k=0;k<fin-inicio;k++){
		if(temp[k]<pivot){
			array[inicio+k] = temp[k];
			pos++;
		}else{
			array[fin-p_f] = temp[k];
			p_f++;
		}
	}
	pivot = inicio+pos;

}

void a_quick (long * & array, int inicio, int fin, int pivote, int n_thread){
	if((fin-inicio)<((n_thread*4))){
		sort (array+inicio,array+fin);
	}else{
		int * init = new int [n_thread];
		int * piv  = new int [n_thread];
		thread * workers = new thread [n_thread];
		int t = (fin-inicio)/n_thread;
		cout<<"El pivote es: "<<array[pivote]<<endl;
		for (int i=0;i<n_thread;i++){
			init[i] = i*t;
			int max = (i==n_thread-1)? fin-1: init[i]+t-1;
			printf("Min: %d,Max: %d\n",init[i],max);
			piv[i]= array[pivote];
			workers[i] = thread(order, ref(array),init[i],max,ref(piv[i]));
		}
		for (int i=0;i<n_thread;i++){
			workers[i].join();
		}
		for (int i=0;i<n_thread;i++){
			printf("Soy %d: pivot: %d, init: %d\n",i,piv[i],init[i]);
		}
		long o_array [fin-inicio];
		int pos = 0;
		int n_piv = init[n_thread-1];
		for (int i=0;i<n_thread;i++){
			for(int k=init[i];k<piv[i];k++){
				cout<<array[k]<<"-"<<k<<", ";
				o_array[pos] = array[k];
				pos++;
			}
			cout<<endl;
		}
		for (int i=0;i<n_thread;i++){
			int max = (i==n_thread-1)? fin-1: init[i]+t-1;
			for(int k=piv[i];k<=max;k++){
				cout<<array[k]<<"-"<<k<<", ";
				o_array[pos] = array[k];
				pos++;
			}
			cout<<endl;
		}
		cout<<"Aqui: "<<n_piv<<endl;
		for(int i=0;i<fin-inicio;i++){
			array[i] = o_array[i];
			cout<<o_array[i]<<", ";
		}
		cout<<endl;
		
		a_quick(array, inicio, n_piv,array[inicio],n_thread);
		a_quick(array, n_piv,(fin-inicio),array[n_piv], n_thread);
		
	}
	
	
}

void print_a (long * & array, int tam){
	for(int i=0;i<tam;i++)
		cout<<array[i]<<", ";
	cout<<endl;
}

int main(){
	srand(std::time(0));
	long tam = 9;
	int max = 10;
	long * array =  new long [tam];
	for (int i = 0;i<tam;i++)
		array[i] = rand()%max;
	print_a(array,tam);
	a_quick(array, 0, tam, 0, 2);
	print_a(array,tam);
	
}
