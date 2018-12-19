
/*
----------------------------------
Version    : ??
File Name :     factorial_reduce.py
Description :   
Author  :       xijun1
Email   :       xijun1@staff.weibo.com
Date    :       2018/12/17
-----------------------------------
Change Activity  :   2018/12/17
-----------------------------------
__author__ = 'xijun1'
*/

//

#include "mpi.h"
#include "iostream"
using std::cout;
using std::endl;
using  std::cin;
int main(int argc , char * argv[] ){

    int cur_rank , size , nprocessor;

    int numbers , globalFact = -1 , localFact ;
    MPI_Status status;
    MPI_Init( &argc , &argv );
    MPI_Comm_rank(MPI_COMM_WORLD , &cur_rank);
    MPI_Comm_size( MPI_COMM_WORLD , &nprocessor);

    numbers= cur_rank+1;

    MPI_Reduce( &numbers , &globalFact , 1 , MPI_INT , MPI_PROD , 3 , MPI_COMM_WORLD);  //root=3 指定哪一个为root 也就是Master

    {
        std::cout<<"Current rank: "<<cur_rank<<"  Computing the factorial in  MPI: "<<nprocessor << " processus = "<< globalFact <<std::endl;
    }
    localFact =1;
    for (int i = 0; i < nprocessor ; ++i) {
        localFact*=(i+1);
    }
    {
        std::cout<<"Versus local factorial: "<<localFact<<std::endl;
    }
    MPI_Finalize();
    return 0;
}