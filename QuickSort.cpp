#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "winbase.h"
#include "Lista.h"
#include "Vector"
#include <fstream>
#include <time.h>
#include <ctime>
using namespace std;
struct Parametro
{
       vector<int>* V;
       int a;
       int b;
       Parametro(vector<int>* V,int a,int b)
       {
                               this->V=V;
                               this->a=a;
                               this->b=b;
       }
};

int quicksort(vector<int>* V,int izq,int der)
{
    int left=izq;
    int right=der;
    int pivote=(*V)[izq];
    int tmp;
	while(left<right)
	{
                     while((*V)[right]>pivote)
                     {
                                              right--;
                     }
                     while((left<right)&&((*V)[left])<=pivote)
                     {
                                                              left++;
                     }
                     if(left<right)
                     {
                                   swap((*V)[right],(*V)[left]);
                     }
 }
 swap((*V)[right],(*V)[izq]);
 return right;
}

Lista Quicksort(vector<int>& V,int threads)
{
      Lista A;
      A.Add(-1);
      A.Add(V.size());
      int* p;
      threads--;
      int izq=0;
      int der=V.size()-1;
      while(0<threads)
      {
                      A.Add(quicksort(&V,izq,der));
                      p=A.Mayor();
                      izq=p[0]+1;
                      der=p[1]-1;
                      threads--;
      }
      return A;
}

void QUICK(vector<int>* V,int izq,int der)
{
     if(izq<der)
     {
              int pivot=quicksort(V,izq,der);
              QUICK(V,izq,pivot-1);
              QUICK(V,pivot+1,der);
     }
}

HANDLE hMutex;
DWORD WINAPI QuickSort(LPVOID lpParameter)
{
      Parametro* A=(Parametro*)lpParameter;
      QUICK((A->V),(A->a)+1,(A->b)-1);
      return 0;
}

void gravar(char* file)
{
     ofstream Ar(file);
     int a;
     for(int i=0;i<10000000;i++)
     {
             a=rand();
             Ar<<a<<endl;
     }
}

int main()
{
    cout<<endl<<'\t'<<'\t'<<'\t'<<"--QuickSort By Avenger---"<<endl<<endl;

    double start, finish;

    //gravar("Entrada.txt");
    //cout<<"Termino De grabar"<<endl<<endl;

    int numero=20, tam = 100000000;
    Lista A;
    vector<int> V;
    cout<<"Loading..."<<endl;
    //ifstream Ar("Entrada.txt");
    int d;
    //while(Ar>>d)
    //{
    //            V.push_back();
    //}

    for (int i = 0; i < tam; ++i) {
        V.push_back(rand());
    }


    cout<<"Tamanho: "<<V.size()<<endl;
    cout<<"Insertando Limites..."<<endl;
    A=Quicksort(V,numero);

    cout<<"Ordenando"<<endl;
    const int nThreads=numero;
    unsigned long id[nThreads];
    HANDLE tHandle[nThreads];

    //hMutex=::CreateMutex(NULL,NULL,NULL);

    clock_t begin = clock();
    for(int i=0;i<nThreads;i++)
    {
            id[i]=1;
            Parametro* tmp=new Parametro(&V,A[i],A[i+1]);
            tHandle[i]=::CreateThread(NULL,1024,QuickSort,(void*)(tmp),NULL,NULL);
    }
    ::WaitForMultipleObjects(nThreads,tHandle,true,INFINITE);
    clock_t end = clock();

    printf("Elapsed time = %f seconds\n", double(end - begin)/ CLOCKS_PER_SEC);


    //printf("Elapsed time = %e seconds\n", finish-start);

    /*cout<<"Gravando..."<<endl;
    ofstream Fi("Vector.txt");
    vector<int>::iterator i;
    for(i=V.begin();i!=V.end();i++)
    {
                                   Fi<<*i<<endl;
    }*/
    system("pause");
    return 0;
}

