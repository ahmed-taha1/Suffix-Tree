#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

struct Node;

bool equals(Node *n1, Node *n2);

ostream &operator<<(ostream &out, Node *&n);

struct node {
    Node *val;
    node *next;

    node() {
        val = 0;
        next = nullptr;
    }
};

class LinkedList {
protected:
    node *head;
    node* tail;
    int size;
public:
    LinkedList() {
        head = tail = nullptr;
        size = 0;
    }

    ~LinkedList() {
        clear();
    }

    void insert(Node *val) {
        node* temp = new node;
        temp->val = val;
        temp->next = nullptr;
        if(head == nullptr){
            head = tail = temp;
        }
        else{
            tail->next = temp;
            tail = temp;
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
    int leafSuffNum;
    int suffNum;

    Node() {
        suffNum = -1;
        leafSuffNum = -1;
    }

    Node(int num) {
        suffNum = num;
    }

    // Recursive function to remove nodes with a single child
    void removeSingleChildNodes() {
        if (adj.linkedListSize() == 0){
            return;
        }
        if (adj.linkedListSize() == 1) {
            Node *child = adj.getArray()[0];
            adj.clear();  // Clear the adjacency list
            suffNum = child->suffNum;  // Update the current node's suffNum
            leafSuffNum = child->leafSuffNum; // Update the current node's leafSuffNum
            this->adj = child->adj;  // Move child's adjacency list to the current node
//            delete child;  // Delete the child node
            removeSingleChildNodes();  // Recursively remove from the updated current node
        } else {
            Node** arr = adj.getArray();
            for (int i = 0;i < adj.linkedListSize(); i++) {
                arr[i]->removeSingleChildNodes();  // Recursively remove from each child
            }
        }
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
//        root->adj.print();
    }

    ~SuffixTree() {
        delete root;
    }

    bool Search(char *s) {
        Node *curr = root;
        for (int i = 0; i < strlen(s); i++) {
            bool found = 0;
            Node **arr = curr->adj.getArray();
//            Node *response = curr->adj.getElement(n);
//            if (response != nullptr) {
//                curr = response;
//            } else {
//                return false;
//            }
            for (int j = 0; j < curr->adj.linkedListSize(); j++) {
                if (word[arr[j]->suffNum] == s[i]) {
                    found = true;
                    curr = arr[j];
                    break;
                }
            }
            if (!found)
                return false;
        }
        return true;
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
            if(arr[i]->leafSuffNum != -1)
                cout << arr[i]->leafSuffNum << " ";
            dfs(arr[i]);
            cout << '\n';
        }
    }

    void printDfs() {
        dfs(root);
    }

private:
    void insert(char *s) {
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
                    curr->leafSuffNum = i;
                }
            }
        }

        // delete internal nodes that have only one child

        root->removeSingleChildNodes();

    }
};

int main() {
    // Construct a suffix tree containing all suffixes of "bananabanaba$"

    //            0123456789012
    SuffixTree t("bananabanaba$");

    cout << t.Search("ana"); // Prints: 1 3 7
    cout << t.Search("naba"); // Prints: 4 8

    // Add test cases here.
//    SuffixTree t("baa$");

    t.printDfs();

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