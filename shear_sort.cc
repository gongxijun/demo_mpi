
/*
----------------------------------
Version    : ??
File Name :     ShearSort.py
Description :   
Author  :       xijun1
Email   :       xijun1@staff.weibo.com
Date    :       2018/12/18
-----------------------------------
Change Activity  :   2018/12/18
-----------------------------------
__author__ = 'xijun1'
*/

#include "mpi.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cassert>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

//归并排序
bool Less(int a, int b){
    return a <b;
}

bool Greater(int a, int b){
    return a > b;
}
//升序
bool Merge(int *A ,int ps ,int mid , int len , bool( * Comp)(int a , int b) ){
    int i=ps,j=mid,cnt=0;
    int C[len-ps+1];
    bool is_change = false;
    while(i<mid&&j<len ){
        if(Comp(A[i] , A[j])) {
            C[cnt++] = A[j++];
            is_change= true;
        }
        else C[cnt++]=A[i++];
    }
    while(i<mid) C[cnt++]=A[i++];
    while(j<len) C[cnt++]=A[j++];
    for(i=ps; i<len ;i++)
        A[i]=C[i-ps];
    return is_change;
}


//非递归版
bool Msort_(int *arg , int ps , int len  , bool( * Comp)(int a , int b) ){

    int s,t=1;
    bool is_change = false;
    while(ps+t<=len){
        s=t;
        t=2*s;
        int pos=ps;
        while(pos+t<=len){
            is_change |= Merge(arg,pos,pos+s,pos+t , Comp);
            pos+=t;  //移动
        }
        if(pos+s<len)
            is_change |= Merge(arg,pos,pos+s,len , Comp);
    }
    if(ps+s<len)
        is_change |= Merge(arg,ps,ps+s,len , Comp);

    return is_change;
}


int main( int argc , char * argv []){
    int process_id , num_process;
    MPI_Init( & argc , &argv);
    //make  process_id = 0 to master; other slaves
    MPI_Comm_size( MPI_COMM_WORLD , &num_process);
    MPI_Comm_rank( MPI_COMM_WORLD , &process_id);
    assert( num_process == 4 );
    int recv_buf [4];
    int brr[16];
    unsigned int part_size=(sizeof(recv_buf)/sizeof(recv_buf[0]));
    //init
    if( 0 == process_id ){
        int arr[] = {3,11,6,16, 8,1,5,10,
                   14,7,12,2, 4,13,9,15};
        int len  =sizeof(arr)/ sizeof(arr[0]);
        memcpy( brr , arr , len* sizeof(arr[0]));
    }else{
        int len  =sizeof(brr)/ sizeof(brr[0]);
        memset(brr,0, len* sizeof(brr[0]));
    }

    int epoch=0;
    while( epoch < 5 ){
    if( 0 == process_id){
        //div part len( brr )/ num_process
        part_size = (sizeof(brr)/sizeof(brr[0]))/num_process;
        //send
        for (int i = 0; i < num_process; ++i) {
            MPI_Send(&brr[i*part_size] ,part_size ,MPI_INT ,i,i,MPI_COMM_WORLD);
        }
    }
    {
        //work calc

        MPI_Recv(&recv_buf ,part_size,MPI_INT ,0 ,process_id,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        if( epoch&1 ){
            Msort_(recv_buf, 0, sizeof(recv_buf) / sizeof(recv_buf[0]),Greater);
        }else{
            if (process_id & 1) {
                Msort_(recv_buf, 0, sizeof(recv_buf) / sizeof(recv_buf[0]), Less);
            } else {
                Msort_(recv_buf, 0, sizeof(recv_buf) / sizeof(recv_buf[0]), Greater);
            }
        }
        MPI_Send(&recv_buf ,part_size ,MPI_INT ,0,process_id,MPI_COMM_WORLD);

    }
    //recv
    if(0 == process_id) {
        std::cout << "start epoch: " << epoch << std::endl;
        //recv
        for (int i = 0; i < num_process; ++i) {
            MPI_Recv(&brr[i * part_size], part_size, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < 4; ++j) {
                std::cout << " " << brr[i * part_size + j];
            }
            std::cout << std::endl;
        }
        //check 数组是否需要停止

        //调整数组
        if( epoch+1<5 ) {
            for (int i = 0; i < 4; ++i) {
                for (int j = i; j < 4; ++j) {
                    int tmp = brr[(i * part_size) + j];
                    brr[(i * part_size) + j] = brr[(j * part_size) + i];
                    brr[(j * part_size) + i] = tmp;
                }
            }
        }
    }
      epoch++;
    }

    if(0 == process_id) {
        std::cout<<"---------------------------"<<std::endl;
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                std::cout << brr[j*4 + i] << "  ";
            }
            std::cout << std::endl;

        }
    }
    MPI_Finalize();
    return 0;
}

//

