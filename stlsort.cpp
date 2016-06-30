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
	long tam =10000;
	int max = 10000000;
	c_time c;
	//long temp [] {1, 6, 5, 9, 9, 4, 8, 6, 5};
	long * array = new long [tam];// = temp;
	for (int i = 0;i<tam;i++)
		array[i] = rand()%max;
	//print_a(array,tam);
	//order(array, 10, tam, pivot, pivot);
	c.init();
	sort(array, array+tam);
	c.stop();
	c.print();
	//print_a(array,tam);
	
}


