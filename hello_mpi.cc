#include <iostream>
#include "mpi.h"
int main( int argc , char * argv []  ){

int id , p ,name_len ; 
char processor_name [ MPI_MAX_PROCESSOR_NAME  ];
MPI::Init( argc  , argv );

p = MPI::COMM_WORLD.Get_size();

id  = MPI::COMM_WORLD.Get_rank();

MPI_Get_processor_name( processor_name , &name_len  );

std::cout<< "Processor " << processor_name << " ID : "<< id << "Welcome to MPI ~"<<std::endl;
MPI::Finalize();
return 0;
}
