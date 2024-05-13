#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;


struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;
};

int height(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int balanceFactor(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

AVLNode* insert(AVLNode* node, int key) {
    if (node == nullptr)
        return new AVLNode{ key, nullptr, nullptr, 1 };

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Duplicate keys not allowed

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = balanceFactor(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

bool isIdentical(AVLNode* T, AVLNode* S ) { 
    if (T == nullptr && S == nullptr)
        return true;
    if (T == nullptr || S == nullptr)
        return false;
    return (T->key == S->key && isIdentical(T->left, S->left) && isIdentical(T->right, S->right));
}

bool isSubtree(AVLNode* T, AVLNode* S) {
    if (T == nullptr)
        return false;
    if (T->key == S->key && isIdentical(T, S))
        return true;
    return isSubtree(T->left, S) || isSubtree(T->right, S);
}

void inorderTraversal(AVLNode* root, vector<int>& res) {
    if (root == nullptr)
        return;
    inorderTraversal(root->left, res);
    res.push_back(root->key);
    inorderTraversal(root->right, res);
}

void printTreeUtil(AVLNode* root, int space) {
    if (root == nullptr)
        return;

    space += 5;

    printTreeUtil(root->right, space);
    cout << endl;
    for (int i = 5; i < space; i++)
        cout << " ";
    cout << root->key << "\n";
    printTreeUtil(root->left, space);
}

void printTree(AVLNode* root) {
    printTreeUtil(root, 0);
}

int main() {
    setlocale(LC_ALL, "ru");
    ifstream inputFile("input.txt");
    ifstream inputFile2("input2.txt");

    if (!inputFile) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }
    if (!inputFile2) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    ofstream outputFile("output.txt");
    if (!outputFile) {
        cerr << "Failed to create output file." << endl;
        return 1;
    }

    vector<int> data1, data2;
    int num;
    while (inputFile >> num)
        data1.push_back(num);
    while (inputFile2 >> num)
        data2.push_back(num);

    AVLNode* tree1 = nullptr;
    AVLNode* tree2 = nullptr;

    // Constructing tree 1
    if (!data1.empty()) {
        for (int key : data1)
            tree1 = insert(tree1, key);
    }
    else {
        cout << "Tree 1 is empty." << endl;
        outputFile << "Tree 1 is empty." << endl;
    }

    // Constructing tree 2
    if (!data2.empty()) {
        for (int key : data2)
            tree2 = insert(tree2, key);
    }
    else {
        cout << "Tree 2 is empty." << endl;
        outputFile << "Tree 2 is empty." << endl;
    }

    if (tree1 == nullptr || tree2 == nullptr) {
        cout << "Cannot check subtree relationship as one of the trees is empty." << endl;
        outputFile << "Cannot check subtree relationship as one of the trees is empty." << endl;
        return 0;
    }

    vector<int> inorder1, inorder2;
    inorderTraversal(tree1, inorder1);
    inorderTraversal(tree2, inorder2);

    bool isSub = isSubtree(tree2, tree1);
    bool areIdentical = isIdentical(tree1, tree2);

    if (areIdentical) {
        cout << "The trees are completely identical." << endl;
        outputFile << "The trees are completely identical." << endl;
    }
    else if (!isSub) {
        cout << "The trees are not identical, and Tree 1 is not a subtree of Tree 2." << endl;
        outputFile << "The trees are not identical, and Tree 1 is not a subtree of Tree 2." << endl;
    }
    else {
        outputFile << "Tree 1 is a subtree of Tree 2." << endl;
        cout << "Tree 1 is a subtree of Tree 2." << endl;
    }

    cout << "Tree 1:" << endl;
    printTree(tree1);
    cout << endl << "Tree 2:" << endl;
    printTree(tree2);

    return 0;
}

