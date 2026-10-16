//Written by Austin Devore

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <string>

//Create the mutex's for locking access:
    std::mutex countmtx; //Updating shared COUNTER
    std::mutex arraymtx; //Updating the shared HISTOGRAM array

//Create values that will be shared by each thread
    std::vector<int>HISTOGRAM(1001);
    int COUNTER{1};
    bool nolock{false};//
    
//Collatz calculation function. Returns Stopping Time(steps taken to reach 1)
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
            //Lock the COUNT access to prevent Race Conditions
            countmtx.lock();
                if(COUNTER>problemCount){ 
                    countmtx.unlock();
                    break;}
                int count{COUNTER};
                COUNTER++;
            countmtx.unlock();
            int steps {collatzFunction(count)};
            //Lock adding to the HISTOGRAM Array to prevent inacurate incrementation
            arraymtx.lock(); 
                if(steps>1000){
                    arraymtx.unlock();
                    return;
                }
                HISTOGRAM[steps]++;
            arraymtx.unlock();
        }
    }

//getting and validating problemCount
    int getProblemCount(char* problemCount){
        int x = std::stoi(problemCount);
        if(x == 0)return 0;
        return x;
    }

int main(int argc, char* argv[]){

//Initialize variables
    std::chrono::steady_clock::time_point start, end;
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
    if(getThreadCount <= 0 || getThreadCount > 8){
        std::cout<<"Please enter a Thread Count between 1-8\n";
        return 1;
    }
    size_t threadCount = size_t(getThreadCount);

//start timing the threads time taken to solve the sequence
    start = std::chrono::steady_clock::now();

    for(size_t i = 0; i < threadCount; i++){
        threads.push_back(std::thread(worker, problemCount, nolock));
    }

    for(size_t i = 0; i < threadCount; i++){
        threads.at(i).join();
    }

    end = std::chrono::steady_clock::now();

//Output to a CSV file to build hte Histogram 
    for(size_t i=0;i<HISTOGRAM.size();i++){
        std::cout<<i<<','<<HISTOGRAM[i]<<'\n';
    }

    double time_taken = std::chrono::duration<double>(end - start).count();

//print program information to the cerr stream 
    std::cerr<<problemCount<<','<<threadCount<<','<<time_taken<<'\n';
}
