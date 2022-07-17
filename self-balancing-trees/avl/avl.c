#include <stdio.h>
#include "../../include/node.h"

int max(int a, int b) {
  return a>b ? a : b;
}

int min(int a, int b) {
  return a<b ? a : b;
}

int height(Node* root, int h) {
  printf("in height, h = %d\n", h);
  if(root->left == NULL && root->right == NULL) {
    return h;
  } else {
    if(root->left == NULL){ h = height(root->right, h);}
    else if(root->right == NULL){h = height(root->left, h);}
    else {h = max(height(root->left, h), height(root->right, h));}
    return h;
  }
}

void addNode(Node* root) {
}

int main(){
  Node* root = makeNode(1);
  makeTreeOfDepthN(root, 4);
  int h = height(root, 1);
  printf("height is %d\n", h);
  deleteTree(root);

  return 0;}
