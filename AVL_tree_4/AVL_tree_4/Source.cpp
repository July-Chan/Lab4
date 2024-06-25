#include <iostream>
#include <algorithm> // для функції max
using namespace std;

// Вузол бінарного дерева
struct Node {
    int data;
    Node* left;
    Node* right;
    int height; // Висота піддерева з коренем у даному вузлі

    Node(int value) {
        data = value;
        left = right = nullptr;
        height = 1;
    }
};

// Клас АВЛ-дерева
class AVLTree {
public:
    Node* root;

    AVLTree() {
        root = nullptr;
    }

    // Додавання вузла
    void insert(int value) {
        root = insertRec(root, value);
    }

    // Пошук вузла
    bool search(int value) {
        return searchRec(root, value);
    }

    // Видалення вузла
    void remove(int value) {
        root = removeRec(root, value);
    }

    // Виведення дерева (обхід в порядку зростання)
    void inorder() {
        inorderRec(root);
        cout << endl;
    }

    // Балансування дерева
    void balance() {
        balanceRec(root);
    }

private:
    // Визначення висоти вузла
    int height(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    // Поворот вправо
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Виконуємо обертання
        x->right = y;
        y->left = T2;

        // Оновлюємо висоти
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // Повертаємо новий корінь
        return x;
    }

    // Поворот вліво
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Виконуємо обертання
        y->left = x;
        x->right = T2;

        // Оновлюємо висоти
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        // Повертаємо новий корінь
        return y;
    }

    // Рекурсивне додавання вузла з підтримкою балансування
    Node* insertRec(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }
        if (value < node->data) {
            node->left = insertRec(node->left, value);
        }
        else if (value > node->data) {
            node->right = insertRec(node->right, value);
        }
        else {
            return node; // Вузол з таким значенням вже існує
        }

        // Оновлюємо висоту поточного вузла
        node->height = 1 + max(height(node->left), height(node->right));

        // Визначаємо фактор балансу
        int balanceFactor = getBalance(node);

        // Якщо дерево незбалансоване, потрібні повороти
        if (balanceFactor > 1 && value < node->left->data) {
            return rotateRight(node);
        }
        if (balanceFactor < -1 && value > node->right->data) {
            return rotateLeft(node);
        }
        if (balanceFactor > 1 && value > node->left->data) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balanceFactor < -1 && value < node->right->data) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        // Повертаємо незмінений вузол
        return node;
    }

    // Рекурсивний пошук вузла
    bool searchRec(Node* node, int value) {
        if (node == nullptr) {
            return false;
        }
        if (node->data == value) {
            return true;
        }
        if (value < node->data) {
            return searchRec(node->left, value);
        }
        return searchRec(node->right, value);
    }

    // Рекурсивне видалення вузла з підтримкою балансування
    Node* removeRec(Node* node, int value) {
        if (node == nullptr) {
            return node;
        }

        if (value < node->data) {
            node->left = removeRec(node->left, value);
        }
        else if (value > node->data) {
            node->right = removeRec(node->right, value);
        }
        else {
            // Вузол для видалення знайдено

            // Вузол з одним або нульовим нащадком
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = (node->left != nullptr) ? node->left : node->right;

                // Випадок без нащадків
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else { // Випадок одного нащадка
                    *node = *temp; // Копіюємо вміст temp в node
                }
                delete temp;
            }
            else {
                // Вузол з двома нащадками: заміна вузла з мінімальним значенням
                Node* temp = minValueNode(node->right);

                // Копіюємо значення мінімального вузла до поточного вузла
                node->data = temp->data;

                // Видаляємо мінімальний вузол
                node->right = removeRec(node->right, temp->data);
            }
        }

        // Якщо дерево містить лише один вузол, повертаємо його
        if (node == nullptr) {
            return node;
        }

        // Оновлюємо висоту поточного вузла
        node->height = 1 + max(height(node->left), height(node->right));

        // Визначаємо фактор балансу
        int balanceFactor = getBalance(node);

        // Якщо дерево незбалансоване, потрібні повороти
        if (balanceFactor > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balanceFactor > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balanceFactor < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balanceFactor < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        // Повертаємо незмінений вузол
        return node;
    }

    // Пошук вузла з мінімальним значенням
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Рекурсивний обхід в порядку зростання
    void inorderRec(Node* node) {
        if (node != nullptr) {
            inorderRec(node->left);
            cout << node->data << " ";
            inorderRec(node->right);
        }
    }

    // Отримання фактора балансу для вузла
    int getBalance(Node* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    // Рекурсивне балансування всього дерева
    void balanceRec(Node* node) {
        if (node != nullptr) {
            balanceRec(node->left);
            balanceRec(node->right);
            root = balanceSubtree(node);
        }
    }

    // Балансування піддерева
    Node* balanceSubtree(Node* node) {
        int balanceFactor = getBalance(node);

        // Якщо піддерево незбалансоване, потрібні повороти
        if (balanceFactor > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        if (balanceFactor > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balanceFactor < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        if (balanceFactor < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }
};

int main() {
    AVLTree tree;

    // Додавання елементів
    tree.insert(30);
    tree.insert(20);
    tree.insert(40);
    tree.insert(10);
    tree.insert(25);
    tree.insert(35);
    tree.insert(50);

    // Виведення дерева
    cout << "Inorder traversal before deletion:" << endl;
    tree.inorder();

    // Видалення елементу, що призведе до потреби в обертах вліво та вправо
    tree.remove(10);

    // Виведення дерева після видалення
    cout << "\nInorder traversal after deletion of 10:" << endl;
    tree.inorder();

    return 0;
}
