#include <iostream>
#include <thread>
#include <time>
int getProblemCount(char* problemCount){
    int x = std::stoi(problemCount);
    if(x == 0)return 0;
    return x;
}

size_t collatzFunction(int problemCount){
    if(problemCount % 2 == 0){
        problemCount = problemCount / 2;
    }else if(problemCount % 2 != 0 && problemCount !=0 ){
        problemCount = (3*problemCount) + 1;
    }

}

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cout<<"Please enter the correct arguments: [Problem_Count, Num_Threads]\n";
        return 1;
    }
    int problemCount {getProblemCount(argv[1])};
    if(problemCount == 0){
        std::cout<<"Please try again with a non-zero Problem Count\n";
        return 1;
    }
    clock_gettime(collatzFunction(problemCount));
}
