#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include <mutex>
#include <string>
//create mutex
    std::mutex countmtx;
    std::mutex arraymtx;

//created shared values
    std::vector<int>HISTOGRAM(1001);
    int COUNTER{1};
    bool nolock{false};
    
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
    void worker(int problemCount,bool nolock){
        if(nolock){
            while(true){
                if(COUNTER>problemCount)break;
                int count{COUNTER};
                COUNTER++;
                int steps{collatzFunction(count)};
                if(steps>1000)break;
                HISTOGRAM[steps]++;
            
            }
            return;
        }
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
                if(steps>1000) return;
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
    if(argc == 4 && std::string(argv[3]) == "-nolock"){
        nolock=true;
    }else if(argc==4 && std::string(argv[3])!="-nolock"){
        std::cout<<"Your 4th parameter must be spelled -nolock if you want to use it.\n";
        return 1;
    }
    if(argc < 3 || argc>4){
        std::cout<<"Please enter the correct number of arguments: [Problem_Count, Num_Threads] & [-nolock]\n";
        return 1;
    }
    int problemCount {getProblemCount(argv[1])};
    if(problemCount == 0){
        std::cout<<"Please try again with a non-zero Problem Count\n";
        return 1;
    }

    int getThreadCount = std::stoi(argv[2]);
    if(getThreadCount <= 0){
        std::cout<<"Invalid Argument: User entered, threadCount <= 0\n";
        return 1;
    }
    size_t threadCount = size_t(getThreadCount);

//start timing the threads time taken to solve the sequence
    clock_gettime(CLOCK_REALTIME,&start);
        for(size_t i=0;i<threadCount;i++){
            threads.push_back(std::thread(worker,problemCount,nolock));
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
