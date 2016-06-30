#include <iostream>
#include "Nodo.h"
using namespace std;
class Lista
{
      private:
              Nodo* phead;
      public:
             Lista():phead(0){};
             bool Find(int pos,Nodo**& p)
             {
                  p=&phead;
                  while(*p)
                  {
                           if((*p)->pos==pos) return 1;
                           if((*p)->pos>pos) return 0;
                           p=&((*p)->psig);
                  }
                  return 0;
             }
             bool Add(int pos)
             {
                  Nodo** p;
                  if(Find(pos,p)) return 0;
                  Nodo* pNuevo=new Nodo(pos);
                  pNuevo->psig=*p;
                  *p=pNuevo;
                  return 1;
             }
             Lista operator= (Lista A)
             {
                   Nodo* tmp=A.phead;
                   while(tmp)
                   {
                             Add(tmp->pos);
                             tmp=tmp->psig;
                   }
                   return *this;
             }
             int operator[] (int pos)
             {
                 Nodo* tmp=phead;
                 while(tmp&&(0<pos))
                 {
                       tmp=tmp->psig;
                       pos--;
                 }
                 if(!tmp) return -1;
                 return tmp->pos;
             }
             int* Mayor()
             {
                  Nodo* tmp=phead;
                  int m=0;
                  int a=tmp->pos;
                  int *p=new int[2];
                  p[0]=p[1]=a;
                  
                  tmp=phead->psig;
                  while(tmp)
                  {
                            if(m<tmp->pos-a)
                            {
                                               p[0]=a;
                                               p[1]=tmp->pos;
                                               m=p[1]-p[0];
                            }
                            a=tmp->pos;
                            tmp=tmp->psig;
                  }
                  return p;
             }
             void Print()
             {
                  Nodo* tmp=phead;
                  while(tmp)
                  {
                            cout<<tmp->pos<<" ";
                            tmp=tmp->psig;
                  }
                  cout<<endl<<endl;
             }
             
};
