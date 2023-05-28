#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <chrono>

using namespace std;

int checkInput(){
    int input;
    try {
        cin >> input;
        if (cin.fail()) {
            throw 1;
        }
    } catch (int exception) {
        cout << "ERROR!!!";
        exit(0);
    }
    cin.sync();
    cout << "\n";
    return input;
}

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

    // Конструктор для узла
    explicit Node(int value) : data(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;

    // Вспомогательные функции
    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void fixViolation(Node* newNode);
    void fixDoubleBlack(Node* x);
    void deleteNode(Node* node);
    Node* findMinimum(Node* node);
    std::string getColorString(Color color);
    void inorderHelper(Node* node, std::stringstream& ss, std::string indent, bool last);
public:

    RedBlackTree() : root(nullptr) {}
    Node* getRoot();
    // Основные функции
    void insert(int value);
    void remove(int value);
    Node * search(int value);
    void printTree(ofstream& output);
    void cleanTree(Node* node);
    void action(string str, int type, ofstream& output_key, ofstream& output_ans);
};

Node* RedBlackTree::getRoot(){
    return root;
}

// Левый поворот вокруг узла x
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

// Правый поворот вокруг узла x
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

// Вспомогательная функция для вставки узла и восстановления свойств красно-черного дерева
void RedBlackTree::fixViolation(Node* newNode) {
    Node* parent = nullptr;
    Node* grandparent = nullptr;

    while (newNode != root && newNode->color != BLACK && newNode->parent->color == RED) {
        parent = newNode->parent;
        grandparent = parent->parent;

        // Случай A: Родитель узла левый для деда
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;

            // Случай 1: Дядя узла является красным цветом
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                newNode = grandparent;
            } else {
                // Случай 2: Узел является правым дочерним узлом родителя
                if (newNode == parent->right) {
                    leftRotate(parent);
                    newNode = parent;
                    parent = newNode->parent;
                }

                // Случай 3: Узел является левым дочерним узлом родителя
                rightRotate(grandparent);
                std::swap(parent->color, grandparent->color);
                newNode = parent;
            }
        }
            // Случай B: Родитель узла правый для деда
        else {
            Node* uncle = grandparent->left;

            // Случай 1: Дядя узла является красным цветом
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                newNode = grandparent;
            } else {
                // Случай 2: Узел является левым дочерним узлом родителя
                if (newNode == parent->left) {
                    rightRotate(parent);
                    newNode = parent;
                    parent = newNode->parent;
                }

                // Случай 3: Узел является правым дочерним узлом родителя
                leftRotate(grandparent);
                std::swap(parent->color, grandparent->color);
                newNode = parent;
            }
        }
    }

    root->color = BLACK;
}

