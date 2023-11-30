#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

#include <bits/stdc++.h>

using namespace std;
struct Node;

struct node {
    Node *val;
    node *next;

    node() {
        val = 0;
        next = nullptr;
    }
};

struct Node;

bool equals(Node *n1, Node *n2);

ostream &operator<<(ostream &out, Node *&n);

class LinkedList {
protected:
    node *head;
    int size;
public:
    LinkedList() {
        head = nullptr;
        size = 0;
    }

    ~LinkedList() {
        clear();
    }

    void insert(Node *val) {
        node *temp = new node();
        temp->val = val;
        temp->next = nullptr;
        if (head == nullptr) {
            head = temp;
        } else {
            node *temp2 = head;
            while (temp2->next != nullptr) {
                temp2 = temp2->next;
            }
            temp2->next = temp;
        }
        size++;
    }

    Node *getElement(Node *&val) {
        node *temp = head;
        while (temp != nullptr) {
            if (equals(temp->val, val)) {
                return temp->val;
            }
            temp = temp->next;
        }
        return NULL;
    }

    Node **getArray() {
        Node **arr = new Node *[size];
        int counter = 0;
        node *temp = head;
        while (temp != nullptr) {
            arr[counter++] = temp->val;
            temp = temp->next;
        }
        return arr;
    }

    int linkedListSize() const {
        return size;
    };

    void clear() {
        while (head != nullptr) {
            node *temp = head;
            head = head->next;
            delete temp;
            size--;
        }
    }

    void print() const {
        node *temp = head;
        while (temp != nullptr) {
            cout << temp->val << " ";
            temp = temp->next;
        }
        cout << endl;
    }

};

struct Node {
    LinkedList adj;
    int suffNum;

    Node() {
        suffNum = -1;
    }

    Node(int num) {
        suffNum = num;
    }
};

ostream &operator<<(ostream &out, Node *&n) {
    out << n->suffNum;
    return out;
}

bool equals(Node *n1, Node *n2) {
    return (n1->suffNum == n2->suffNum);
}

class SuffixTree {
public:
    Node *root;
    int size;
    char *word;

    SuffixTree(char s[]) {
        root = new Node();
        size = strlen(s);
        word = new char[size];
        strcpy(word, s);

        insert(s);
        root->adj.print();
    }

    char *substring(int start) {
        char *substr = new char[(size - start) + 1];
        strcpy(substr, word + start);
        return substr;
    }

    void printCharArray(char arr[]) {
        for (int i = 0; i < strlen(arr); ++i) {
            cout << arr[i];
        }
    }

    void dfs(Node *node) {
        Node **arr = node->adj.getArray();
        for (int i = 0; i < node->adj.linkedListSize(); i++) {
            cout << arr[i]->suffNum << " ";
            dfs(arr[i]);
            cout << '\n';
        }
    }

    void printDfs() {
        dfs(root);
    }

    bool Search(string s) {
        Node *curr = root;
        for (int i = 0; i < s.size(); i++) {
//            bool found = 0;
            Node *n = new Node(i);
            Node *response = curr->adj.getElement(n);
            if (response != NULL) {
                curr = response;
            } else {
                return 0;
            }
//            for (int j = 0; j < curr->adj.size(); j++) {
//                if (curr->adj[j]->suffNum == i) {
//                    found = 1;
//                    curr = curr->adj[j];
//                    break;
//                }
//            }
//            if (!found)
//                return 0;
        }
        return 1;
    }

    ~SuffixTree() {
        delete root;
    }

private:
    void insert(char s[]) {
        root = new Node();
        for (int i = 0; i < size; i++) {
            Node *curr = root;

            for (int j = i; j < size; j++) {
                bool found = 0;
                Node **arr = curr->adj.getArray();
//                Node *response = curr->adj.getElement(new Node(j)); // get array
//                if (response != NULL) {
//                    curr = response;
//                }
                for (int k = 0; k < curr->adj.linkedListSize(); k++) {
                    if (s[j] == s[arr[k]->suffNum]) {
                        found = 1;
                        curr = arr[k];
                        break;
                    }
                }
                if (!found) {
                    Node *node = new Node();
                    node->suffNum = j;
                    curr->adj.insert(node);
                    curr = node;
                }
                if (j == size - 1) {
                    curr->suffNum = i;
                }
            }
        }

        // delete internal nodes that have only one child


    }
};

int main() {
    // Construct a suffix tree containing all suffixes of "bananabanaba$"

    //               0123456789012
    SuffixTree t("bananabanaba$");

    cout << t.Search("ana"); // Prints: 1 3 7
    cout << t.Search("naba"); // Prints: 4 8
    Node *n1 = new Node();
    n1->suffNum = 1;
    cout << equals(n1, new Node(1));
    cout << '\n';

    cout << n1->suffNum << " " << (new Node(1))->suffNum << " ";
    // Add test cases here.

//    t.printDfs();

    return 0;
}
//0 1 2 12
//000 1 2 3 4 5 6 7 8 9 10 11 0
//
//
//
//
//
//
//
//
//10 11 6
//
//
//
//
//10
//
//
//1 2 3 4 5 6 7 8 9 10 11 1
//
//
//
//
//
//
//
//
//6 7 8 9 10 11 3
//
//
//
//
//7
//
//
//
//
//6 7 8 9 10 11 5
//
//
//
//
//9
//
//
//11
//
//2 3 4 5 6 7 8 9 10 11 2
//
//
//
//
//
//
//
//
//6 7 8 9 10 11 4
//
//
//
//
//8
//
//
//
//
//12