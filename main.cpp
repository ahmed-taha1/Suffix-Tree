#include <cstring>
#include <iostream>

using namespace std;
struct Node;

struct LinkedListNode {
    Node *val;
    LinkedListNode *next;

    LinkedListNode()
    {
        val = nullptr;
        next = nullptr;
    }
};

class LinkedList {
protected:
    LinkedListNode *head;
    LinkedListNode *tail;
    int size;
public:
    LinkedList()
    {
        head = tail = nullptr;
        size = 0;
    }
    void copyList(const LinkedList &other);

    // deep copy the adjacency list
    LinkedList &operator=(const LinkedList &other)
    {
        if (this != &other)
        {
            clear();
            copyList(other);
        }
        return *this;
    }

    void insert(Node *val)
    {
        LinkedListNode *temp = new LinkedListNode;
        temp->val = val;
        temp->next = nullptr;
        if (head == nullptr) head = tail = temp;
        else
        {
            tail->next = temp;
            tail = temp;
        }
        size++;
    }

    Node **getArray()
    {
        if (size < 0)
        {
            size = 0;
            return new Node *[0];
        }
        Node **arr = new Node *[size];
        LinkedListNode *temp = head;
        for (int i = 0; i < size; i++)
        {
            arr[i] = temp->val;
            temp = temp->next;
        }
        return arr;
    }

    int linkedListSize() const
    {
        return size;
    };

    void clear()
    {
        while (head != nullptr)
        {
            LinkedListNode *temp = head;
            head = head->next;
            delete temp;
            size--;
        }
        head = tail = nullptr;
        size = 0;
    }
};

struct Node {
    LinkedList adj;
    int suffNum;
    int leafSuffNum;

    Node()
    {
        suffNum = -1;
        leafSuffNum = -1;
    }
    // deep copy constructor for Node
    Node(const Node &other)
    {
        suffNum = other.suffNum;
        leafSuffNum = other.leafSuffNum;
        adj = other.adj; // deep copy the adjacency list
    }
};

void LinkedList::copyList(const LinkedList &other)
{
    LinkedListNode *otherTemp = other.head;
    while (otherTemp != nullptr)
    {
        insert(new Node(*(otherTemp->val))); // deep copy of Node
        otherTemp = otherTemp->next;
    }
}

class SuffixTree {
public:
    // the root
    Node *root;
    // length of the string
    int size;
    // the string
    char *word;
    char *mxLastReached;

    /**
     * Constructor.
     * @param s The word to build the tree with
     */
    SuffixTree(char s[])
    {
        // initialize root
        root = new Node();
        // initialize size
        size = strlen(s);
        // initialize word
        word = new char[size];
        strcpy(word, s);
        mxLastReached = "";
        // build the tree
        insert();
    }

    ~SuffixTree()
    {
        delete root;
    }

    /**
     * This function checks if the str is found.
     * @param str The substring.
     */
    void Search(char *str)
    {
        // updated to true if the substring is found
        bool found = 0;
        // start searching for str
        printMatching(str, root, "", found);
        // if not found after traversing the tree, print suitable message.
        if (!found) cout << "Not found";
        cout << '\n';
    }

private:
    /**
     * This method traverses the tree recursively till substring is found
     * @param str The substring
     * @param n Starting Node
     * @param last Prefix of current node
     * @param found The boolean, updated if str is found
     */
    void printMatching(char *str, Node *n, char *last, bool &found)
    {
        // if str is found in the current prefix
        if (contains(last, str))
        {
            found = 1;
            printLeavesFrom(n);
            return;
        }
        // break if node is leaf, it didn't match any of this path.
        if (n->leafSuffNum != -1) return;
        Node *curr = n;
        Node **arr = curr->adj.getArray(); // get all children of current node
        for (int i = 0; i < curr->adj.linkedListSize(); i++)
        {
            // find minimum child suffix number
            int minSuffFromChilds = getMinSuffNum(arr[i]->adj);
            // merge the prefix along the path, with the substring from current node till minimum child
            char *newLast = merge(last, substring(arr[i]->suffNum, minSuffFromChilds));
            // find the suitable child to traverse its subtree
            if (contains(newLast, str) || contains(str, newLast))
            {
                printMatching(str, arr[i], newLast, found);
                break;
            }
        }
    }

    /**
     * Recursive function to print all leaves from subtree rooted at node.
     * @param node
     */
    void printLeavesFrom(Node *node)
    {
        if (node->leafSuffNum != -1)
        {
            cout << node->leafSuffNum << " ";
            return;
        }
        Node **arr = node->adj.getArray();
        // do the recursive call on each child
        for (int i = 0; i < node->adj.linkedListSize(); i++) printLeavesFrom(arr[i]);
    }

