
/*
----------------------------------
Version    : ??
File Name :     ring_broadcast_mpi.py
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

int main( int argc , char * argv []){
    int rank  , size ,value ;
    MPI_Status mpiStatus;
    MPI_Init( & argc ,  & argv); // init
    MPI_Comm_size( MPI_COMM_WORLD , &size ) ;   //当前进程数量
    MPI_Comm_rank( MPI_COMM_WORLD , &rank) ;    //当前正在运行的进程的标识号
    do{
        std::cout<<"rank: "<<rank<<std::endl;
        if( rank ==0 ) {
            /*** master node to sends out the data */
            scanf("%d", &value);
            MPI_Send(&value, 1, MPI_INT, rank + 1, 0  , MPI_COMM_WORLD);
        }else{
            /** salve nodes block on receive the send on the value **/
           MPI_Recv(&value , 1 , MPI_INT , rank -1  , 0 , MPI_COMM_WORLD , &mpiStatus );
            if( rank < size  -1){
                /***当 当前该节点接收到消息的时候，它需要将消息进一步传递下一个节点，root:0 -s->node:1 { node:1 -r-> root:0  } -s->* next_node:2 ***/
                MPI_Send( &value , 1 , MPI_INT , rank + 1  , 0 , MPI_COMM_WORLD);
            }
            printf(" process %d got %d\n", rank , value);
        }
    }while( value > 0);
    MPI_Finalize() ;
    return 0;
}