/*CS880 final project
  File: digit.h
  author: Tianyi Gu
  digit strcut and class struct for K-NN digit recognizer  
*/  
#ifndef DIGIT_H
#define DIGIT_H
 
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <vector>
//#include <sstream>
//using namespace std;

#include <time.h>
//struct digit
struct digitNode{
  // default constructor
digitNode(){}  
  int* x;
  int y;
  int py;
  int d2;
  float d;
  void clear(){
    delete [] x;
  };

  bool operator<(const digitNode&  _dNode) const {
    return d<_dNode.d;
  };

  void operator=(const digitNode&  _dNode){
    x=_dNode.x;
    y=_dNode.y;
    d2=_dNode.d2;
    d=_dNode.d;
  };
  
};

  


//struct class
struct classNode{
  // default constructor
classNode(){si=0;p=0;}   
  int si;
  float p;
  
  bool operator>(const classNode&  _cNode){
    return p>_cNode.p;
  };
};
#endif
