//
//  AVL.h
//  9_lab_tester
//
//  Created by William Pinkston on 12/7/16.
//  Copyright © 2016 Xunil_programming. All rights reserved.
//

#ifndef AVL_h
#define AVL_h

#include "AVLInterface.h"
#include "NodeInterface.h"

class AVL : public AVLInterface {
private:
    
    //
    class Node : public NodeInterface {
    public:
        Node* leftChild;
        Node* rightChild;
        int value;
        int height;//NEW
        
        Node() {
            leftChild = NULL;
            rightChild = NULL;
        }
        ~Node() {}
        int getData() {
            return value;
        }
        NodeInterface *getLeftChild() {
            return leftChild;
        }
        NodeInterface *getRightChild() {
            return rightChild;
        }
        int getHeight() {
            return height;
        }
    };
    //
    Node* _Root;
    
public:
    AVL();
    ~AVL();
    NodeInterface * getRootNode();
    bool add(int data);
    bool remove(int data);
    
    //AVL helper functions
    void rebalance(Node* here);
    void rotateLeft(Node* a);
    void rotateRight(Node* a);
    int getTreeHeight(Node* here);
    void updateHeights(Node* here);
    
    //BST helper functions
    bool addHelper(Node* &here, int data);
    bool removeHelper(Node* here, int data);
    Node* findVal(Node* find, int data);
    Node* findValR(Node* find, int data);
    Node* findLeftGreatest(Node* remL_Child);
};



#endif /* AVL_h */





