//
//  AVL.cpp
//  9_lab_tester
//
//  Created by William Pinkston on 12/7/16.
//  Copyright © 2016 Xunil_programming. All rights reserved.
//

#include <stdio.h>
#include <cstdlib>
#include "AVL.h"

AVL::AVL()
{
    _Root = NULL;
}

AVL::~AVL()
{
    //if we even need it, we can recycle the code from BST::clear() in lab 6
}

NodeInterface * AVL::getRootNode()
{
    return _Root;
}

bool AVL::add(int data)
{
    bool retVal = addHelper(_Root, data);
    //
    //
    return retVal;
}

bool AVL::remove(int data)
{
    Node* remR = _Root;
    bool retVal = removeHelper(remR, data);
    updateHeights(_Root);
    
    return retVal;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void AVL::rebalance(Node* here)
{
    if (here->leftChild == NULL && here->rightChild == NULL) return;
    if (getTreeHeight(here->leftChild) - getTreeHeight(here->rightChild) > 1)
    {
        //left is too long
        //LR
        if (getTreeHeight(here->leftChild->leftChild) < getTreeHeight(here->leftChild->rightChild))
        {
            //rotate here->leftChild left
            //--//
            Node* temp = here->leftChild;
            here->leftChild = here->leftChild->rightChild;
            here->leftChild->leftChild = temp;
            //--//might not be the whole solution but its a start
        }
        //LL
        //rotate here right
        Node* here_parent = findVal(_Root, here->value);
        if (here_parent == here) /* here == _Root!!*/
        {
            _Root = here->leftChild;
            _Root->rightChild = here;
        }
        else
        {
            Node* temp = here_parent->leftChild;
            here_parent->leftChild = here_parent->leftChild->leftChild;
            here_parent->leftChild->rightChild = temp;
        }
        
    }
    if (getTreeHeight(here->leftChild) - getTreeHeight(here->rightChild) < -1)
    {
        //right is too long
        //RL
        if (getTreeHeight(here->rightChild->leftChild) > getTreeHeight(here->rightChild->rightChild))
        {
            //rotate here->rightChild right
            //--//
            Node* temp = here->rightChild;
            here->rightChild = here->rightChild->leftChild;
            here->rightChild->rightChild = temp;
            //--//
        }
        //RR
        //rotate here left
        Node* here_parent = findVal(_Root, here->value);
        if (here_parent == here) /* here == _Root!!*/
        {
            _Root = here->rightChild;
            _Root->leftChild = here;
        }
        else
        {
            Node* temp = here_parent->rightChild;
            here_parent->rightChild = here_parent->rightChild->rightChild;
            here_parent->rightChild->leftChild = temp;
        }
    }
    
    return;
}

int AVL::getTreeHeight(Node* here)
{
    if (here == NULL)
    {
        return 0;
    }
    else
    {
        return here->height;
    }
}

void AVL::updateHeights(Node* here)
{
    if (here == NULL) return;
    if (here->leftChild == NULL && here->rightChild == NULL) { here->height = 1; return; };
    updateHeights(here->leftChild);
    updateHeights(here->rightChild);
    if (getTreeHeight(here->leftChild) > getTreeHeight(here->rightChild))
    {
        if (here->height != getTreeHeight(here->leftChild) + 1) { here->height = getTreeHeight(here->leftChild) + 1; }
    }
    else
    {
        if (here->height != getTreeHeight(here->rightChild) + 1) { here->height = getTreeHeight(here->rightChild) + 1; }
    }
    return;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

bool AVL::addHelper(Node* &here, int data)
{
    //    cout << "addHelper(" << data << ")" << endl;
    if (here == NULL)
    {
        here = new Node();
        here->value = data;
        here->height = 1;
        return true;
    }
    if (here->value == data)
    {
        return false;
    }
    
    if (data < here->value)
    {
        bool retVal = addHelper(here->leftChild, data);
        if(retVal == true)
        {
            if (getTreeHeight(here->leftChild) > getTreeHeight(here->rightChild))
            {
                if (here->height != getTreeHeight(here->leftChild) + 1) { here->height = getTreeHeight(here->leftChild) + 1; }
            }
            else
            {
                if (here->height != getTreeHeight(here->rightChild) + 1) { here->height = getTreeHeight(here->rightChild) + 1; }
            }
        }
        rebalance(here);//bugtest
        return retVal;
    }
    else if (data > here->value)
    {
        bool retVal = addHelper(here->rightChild, data);
        if (retVal == true)
        {
            if (getTreeHeight(here->leftChild) > getTreeHeight(here->rightChild))
            {
                if (here->height != getTreeHeight(here->leftChild) + 1) { here->height = getTreeHeight(here->leftChild) + 1; }
            }
            else
            {
                if (here->height != getTreeHeight(here->rightChild) + 1) { here->height = getTreeHeight(here->rightChild) + 1; }
            }
        }
        rebalance(here);//bugtest plz kthxby
        return retVal;
    }
    
    else
    {
        return false;
    }
}

//-----
//-----

bool AVL::removeHelper(Node* here, int data)
{
    //    cout << "removeHelper(" << data << ")" << endl;
    Node* parentNode = findVal(here, data);
    if (parentNode == NULL) return false;
    if (data == parentNode->value && parentNode == _Root)//delete root
    {
        /////
        if (parentNode->leftChild == NULL && parentNode->rightChild == NULL)
        {
            _Root = NULL;
            delete parentNode;
        }
        else
        {
            if (parentNode->leftChild != NULL && parentNode->rightChild != NULL)
            {
                //else check for replacement on left tree
                Node* replaceRoot = findLeftGreatest(parentNode->leftChild);
                if (replaceRoot == NULL)
                {
                    //left tree empty
                    _Root = parentNode->rightChild;
                    delete parentNode;
                }
                else
                {
                    int hold = replaceRoot->value;
                    remove(hold);
                    parentNode->value = hold;
                }
            }
            else if (parentNode->leftChild == NULL)
            {
                _Root = parentNode->rightChild;
                delete parentNode;
            }
            else
            {
                _Root = parentNode->leftChild;
                delete parentNode;
            }
        }
        return true;
        
        
        /////
    }
    
    Node* temp = NULL;
    bool isLeft = false;
    if (data < parentNode->value)
    {
        temp = parentNode->leftChild;
        isLeft = true;
    }
    if (data > parentNode->value)
    {
        temp = parentNode->rightChild;
        isLeft = false;
    }
    
    
    if (temp->getLeftChild() != NULL && temp->getRightChild() != NULL)
    {
        Node* replaceVal = findLeftGreatest(temp->leftChild);
        int hold = replaceVal->value;
        remove(hold);
        temp->value = hold;
    }
    else
    {
        //use the skip link thing
        if (temp->getLeftChild() == NULL)
        {
            if (isLeft)
            {
                parentNode->leftChild = temp->rightChild;
            }
            else
            {
                parentNode->rightChild = temp->rightChild;
            }
        }
        else
        {
            if (isLeft)
            {
                parentNode->leftChild = temp->leftChild;
            }
            else
            {
                parentNode->rightChild = temp->leftChild;
            }
        }
        delete temp;
        temp = NULL;
    }
    return true;
}

AVL::Node* AVL::findVal(Node* find, int data)
{
    //    cout << "findVal(" << data << ")" << endl;
    //check the root
    if (find == NULL) return NULL;
    if (find->value == data) return find;
    
    return findValR(find, data);
}

AVL::Node* AVL::findValR(Node* find, int data)
{
    //    cout << "findValR(" << find->value << ", " << data << ")" << endl;
    //checks one node ahead, so that it can return the parent node
    if (find->leftChild == NULL && find->rightChild == NULL) return NULL;
    if (data < find->value && find->leftChild == NULL) return NULL;
    if (data < find->value)
    {
        if (find->leftChild->value == data) return find;
        return findValR(find->leftChild, data);
    }
    else
    {
        if (data > find->value && find->rightChild == NULL) return NULL;
        if (find->rightChild->value == data) return find;
        return findValR(find->rightChild, data);
    }
}

AVL::Node* AVL::findLeftGreatest(Node* remL_Child)
{
    //    cout << "findLeftGreatest(" << remL_Child->value << ")" << endl;
    if (remL_Child == NULL) return NULL;
    if (remL_Child->rightChild == NULL)
    {
        return remL_Child;
    }
    else return findLeftGreatest(remL_Child->rightChild);
}
