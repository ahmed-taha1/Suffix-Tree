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

    // deep copy the adjacency list
    LinkedList &operator=(const LinkedList &other) {
        if (this != &other) {
            clear();
            copyList(other);
        }
        return *this;
    }

    void copyList(const LinkedList &other);

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

    // deep copy constructor for Node
    Node(const Node &other) {
        suffNum = other.suffNum;
        leafSuffNum = other.leafSuffNum;

        // deep copy the adjacency list
        adj = other.adj;
    }
};

void LinkedList::copyList(const LinkedList &other) {
    LinkedListNode *otherTemp = other.head;
    while (otherTemp != nullptr) {
        insert(new Node(*(otherTemp->val))); // Creating a deep copy of Node
        otherTemp = otherTemp->next;
    }
}

// overload output operator to print suffix number from node
ostream &operator<<(ostream &out, Node *&n) {
    out << n->suffNum;
    return out;
}

/**
 * @param n1 Node 1
 * @param n2 Node 2
 * @return true if the suffix numbers matched, false otherwise
 */
bool equals(Node *n1, Node *n2) {
    return (n1->suffNum == n2->suffNum);
}

class SuffixTree {
public:
    // the start of the tree
    Node *root;
    // length of the original string
    int size;
    // the original string
    char *word;
    char *mxLastReached;

    /**
     * Suffix tree constructor.
     * @param s The word to build the tree with
     */
    SuffixTree(char s[]) {
        // initialize root
        root = new Node();
        // initialize size
        size = strlen(s);
        // initialize word
        word = new char[size];
        strcpy(word, s);
        mxLastReached = "";
        // build the tree
        insert(s);
    }

    ~SuffixTree() {
        delete root;
    }

    /**
     * This function checks if the substring is present in the string or not, prints all suffix start positions (tree leaves) for it.
     * @param str The substring.
     */
    void Search(char *str) {
        // updated to true if the substring is found
        bool found = 0;
        // start searching for str
        printMatching(str, root, "", found);
        // if not found after traversing the tree, print suitable message.
        if (!found) {
            cout << "Not found";
        }
    }

    /**
     * This method traverses the tree recursively till substring is found, stops at leaves
     * @param str The substring to find
     * @param n Starting Node
     * @param last The prefix of current node
     * @param found The boolean to be updated if str is found
     */
    void printMatching(char *str, Node *n, char *last, bool &found) {
        // if str is found in the current prefix print all suffix start positions (tree leaves) suffix numbers for furrent node
        if (contains(last, str)) {
            found = 1;
            printLeavesFrom(n);
            return;
        }
        // break if node is leaf, it didn't math any of this path.
        if (n->leafSuffNum != -1) {
            return;
        }
        Node *curr = n;
        // get all children of current node
        Node **arr = curr->adj.getArray();
        for (int i = 0; i < curr->adj.linkedListSize(); i++) {
            // find minimum child suffix number
            int minSuffFromChilds = getMinSuffNum(arr[i]->adj); // newlast + substring(suffNum, minSuffFromChilds)
            // merge the prefix along the path, with the substring from current node till minimum child
            char *newLast = merge(last, substring(arr[i]->suffNum, minSuffFromChilds));
            // find the suitable child to traverse its subtree
            if (contains(newLast, str) || contains(str, newLast)) {
                printMatching(str, arr[i], newLast, found);
                break;
            }
        }
    }

    /**
     * Recursive function to print all leaves from subtree rooted at node.
     * @param node
     */
    void printLeavesFrom(Node *node) {
        if (node->leafSuffNum != -1) {
            cout << node->leafSuffNum << " ";
            return;
        }
        // loop over the children of node and do the recursive call on each child
        Node **arr = node->adj.getArray();
        for (int i = 0; i < node->adj.linkedListSize(); i++) {
            printLeavesFrom(arr[i]);
        }
    }

    /**
     * @param start index in the word
     * @return The substring from start index till the end of the string
     */
    char *substring(int start) {
        // create string with required substring size
        char *substr = new char[(size - start) + 1];
        strcpy(substr, word + start);
        return substr;
    }

    /**
     * @param start index in the word
     * @param end index in the word
     * @return The substring from start index till end index
     */
    char *substring(int start, int end) {
        // calculate required substring size
        int substringLength = end - start;
        // create string with required substring size
        char *substr = new char[substringLength + 1];
        strncpy(substr, word + start, substringLength);
        substr[substringLength] = '\0';
        return substr;
    }

    /**
     * Checks if substring is prefix in string
     * @param query The string
     * @param sub The substring
     * @return true if sub is prefix in query, false otherwise
     */
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

