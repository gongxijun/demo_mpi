
/*
----------------------------------
Version    : ??
File Name :     montecarlo_pi.py
Description :   
Author  :       xijun1
Email   :
Date    :       2018/12/17
-----------------------------------
Change Activity  :   2018/12/17
-----------------------------------
__author__ = 'xijun1'
*/

//
#include "mpi.h"
#include "iostream"
#include "ctime"
using std::cin ;
using std::cout ;
using  std::endl;

const int  INT_MAX_ = 1.0e9;

int main(int argc  , char * argv[] ){
    std::cout<< INT_MAX_<<std::endl;
    MPI_Init( &argc , &argv);
    
    int cur_np_id , np_num ,inside=0 , outside ,points=10000;
    double x, y ,pi_comp , real_pi =3.1415926535897;
    
    MPI_Comm_size( MPI_COMM_WORLD , & np_num );
    MPI_Comm_rank( MPI_COMM_WORLD , & cur_np_id );
    
    if( cur_np_id ==0 ){
        //make np_id to master node /root node ;
        for (int i = 1; i < np_num ; ++i) {
            MPI_Send( &points , 1, MPI_INT , i , i ,MPI_COMM_WORLD);
        }
    }else{
        MPI_Recv( &points , 1 , MPI_INT , 0 , cur_np_id , MPI_COMM_WORLD ,MPI_STATUS_IGNORE);
    }

    double * rands = new double [ 2*points ];
    //srand(std::time());
    for (int j = 0; j < 2*points ; ++j) {
        rands[ j ] = random();
        if ( rands[ j ] <= INT_MAX_ ){
            j++;
        }
    }
    for (int k = 0; k <points ; ++k) {
        double  x = rands[ 2*k ]/INT_MAX_;
        double y = rands[ 2*k + 1]/INT_MAX_;
        if( (x*x+y*y) < 1){
            ++inside;
        }
    }

    delete  [] rands;


    if( cur_np_id == 0 ){
        for (int i = 1; i < np_num; ++i) {
            int temp ;
            MPI_Recv( &temp , 1 , MPI_INT , i ,i ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            inside+=temp;
            std::cout<<"inside: "<<inside<<std::endl;
        }
    }else{
        MPI_Send(&inside , 1, MPI_INT , 0, cur_np_id , MPI_COMM_WORLD);
    }

    if(cur_np_id == 0 ){
        pi_comp =  (4.0*inside)/(np_num*points);
        std::cout<<"total: "<<np_num*points<<"pi_comp : "<< pi_comp <<" real_pi: "<<real_pi<<std::endl;
    }
    MPI_Finalize();
    return 0;
}