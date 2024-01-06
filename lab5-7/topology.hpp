#include <vector>
#include <cstdio>
#include <stdlib.h>

using namespace std;

struct Node{
    int id;
    bool is_available;
    vector<Node *> _children;
};

Node* create_node(int id){
    Node *new_node = new(Node);
    new_node->id = id;
    new_node->_children = {};
    new_node->is_available = true;
    return new_node;
}

void add_node(Node* prev, int id){
    if ((prev->id != -1) && (prev->_children.size() != 0)){
        perror("Error: can't put new node in the middle of the list!");
        exit(-1);
    }
    else{
        Node* new_node = create_node(id);
        prev->_children.push_back(new_node);
    }
}

Node* search(Node* root, int id){
    if (root->id == id){
        return root;
    }

    for(size_t i = 0; i < root->_children.size(); ++i){
        Node *result = search(root->_children[i], id);
        if (result != nullptr) {
            return result; 
        }
    }

    return nullptr; 
}

void stop_node(Node* node){
    node->is_available = false;
}

void delete_topology(Node* root){
    for(size_t i = 0; i < root->_children.size(); ++i){
        delete_topology(root->_children[i]);
    }
    
    delete root;
}


bool exist(Node* root, int id){
    if(search(root, id) != nullptr){
        return true;
    }
    return false;
}