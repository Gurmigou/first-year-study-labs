// Created by Yehor on 05.04.2021.

#ifndef UNIVPROJECT_MYREDBLACKTREE_H
#define UNIVPROJECT_MYREDBLACKTREE_H

#include <iostream>

using namespace std;

template<class T>
class MyRedBlackTree {
private:
    enum Color {
        RED,
        BLACK
    };

    class TreeNode {
    public:
        T value;
        Color color;
        TreeNode* parent;
        TreeNode* left;
        TreeNode* right;

        TreeNode(T value, Color color, TreeNode *parent, TreeNode *left, TreeNode *right) :
                value(value), color(color), parent(parent), left(left), right(right) {};

        TreeNode(Color color, TreeNode *parent, TreeNode *left, TreeNode *right) :
            color(color), parent(parent), left(left), right(right) {}
    };

    // an element which represent a NULL node
    TreeNode* NULL_NODE = new TreeNode{BLACK, NULL_NODE, NULL_NODE, NULL_NODE};
    // root of the tree
    TreeNode* root = NULL_NODE;
    // comparator which is used to compare nodes
    int(*comparator)(T& first, T& second) = nullptr;
    // a number of nodes in the tree
    int size = 0;

                                /* Internal private methods */
    void leftRotate(TreeNode* x) {
        // не рассматриваем случай, когда x.right == NULL_NODE
        if (x->right == NULL_NODE)
            return;

        TreeNode* y = x->right;
        x->right = y->left; // установить бетта
        if (y->left != NULL_NODE)
            y->left->parent = x;

        // передать родителя x узлу y
        y->parent = x->parent;

        // проверить случай, когда x - корень дерева
        if (x == root) // также ознчает, что x.parent == NULL_NODE
            root = y;
        else if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;

        // установка связи между узлами y и x
        y->left = x;
        x->parent = y;
    }

    void rightRotate(TreeNode* y) {
        // не рассматриваем случай, когда y.left == NULL_NODE
        if (y->left == NULL_NODE)
            return;

        TreeNode* x = y->left;
        y->left = x->right; // установить бетта
        if (x->right != NULL_NODE)
            x->right->parent = y;

        // передать родителя y узлу x
        x->parent = y->parent;
        // проверить случай, когда y - корень дерева
        if (y == root) // также ознчает, что y.parent == NULL_NODE
            root = x;
        else if (y->parent->left == y)
            y->parent->left = x;
        else
            y->parent->right = x;

        // установка связи между узлами x и y
        x->right = y;
        y->parent = x;
    }

    TreeNode* findNodeByValue(T& value) {
        TreeNode* cur = root;
        while (cur != NULL_NODE) {
            int valueCompared = this->comparator(value, cur->value);
            if (valueCompared == 0)
                // означает, что элемент найден
                return cur;
            else if (valueCompared < 0)
                cur = cur->left;
            else
                cur = cur->right;
        }
        return NULL_NODE;
    }

