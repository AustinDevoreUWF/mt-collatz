#include <iostream>
#include <thread>
#include <time.h>

//getting the problemcount for argument purposes
    int getProblemCount(char* problemCount){
        int x = std::stoi(problemCount);
        if(x == 0)return 0;
        return x;
    }

//worker Function
    size_t collatzFunction(long long n){
        size_t count{0};
        while(n !=1){
            count++;
            if(n % 2 == 0){
                n = n / 2;
            }else if(n % 2 != 0){
                n = (3*n) + 1;
            }
        }
        return count;
    }

int main(int argc, char* argv[]){
struct timespec start, end;

//Confirming user inputs
    if(argc != 3){
        std::cout<<"Please enter the correct arguments: [Problem_Count, Num_Threads]\n";
        return 1;
    }
    int problemCount {getProblemCount(argv[1])};
    if(problemCount == 0){
        std::cout<<"Please try again with a non-zero Problem Count\n";
        return 1;
    }

clock_gettime(CLOCK_REALTIME,&start);
//loop around the problem count something to do with threading happens here
        size_t COUNT = 1;
        while(COUNT <= problemCount){
            std::cout<<collatzFunction(COUNT);
            std::cout<<'\n';
            COUNT++;
        }
clock_gettime(CLOCK_REALTIME,&end);
double time_taken=(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
std::cout<<time_taken<<'\n';
}
