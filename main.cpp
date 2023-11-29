#include <bits/stdc++.h>
using namespace std;


template<typename T>
class Vector{
    private:
        int currentElements;
        int capacity;
        T* arr;

    public:
        Vector<T>(){
            this->currentElements = 0;
            this->capacity = 1;
            arr = new T[capacity];
        }

        int size(){
            return currentElements;
        }

        void push_back(T t){
            if(capacity == currentElements){
                capacity *= 2;
                T* temp = new T[capacity];
                for(int i = 0; i < currentElements; i++)
                    temp[i] = arr[i];
            }
            arr[currentElements] = t;
            currentElements++;
        }

        T operator[](int n){
            return arr[n];
        }
};

struct Edge;


struct Node{
    Vector<Edge> adj;
    int suffNum;
    Node(){
        suffNum = -1;
    }
};

struct  Edge{
    Node* node;
    char val;
    Edge(){
        node = nullptr;
    }
};

class SuffixTrie{
    public:
        Node* root;
        SuffixTrie(string s){
            root = nullptr;
            insert(s);
        }

        void printBfs(){
            queue<Node*> q;
            q.push(root);
            while(!q.empty()){
                Node* cur = q.front();
                q.pop();
                for(int i = 0; i < cur->adj.size(); i++){
                    cout << cur->adj[i].val << " ";
                    q.push(cur->adj[i].node);
                }
                cout << "\n";
            }
        }

        void dfs(Node* node){
            for(int i = 0; i < node->adj.size(); i++){
                cout << node->adj[i].val;
                dfs(node->adj[i].node);
                cout << '\n';
            }
        }

        void printDfs(){
            dfs(root);
        }

        bool search(string s){
            Node* curr = root;
            for(int i = 0; i < s.size(); i++){
                bool found = 0;
                for(int j = 0; j < curr->adj.size(); j++){
                    if(curr->adj[j].val == s[i]){
                        found = 1;
                        curr = curr->adj[j].node;
                        break;
                    }
                }
                if(!found)
                    return 0;
            }
            return 1;
        }
        ~SuffixTrie(){
            delete root;
        }

    private:
        void insert(string s){
            s += "$";
            root = new Node();
            for(int i = 0; i < s.size(); i++){
                Node* curr = root;

                for(int j = i; j < s.size(); j++){
                    bool found = 0;
                    for(int k = 0; k < curr->adj.size(); k++){
                        if(s[j] == curr->adj[k].val){
                            found = 1;
                            curr = curr->adj[k].node;
                            break;
                        }
                    }
                    if(!found){
                        Edge edge;
                        edge.val = s[j];
                        edge.node = new Node();
                        curr->adj.push_back(edge);
                        curr = edge.node;
                    }
                    if(j == s.size() - 1){
                        curr->suffNum = i;
                    }
                }
            }
        }

};

int main(){
    string s = "banana";
    SuffixTrie t(s);
    // t.printBfs();
    cout << t.search("ana");
}