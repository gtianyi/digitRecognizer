/*CS880 final project
  File: recognizer.cpp
  author: Tianyi Gu
  K-NN digit recognizer main function
*/ 
#include "digit.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <queue>
//#include <time.h>
#include <sys/time.h>

using namespace std;

void printImage(vector<digitNode> inputdigit, string m){
  //output
  for(int i =0;i<inputdigit.size();i++){
    time_t rawtime;
    struct tm * timeinfo;
    char ctstr [7];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (ctstr,7,"%H%M%S",timeinfo);
    stringstream sy,si,py;
    py<<inputdigit[i].py;
    sy<<inputdigit[i].y;
    si<<i;
    string filename= "./images/"+m+"_y"+sy.str()+"_p"+py.str()+"_"
        +ctstr+"_i"+si.str();
    const char* cfilename=filename.c_str();
    ofstream imagefile;
    imagefile.open(cfilename);
    imagefile<< "P2"<<endl;
    imagefile<<"28  28"<<endl;
    imagefile<<"255"<<endl;
    int p=0;
    for(int a=0;a<28;a++)
    {
      for(int b=0;b<28;b++)
      {
        imagefile<<inputdigit[i].x[p]<< "  ";
        p++;
      }
      imagefile<<endl;
    }
    imagefile.close();
  }
}

float doknn(int _dimension,int _k,vector<digitNode>& inputdigit,vector<digitNode>& outdigit,vector<digitNode>* mdigit){
  classNode cnodes [10];
  int missc=0;
  float missrate=0;
  //do knn for every testdigit
  for(int i=0;i<outdigit.size();i++){
    //calc distance
    priority_queue<digitNode> pQueue;
    for(int j=0;j<inputdigit.size();j++){
      inputdigit[j].d=0;
      inputdigit[j].d2=0;
      for(int k=0;k<_dimension;k++){
        inputdigit[j].d2+=(inputdigit[j].x[k]-outdigit[i].x[k])*

            (inputdigit[j].x[k]-outdigit[i].x[k]);
      }
      inputdigit[j].d=-sqrt((float)inputdigit[j].d2);
      pQueue.push(inputdigit[j]);
    }    
    //get first k
    for(int j=0;j<10;j++){
      cnodes[j].si=0;
      cnodes[j].p=0;
    }
    for(int j=0;j<_k;j++){
      digitNode _dig=pQueue.top();
      pQueue.pop();
      cnodes[_dig.y].si++;
    }
    float _p=-1;
    for(int j=0;j<10;j++){
      cnodes[j].p=(float)cnodes[j].si/(float)_k;
      if(_p<cnodes[j].p){
        _p=cnodes[j].p;
        outdigit[i].py=j;
      }
    }
    if(outdigit[i].py!=outdigit[i].y){
      missc++;
      if(mdigit){
        (*mdigit).push_back(outdigit[i]);        
      }
    }
    cout<<"curout:"<<i<<"missc"<<missc<<" k: "<<_k<<endl;
  }
  missrate=(float)missc/(float)outdigit.size();
  return missrate;  
}



