#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct Node {
    int data;
    Node *left;
    Node *right;

    Node(int x) {
        data = x;
        left = right = nullptr;
    }
};

class GraphvizBST {
public:
    static void saveDotFile(const std::string &filename, const std::string &dotContent) {
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << dotContent;
            outFile.close();
            std::cout << "DOT file saved: " << filename << std::endl;
        } else {
            std::cerr << "Error: Could not open file " << filename << std::endl;
        }
    }

    static std::string generateDot(const Node *root) {
        std::string dot = "digraph BST {\n";
        dot += "    node [fontname=\"Arial\"];\n";
        if (!root) dot += "    empty [label=\"Empty Tree\", shape=none];\n";
        else dot += generateDotHelper(root);
        dot += "}\n";
        return dot;
    }

private:
    static std::string generateDotHelper(const Node *node) {
        if (!node) return "";
        std::string result;
        if (node->left) {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->left->data) + " [label=\"L\"];\n";
            result += generateDotHelper(node->left);
        } else {
            std::string nullNode = "nullL" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        if (node->right) {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->right->data) + " [label=\"R\"];\n";
            result += generateDotHelper(node->right);
        } else {
            std::string nullNode = "nullR" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        return result;
    }
};

class Bst {
    Node *root;

    void _print(Node *subroot) {
        if (!subroot) return;
        _print(subroot->left);
        cout << subroot->data << " ";
        _print(subroot->right);
    }

    void _insert(Node *&subroot, int x) {
        if (!subroot) {
            subroot = new Node(x);
        } else {
            if (x < subroot->data) _insert(subroot->left, x);
            else if (x > subroot->data) _insert(subroot->right, x);
        }
    }

    Node* _findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    Node* _delete(Node* subroot, int x) {
        if (!subroot) return nullptr;

        if (x < subroot->data) {
            subroot->left = _delete(subroot->left, x);
        } else if (x > subroot->data) {
            subroot->right = _delete(subroot->right, x);
        } else {
            // Case 1 & 2: No child or one child
            if (!subroot->left) {
                Node* temp = subroot->right;
                delete subroot;
                return temp;
            } else if (!subroot->right) {
                Node* temp = subroot->left;
                delete subroot;
                return temp;
            }
            // Case 3: Two children
            Node* temp = _findMin(subroot->right); // Inorder Successor
            subroot->data = temp->data;
            subroot->right = _delete(subroot->right, temp->data);
        }
        return subroot;
    }

public:
    Bst() { root = nullptr; }
    void insert(int x) { _insert(root, x); }
    void deleteNode(int x) { root = _delete(root, x); }
    void print() { _print(root); cout << endl; }
    void saveDotFile(const std::string &filename) {
        std::string dotContent = GraphvizBST::generateDot(root);
        GraphvizBST::saveDotFile(filename, dotContent);
    }
};

int main() {
    // TEST CASE 1: Empty Tree
    Bst tree1;
    cout << "Testing Delete on Empty Tree: ";
    tree1.deleteNode(10); 
    tree1.print();

    // TEST CASE 2: Single Node Tree
    Bst tree2;
    tree2.insert(50);
    cout << "Tree before deleting root: "; tree2.print();
    tree2.deleteNode(50);
    cout << "Tree after deleting root: "; tree2.print();

    // TEST CASE 3: Linked List Style (Sequential)
    Bst tree3;
    tree3.insert(10); tree3.insert(20); tree3.insert(30);
    cout << "Linked list style tree: "; tree3.print();
    tree3.deleteNode(20);
    cout << "After deleting middle (20): "; tree3.print();
    tree3.saveDotFile("linked_list_test.dot");

    // TEST CASE 4: Complex Tree with all deletion cases
    Bst tree4;
    int vals[] = {40, 20, 60, 10, 30, 50, 70};
    for(int v : vals) tree4.insert(v);

    cout << "Complex tree before deletions: "; tree4.print();
    tree4.saveDotFile("complex_before.dot");

    tree4.deleteNode(10); // Case 1: Leaf
    tree4.deleteNode(60); // Case 2: One child (70 or 50 will move up)
    tree4.deleteNode(20); // Case 3: Two children

    cout << "Complex tree after deletions (10, 60, 20): "; tree4.print();
    tree4.saveDotFile("complex_after.dot");

    return 0;
}