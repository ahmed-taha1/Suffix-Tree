#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

struct Node;

bool equals(Node *n1, Node *n2);

ostream &operator<<(ostream &out, Node *&n);

struct LinkedListNode {
    Node *val;
    LinkedListNode *next;

    LinkedListNode() {
        val = 0;
        next = nullptr;
    }
};

class LinkedList {
protected:
    LinkedListNode *head;
    LinkedListNode *tail;
    int size;
public:
    LinkedList() {
        head = tail = nullptr;
        size = 0;
    }

//    ~LinkedList() {
//        clear();
//    }

    void insert(Node *val) {
        LinkedListNode *temp = new LinkedListNode;
        temp->val = val;
        temp->next = nullptr;
        if (head == nullptr) {
            head = tail = temp;
        } else {
            tail->next = temp;
            tail = temp;
        }
        size++;
    }

    Node *getElement(Node *&val) {
        LinkedListNode *temp = head;
        while (temp != nullptr) {
            if (equals(temp->val, val)) {
                return temp->val;
            }
            temp = temp->next;
        }
        return NULL;
    }

    Node **getArray() {
        if (size < 0) {
            size = 0;
            return new Node *[0];
        }
        Node **arr = new Node *[size];
        LinkedListNode *temp = head;
        for (int i = 0; i < size; i++) {
            arr[i] = temp->val;
            temp = temp->next;
        }
        return arr;
    }

    int linkedListSize() const {
        return size;
    };

    void clear() {
        while (head != nullptr) {
            LinkedListNode *temp = head;
            head = head->next;
//            delete temp;
            size--;
        }
        head = tail = nullptr;
        size = 0;
    }

