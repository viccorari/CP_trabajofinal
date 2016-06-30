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

void order(long *array, long inicio, long fin, long gpivot, long &pivot)
{
	long in = inicio;
	long fi = fin;
	
	long p_i=0;
	long p_f=0;
	
	long tam = fi-in;
	long * temp = new long [tam];
	
	for (long i=in;i<fin;i++){
		if(array[i]<gpivot){
			temp[p_i] = array[i];
			p_i++;
		}else{
			temp[tam-(p_f)] = array[i];
			p_f++;
		}
	}
	long p = 0;
	for (long i=in;i<fin;i++){
		array[i] = temp[p++];
	}
	pivot = fi-p_f;
}

void a_quick (long * & array, int inicio, int fin, int pivote, int n_thread){
	cout<<"INIT, inicio: "<<inicio<<", Fin: "<<fin<<endl; 
	if((fin-inicio)<((n_thread*4))){
		sort (array+inicio,array+fin);
	}else{
		long * init = new long [n_thread];
		long * piv  = new long [n_thread];
		thread * workers = new thread [n_thread];
		int t = (fin-inicio)/n_thread;
		cout<<"TTTTT: "<<t<<endl;
		long gpivote = array[pivote];
		cout<<"El pivote es: "<<gpivote<<endl;
		for (int i=0;i<n_thread;i++){
			init[i] = inicio+i*t;
			long max = (i==n_thread-1)? fin: init[i]+t-1;
			printf("Min: %ld,Max: %ld\n",init[i],max);
			workers[i] = thread(order, ref(array),init[i],max,gpivote,ref(piv[i]));
		}
		for (int i=0;i<n_thread;i++){
			workers[i].join();
		}
		for (int i=0;i<n_thread;i++){
			printf("Soy %d: init: %ld, pivot: %ld\n",i,init[i],piv[i]);
		}
		long o_array [fin-inicio];
		int pos = 0;
		int n_piv = 0;
		for (int i=0;i<n_thread;i++){
			n_piv+= (piv[i]-init[i]);
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
		cout<<"inicio: "<<inicio<<", piv: "<<n_piv<<", fin: "<<fin<<endl;
		a_quick(array, inicio, n_piv,array[inicio],n_thread);
		a_quick(array, n_piv,fin,array[fin-1], n_thread);
	}
}

void print_a (long * & array, int tam){
	for(int i=0;i<tam;i++)
		cout<<array[i]<<"\t";
	cout<<endl;
	for(int i=0;i<tam;i++)
		cout<<i<<"\t";
	cout<<endl;
}

int main(){
	srand(std::time(0));
	long tam = 19;
	int max = 10;
	 
	long * array = new long [tam];
	for (int i = 0;i<tam;i++)
		array[i] = rand()%max;
	print_a(array,tam);
	long pivot = array[0];
	cout<<"Pivot  :"<<pivot<<", Tam: "<<tam<<endl;

	a_quick(array, 0, tam, 0,8);
	print_a(array,tam);
	cout<<"Pivot f:"<<pivot<<endl;
	
}






