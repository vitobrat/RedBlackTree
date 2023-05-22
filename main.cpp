#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

enum Color {
    RED,
    BLACK
};

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int value) : data(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;

    // Вспомогательные приватные функции
    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void fixViolation(Node* x, bool isInsertion);
    void inorderHelper(Node* node, std::stringstream& ss, std::string indent, bool last);
    std::string getColorString(Color color);

public:
    RedBlackTree() : root(nullptr) {}

    // Публичные функции
    void insert(int value);
    void printTree();
};

// Вспомогательная функция для выполнения левого поворота
void RedBlackTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != nullptr)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// Вспомогательная функция для выполнения правого поворота
void RedBlackTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;

    if (y->right != nullptr)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}

// Вспомогательная функция для исправления нарушений красно-черного свойства
void RedBlackTree::fixViolation(Node* x, bool isInsertion) {
    while (x != root && x->color == RED && x->parent->color == RED) {
        Node* parent = x->parent;
        Node* grandparent = parent->parent;

        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;

            // Случай 1: Дядя также является красным
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                x = grandparent;
            } else {
                // Случай 2: x - правый потомок своего родителя
                if (x == parent->right) {
                    leftRotate(parent);
                    x = parent;
                    parent = x->parent;
                }

                // Случай 3: x - левый потомок своего родителя
                rightRotate(grandparent);
                std::swap(parent->color, grandparent->color);
                x = parent;
            }
        } else {
            Node* uncle = grandparent->left;

            // Случай 1: Дядя также является красным
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                x = grandparent;
            } else {
                // Случай 2: x - левый потомок своего родителя
                if (x == parent->left) {
                    rightRotate(parent);
                    x = parent;
                    parent = x->parent;
                }

                // Случай 3: x - правый потомок своего родителя
                leftRotate(grandparent);
                std::swap(parent->color, grandparent->color);
                x = parent;
            }
        }
    }

    root->color = BLACK;
}

// Публичная функция для вставки элемента в красно-черное дерево
void RedBlackTree::insert(int value) {
    Node* newNode = new Node(value);

    // Вставляем новый узел, как в обычное BST
    Node* current = root;
    Node* parent = nullptr;

    while (current != nullptr) {
        parent = current;

        if (value < current->data)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;

    if (parent == nullptr)
        root = newNode;
    else if (value < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    // Исправляем нарушения красно-черного свойства
    fixViolation(newNode, true);
}

// Вспомогательная функция для рекурсивного обхода дерева в порядке возрастания (inorder)
void RedBlackTree::inorderHelper(Node* node, std::stringstream& ss, std::string indent, bool last) {
    if (node == nullptr)
        return;

    std::string colorStr = getColorString(node->color);
    std::string nodeStr = std::to_string(node->data);

    if (node->right) {
        inorderHelper(node->right, ss, indent  + "      ", false);
    }

    ss << indent;

    if (last) {
        ss << "\\--";
        indent += "   ";
    } else {
        ss << "/--";
        indent += "|   ";
    }

    ss << colorStr << nodeStr << std::endl;

    if (node->left) {
        inorderHelper(node->left, ss, indent + "   ", true);
    }
}

// Вспомогательная функция для получения строкового представления цвета узла
std::string RedBlackTree::getColorString(Color color) {
    return (color == RED) ? "[R] " : "[B] ";
}

// Публичная функция для выполнения красивого вывода дерева
void RedBlackTree::printTree() {
    std::stringstream ss;
    inorderHelper(root, ss, "", true);
    std::cout << ss.str() << std::endl;
}

// Пример использования
int main() {
    RedBlackTree tree;
    tree.insert(4);
    tree.insert(2);
    tree.insert(1);
    tree.insert(3);
    tree.insert(6);
    tree.insert(5);
    tree.insert(7);
    tree.insert(9);
    tree.insert(8);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);


    tree.printTree();
    getchar();
    return 0;
}
