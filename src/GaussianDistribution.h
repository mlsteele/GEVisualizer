#pragma once
#include <iostream>
#include <fstream>

using namespace std;

#define SQRT_TWO_PI 2.50662827463

class GaussianDistribution{
public:
    GaussianDistribution(){
        N = 0;
        weight = 0;
    }
    
    GaussianDistribution(const GaussianDistribution &rhs){
        this->N = rhs.N;
        this->weight = rhs.weight;
        this->mu = rhs.mu;
        this->sigma = rhs.sigma;
    }
    
    ~GaussianDistribution(){
        
    }
    
    GaussianDistribution& operator=(const GaussianDistribution &rhs){
        if( this != &rhs ){
            this->N = rhs.N;
            this->weight = rhs.weight;
            this->mu = rhs.mu;
            this->sigma = rhs.sigma;
        }
        return *this;
    }
    
    bool init(vector<double > mu,vector<double> sigma,double weight=1){
        if( mu.size() != sigma.size() ) return false;
        N = mu.size();
        this->weight = weight;
        this->mu = mu;
        this->sigma = sigma;
        return true;
    }
    
    double compute(vector<double> x){
        if( x.size() != N ) return -1;
        
        double y = 0;
        for(unsigned int i=0; i<N; i++){
            y += (1.0/(sigma[i]*SQRT_TWO_PI)) * exp( - ( SQR(x[i]-mu[i])/(2*SQR(sigma[i])) ) );
        }
        return y*weight;
    }
    
    double inline SQR(double x){ return x*x; }
    
    unsigned int N;
    double weight;
    vector< double > mu;
    vector< double > sigma;

};
