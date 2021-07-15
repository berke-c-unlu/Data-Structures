#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include <iostream>

#include "Node.h"

template<class T>
class LinkedList {
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const T arr[], int arrSize);
    LinkedList(const LinkedList<T> &obj);

    ~LinkedList();

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;

    int getNumberOfNodes() const;
    bool isEmpty() const;
    bool contains(Node<T> *node) const;

    void insertAtTheHead(const T &data);
    void insertAtTheTail(const T &data);
    void insertSorted(const T &data);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeAllNodes();

    void print() const;

    T *toArray() const;

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    Node<T> *tail;
};

template<class T>
LinkedList<T>::LinkedList() {
    head=NULL;
    tail=NULL;
}

template<class T>
LinkedList<T>::LinkedList(const T arr[], int arrSize) {
    head=NULL;
    tail=NULL;
    int i;
    for(i=0;i<arrSize;i++){
        insertAtTheTail(arr[i]);
    }
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj) {
    head=NULL;
    tail=NULL;
    *this=obj;
}

template<class T>
LinkedList<T>::~LinkedList() {
    removeAllNodes();
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const {
    if(head==NULL){
        return nullptr;
    }
    else{
        return head;
    }
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const {
    if(head==NULL){
        return nullptr;
    }
    else{
        return tail;
    }
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const {
    Node <T> *ptr=head;
    while(ptr){
        if(data==ptr->data){
            return ptr;
        }
        else{
            ptr=ptr->next;
        }
    }
    return nullptr;
}

template<class T>
int LinkedList<T>::getNumberOfNodes() const {
    Node <T> *ptr=head;
    int count=0;
    while(ptr){
        count++;
        ptr=ptr->next;
    }
    return count;
}

template<class T>
bool LinkedList<T>::isEmpty() const {
    return head==NULL;
}

template<class T>
bool LinkedList<T>::contains(Node<T> *node) const {
    Node<T> *ptr;
    ptr=head;
    while (ptr){
        if((ptr->prev==node->prev) &&(ptr->data==node->data) &&(ptr->next==node->next)){
            return true;
        }
        ptr=ptr->next;
    }
    return false;
}

template<class T>
void LinkedList<T>::insertAtTheHead(const T &data) {
    Node<T> *new_head;
    if(head==NULL){
        new_head=new Node<T>(data,NULL,tail);
        head=new_head;
        tail=new_head;
    }
    else{
        new_head=new Node<T>(data,NULL,head);
        head->prev=new_head;
        head=new_head;
    }
}

template<class T>
void LinkedList<T>::insertAtTheTail(const T &data) {
    Node<T> *new_tail;
    if(tail==NULL){
        new_tail=new Node<T>(data,head,NULL);
        tail=new_tail;
        head=new_tail;
    }
    else{
        new_tail=new Node<T>(data,tail,NULL);
        tail->next=new_tail;
        tail=new_tail;
    }
}

template<class T>
void LinkedList<T>::insertSorted(const T &data) {
    Node<T> *tmp=head;
    Node<T> *new_node;
    if(head==NULL){
        insertAtTheHead(data);
    }
    else if(data<=head->data){
        insertAtTheHead(data);
    }
    else if(data>=tail->data){
        insertAtTheTail(data);
    }
    else{
        while(tmp){
            if(tmp->data>=data){
                break;
            }
            tmp=tmp->next;
        }
        new_node=new Node<T>;
        new_node->data=data;
        new_node->next=tmp;
        new_node->prev=tmp->prev;
        tmp->prev->next=new_node;
        tmp->prev=new_node;
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node) {
    Node<T> *temp;
    if(head!=NULL){
        temp=head;
        if((getNumberOfNodes()==1) &&  (node->data == head->data)){
            head=nullptr;
            tail=nullptr;
            delete node;
        }
        else if(node->data==head->data&&node->prev==NULL && node->next==head->next){
            head->next->prev=NULL;
            head=head->next;
            delete node;
        }
        else if(node->data==tail->data && node->next == NULL && node->prev==tail->prev){
            tail->prev->next=NULL;
            tail=tail->prev;
            delete node;
        }
        else{
            while(temp){
                bool prevEq=temp->prev==node->prev;
                bool dataEq=temp->data==node->data;
                bool nextEq=temp->next==node->next;
                if(prevEq && dataEq && nextEq){
                    temp->prev->next=temp->next;
                    temp->next->prev=temp->prev;
                    delete node;
                    break;
                }
                temp=temp->next;
            }
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data) {
    Node<T> *ptr;
    Node<T> *node;
    if(head!=NULL){
        ptr=head;
        if((getNumberOfNodes()==1) &&(data == head->data)){
            node=head;
            head=nullptr;
            tail=nullptr;
            delete node;
        }
        else if(head->data==data){
            node=head;
            head->next->prev=NULL;
            head=head->next;
            delete node;
        }
        else if(tail->data==data){
            node=tail;
            tail->prev->next=NULL;
            tail=tail->prev;
            delete node;
        }

        else{
            while(ptr){
                if(ptr->data==data){
                    node=ptr;
                    ptr->prev->next=ptr->next;
                    ptr->next->prev=ptr->prev;
                    delete node;
                    break;
                }
                ptr=ptr->next;
            }
        }
    }
}

template<class T>
void LinkedList<T>::removeAllNodes() {
    Node<T> *prev=head;
    Node<T> *curr=head;
    while(curr){
        curr=curr->next;
        delete prev;
        prev=curr;
    }
    head=nullptr;
    tail=nullptr;
}

template<class T>
void LinkedList<T>::print() const {
    std::cout << "Printing the linked list ..." << std::endl;

    if (this->isEmpty()) {
        std::cout << "empty" << std::endl;
        return;
    }

    Node<T> *node = this->head;

    while (node) {
        std::cout << *node << std::endl;
        node = node->next;
    }
}

template<class T>
T *LinkedList<T>::toArray() const {
    T *arr= new T[getNumberOfNodes()];
    Node<T> *ptr=head;
    int i=0;
    while (ptr){
        arr[i++]=ptr->data;
        ptr=ptr->next;
    }
    return arr;
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs) {
    if(this != &rhs){
        removeAllNodes();
        Node<T> *tmp;
        if(rhs.head==NULL){
            this->head=nullptr;
            this->tail=nullptr;
        }
        else{
            tmp=rhs.head;
            while(tmp){
                this->insertAtTheTail(tmp->data);
                tmp=tmp->next;
            }
        }
    }
    return *this;
}


#endif //LINKEDLIST_H