    void print() const {
        LinkedListNode *temp = head;
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

    void removeSingleChildNodes() {
        if (adj.linkedListSize() == 0) {
            return;
        }
        if (adj.linkedListSize() == 1) {
            Node *child = adj.getArray()[0];
            adj.clear();
            leafSuffNum = child->leafSuffNum;
            this->adj = child->adj;
            delete child;
            removeSingleChildNodes();
        } else {
            Node **arr = adj.getArray();
            for (int i = 0; i < adj.linkedListSize(); i++) {
                arr[i]->removeSingleChildNodes();
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
    }

    ~SuffixTree() {
        delete root;
    }


    void Search(char *str) {
        bool found = 0;
        printMatching(str, root, "", found);
        if (!found)
            cout << "Not found";
    }

    void printMatching(char *str, Node *n, char *last, bool &found) {
        if (contains(last, str)) {
            found = 1;
            printLeavesFrom(n);
            return;
        }
        if (n->leafSuffNum != -1) {
            return;
        }
        Node *curr = n;
        Node **arr = curr->adj.getArray();
        for (int i = 0; i < curr->adj.linkedListSize(); i++) {
            int minSuffFromChilds = getMinSuffNum(arr[i]->adj); // newlast + substring(suffNum, minSuffFromChilds)
            char *newLast = merge(last, substring(arr[i]->suffNum, minSuffFromChilds));
            if (contains(newLast, str) || contains(str, newLast)) {
                printMatching(str, arr[i], newLast, found);
                break;
            }
        }
    }

    char *substring(int start) {
        char *substr = new char[(size - start) + 1];
        strcpy(substr, word + start);
        return substr;
    }

    char *substring(int start, int end) {
        int substringLength = end - start;
        char *substr = new char[substringLength + 1];
        strncpy(substr, word + start, substringLength);
        substr[substringLength] = '\0';
        return substr;
    }

    bool contains(char *query, char *sub) {
        if (strlen(sub) > strlen(query))
            return false;
        for (int i = 0; i < strlen(sub); ++i) {
            if (query[i] != sub[i]) {
                return false;
            }
        }
        return true;
    }

    char *merge(char *str1, char *str2) {
        int len1 = strlen(str1);
        int len2 = strlen(str2);

        char *result = new char[len1 + len2 + 1];

        strcpy(result, str1);
        strcat(result, str2);

        return result;
    }

    int getMaxCommonPrefixLength(char *str1, char *str2) {
        for (int i = 0; i < min(strlen(str1), strlen(str2)); ++i) {
            if (str1[i] != str2[i]) {
                return i;
            }
        }
        return 0;
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
//            cout << "size: " << arr[i]->adj.linkedListSize() << " ";
            if (arr[i]->leafSuffNum != -1)
                cout << arr[i]->leafSuffNum << " ";
            dfs(arr[i]);
            cout << '\n';
        }
    }

    void printDfs() {
        dfs(root);
    }

    void printLeavesFrom(Node *node) {
        if (node->leafSuffNum != -1) {
            cout << node->leafSuffNum << " ";
            return;
        }

        Node **arr = node->adj.getArray();
        for (int i = 0; i < node->adj.linkedListSize(); i++) {
            printLeavesFrom(arr[i]);
        }
    }

private:
    int getMinSuffNum(LinkedList adj) {
        Node **arr = adj.getArray();
        int ret = this->size;
        for (int i = 0; i < adj.linkedListSize(); i++) {
            ret = min(arr[i]->suffNum, ret);
        }
        return ret;
    }

    static bool isSubstrExist(const char *s1, char *s2) {
        if (strlen(s2) > strlen(s1))
            return false;
        for (int i = 0; i < strlen(s2); i++) {
            if (s1[i] != s2[i])
                return false;
        }
        return true;
    }

    void insert(char *s) {
        root = new Node();
        for (int i = 0; i < size; i++) {
            char *currSuffix = substring(i);
            Node **arr = root->adj.getArray();
            bool found = false;
            for (int j = 0; j < root->adj.linkedListSize(); j++) {
                char *currBranchSuffix = substring(arr[i]->suffNum);
                int maxCommonPrefixLength = getMaxCommonPrefixLength(currSuffix, currBranchSuffix);
                if(maxCommonPrefixLength != 0){
                    found = true;
//                    int oldLeafSuffixNum =
                    //*********//
                    break;
                }
            }
//            if (!found) {
//                Node *node = new Node();
//                node->suffNum = j;
//                curr->adj.insert(node);
//                curr = node;
//            }
        }
    }

//    void insert(char *s) {
//        root = new Node();
//        for (int i = 0; i < size; i++) {
//            Node *curr = root;
//
//            for (int j = i; j < size; j++) {
//                bool found = 0;
//                Node **arr = curr->adj.getArray();
//                for (int k = 0; k < curr->adj.linkedListSize(); k++) {
//                    if (s[j] == s[arr[k]->suffNum]) {
//                        found = 1;
//                        curr = arr[k];
//                        break;
//                    }
//                }
//                if (!found) {
//                    Node *node = new Node();
//                    node->suffNum = j;
//                    curr->adj.insert(node);
//                    curr = node;
//                }
//
//                if (j == size - 1) {
//                    curr->leafSuffNum = i;
//                }
//            }
//        }
//
//        // delete internal nodes that have only one child
//
//        root->removeSingleChildNodes();
//
//    }
};

void printDashes() {
    cout << "--------------------" << endl;
}

int main() {
    // Construct a suffix tree containing all suffixes of "bananabanaba$"

    //            0123456789012
//    SuffixTree t("bananabanaba$");
//
//    t.Search("ana"); // Prints: 1 3 7
//    cout << '\n';
//    t.Search("naba"); // Prints: 4 8
//
//    // Add test cases here.
//    cout << "\n";
//    t.Search("kbanana$"); // NF
//    cout << '\n';
//    printDashes();
//
//    SuffixTree t2("AhmedTaha$");
//    t2.Search("Taha"); // 5
//    cout << '\n';
//    t2.Search("Tahb"); // NF
//    cout << '\n';
//    t2.Search("a"); // 6 8
//    cout << '\n';
//    printDashes();
//
//    SuffixTree t3("YoussefMoataz$");
//    t3.Search("ssef"); // 3
//    cout << '\n';
//    t3.Search("Moataz"); // 7
//    cout << '\n';
//    t3.Search("Motaz"); // Nf
//    cout << '\n';
//    printDashes();

//    t.printDfs();

    return 0;
}