int main(int argc, char ** argv) {         
 
  //input parameters
  int _class=10;
  int _dimension=784;
  //stringstream spk(argv[1]);  
  int _k=1;
  //spk >> _k;
  int _foldnum=5;
  int _cvpercent=1;
  vector<digitNode> inputdigit;
  bool iscv=true;
  bool isktest=false;
  bool oneknn=false;
  //vector<classNode> cnodes;
  
  //read train data
  ifstream file ( "train.csv" ); 
  string value;
  while (getline( file, value))
  {    
    stringstream ss(value);
    digitNode dig;
    int* x = new int[_dimension];
    int i=0;
    getline(ss, value, ',');
    stringstream v(value);
    int y;
    v>>y;
    while (getline(ss, value, ','))
    {
      stringstream v1(value);
      v1>>x[i];      
      i++;
    }
    dig.x=x;
    dig.y=y;   
    inputdigit.push_back(dig);
  }
  vector<digitNode> outdigit;  
  //read test data
  ifstream tfile ( "test.csv" );  
  while (getline( tfile, value) )
  {    
    stringstream ss(value);
    digitNode dig;
    int* x = new int[_dimension];
    int i=0;
    getline(ss, value, ',');
    stringstream v(value);
    int y;
    v>>y;
    while (getline(ss, value, ','))
    {
      stringstream v1(value);
      v1>>x[i];      
      i++;
    }
    dig.x=x;
    dig.y=y; 
    outdigit.push_back(dig);
  }
  

  //cross validation 
  if(iscv){
    //generate folds
    vector<vector<digitNode > > folds;   
    int fnum=400;
    for(int i=0;i<_foldnum;i++){
      vector<digitNode> remaininput;
      remaininput = outdigit;
      //if(i==_foldnum-1) fnum=remaininput.size();
      vector<digitNode> f;
      for(int j=0;j<fnum;j++){
        struct timeval tv;    
        gettimeofday(&tv,NULL);    
        int seed= (tv.tv_sec * 1000 + tv.tv_usec / 1000)%91;   
        srand(seed);
        int aindex=rand()%remaininput.size();
        f.push_back(remaininput[aindex]);
        remaininput.erase(remaininput.begin()+aindex);
      }
      folds.push_back(f);      
    }
    
    //try Ks find best
    time_t rawtime;
    struct tm * timeinfo;
    char ctstr [7];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (ctstr,7,"%H%M%S",timeinfo);
    ofstream myfile;
    stringstream sst,ssf;
    sst<<ctstr;
    ssf<<_foldnum;
    string _fname1= "./data/knn_"+ssf.str()+"f_"+sst.str(); 
    const char* fname1 = _fname1.c_str();
    myfile.open (fname1);
  
    float _mrate=1;    
    for(int tk=1;tk<10;tk++){
      int k=tk;
      float mtestrate=0;
      float mtrainrate=0;
      vector<digitNode> inputtest;  
      // for(int j=0;j<400;j++){
      //   inputtest.push_back(inputdigit[j]);
      // }
      // mtrainrate= doknn(_dimension,k,inputdigit,inputtest,0);
      for(int i=0;i<_foldnum;i++){              
        float cmtestrate = doknn(_dimension,k,inputdigit,folds[i],0);              //cout<<"cmrate:"<<cmrate<<endl;
        //mtrainrate = mtrainrate+ (cmtrainrate-mtrainrate)/(i+1);
        mtestrate = mtestrate+ (cmtestrate-mtestrate)/(i+1);
      }
      
      cout<<"k: "<<k<<endl;
      myfile<<"k: "<<k<<" "<<"mtrainrate: "<<mtrainrate<<
          " mtestrate: "<<mtestrate<<endl;
      if(_mrate>mtestrate){
        _mrate=mtestrate;
        _k=k;
      }
    }
    myfile.close();
  }
  //doknn use best k;
  if(isktest){
    vector<digitNode> inputtest;
    for(int i=0;i<400;i++){
      inputtest.push_back(inputdigit[i]);
    }
    for(_k=1;_k<=10;_k++){      
      float _mter = doknn(_dimension,_k,inputdigit,outdigit,0);
      float _mtrr = doknn(_dimension,_k,inputdigit,inputtest,0);
      cout<<"finalmr: "<<_mter<<" mrtrain "<<_mtrr<<" k: "<<_k;
    }
  }
  else if(oneknn){
    vector<digitNode> mdigit;
    float _mr = doknn(_dimension,_k,inputdigit,outdigit,&mdigit);
    stringstream sk;
    sk<<_k;
    printImage(mdigit,"k_"+sk.str());
    cout<<"finalmr: "<<_mr<<"k: "<<_k<<endl;
  }
  //free memory
  for(int i=0;i<inputdigit.size();i++){
    delete [] inputdigit[i].x;
  }
  for(int i=0;i<outdigit.size();i++){
    delete [] outdigit[i].x;
  }
}
