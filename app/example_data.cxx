
#include <iostream>

#include <TRandom.h>

int main(int argc, char  **argv) {

    for (int i=0; i<1000; ++i) {
        std::cout << " " <<  gRandom->Gaus(0,1)
                  << " " <<  gRandom->Gaus(1,2)
                  << " " <<  gRandom->Gaus(2,3)
                  << " " <<  gRandom->Uniform(0,1)
                  << " " <<  gRandom->Uniform(-1,1)
                  << " " <<  gRandom->Uniform(-2,2)
                  << std::endl;        
    }

}
