#include <gmpxx.h>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

vector<string*> v;
bool quiet = 0;
short tasks = 1;

struct Node{
    Node():value(0),parent(0),left(0),right(0),operation(0){}
    mpz_class value;
    Node* parent;
    Node* left;
    Node* right;
    char operation;
};

Node* parse(const char* expr, int size){
    int currentDigit = 0;
    Node* currentNode = new Node;
    while(currentDigit < size){
        switch(expr[currentDigit]){
            case(48):case(49):case(50):case(51):case(52):case(53):case(54):
            case(55):case(56):case(57):
                currentNode->value *= 10;
                currentNode->value += (expr[currentDigit]-48);
                break;
            case('+'):case('-'):case('/'):case('*'):
                currentNode = currentNode->parent;
                currentNode->operation = expr[currentDigit];
                currentNode->right = new Node;
                currentNode->right->parent = currentNode;
                currentNode = currentNode->right;
                break;
            case(')'):
                currentNode = currentNode->parent;
                switch(currentNode->operation){
                    case('+'):
                        currentNode->value = currentNode->left->value +
                            currentNode->right->value;
                        break;
                    case('-'):
                        currentNode->value = currentNode->left->value -
                            currentNode->right->value;
                        break;
                    case('*'):
                        currentNode->value = currentNode->left->value *
                            currentNode->right->value;
                        break;
                    case('/'):
                        if (currentNode->right->value == 0)
                            currentNode->right->value = 1;
                        currentNode->value = currentNode->left->value /
                            currentNode->right->value;
                        break;
                }
                break;
            case('('):
            {
                Node* node = new Node;
                node->parent = currentNode;
                currentNode->left = node;
                currentNode = node;
                break;
            }
            default:
                abort();
        }
        ++currentDigit;
    }
    return currentNode;
}

void solve(int start){
    for(int i = start; i < v.size(); i += tasks){
        if(!quiet)cout<<"Solving problem "<<i<<"\n";
        Node* s = parse(v[i]->data(), v[i]->size());
        if(!quiet)cout<<"Solution to problem: "<<i<<" is "<<s->value<<endl;
    }
}

int  main(int argc, char** argv)
{
    cin.sync_with_stdio(false); //magic happens here
    time_t start = time(NULL);
    int d;
    static struct option long_options[] =
    {
        {"quiet",  no_argument,       0, 'q'},
        {"file",   required_argument, 0, 'f'},
        {"output", required_argument, 0, 'o'},
        {"tasks",  required_argument, 0, 't'},
        {0,0,0,0}
    };
    while(1){
        int option_index = 0;
        d = getopt_long(argc,argv,"qf:o:t:", long_options, &option_index);
        if(d == -1) break;
        switch(d)
        {
            case 'q':
                quiet=1;
                break;
            case 'f':
                close(0);
                open(optarg, O_RDONLY);
                break;
            case 'o':
                close(1);
                open(optarg, O_CREAT|O_WRONLY|O_TRUNC, 0666);
                break;
            case 't':
                tasks = atoi(optarg);
                break;
            case '?':
                break;
            default:
                abort();
        }
    }
    if(!quiet)cout<<"Reading from input\n";
    string* lineHolder = new string;
    while(getline(cin, *lineHolder)){
        v.push_back(lineHolder);
        lineHolder = new string;
    }
    if(!quiet)cout<<"Input read, solving now..."<<endl;
    for(int i = 0; i < tasks;++i){
        if(fork()){
            continue;
        }
        else{
            solve(i);
            exit(0);
        }
    }
    pid_t status;
    for(int i = 0; i < tasks; ++i)
        wait(&status);
    cout<<"Solution ready, time: "<<time(NULL) - start<<endl;
    return 0;
}