void RedBlackTree::deleteNode(Node* node) {
    Node* deleteNode = nullptr;
    if (node->left != nullptr && node->right != nullptr){
        Node* help = findMinimum(node->right);
        swap(node->data, help->data);
        node = help;
        if(node->left != nullptr){
            if(node == root) {
                root = node->left;
                deleteNode = node;
                node->left->color = BLACK;
                node->left->parent = nullptr;
            } else{
                deleteNode = node;
                node->left->parent = node->parent;
                node->left->color = BLACK;
                if(node == node->parent->left) node->parent->left = node->left;
                else node->parent->right = node->left;
            }

        }else if(node->right != nullptr){
            if(node == root) {
                root = node->right;
                deleteNode = node;
                node->right->color = BLACK;
                node->right->parent = nullptr;
            } else{
                deleteNode = node;
                node->right->parent = node->parent;
                node->right->color = BLACK;
                if(node == node->parent->right) node->parent->right = node->right;
                else node->parent->left = node->right;
            }
        }else{
            if(node == root) {
                root = nullptr;
                delete node;
                return;
            }
            if(node->color == BLACK){
                fixDoubleBlack(node);
            }
            if(node == node->parent->right) node->parent->right = nullptr;
            else node->parent->left = nullptr;
            deleteNode = node;
        }
    }else{
        if(node->left != nullptr){
            if(node == root) {
                root = node->left;
                deleteNode = node;
                node->left->color = BLACK;
                node->left->parent = nullptr;
            } else{
                deleteNode = node;
                node->left->parent = node->parent;
                node->left->color = BLACK;
                if(node == node->parent->left) node->parent->left = node->left;
                else node->parent->right = node->left;
            }

        }else if(node->right != nullptr){
            if(node == root) {
                root = node->right;
                deleteNode = node;
                node->right->color = BLACK;
                node->right->parent = nullptr;
            } else{
                deleteNode = node;
                node->right->parent = node->parent;
                node->right->color = BLACK;
                if(node == node->parent->right) node->parent->right = node->right;
                else node->parent->left = node->right;
            }
        }else{
            if(node == root) {
                root = nullptr;
                delete node;
                return;
            }
            if(node->color == BLACK){
                fixDoubleBlack(node);
            }
            if(node == node->parent->right) node->parent->right = nullptr;
            else node->parent->left = nullptr;
            deleteNode = node;
        }
    }
    delete deleteNode;
}



// Функция для исправления случая двойного черного узла
void RedBlackTree::fixDoubleBlack(Node* x) {
    if (x == nullptr || x == root)
        return;

    Node* sibling = nullptr;
    bool isLeftChild = false;

    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            sibling = x->parent->right;
            isLeftChild = true;
        } else {
            sibling = x->parent->left;
            isLeftChild = false;
        }

        if (sibling == nullptr)
            return;

        // Случай 1: Брат узла является красным
        if (sibling->color == RED) {
            sibling->color = BLACK;
            x->parent->color = RED;

            if (isLeftChild)
                leftRotate(x->parent);
            else
                rightRotate(x->parent);

            sibling = (isLeftChild) ? x->parent->right : x->parent->left;
        }

        // Случай 2: Оба ребенка брата являются черными
        if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
            (sibling->right == nullptr || sibling->right->color == BLACK)) {
            sibling->color = RED;
            x = x->parent;
        } else {
            // Случай 3: Левый ребенок брата является красным, а правый - черным
            if (isLeftChild) {
                if (sibling->right == nullptr || sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(sibling);
                    sibling = x->parent->right;
                }
            } else {
                if (sibling->left == nullptr || sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(sibling);
                    sibling = x->parent->left;
                }
            }

            // Случай 4: Правый ребенок брата является красным
            sibling->color = x->parent->color;
            x->parent->color = BLACK;

            if (isLeftChild) {
                if (sibling->right != nullptr)
                    sibling->right->color = BLACK;

                leftRotate(x->parent);
            } else {
                if (sibling->left != nullptr)
                    sibling->left->color = BLACK;

                rightRotate(x->parent);
            }

            break;
        }
    }

    if (x != nullptr)
        x->color = BLACK;

    // Дополнительный случай: Отец и брат черные, и у брата нет черных детей
    if (x != nullptr && x->parent != nullptr) {
        Node* parent = x->parent;
        sibling = (parent->left == x) ? parent->right : parent->left;

        if (sibling != nullptr && sibling->color == BLACK &&
            ((sibling->left == nullptr || sibling->left->color == BLACK) &&
             (sibling->right == nullptr || sibling->right->color == BLACK))) {
            fixDoubleBlack(parent);
        }
    }
}





// Поиск минимального элемента в поддереве с корнем node
Node* RedBlackTree::findMinimum(Node* node) {
    while (node->left != nullptr)
        node = node->left;

    return node;
}