    /**
     * @param start index
     * @return The substring from start index till the end of the string
     */
    char *substring(int start)
    {
        char *substr = new char[(size - start) + 1]; // string with required size
        strcpy(substr, word + start);
        return substr;
    }

    /**
     * @param start index
     * @param end index
     * @return The substring from start index till end index
     */
    char *substring(int start, int end)
    {
        int substringLength = end - start; // required substring size
        char *substr = new char[substringLength + 1]; // string with required size
        strncpy(substr, word + start, substringLength);
        substr[substringLength] = '\0';
        return substr;
    }

    /**
     * Checks if sub is prefix in query
     * @param query The string
     * @param sub The substring
     * @return true if sub is prefix in query, false otherwise
     */
    bool contains(char *query, char *sub)
    {
        if (strlen(sub) > strlen(query)) return false;
        for (int i = 0; i < strlen(sub); ++i)
        {
            if (query[i] != sub[i]) return false;
        }
        return true;
    }

    /**
     * Merges two strings
     * @param str1
     * @param str2
     * @return the concatenation of str1 and str2
     */
    char *merge(char *str1, char *str2)
    {
        int len1 = strlen(str1);
        int len2 = strlen(str2);
        char *result = new char[len1 + len2 + 1]; // create new string with the required length
        strcpy(result, str1);
        strcat(result, str2);
        return result;
    }

    /**
     * find the min suffix number from the adj list
     * @param adj adjacency list
     * @return min suffix number from the list
     */
    int getMinSuffNum(LinkedList adj)
    {
        Node **arr = adj.getArray();
        // initially set the ret with original word size bcs if not found that's mean this adj list related to leaf node so we will return the size of the word
        int ret = this->size;
        for (int i = 0; i < adj.linkedListSize(); i++) ret = min(arr[i]->suffNum, ret);
        return ret;
    }

    /**
     * insert the string to the tree
     */
    void insert()
    {
        root = new Node();
        for (int i = 0; i < size; i++)
        {
            // at each iteration generate suffixes from the starting i
            char *currSuffix = substring(i);
            // get the node from the tree which has the max common prefix length with the current suffix
            Node *curr = getMaxCommonPrefixLengthNode(root, "", currSuffix);
            // get the max common prefix length between max string reached from the previous call and current suffix which we want to insert
            int maxCommonPrefixLength = getMaxCommonPrefixLength(currSuffix, mxLastReached);

            // check if there is no common prefix length
            // occur only at the root
            // just insert the new node with its properties and make the root's leaf suffix num = -1 (which is at curr, got from the function call)
            if (maxCommonPrefixLength == 0)
            {
                Node *n1 = new Node();
                n1->leafSuffNum = i;
                n1->suffNum = maxCommonPrefixLength + i;
                curr->leafSuffNum = -1;
                curr->adj.insert(n1);
            } // if the node is leaf, split it into two nodes
            else if (curr->adj.linkedListSize() == 0)
            {
                // create two nodes
                Node *n1 = new Node();
                Node *n2 = new Node();
                // first one has the same leaf suffix number, second one is the one that we want to insert so it's leaf suff num will equal i
                n1->leafSuffNum = curr->leafSuffNum;
                n2->leafSuffNum = i;
                // assign the suffix numbers by adding the max common prefix length to their leaf suffix number
                n1->suffNum = n1->leafSuffNum + maxCommonPrefixLength;
                n2->suffNum = maxCommonPrefixLength + i;
                // curr now is not a leaf so make its property to -1 , add the two nodes to curr
                curr->leafSuffNum = -1;
                curr->adj.insert(n1);
                curr->adj.insert(n2);
            }
            // if the node is not leaf
            else
            {
                // if the max common prefix found is less than node string length (it's min child suffix number - it's suffix number) that's mean they aren't share the same prefix
                if (maxCommonPrefixLength < getMinSuffNum(curr->adj) - curr->suffNum)
                {
                    Node *oldSubTree = new Node();
                    oldSubTree->adj = LinkedList(curr->adj);    // copy the old subtree from the curr
                    curr->adj.clear();  // remove it from the curr
                    oldSubTree->suffNum = maxCommonPrefixLength + curr->suffNum; // make it's suffix number = max common length + curr suffix number
                    Node *n = new Node();
                    n->suffNum = i + maxCommonPrefixLength;
                    n->leafSuffNum = i;
                    // insert them to the current node so we splitted it to itself and the new node
                    curr->adj.insert(n);
                    curr->adj.insert(oldSubTree);
                }
                // if they shar the same prefix normal case, just insert the node with its leaf suffix number to the parent adj list, make its suffix number = max  prefix common length + suff number
                else
                {
                    Node *n = new Node();
                    n->leafSuffNum = i;
                    n->suffNum = maxCommonPrefixLength + i;
                    curr->adj.insert(n);
                }
            }
        }
    }

