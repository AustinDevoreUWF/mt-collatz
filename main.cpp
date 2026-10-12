#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include <fstream>//used for file-writting

std::vector<int>HISTOGRAM(1001);
int COUNTER{1};

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
std::ofstream file("histogram.csv");
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
    int threadCount = std::stoi(argv[2]);
/*we are solving problem 1,
we return the stopping time, we need to take the stopping time and increment
the array at location stopping time

might be doing an excess assignment on position.
*/
clock_gettime(CLOCK_REALTIME,&start);
//loop around the problem count something to do with threading happens here
        size_t COUNT = 1;
        size_t position{};
        while(COUNT <= problemCount){
             position = collatzFunction(COUNT);//so this functions return is the index
             HISTOGRAM[position]++;//simplified version of =H[p+1];
            COUNT++;
        }
clock_gettime(CLOCK_REALTIME,&end);
double time_taken=(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

//now build the csv file
for(size_t i=0;i<HISTOGRAM.size();i++){
    file<<problemCount<<','<<threadCount<<','<<time_taken<<'\n';
}
for(size_t i=0;i<HISTOGRAM.size();i++){
    std::cerr<<i<<",frequency_of_stopping_time("<<HISTOGRAM[i]<<")\n";
}
std::cerr<<time_taken<<'\n';
}
