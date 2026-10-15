#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include <mutex>

//create mutex
    std::mutex countmtx;
    std::mutex arraymtx;

//created shared values
    std::vector<int>HISTOGRAM(1001);
    int COUNTER{1};

    
//Actual Math, returns stoppingTime(steps taken to reach 1)
    int collatzFunction(long long n){
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


//Threads use this Worker function
    void worker(int problemCount){
        while(true){
            countmtx.lock();
                if(COUNTER>problemCount){ 
                    countmtx.unlock();
                    break;}
                int count{COUNTER};
                COUNTER++;
            countmtx.unlock();
            int steps {collatzFunction(count)};
            arraymtx.lock();
                HISTOGRAM[steps]++;//simplified version of =H[p+1];
            arraymtx.unlock();
        }
    }

//getting the problemcount for argument purposes
    int getProblemCount(char* problemCount){
        int x = std::stoi(problemCount);
        if(x == 0)return 0;
        return x;
    }

int main(int argc, char* argv[]){
//Init variables
    struct timespec start, end;
    std::vector<std::thread>threads;

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

//start timing the threads time taken to solve the sequence
    clock_gettime(CLOCK_REALTIME,&start);
        for(size_t i=0;i<threadCount;i++){
            threads.push_back(std::thread(worker,problemCount));
        }
        for(size_t i=0;i<threadCount;i++){
            threads.at(i).join();
        }
    clock_gettime(CLOCK_REALTIME,&end);
//now build the csv file
    for(size_t i=0;i<HISTOGRAM.size();i++){
        std::cout<<i<<','<<HISTOGRAM[i]<<'\n';
    }

//time-taken 
    double time_taken = (end.tv_sec - start.tv_sec)+ (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    std::cerr<<problemCount<<','<<threadCount<<','<<time_taken<<'\n';
}