    /**
     * Ситуацие при вставке, которые нарушают структуру
     * (не удовлетворяют пяти свойствам) красно-черного дерева:
     * 1) Вставленный красный узел является корнем дерева. Будет нарушено свойство (2);
     * 2) Родитель вставленного красного узла - так же красный. Будет нарушено свойство (4);
     * @param z - вставляемый в дерево новый элемент
     */
    void addOperationFixUpRBT(TreeNode* z) {
        while (z->parent->color == RED) {
            // случай, если текущий узел находится в левом поддереве относительно прародителя
            if (z->parent == z->parent->parent->left) {
                TreeNode* uncle = z->parent->parent->right;
                // если дядя красный
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            // случай, если текущий узел находится в правод поддереве относительно прародителя
            else {
                TreeNode* uncle = z->parent->parent->left;
                // если дядя красный
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }


    void inorderTreeWalkHelper(TreeNode* cur, int recDeep, void(*valuePrinter)(T& obj)) {
        if (cur != NULL_NODE) {
            /*
             * !!!
             * Порядок right -> center -> left зделан чтобы красиво отобразить вывод в консоль.
             * Правильный же порядок должен быть left -> center -> right.
             */
            inorderTreeWalkHelper(cur->right, recDeep + 1, valuePrinter);
            for (int i = 0; i < recDeep * 5; ++i) cout << ' ';
            if (cur->color == RED) cout << "R ";
            else cout << "B ";
            valuePrinter(cur->value);
            inorderTreeWalkHelper(cur->left, recDeep + 1, valuePrinter);
        }
    }

    TreeNode* getMaximumNodeRelatively(TreeNode* node) {
        while (node->right != NULL_NODE)
            node = node->right;
        return node;
    }

    TreeNode* getMinimumNodeRelatively(TreeNode* node) {
        while (node->left != NULL_NODE)
            node = node->left;
        return node;
    }

    // input:  parent -> u -> v
    // output: parent -> v. Node 'u' has been deleted
    void transplantRemoveHelper(TreeNode* u, TreeNode* v) {
        if (u->parent == NULL_NODE)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    void removeOperationFixUpRBT(TreeNode* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                TreeNode* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                }
                else {
                    if (w->right->color == BLACK) {
                        w->left->color == BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else {
                TreeNode* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                }
                else {
                    if (w->left->color == BLACK) {
                        w->right->color == BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void deleteTree(TreeNode* node) {
        if (node != NULL_NODE) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    explicit MyRedBlackTree(int(*comparator)(T& first, T& second)) {
        this->comparator = comparator;
    }

    ~MyRedBlackTree() {
        deleteTree(root);
        root = nullptr;
        delete NULL_NODE;
        NULL_NODE = nullptr;
    }

    void clear() {
        deleteTree(root);
        root = nullptr;
        size = 0;
    }

    int getSize() {
        return size;
    }

    void remove(T element) {
        TreeNode* z = findNodeByValue(element);
        if (z == NULL_NODE)
            return;
        TreeNode* y = z;
        TreeNode* x;
        Color yOriginalColor = y->color;
        if (z->left == NULL_NODE) {
            x = z->right;
            transplantRemoveHelper(z, z->right);
        }
        else if (z->right == NULL_NODE) {
            x = z->left;
            transplantRemoveHelper(z, z->left);
        }
        else {
            y = getMinimumNodeRelatively(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                transplantRemoveHelper(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplantRemoveHelper(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if (yOriginalColor == BLACK)
            removeOperationFixUpRBT(x);
        delete z;
    }

    void add(T element) {
        TreeNode* rootLocal = root;
        TreeNode* prev = NULL_NODE;
        // поиск необходимого места для вставки узла
        while (rootLocal != NULL_NODE) {
            prev = rootLocal;
            int valueCompared = this->comparator(element, rootLocal->value);
            if (valueCompared < 0)
                rootLocal = rootLocal->left;
            else if (valueCompared > 0)
                rootLocal = rootLocal->right;
            else {
                rootLocal->value = element;
                return;
            }
        }
        // создание нового узла, окрашеного в КРАСНЫЙ цвет
        TreeNode* newTreeNode = new TreeNode(element, RED, prev, NULL_NODE, NULL_NODE);
        if (root == NULL_NODE)
            root = newTreeNode;
        else if (comparator(element, prev->value) < 0)
            prev->left = newTreeNode;
        else
            prev->right = newTreeNode;
        this->size++;
        addOperationFixUpRBT(newTreeNode);
    }

    void inorderWalk(void(*valuePrinter)(T& obj)) {
        inorderTreeWalkHelper(root, 0, valuePrinter);
    }

    bool contains(T value) {
        TreeNode* foundNode = findNodeByValue(value);
        return foundNode != NULL_NODE;
    };

    T getMinimumElementInTree() {
        TreeNode* found = getMinimumNodeRelatively(this->root);
        return found->value;
    }

    T getMaximumElementInTree() {
        TreeNode* found = getMaximumNodeRelatively(this->root);
        return found->value;
    }

    T getBiggerElementOf(T value) {
        TreeNode* foundNode = findNodeByValue(value);
        if (foundNode == NULL_NODE)
            return value;
        if (foundNode->right != NULL_NODE) {
            TreeNode* found = getMinimumNodeRelatively(foundNode->right);
            return found->value;
        }
        TreeNode* parent = foundNode->parent;
        if (parent == NULL_NODE)
            return value;
        while (parent->right == foundNode) {
            foundNode = parent;
            parent = parent->parent;
        }
        return parent->value;
    }

    T getSmallerElementOf(T value) {
        TreeNode* foundNode = findNodeByValue(value);
        if (foundNode == NULL_NODE)
            return value;
        if (foundNode->left != NULL_NODE) {
            TreeNode* found = getMaximumNodeRelatively(foundNode->left);
            return found->value;
        }
        TreeNode* parent = foundNode->parent;
        if (parent == NULL_NODE)
            return value;
        while (parent->left == foundNode) {
            foundNode = parent;
            parent = parent->parent;
        }
        return parent->value;
    }
};

#endif //UNIVPROJECT_MYREDBLACKTREE_H