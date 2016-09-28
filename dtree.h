/*CS880 final project
  File: treeNode.h
  author: Tianyi Gu
  treeNode strcut for random forest digit recognizer  
*/  
#ifndef DTREE_H
#define DTREE_H
//struct digit
struct treeNode{
  // default constructor
  treeNode(){isleaf=0;pruningcheck=0;}  
  int pixel;
  int csize;  
  treeNode ** childs; 
  int isleaf;
  int tmode;
  int target;
  float delta;
  int pruningcheck;
  void clear(){
    delete [] childs;    
  };

};

struct catalog{
  catalog(){}  
  int gmin;
  int gmax;
  void operator=(const catalog&  _c){
    gmin=_c.gmin;
    gmax=_c.gmax;   
  };
};
#endif