    /**
     * get the max common prefix length from the starting for two strings
     * @param str1
     * @param str2
     * @return max common prefix length between them
     */
    int getMaxCommonPrefixLength(char *str1, char *str2)
    {
        for (int i = 0; i < min(strlen(str1), strlen(str2)); ++i)
        {
            if (str1[i] != str2[i]) return i;
            // full matching
            if (i == min(strlen(str1), strlen(str2)) - 1) return min(strlen(str1), strlen(str2));
        }
        return 0;
    }

    /**
     * searching for the max common prefix length node with a specific suffix
     * @param curr initially started with root
     * @param last updatable string when we go depth at each node by adding to it , the current node substring
     * @param currSuffix the suffix that we want to search for
     * @return max common prefix length node with the current suffix
     */
    Node *getMaxCommonPrefixLengthNode(Node *curr, char *last, char *currSuffix)
    {
        if (curr->adj.linkedListSize() == 0) return curr;

        // store parent common prefix
        int parentMaxCommonPrefix = getMaxCommonPrefixLength(currSuffix, last);
        Node *foundNode = nullptr;
        int mx = parentMaxCommonPrefix;

        // find the greatest common prefix between curr children
        Node **arr = curr->adj.getArray();
        for (int i = 0; i < curr->adj.linkedListSize(); i++)
        {
            char *currentChildSuffix = merge(last, substring(arr[i]->suffNum, getMinSuffNum(arr[i]->adj)));
            int childtMaxCommonPrefix = getMaxCommonPrefixLength(currSuffix, currentChildSuffix);
            if (childtMaxCommonPrefix > mx)
            {
                mx = childtMaxCommonPrefix;
                foundNode = arr[i];
            }
        }
        // if the parent common prefix is greater than it's children return the parent
        if (parentMaxCommonPrefix == mx) return curr;
        // else if there is a child has common prefix greater than the parent
        // add to the last string this child substring that child stores and go in depth to this node
        else
        {
            char *newLast = merge(last, substring(foundNode->suffNum, getMinSuffNum(foundNode->adj)));
            // store the max reached string
            this->mxLastReached = newLast;
            // traverse depth
            return getMaxCommonPrefixLengthNode(foundNode, newLast, currSuffix);
        }
    }
};

void printDashes()
{
    cout << "--------------------" << endl;
}

int main()
{
    SuffixTree t1("bananabanaba$");
    t1.Search("ana"); // 1 3 7
    t1.Search("naba"); // 4 8
    t1.Search("kbanana$"); // NF
    printDashes();

    SuffixTree t2("AhmedTaha$");
    t2.Search("Taha"); // 5
    t2.Search("Tahb"); // NF
    t2.Search("a"); // 6 8
    printDashes();

    SuffixTree t3("YoussefMoataz$");
    t3.Search("ssef"); // 3
    t3.Search("Moataz"); // 7
    t3.Search("Motaz"); // NF
    printDashes();

    SuffixTree t4("IsmailMagdy$");
    t4.Search("a"); // 3 7
    t4.Search("Ma$"); // NF
    t4.Search("Mag"); // 6
    printDashes();

    SuffixTree t5("Hello World$");
    t5.Search(" "); // 5
    t5.Search("Hell"); // 0
    t5.Search("o"); // 4 7
    printDashes();

    SuffixTree t6("papatpaptpa$");
    t6.Search("p"); // 7 0 5 2 9
    t6.Search("pap"); // 0 5
    t6.Search("pa"); // 0 5 2 9
    printDashes();

    SuffixTree t7("papatpappa$");
    t7.Search("a"); // 1 6 3 9
    t7.Search("pap"); // 0 5
    t7.Search("apt"); // NF
    printDashes();

    SuffixTree t8("Tomorrow's lab will be cancelled and will schedule another slot with you.$");
    t8.Search("will"); // 15 37
    t8.Search("you"); // 69
    t8.Search("ll"); // 17 39 28
    printDashes();

    SuffixTree t9("AdvancedDataStructure");
    t9.Search("ll"); // NF
    t9.Search("D"); // 8
    t9.Search("c"); // 5 16
    printDashes();

    SuffixTree t10("Assignment 1 -> suffix tree");
    t10.Search("s"); // 1 2 16
    t10.Search("D"); // NF
    t10.Search("ee"); // 25
    printDashes();
}