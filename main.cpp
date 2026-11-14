/*
 *  Author: Morar Dragoș Bogdan
 *  Group: 30424
 *  Lab: 7
 *  Subject: Multiway trees
 *      Traversal:
 *  Inorder on a binary tree built recursively from a sorted array.
 *  Recursive version: O(n) time
 *  Iterative version: O(n) time O(1) extra memory
 *      Transformations:
 *  R1 pretty print: for each node it scans the whole parent vector to find
 *  children -> O(n^2), but needs no extra memory (simple, different from R2/R3).
 *  T1 (R1 -> R2): O(n) time. Pass 1 counts children per parent, then each
 *  node gets a child array of exactly that size, pass 2 fills them in.
 *  Extra memory: O(n) (the node array, the child arrays, a fill counter).
 *  T2 (R2 -> R3): O(n) time, each node is visited once. first child -> left,
 *  next sibling -> right. Extra memory: the new O(n) tree + recursion stack O(h).
 *  All three pretty prints produce the same output.
 */
#include <iostream>

struct BinaryNode {
    int key;
    BinaryNode *left, *right;
};

BinaryNode* newBinaryNode(int key) {
    return new BinaryNode{key, nullptr, nullptr};
}

BinaryNode* buildBalanced(int* sortedValues, int low, int high) {
    if (low > high) return nullptr;
    int middle = (low + high) / 2;
    BinaryNode* node = newBinaryNode(sortedValues[middle]);
    node->left = buildBalanced(sortedValues, low, middle - 1);
    node->right = buildBalanced(sortedValues, middle + 1, high);
    return node;
}

void inorderRecursive(BinaryNode* node) {
    if (!node) return;
    inorderRecursive(node->left);
    std::cout << node->key << " ";
    inorderRecursive(node->right);
}

void inorderIterative(BinaryNode* current) {
    while (current) {
        if (!current->left) {
            std::cout << current->key << " ";
            current = current->right;
        } else {
            BinaryNode* predecessor = current->left;
            while (predecessor->right && predecessor->right != current)
                predecessor = predecessor->right;
            if (!predecessor->right) {
                predecessor->right = current;
                current = current->left;
            } else {
                predecessor->right = nullptr;
                std::cout << current->key << " ";
                current = current->right;
            }
        }
    }
}

void indent(int level) {
    for (int step = 0; step < level; step++) std::cout << "    ";
}

void prettyPrintParents(int* parent, int size, int node, int level) {
    indent(level);
    std::cout << node << "\n";
    for (int candidate = 1; candidate <= size; candidate++)
        if (parent[candidate] == node)
            prettyPrintParents(parent, size, candidate, level + 1);
}

struct MultiNode {
    int key;
    int childCount;
    MultiNode** children;
};

MultiNode* parentsToMultiway(int* parent, int size) {
    MultiNode* nodes = new MultiNode[size + 1];
    int* filled = new int[size + 1];
    for (int index = 1; index <= size; index++) {
        nodes[index].key = index;
        nodes[index].childCount = 0;
        filled[index] = 0;
    }
    for (int index = 1; index <= size; index++)
        if (parent[index] != -1) nodes[parent[index]].childCount++;
    for (int index = 1; index <= size; index++)
        nodes[index].children = new MultiNode*[nodes[index].childCount];
    MultiNode* root = nullptr;
    for (int index = 1; index <= size; index++) {
        if (parent[index] == -1)
            root = &nodes[index];
        else
            nodes[parent[index]].children[filled[parent[index]]++] = &nodes[index];
    }
    delete[] filled;
    return root;
}

void prettyPrintMultiway(MultiNode* node, int level) {
    indent(level);
    std::cout << node->key << "\n";
    for (int child = 0; child < node->childCount; child++)
        prettyPrintMultiway(node->children[child], level + 1);
}

BinaryNode* multiwayToBinary(MultiNode* multiNode) {
    BinaryNode* binaryNode = newBinaryNode(multiNode->key);
    BinaryNode* previousSibling = nullptr;
    for (int child = 0; child < multiNode->childCount; child++) {
        BinaryNode* converted = multiwayToBinary(multiNode->children[child]);
        if (previousSibling) previousSibling->right = converted;
        else binaryNode->left = converted;
        previousSibling = converted;
    }
    return binaryNode;
}

void prettyPrintBinary(BinaryNode* node, int level) {
    if (!node) return;
    indent(level);
    std::cout << node->key << "\n";
    prettyPrintBinary(node->left, level + 1);
    prettyPrintBinary(node->right, level);
}

void demo1() {
    int sortedValues[7] = {1, 2, 3, 4, 5, 6, 7};
    BinaryNode* tree = buildBalanced(sortedValues, 0, 6);
    std::cout << "Inorder recursive: ";
    inorderRecursive(tree);
    std::cout << "\nInorder iterative: ";
    inorderIterative(tree);
    std::cout << "\n\n";
}

void demo2() {
    int parent[10] = {0, 2, 7, 5, 2, 7, 7, -1, 5, 2};
    int size = 9;
    int rootIndex = 0;
    for (int index = 1; index <= size; index++)
        if (parent[index] == -1) rootIndex = index;

    std::cout << "R1:\n";
    prettyPrintParents(parent, size, rootIndex, 0);

    MultiNode* multiRoot = parentsToMultiway(parent, size);
    std::cout << "\nR2:\n";
    prettyPrintMultiway(multiRoot, 0);

    BinaryNode* binaryRoot = multiwayToBinary(multiRoot);
    std::cout << "\nR3:\n";
    prettyPrintBinary(binaryRoot, 0);
}

int main() {
    demo1();
    demo2();
    return 0;
}