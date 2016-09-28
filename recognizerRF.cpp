/*CS880 final project
  File: recognizerRF.cpp
  author: Tianyi Gu
  random forest digit recognizer main function
*/
#include "digit.h"
#include "dtree.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <queue>
#include <sys/time.h>

using namespace std;

void printImage(vector<digitNode>& inputdigit, string m){
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
int checkAllSame(vector<digitNode>& examples){  
  int y=examples[0].y;
  for(int i=1;i<examples.size();i++){
    if(y!=examples[i].y) return 0;
  }
  return 1;
}

int findModeClass(classNode* cnodes){  
  int ret=-1;
  int maxc=-1; 
  for(int i=0;i<10;i++){
    if(maxc<cnodes[i].si){
      ret=i;
      maxc=cnodes[i].si;
    }
  }
  return ret;
}

classNode* getClass(vector<digitNode>& examples){
  classNode* cnodes =new classNode[10];  
  for(int i=0;i<examples.size();i++){
    cnodes[examples[i].y].si++;
  }
  return cnodes;
}

int findBestPixel(vector<int>& pixs,vector<digitNode>& examples,
                  catalog* catalogs, int _catalogNum){
  int ret=-1;
  float e=100;
  for(int i=0;i<pixs.size();i++){
    float reminder=0;
    for(int v=0;v<_catalogNum;v++){
      vector<digitNode> newexamples;
      classNode cnodes [10];
      for(int j=0;j<examples.size();j++){
        if(examples[j].x[pixs[i]]>=catalogs[v].gmin&&
           examples[j].x[pixs[i]]<catalogs[v].gmax){
          newexamples.push_back(examples[j]);
          cnodes[examples[j].y].si++;
        }
      }
      float weight=(float)newexamples.size()/(float)examples.size();
      if(weight==0) continue;     
      float sumh=0;      
      for(int k=0;k<10;k++){        
        cnodes[k].p=(float)cnodes[k].si/(float)newexamples.size();
        if(cnodes[k].p>0){
          sumh+= cnodes[k].p*(log( cnodes[k].p)/log(2));
        }
      }
      reminder+=weight*(-sumh);
      
    }
    if(e>reminder){
      e=reminder;
      ret=i;
    }
  }
  return ret;  
}

treeNode* dtl(vector<digitNode>& examples, vector<int>& pixs,
              treeNode* def, catalog* catalogs, int _catalogNum){ 
  if(examples.size()==0) return def;
  delete def;
  treeNode* n= new treeNode();  
  if(checkAllSame(examples)){    
    n->isleaf=1;
    n->target=examples[0].y;    
    return n;
  }
  if(pixs.size()==0){    
    n->isleaf=1;
    classNode* thisclasses=getClass(examples);
    n->target=findModeClass(thisclasses);
    delete [] thisclasses;
    return n;
  }
  int bestpixindex= findBestPixel(pixs,examples,catalogs,_catalogNum);
  n->pixel=pixs[bestpixindex];
  n->childs=new treeNode*[_catalogNum];  
  for(int i=0;i<_catalogNum;i++){
    vector<digitNode> newexamples;   
    for(int j=0;j<examples.size();j++){
      if(examples[j].x[n->pixel]>=catalogs[i].gmin&&
         examples[j].x[n->pixel]<catalogs[i].gmax){
        newexamples.push_back(examples[j]);
      }
    }    
    vector<int> newpixs=pixs;    
    newpixs.erase(newpixs.begin()+bestpixindex);
    treeNode* curdef= new treeNode();
    curdef->isleaf=1;
    classNode* thisclasses=getClass(newexamples);
    curdef->target=findModeClass(thisclasses);
    delete [] thisclasses;
    treeNode* subtree=dtl(newexamples,newpixs,curdef,
                          catalogs,_catalogNum);    
    n->childs[i]=subtree;    
  } 
  return n;
}

float doClassify(treeNode** root,vector<digitNode>& outdigit,
                 vector<digitNode>* mdigit,
                 catalog* catalogs, int _catalogNum,
                 int _treeNum){
  float missrate=0;
  int missc=0;
  //do RF for every testdigit
  for(int i=0;i<outdigit.size();i++){
    classNode cnodes [10];
    for(int j=0;j<_treeNum;j++){
      treeNode* curNode=root[j];
      digitNode data=outdigit[i];
      while(!curNode->isleaf){
        for(int v=0;v<_catalogNum;v++){
          if(data.x[curNode->pixel]>=catalogs[v].gmin&&
             data.x[curNode->pixel]<catalogs[v].gmax){
            curNode=curNode->childs[v];
            break;
          }
        }
      }
      cnodes[curNode->target].si++;
    }
    outdigit[i].py=findModeClass(cnodes);
    if(outdigit[i].py!=outdigit[i].y){
      missc++;     
      if(mdigit){
        (*mdigit).push_back(outdigit[i]);        
      }
    }
    cout<<"curout:"<<i<<"missc"<<missc<<endl;    
  }
  missrate=(float)missc/(float)outdigit.size();
  return missrate;  
}

void deleteTreeNode(treeNode* n,int _catalogNum){
  if(n->isleaf) {delete n; return;}
  for(int i=0;i<_catalogNum;i++){
    deleteTreeNode(n->childs[i],_catalogNum);
  }
  delete [] n->childs;
  delete n;
}

int main(int argc, char ** argv) {         
 
  //input parameters
  int _class=10;
  int _dimension=784;
  stringstream sca(argv[2]);
  int _catalogNum;//greater than 3
  sca>>_catalogNum;
  int _wvalue=5;
  int _bvalue=250;
  stringstream st(argv[1]);
  int _treeNum;
  st>> _treeNum;
  st.clear();
  stringstream ssp(argv[3]);
  int _sampleNum;
  ssp>>_sampleNum;
  stringstream satt(argv[4]);
  int _attriNum;
  satt>>_attriNum;
  //create catalogs
  catalog catalogs[_catalogNum];
  catalogs[0].gmin=0;
  catalogs[0].gmax=_wvalue;
  catalogs[1].gmin=_bvalue;
  catalogs[1].gmax=256;
  int colorintval=(_bvalue-_wvalue)/(_catalogNum-2);
  for(int i=2;i<_catalogNum;i++){
    catalogs[i].gmin=_wvalue+(i-2)*colorintval;
    if(i==_catalogNum-1){
      catalogs[i].gmax=_bvalue;
    }
    else{
      catalogs[i].gmax=_wvalue+(i-1)*colorintval;
    }
  }

  //read train data
  vector<digitNode> inputdigit;  
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

  //read test data
  vector<digitNode> outdigit;    
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

  //train n trees
  treeNode* forest[_treeNum];
  vector<int> pixs;
  for(int i=0;i<_dimension;i++){
    pixs.push_back(i);
  }
  for(int i=0;i<_treeNum;i++){
    vector<digitNode> trainset = inputdigit;
    for(int j=0;j<inputdigit.size()-_sampleNum;j++){
      struct timeval tv;    
      gettimeofday(&tv,NULL);    
      int seed= (tv.tv_sec * 1000 + tv.tv_usec / 1000)%91;   
      srand(seed);
      int aindex=rand()%trainset.size();
      trainset.erase(trainset.begin()+aindex);
    }
    vector<int> features;
    vector<int> _pixs=pixs;
    for(int j=0;j<_attriNum;j++){
      struct timeval tv;    
      gettimeofday(&tv,NULL);    
      int seed= (tv.tv_sec * 1000 + tv.tv_usec / 1000)%91;   
      srand(seed);
      int aindex=rand()%_pixs.size();
      features.push_back(_pixs[aindex]);
      _pixs.erase(_pixs.begin()+aindex);
    }
    forest[i]= dtl(trainset,features,0,catalogs,_catalogNum);
    cout<<"trainTree: "<<i<<endl;
  }
  
  //doClassification
  vector<digitNode> mdigit;
  //float _mr = doClassify(treeRoot,outdigit,&mdigit,
  //                       catalogs,_catalogNum);
  float _mr = doClassify(forest,outdigit,&mdigit,
                         catalogs,_catalogNum,_treeNum);
  stringstream sc;
  sc<<_catalogNum;
  printImage(mdigit,"c_"+sc.str());
  cout<<"finalmr: "<<_mr<<" treeNum: "<<_treeNum<<
      " samples: "<<_sampleNum<<" features: "<<_attriNum<<
      " cata: "<<_catalogNum<<endl;    
 
  //free memory
  for(int i=0;i<inputdigit.size();i++){
    delete [] inputdigit[i].x;
  }
  for(int i=0;i<outdigit.size();i++){
    delete [] outdigit[i].x;
  }
  for(int i=0;i<_treeNum;i++){
    deleteTreeNode(forest[i],_catalogNum);
  }
}
