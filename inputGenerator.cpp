#include <iostream>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
using namespace boost;
using namespace std;

boost::mt19937 gen(time(NULL));

struct Node{
    Node(int depth):value(0),left(0),right(0),operation(0){
        if(depth != 0){
            left = new Node(depth - 1);
            right = new Node(depth - 1);
            switch(gen()%4){
                case(0): operation = '+';break;
                case(1): operation = '-';break;
                case(2): operation = '*';break;
                case(3): operation = '/';break;
            }
        }
        else{
            value = gen();
        }
    }
    unsigned long long int value;
    Node* left;
    Node* right;
    char operation;
};

void printTree(Node* tree){
    if(tree){
        if(tree->left || tree->right){
            cout<<'(';
            printTree(tree->left);
            cout<<tree->operation;
            printTree(tree->right);
            cout<<')';
        }
        else{
            cout<<tree->value;
        }
    }
}

int main(){
    for(int i = 0; i<48;++i){
        Node * n = new Node(20);
        printTree(n);
        cout<<endl;
    }
}
