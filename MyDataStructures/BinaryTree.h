// Created by Yehor on 06.03.2021.

#ifndef UNIVPROJECT_BINARYTREE_H
#define UNIVPROJECT_BINARYTREE_H

#include "iostream"
#include "MyStack.h"

//int comparator(int& d1, int& d2) {
//    return (d1 == d2) ? 0  :
//           (d1 < d2)  ? -1 : 1;
//}

template<class T>
class BinaryTree {
private:

    class TreeNode {
    public:
        T value;
        TreeNode* parent;
        TreeNode* left;
        TreeNode* right;

        TreeNode(T value, TreeNode *parent, TreeNode *left, TreeNode *right) :
                value(value), parent(parent),left(left), right(right) {
        }

        ~TreeNode() {}
    };

    TreeNode* treeRoot = nullptr;
    int size = 0;
    int(*comparator)(T& first, T& second) = nullptr;

    TreeNode* findNodeByValue(T& value) {
        TreeNode* cur = treeRoot;
        while (cur != nullptr) {
            int valueCompared = this->comparator(value, cur->value);
            if (valueCompared == 0)
                // means that the required element has been found
                return cur;
            else if (valueCompared < 0)
                cur = cur->left;
            else
                cur = cur->right;
        }
        return nullptr;
    }

