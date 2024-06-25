#include <iostream>
#include <algorithm> // ��� ������� max
#include<Windows.h>
using namespace std;

// ����� �������� ������
struct Node {
    int data;
    Node* left;
    Node* right;
    int height; // ������ �������� � ������� � ������ ����

    Node(int value) {
        data = value;
        left = right = nullptr;
        height = 1;
    }
};

// ���� ���-������
class AVLTree {
public:
    Node* root;

    AVLTree() {
        root = nullptr;
    }

    // ��������� �����
    void insert(int value) {
        root = insertRec(root, value);
    }

    // ����� �����
    bool search(int value) {
        return searchRec(root, value);
    }

    // ��������� �����
    void remove(int value) {
        root = removeRec(root, value);
    }

    // ��������� ������ (����� � ������� ���������)
    void inorder() {
        inorderRec(root);
        cout << endl;
    }

    // ������������ ������
    void balance() {
        balanceRec(root);
    }

private:
    // ���������� ������ �����
    int height(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    // ������� ������
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // �������� ���������
        x->right = y;
        y->left = T2;

        // ��������� ������
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // ��������� ����� �����
        return x;
    }

    // ������� ����
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // �������� ���������
        y->left = x;
        x->right = T2;

        // ��������� ������
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        // ��������� ����� �����
        return y;
    }

    // ���������� ��������� ����� � ��������� ������������
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
            return node; // ����� � ����� ��������� ��� ����
        }

        // ��������� ������ ��������� �����
        node->height = 1 + max(height(node->left), height(node->right));

        // ��������� ������ �������
        int balanceFactor = getBalance(node);

        // ���� ������ ��������������, ������ ��������
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

        // ��������� ��������� �����
        return node;
    }

    // ����������� ����� �����
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

    // ���������� ��������� ����� � ��������� ������������
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
            // ����� ��� ��������� ��������

            // ����� � ����� ��� �������� ��������
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = (node->left != nullptr) ? node->left : node->right;

                // ������� ��� �������
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else { // ������� ������ �������
                    *node = *temp; // ������� ���� temp � node
                }
                delete temp;
            }
            else {
                // ����� � ����� ���������: ����� ����� � ��������� ���������
                Node* temp = minValueNode(node->right);

                // ������� �������� ���������� ����� �� ��������� �����
                node->data = temp->data;

                // ��������� ��������� �����
                node->right = removeRec(node->right, temp->data);
            }
        }

        // ���� ������ ������ ���� ���� �����, ��������� ����
        if (node == nullptr) {
            return node;
        }

        // ��������� ������ ��������� �����
        node->height = 1 + max(height(node->left), height(node->right));

        // ��������� ������ �������
        int balanceFactor = getBalance(node);

        // ���� ������ ��������������, ������ ��������
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

    // ���������� ������� ������� �����
    int getBalance(Node* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    // ����� ����� � ��������� ��������� � �������
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // ����������� ����� � ������� ���������
    void inorderRec(Node* node) {
        if (node != nullptr) {
            inorderRec(node->left);
            cout << node->data << " ";
            inorderRec(node->right);
        }
    }

    // ���������� ������������ ������ ������
    void balanceRec(Node* node) {
        if (node != nullptr) {
            balanceRec(node->left);
            balanceRec(node->right);
            root = balanceSubtree(node);
        }
    }

    // ������������ ��������
    Node* balanceSubtree(Node* node) {
        int balanceFactor = getBalance(node);

        // ���� �������� ��������������, ������ ��������
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

// ������� ��� ������ ���� �� ������� ������ �����������
void displayMenu(AVLTree& tree) {
    char choice;
    int value;

    do {
        cout << "\n==== ���� ���-������ ====" << endl;
        cout << "1. ������ �����" << endl;
        cout << "2. ����� ����� � �����" << endl;
        cout << "3. �������� �����" << endl;
        cout << "4. ����� ��� �������" << endl;
        cout << "5. ������������ ������" << endl;
        cout << "6. �����" << endl;
        cout << "������ ��� ����: ";
        cin >> choice;

        switch (choice) {
        case '1':
            cout << "������ �������� ��� ���������: ";
            cin >> value;
            tree.insert(value);
            cout << value << " ������� � ������." << endl;
            break;
        case '2':
            cout << "������ �������� ��� ������: ";
            cin >> value;
            if (tree.search(value)) {
                cout << value << " �������� � �����." << endl;
            }
            else {
                cout << value << " �� �������� � �����." << endl;
            }
            break;
        case '3':
            cout << "������ �������� ��� ���������: ";
            cin >> value;
            tree.remove(value);
            cout << value << " �������� � ������." << endl;
            break;
        case '4':
            cout << "����� ��� �������: ";
            tree.inorder();
            break;
        case '5':
            cout << "������������ ������..." << endl;
            tree.balance();
            cout << "������ ������ ������������." << endl;
            break;
        case '6':
            cout << "����� � ��������." << endl;
            break;
        default:
            cout << "������������ ����. ���� �����, ������ ���������� ������." << endl;
        }

    } while (choice != '6');
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    AVLTree tree;

    // ��������� ����
    displayMenu(tree);

    return 0;
}
