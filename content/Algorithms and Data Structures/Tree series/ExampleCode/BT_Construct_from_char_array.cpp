//
// BT_Construct_from_char_array.cpp
// C++ code, harshly and successfully compiled by g++
//

#include <iostream>
#include <sstream>
#include <queue>


class BinaryTree;
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    char data;
public:
    TreeNode():leftchild(0),rightchild(0),parent(0),data('x'){};
    TreeNode(char s):leftchild(0),rightchild(0),parent(0),data(s){};

    friend class BinaryTree;
};

class BinaryTree{
private:
    TreeNode *root;
public:
    BinaryTree():root(0){};
    BinaryTree(const char* str);

    void LevelorderConstruct(std::stringstream &ss);
    void InsertLevelorder(char data);

    TreeNode* leftmost(TreeNode *current);
    TreeNode* InorderSuccessor(TreeNode *current);
    void Inorder_by_parent();
};

BinaryTree::BinaryTree(const char* str){
    std::stringstream  ss;
    ss << str;                     // magic!
    
    root = new TreeNode;           // allocate memory for root
    ss >> root->data;              // assign character to root
    
    LevelorderConstruct(ss);
}

void BinaryTree::LevelorderConstruct(std::stringstream &ss){
    
    std::queue<TreeNode*> q;         // create a queue to handle level-roder rule
    TreeNode *current = root;        // point *current to root
    char data = 'x';                 // initializa data as 'x'
    
    while (ss >> data) {
        if (data >= 65 && data <= 90){                // 處理current的leftchild
            TreeNode *new_node = new TreeNode(data);  // call constructor TreeNode(char s)
            new_node->parent = current;
            current->leftchild = new_node;
            q.push(new_node);
        }
        if (!(ss >> data)){                           // 有可能char array含有奇數筆資料
            break;                                    // 所以在這裡結束迴圈
        }
        if (data >= 65 && data <= 90){                // 處理current的rightchild
            TreeNode *new_node = new TreeNode;        // call constructor TreeNode()
            new_node->parent = current;
            current->rightchild = new_node;
            new_node->data = data;                    // assign data to new_node
            q.push(new_node);
        }
        current = q.front();                          // 從queue中更新current
        q.pop();                                      // 更新queue
    }
}

void BinaryTree::InsertLevelorder(char data){    

    std::queue<TreeNode*> q;
    TreeNode *current = root;
    
    while (current) {
        if (current->leftchild != NULL){               // current的leftchild沒有空位
            q.push(current->leftchild);                // 將其推進queue中
        }
        else{                                          // current的leftchild有空位
            TreeNode *new_node = new TreeNode(data);   // 建立新的node, 將字母放在這裡
            new_node->parent = current;
            current->leftchild = new_node;
            break;                         
        }
        if (current->rightchild != NULL) {             // current的rightchild沒有空位
            q.push(current->rightchild);               // 將其推進queue中
        }
        else{                                          // current的rightchild有空位
            TreeNode *new_node = new TreeNode(data);   // 建立新的node, 將字母放在這裡
            new_node->parent = current;                
            current->rightchild = new_node;
            break;
        }
        current = q.front();
        q.pop();
    }
}
TreeNode* BinaryTree::leftmost(TreeNode *current){
    while (current->leftchild != NULL){
        current = current->leftchild;
    }
    return current;
}
TreeNode* BinaryTree::InorderSuccessor(TreeNode *current){
    if (current->rightchild != NULL){
        return leftmost(current->rightchild);
    }
    
    // 利用兩個pointer: successor與current做traversal 
    TreeNode *successor = current->parent;   
    while (successor != NULL && current == successor->rightchild) {
        current = successor;
        successor = successor->parent;
    }
    return successor;
}
void BinaryTree::Inorder_by_parent(){
    TreeNode *current = new TreeNode;
    current = leftmost(root);

    while(current){
        std::cout << current->data << " ";
        current = InorderSuccessor(current);
    }
}
int main() {
    const char *a = "A B C D E F x x x G H x I";
    BinaryTree T(a);
    T.Inorder_by_parent();
    std::cout << std::endl;

    T.InsertLevelorder('K');
    T.InsertLevelorder('L');
    T.InsertLevelorder('M');
    T.InsertLevelorder('N');
    T.Inorder_by_parent();
    std::cout << std::endl;

    return 0;
}