    TreeNode* getMaximumNodeRelatively(TreeNode* node) {
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

    TreeNode* getMinimumNodeRelatively(TreeNode* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    bool isLeaf(TreeNode* node) {
        return node->left == nullptr && node->right == nullptr;
    }

    bool hasOnlyRightChildNode(TreeNode* node) {
        return node->right != nullptr && node->left == nullptr;
    }

    void removeAsLeaf(TreeNode* node, bool deleteFromRAM) {
        TreeNode* parent = node->parent;
        if (parent != nullptr) {
            if (parent->left->value == node->value)
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }
        if (deleteFromRAM)
            delete node;
    }

    void removeAsNonRootNode(TreeNode* node, bool deleteFromRAM) {
        TreeNode* child;
        if (node->right == nullptr)
            child = node->left;
        else
            child = node->right;
        TreeNode* parent = node->parent;
        if (parent != nullptr) {
            if (parent->left->value == node->value)
                parent->left = child;
            else
                parent->right = child;
        }
        if (deleteFromRAM)
            delete node;
    }

    void inorderTreeWalkIterative(TreeNode* curNode) {
        if (curNode == nullptr)
            return;

        MyStack<TreeNode*> stack;

        while (curNode != nullptr || stack.getSize() > 0) {

            while (curNode != nullptr) {
                stack.push(curNode);
                curNode = curNode->left;
            }

            curNode = stack.pop();

            // print the leftmost value
            cout << curNode->value << "  ";

            curNode = curNode->right;
        }
    }

    void postorderTreeWalkIterative(TreeNode* curNode) {
        if (curNode == nullptr)
            return;

        MyStack<TreeNode*> stack;

        while (curNode != nullptr || stack.getSize() > 0) {

            while (curNode != nullptr) {
                stack.push(curNode);
                curNode = curNode->right;
            }

            curNode = stack.pop();

            // print the rightmost value
            cout << curNode->value << "  ";

            curNode = curNode->left;
        }
    }

    void preorderTreeWalkIterative(TreeNode* curNode) {
        if (curNode == nullptr)
            return;

        MyStack<TreeNode*> stack;
        stack.push(this->treeRoot);

        while (stack.getSize() != 0) {

            curNode = stack.pop();
            cout << curNode->value << "  ";

            if (curNode->right != nullptr)
                stack.push(curNode->right);
            if (curNode->left != nullptr)
                stack.push(curNode->left);
        }
    }

    void inorderTreeWalkHelper(TreeNode* cur, int recDeep, void(*valuePrinter)(T& obj)) {
        if (cur != nullptr) {
            /*
             * !!!
             * Порядок right -> center -> left зделан чтобы красиво отобразить вывод в консоль.
             * Правильный же порядок должен быть left -> center -> right.
             */
            inorderTreeWalkHelper(cur->right, recDeep + 1, valuePrinter);
            for (int i = 0; i < recDeep * 5; ++i) cout << ' ';
            valuePrinter(cur->value);
            inorderTreeWalkHelper(cur->left, recDeep + 1, valuePrinter);
        }
    }

public:
    BinaryTree(int(*comparator)(T& first, T& second)) {
        this->comparator = comparator;
    }

    int getSize() {
        return this->size;
    }

    void add(T element) {
        if (treeRoot == nullptr)
            treeRoot = new TreeNode(element, nullptr, nullptr, nullptr);
        else {
            TreeNode* root = treeRoot;
            TreeNode* prev = nullptr;

            // a required node will be found or created
            while (root != nullptr) {
                prev = root;
                int valueCompared = this->comparator(element, root->value);
                if (valueCompared < 0)
                    root = root->left;
                else if (valueCompared > 0)
                    root = root->right;
                else {
                    root->value = element;
                    return;
                }
            }

            TreeNode* newTreeNode = new TreeNode(element, prev, nullptr, nullptr);
            if (comparator(element, prev->value) < 0)
                prev->left = newTreeNode;
            else
                prev->right = newTreeNode;
            this->size++;
            this->size++;
        }
    }

    bool contains(T value) {
        TreeNode* foundNode = findNodeByValue(value);
        return foundNode != nullptr;
    };

    T getMinimumElementInTree() {
        TreeNode* found = getMinimumNodeRelatively(this->treeRoot);
        return found->value;
    }

    T getMaximumElementInTree() {
        TreeNode* found = getMaximumNodeRelatively(this->treeRoot);
        return found->value;
    }

    T getBiggerElementOf(T value) {
        TreeNode* foundNode = findNodeByValue(value);
        if (foundNode == nullptr)
            return value;
        if (foundNode->right != nullptr) {
            TreeNode* found = getMinimumNodeRelatively(foundNode->right);
            return found->value;
        }
        TreeNode* parent = foundNode->parent;
        if (parent == nullptr)
            return value;
        while (parent->right == foundNode) {
            foundNode = parent;
            parent = parent->parent;
        }
        return parent->value;
    }

    T getSmallerElementOf(T value) {
        TreeNode* foundNode = findNodeByValue(value);
        if (foundNode == nullptr)
            return value;
        if (foundNode->left != nullptr) {
            TreeNode* found = getMaximumNodeRelatively(foundNode->left);
            return found->value;
        }
        TreeNode* parent = foundNode->parent;
        if (parent == nullptr)
            return value;
        while (parent->left == foundNode) {
            foundNode = parent;
            parent = parent->parent;
        }
        return parent->value;
    }

    void remove(T value) {
        TreeNode* node = findNodeByValue(value);
        if (node == nullptr)
            return;

        // There are 3 situations to delete a node
        if (isLeaf(node))
            removeAsLeaf(node, true);
        else if (hasOnlyRightChildNode(node))
            removeAsNonRootNode(node, true);
        else {
            TreeNode* relativelyMinNode = getMinimumNodeRelatively(node->right);

            // if a new root node isn't a leaf node
            if (hasOnlyRightChildNode(relativelyMinNode))
                removeAsNonRootNode(relativelyMinNode, false);

            TreeNode* relativelyMinNodeParent = relativelyMinNode->parent;
            if (relativelyMinNodeParent != nullptr) {
                if (relativelyMinNodeParent->left->value == relativelyMinNode->value)
                    relativelyMinNodeParent->left = nullptr;
                else
                    relativelyMinNodeParent->right = nullptr;
            }
            relativelyMinNode->parent = node->parent;
            relativelyMinNode->left = node->left;
            relativelyMinNode->right = node->right;
            if (node->left != nullptr)
                node->left->parent = relativelyMinNode;
            if (node->right != nullptr)
                node->right->parent = relativelyMinNode;

            if (node->parent != nullptr) {
                if (node->parent->left->value == node->value)
                    node->parent->left = relativelyMinNode;
                else
                    node->parent->right = relativelyMinNode;
            }

            if (node == this->treeRoot)
                this->treeRoot = relativelyMinNode;

            delete node;
        }
        size--;

    }

    // центрированый (симметричный) обход
    void inorderTreeWalk() {
        inorderTreeWalkIterative(this->treeRoot);
    }

    void inorderWalkRecursive(void(*valuePrinter)(T& obj)) {
        inorderTreeWalkHelper(this->treeRoot, 0, valuePrinter);
    }

    // обход в обратном порядке
    void postorderTreeWalk() {
        postorderTreeWalkIterative(this->treeRoot);
    }

    // обход в прямом порядке
    void preorderTreeWalk() {
        preorderTreeWalkIterative(this->treeRoot);
    }
};

#endif //UNIVPROJECT_BINARYTREE_H