
/*
----------------------------------
Version    : ??
File Name :     mini_param_from_arr.py
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
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

const int maxn = 1000+1;

typedef struct Node{
    float value ;
    int index;
} MPINode ;

int main( int argc , char * argv [] ){

    MPI_Init( &argc , &argv);
    float loca_val [ maxn ] ;
    int cur_rank , size ;
    //MPI_Status status ;
    MPI_Comm_size( MPI_COMM_WORLD , &size );
    MPI_Comm_rank( MPI_COMM_WORLD , &cur_rank );
    MPINode  in , out ;
    srand(2312+cur_rank);
    for (int i = 0; i <  maxn ; ++i) {
        loca_val [ i ] = static_cast<float>(drand48()+1);
    }

    in.value = loca_val[ 0 ];
    in.index =0;
    for (int j = 1; j <  maxn; ++j) {
        if( in.value > loca_val[ j ]){
            in.value = loca_val[ j ];
            in.index = j;
        }
    }
    std::cout<<"local val: "<<in.value <<" local index:  "<<in.index<<std::endl ;

    in.index = cur_rank*maxn + in.index ;
    MPI_Reduce( (void *)&in , (void *)&out , 1,   MPI_FLOAT_INT ,MPI_MINLOC, 3 , MPI_COMM_WORLD);
    int min_val = out.value ;
    int min_index = out.index % maxn;
    int node_rank = out.index / maxn;
    std::cout<<"cur_rank : "<<cur_rank<<" nprocessor: "<<size <<" { min_val: "<<min_val<<"  min_index:"<<min_index<<" node_rank:   "<<node_rank<<" }"<<std::endl;

    MPI_Finalize();
    return 0;
}