    /**
     * Merges two strings
     * @param str1
     * @param str2
     * @return the concatenation of str1 and str2
     */
    char *merge(char *str1, char *str2) {
        int len1 = strlen(str1);
        int len2 = strlen(str2);
        // create new string with the required length
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
            if (i == min(strlen(str1), strlen(str2)) - 1) {
                return min(strlen(str1), strlen(str2));
            }
        }
        return 0;
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

            Node *curr = getMaxCommonPrefixLengthNode(root, "", currSuffix);
            int maxCommonPrefixLength = getMaxCommonPrefixLength(currSuffix, mxLastReached);

            // if the node is leaf
            if (maxCommonPrefixLength == 0) {
                Node *n1 = new Node();
                n1->leafSuffNum = i;
                n1->suffNum = maxCommonPrefixLength + i;
                curr->leafSuffNum = -1;
                curr->adj.insert(n1);

            } else if (curr->adj.linkedListSize() == 0) {
                Node *n1 = new Node();
                Node *n2 = new Node();
                n1->leafSuffNum = curr->leafSuffNum;
                n2->leafSuffNum = i;

                n1->suffNum = n1->leafSuffNum + maxCommonPrefixLength;
                n2->suffNum = maxCommonPrefixLength + i;

                curr->leafSuffNum = -1;
                curr->adj.insert(n1);
                curr->adj.insert(n2);
            } else {
                if (maxCommonPrefixLength <
                    getMinSuffNum(curr->adj) - curr->suffNum) {   // TODO check if it gives negative values
                    Node *oldSubTree = new Node();
                    oldSubTree->adj = LinkedList(curr->adj);
                    curr->adj.clear();
                    oldSubTree->suffNum = maxCommonPrefixLength + curr->suffNum;

                    // mabye deleted
                    curr->leafSuffNum = -1;
                    oldSubTree->leafSuffNum = -1;

                    Node *n = new Node();
                    n->suffNum = i + maxCommonPrefixLength;
                    n->leafSuffNum = i;

                    curr->adj.insert(n);
                    curr->adj.insert(oldSubTree);
                } else {
                    Node *n = new Node();
                    n->leafSuffNum = i;
                    n->suffNum = maxCommonPrefixLength + i;
                    curr->adj.insert(n);
                }
            }
        }
    }

    Node *getMaxCommonPrefixLengthNode(Node *curr, char *last, char *currSuffix) {
        if (curr->adj.linkedListSize() == 0) {
            return curr;
        }

        int parentMaxCommonPrefix = getMaxCommonPrefixLength(currSuffix, last);
        Node *foundNode = nullptr;
        int mx = parentMaxCommonPrefix;

        Node **arr = curr->adj.getArray();
        for (int i = 0; i < curr->adj.linkedListSize(); i++) {
            char *currentChildSuffix = merge(last, substring(arr[i]->suffNum, getMinSuffNum(arr[i]->adj)));
            int childtMaxCommonPrefix = getMaxCommonPrefixLength(currSuffix, currentChildSuffix);
            if (childtMaxCommonPrefix > mx) {
                mx = childtMaxCommonPrefix;
                foundNode = arr[i];
            }
        }
        if (parentMaxCommonPrefix == mx) {
            return curr;
        } else {
            char *newLast = merge(last, substring(foundNode->suffNum, getMinSuffNum(foundNode->adj)));
            this->mxLastReached = newLast;
            return getMaxCommonPrefixLengthNode(foundNode, newLast, currSuffix);
        }
    }

};

void printDashes() {
    cout << "--------------------" << endl;
}

int main() {
    // Construct a suffix tree containing all suffixes of "bananabanaba$"

    //            0123456789012
    SuffixTree t("bananabanaba$");
    t.Search("ana"); // Prints: 1 3 7
    cout << '\n';
    t.Search("naba"); // Prints: 4 8

    // Add test cases here.
    cout << "\n";
    t.Search("kbanana$"); // NF
    cout << '\n';
    printDashes();

    SuffixTree t2("AhmedTaha$");
    t2.Search("Taha"); // 5
    cout << '\n';
    t2.Search("Tahb"); // NF
    cout << '\n';
    t2.Search("a"); // 6 8
    cout << '\n';
    printDashes();

    SuffixTree t3("YoussefMoataz$");
    t3.Search("ssef"); // 3
    cout << '\n';
    t3.Search("Moataz"); // 7
    cout << '\n';
    t3.Search("Motaz"); // NF
    cout << '\n';
    printDashes();

//    t.printDfs();

    return 0;
}