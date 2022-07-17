#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHILDREN 2 
#define RAND_MAX 27

typedef struct Node {
  size_t val;
  size_t child_count;
  struct Node* parent;
  struct Node** children;
  struct Node* left;
  struct Node* right;
} Node;

void random_init() {
  srand(time(NULL));
  return;
}

Node* makeNode(int n) {
  //printf("makenode calloc\n");
  Node* new_node = calloc(1, sizeof(Node));
    new_node->child_count = 0;
    new_node->val = n;
    new_node->parent = NULL;
    //printf("made node %zu.\n", new_node->val);
    return new_node;
}

void addChildNode(Node* parent, size_t index) {
 //printf("adding child node to %zu\n", parent->val);

  if(parent->child_count == MAX_CHILDREN) {
   //printf("%zu has max children.\n", parent->val);
   return;
  }

  if(index >= MAX_CHILDREN) {
    //printf("requested index is out of bounds\n");
    return;
  }

// val of 0 is used to indicate a vacant index in the child array
// should probably use a string instead.

  if(parent->child_count > 0 && parent->children[index]->val > 0){
    //printf("%zu already has a child at index %zu!\n", parent->val, index);
    return;
  } 

  // once we want a node to have a child, allocate for all children at once.
  
  // could probably cut out some redundant space using some kind of hash 
  // table to only allocate at the specified index each time. Not sure
  // how to iterate that when deleting though.

  if(parent->child_count == 0) {
  //printf("make children list of %zu calloc\n", parent->val);
  parent->children = calloc(MAX_CHILDREN, sizeof(Node*));
    for(int i = 0; i < MAX_CHILDREN; i++) {
      //printf("make child %d of %zu calloc\n", i, parent->val);
      parent->children[i] = calloc(1, sizeof(Node));
      parent->children[i]->val = 0;
      parent->children[i]->child_count = 0;
      parent->children[i]->parent = parent;
      //printf("child %d allocated for %zu\n", i, parent->val);
    }  
    parent->left = parent->children[0];
    parent->right = parent->children[1];
  }
  Node* child = parent->children[index];
  parent->child_count++;
  child->parent = parent;
  if(parent->val == 1) {
  // This assumes MAX_CHILDREN is < 10
  // TODO make a better labeling scheme
    child->val = 20 + index;
  } else {
     child->val = child->parent->val + 10 + index;
  }
  //printf("%zu is %zu's parent\n", parent->val, child->val);
}

void makeTreeOfDepthN(Node* root, size_t n) {
  size_t currentDepth = (root->val - root->val%10) /10;
  size_t index_to_go_next = 0;
  printf("currentDepth = %zu\n", currentDepth);
  printf("n = %zu\n", n);
  if(n <= 1){return;}
  if(currentDepth <= n) {
    size_t number_of_nodes_to_try_adding = rand() % MAX_CHILDREN + 1;
    //printf("try to add %zu nodes \n", number_of_nodes_to_try_adding);
    for(int i = 0; i < number_of_nodes_to_try_adding; i++) {
      size_t insertion_index = rand() % MAX_CHILDREN;
      //printf("insert at index %zu\n", insertion_index); 
      addChildNode(root, insertion_index);
      size_t index_to_go_next = insertion_index;
    }
    currentDepth++;

    makeTreeOfDepthN(root->children[index_to_go_next], n-1);
  }
  return;
}

// TODO build a random tree with a given # of leaves


void deleteTree(Node* root) {
  //printf("deleting from %zu, which has %zu children\n",
  //    root->val, root->child_count);

  while(root->child_count > 0) {
    for(int i = 0; i < MAX_CHILDREN; i++ ){
      //printf("checking index %d\n", i);
      if(root->children[i]->val > 0) {
        //printf("go to %zu\n", root->children[i]->val);
        deleteTree(root->children[i]);
      } else {
        free(root->children[i]);
      }
    }
  }
  //printf("free %zu's children allocation\n", root->val);
  free(root->children);
  if(root->val > 1){
    //printf("update %zu's child count\n", root->parent->val);
    root->parent->child_count--;
  }
  //printf("freeing %zu.\n", root->val);
  free(root);
  return;
} 

size_t countLeaves(Node* root, size_t leaf_count){
  if(root->child_count == 0){
    //printf("arrived at leaf %zu\n", root->val);
    return 1;
} else {
    for(int i = 0; i < MAX_CHILDREN; i++) {
      if(root->children[i]->val > 0){
        //printf("%d : count %zu 's leaves\n", i, root->children[i]->val);
        leaf_count += countLeaves(root->children[i], leaf_count);
     }
    }
    return leaf_count;
  }
}

// by branch do we mean nodes with >= 2 children? assume this definition
size_t countBranches(Node* root, size_t branch_count){
  //printf("check if %zu is branching node\n", root->val);
  if(root->child_count >= 2){branch_count++;}
  if(root->child_count > 0){
    for(int i = 0; i < MAX_CHILDREN; i++){
      if(root->children[i]->val > 0){
        branch_count = countBranches(root->children[i], branch_count);
      }
    }
  }
  return branch_count;
}

size_t getChildMaxOfAllBranches(Node* root, size_t current_max){
  if(root->child_count == 0){
    return current_max;
  } else {
    if(root->child_count > current_max){current_max = root->child_count;}
    for(int i = 0; i < MAX_CHILDREN; i++){
      if(root->children[i]->val > 0){
        getChildMaxOfAllBranches(root->children[i], current_max);    
      }
    }
    return current_max;
  }
}