// Вставка элемента в красно-черное дерево
void RedBlackTree::insert(int value) {
    Node* newNode = new Node(value);

    if (root == nullptr) {
        root = newNode;
        root->color = BLACK;
    } else {
        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            if (value < current->data)
                current = current->left;
            else if (value > current->data)
                current = current->right;
            else {
                // Если значение уже присутствует в дереве, просто выходим
                delete newNode;
                return;
            }
        }

        newNode->parent = parent;

        if (value < parent->data)
            parent->left = newNode;
        else
            parent->right = newNode;

        fixViolation(newNode);
    }
}

Node* RedBlackTree::search(int value) {
    if (root == nullptr)
        return nullptr;

    Node* node = root;

    while (node != nullptr) {
        if (value < node->data)
            node = node->left;
        else if (value > node->data)
            node = node->right;
        else {
            return node;
        }
    }
    return nullptr;
}

// Удаление элемента из красно-черного дерева
void RedBlackTree::remove(int value) {
    Node* node = search(value);
    if(node){
        deleteNode(node);
    }
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
void RedBlackTree::printTree(ofstream& output) {
    std::stringstream ss;
    inorderHelper(root, ss, "", true);
    output << ss.str();
}


void RedBlackTree::cleanTree(Node* node) {
    if (node == nullptr)
        return;
    cleanTree(node->left);
    cleanTree(node->right);
    if(node == root) root = nullptr;
    delete node;
}

void RedBlackTree::action(string str, int type, ofstream& output_key, ofstream& output_ans){
    istringstream iss(str); int k;
    while (iss >> k) {
        switch (type) {
            case 0:
            case 1:
                this->insert(k);
                break;
            case 2:
                if (!this->search(k)) output_key << k << " is not exist!\n";
                else{
                    this->remove(k);
                }
                break;
            case 3:
                if (this->search(k)) output_ans << k << " exist; ";
                else output_ans << k << " not exist; ";
                break;
            default:
                output_key << "Error input!";
        }
        output_key << type <<":\n";
        printTree(output_key);
    }
    output_ans << "\n";
    switch (type) {
        case 0:
            output_ans << "Tree after creation:\n";
            break;
        case 1:
            output_ans << "Tree after add elements:\n";
            break;
        case 2:
            output_ans << "Tree after delete elements:\n";
            break;
        case 3:
            output_ans << "Final result:\n";
            break;
    }
    printTree(output_ans);
}

int main() {
    RedBlackTree tree;
    ifstream input_task("C:\\FirstCursProgramm\\C++\\RedBlackTree\\input_task.txt");
    ofstream output_key("C:\\FirstCursProgramm\\C++\\RedBlackTree\\output_key.txt");
    ofstream output_ans("C:\\FirstCursProgramm\\C++\\RedBlackTree\\output_ans.txt");
    if (input_task.is_open() && output_ans.is_open() && output_key.is_open()) {
        string line, creatLine, deleteLine, addLine, searchLine;
        for(int i = 0; i < 5; i++) getline(input_task, line);
        while (getline(input_task, line)) {
            getline(input_task, creatLine);
            getline(input_task, deleteLine);
            getline(input_task, addLine);
            getline(input_task, searchLine);
            auto start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            tree.action(creatLine, 0, output_key, output_ans);
            auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to creat tree in nanoseconds: " << end - start << "ns\n";
            start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            tree.action(deleteLine, 2, output_key, output_ans);
            end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to delete elements tree in nanoseconds: " << end - start << "ns\n";
            start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            tree.action(addLine, 1, output_key, output_ans);
            end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to add elements tree in nanoseconds: " << end - start << "ns\n";
            start = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            tree.action(searchLine, 3, output_key, output_ans);
            end = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            output_key << "time spent to search elements tree in nanoseconds: " << end - start << "ns\n";
            output_ans << "----------------------------";
            output_key << "----------------------------";
            tree.cleanTree(tree.getRoot());

        }
        input_task.close();
        output_ans.close();
        output_key.close();
    } else {
        std::cout << "Could not open the file!!!" << std::endl;
    }
    getchar();
    return 0;
}