/*CS880 final project
  File: recognizerDT.cpp
  author: Tianyi Gu
  Decision Tree digit recognizer main function
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

float getDelta(int psize,int* csize,
               classNode* parent, classNode** children,
               int _catalogNum){
  float ret=0;
  for(int i=0;i<_catalogNum;i++){
    for(int j=0;j<10;j++){
      float ph=(float)parent[j].si*(float)csize[i]/(float)psize;
      if(ph>0) ret+=pow(((float)children[i][j].si-ph),2)/ph;
    }
  }
  return ret;
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
              treeNode* def, catalog* catalogs, int _catalogNum,
              int& nodecount){
  nodecount++;
  if(examples.size()==0) return def;
  delete def;
  treeNode* n= new treeNode();
  classNode* thisclasses=getClass(examples);
  if(checkAllSame(examples)){    
    n->isleaf=1;
    n->target=examples[0].y;
    delete [] thisclasses;
    return n;
  }
  if(pixs.size()==0){    
    n->isleaf=1;
    n->target=findModeClass(thisclasses);
    delete [] thisclasses;
    return n;
  }
  int bestpixindex= findBestPixel(pixs,examples,catalogs,_catalogNum);
  n->pixel=pixs[bestpixindex];
  n->childs=new treeNode*[_catalogNum];
  classNode* childsClass[_catalogNum];
  int childssize[_catalogNum];
  for(int i=0;i<_catalogNum;i++){
    vector<digitNode> newexamples;   
    for(int j=0;j<examples.size();j++){
      if(examples[j].x[n->pixel]>=catalogs[i].gmin&&
         examples[j].x[n->pixel]<catalogs[i].gmax){
        newexamples.push_back(examples[j]);
      }
    }
    classNode* _classN=getClass(newexamples);
    childsClass[i]=_classN;
    childssize[i]=newexamples.size();
    vector<int> newpixs=pixs;    
    newpixs.erase(newpixs.begin()+bestpixindex);
    treeNode* curdef= new treeNode();
    curdef->isleaf=1;
    curdef->target=findModeClass(_classN);    
    treeNode* subtree=dtl(newexamples,newpixs,curdef,
                          catalogs,_catalogNum,nodecount);    
    n->childs[i]=subtree;    
  }
  n->delta=getDelta(examples.size(),childssize,
                    thisclasses,childsClass,_catalogNum);
  n->tmode=findModeClass(thisclasses);
  //free classnode
  delete [] thisclasses;
  for(int i=0;i<_catalogNum;i++){
    delete [] childsClass[i];
  }  
  return n;
}

void dopruning(treeNode* n,int _catalogNum){  
  for(int i=0;i<_catalogNum;i++){
    delete n->childs[i];
  }
  delete [] n->childs;
  n->isleaf=1;
  n->target=n->tmode;
}

int pruningcheck(treeNode* root,int _catalogNum,int& nodecount,
                 int pruningMargin){
  if(root->isleaf) return 0;
  int pc=0;
  for(int i=0;i<_catalogNum;i++){
    pc+=pruningcheck(root->childs[i],_catalogNum,
                     nodecount,pruningMargin);
  }
  if(pc>0) return 1;
  //cout<<"delta: "<<root->delta<<endl;
  if(root->delta<pruningMargin){
    dopruning(root,_catalogNum);
    nodecount-=_catalogNum;
    return 0;
  }
  return 1;
}

float doClassify(treeNode* root,vector<digitNode>& outdigit,
                 vector<digitNode>* mdigit,
                 catalog* catalogs, int _catalogNum){
  float missrate=0;
  int missc=0;
  //do knn for every testdigit
  for(int i=0;i<outdigit.size();i++){
    treeNode* curNode=root;
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
    outdigit[i].py=curNode->target;
    if(outdigit[i].py!=outdigit[i].y){
      missc++;
      if(mdigit){
        (*mdigit).push_back(outdigit[i]);        
      }
    }
    //cout<<"curout:"<<i<<"missc"<<missc<<endl;    
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
  bool ispruning=true;
  stringstream sp(argv[1]); 
  float _pruningMargin;
  sp>>_pruningMargin;
  
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
    //catalog* catas=new catalog[_dimension];
    int i=0;
    getline(ss, value, ',');
    stringstream v(value);
    int y;
    v>>y;
    while (getline(ss, value, ','))
    {
      stringstream v1(value);
      v1>>x[i];
      // for(int j=0;j<_catalogNum;j++){
      //   // if(x[i]>=catalogs[j].gmin&&x[i]<catalogs[i].gmax){
      //   //   catas[i]=catalogs[j];
      //   //   break;
      //   // }
      // }
      i++;
    }
    dig.x=x;
    dig.y=y;
    //dig.cata = catas;
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
  
  vector<int> pixs;
  for(int i=0;i<_dimension;i++){
    pixs.push_back(i);
  }

  //build decision tree
  int nodecount=0;
  treeNode* treeRoot=dtl(inputdigit,pixs,0,catalogs,_catalogNum,
                         nodecount);
  cout<<"alln: "<<nodecount;
  //pruning
  if(ispruning) pruningcheck(treeRoot,_catalogNum,nodecount,
                             _pruningMargin);
  cout<<" afterP: "<<nodecount<<endl;
  //doClassification
  vector<digitNode> mdigit;
  //float _mr = doClassify(treeRoot,outdigit,&mdigit,
  //                       catalogs,_catalogNum);
  float _mr = doClassify(treeRoot,outdigit,0,
                         catalogs,_catalogNum);
  stringstream sc;
  sc<<_catalogNum;
  printImage(mdigit,"c_"+sc.str());
  cout<<"finalmr: "<<_mr<<" delta: "<<_pruningMargin<<
      " nodes: "<<nodecount<<" cataNum: "<<_catalogNum<<endl;    
 
  //free memory
  for(int i=0;i<inputdigit.size();i++){
    delete [] inputdigit[i].x;
  }
  for(int i=0;i<outdigit.size();i++){
    delete [] outdigit[i].x;
  }
  deleteTreeNode(treeRoot,_catalogNum);
}
