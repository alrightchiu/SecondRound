//
// RBT_Insert_Delete.cpp
// C++ code, harshly and successfully compiled by g++
//

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

class RBT;
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    int key;
    string element;
    int color;             // for color: 0: Red, 1: Black
    friend class RBT;
public:
    TreeNode():leftchild(0),rightchild(0),parent(0),key(0),element(""),color(0){}
    TreeNode(int key, string str = ""):leftchild(0),rightchild(0),parent(0),key(key),element(str),color(0){}
    
    int GetKey() const{return key;};
    string GetElement() const{return element;};
};

class RBT{
private:
    TreeNode *root;
    TreeNode *neel;

    void LeftRotation(TreeNode *current);
    void RightRotation(TreeNode *current);
    void InsertFixedUpRBT(TreeNode *current);
    void DeleteFixedUpRBT(TreeNode *current);
    TreeNode* Successor(TreeNode *current);       // called by DeleteRBT()
    TreeNode* Leftmost(TreeNode *current);        // called by Successor()
public:
    RBT(){
        neel = new TreeNode;    // neel 需要配置記憶體
        neel->color = 1;        // neel 是黑色
        root = neel;            // 為了insert, 要把root 初始化成neel
        root->parent = neel;
    };
    
    void InsertRBT(int key, string str);
    void DeleteRBT(int KEY);

    TreeNode* Search(int KEY);   // called by DeleteRBT
    
};

void RBT::DeleteRBT(int KEY){              // 要刪除具有KEY的node
    
    TreeNode *delete_node = Search(KEY);   // 先確認RBT中是否存在具有KEY的node
    if (delete_node == NULL) {
        std::cout << "data not found.\n";
        return;
    }

    TreeNode *y = 0;     // 真正要被刪除並釋放記憶體的node
    TreeNode *x = 0;     // 要被刪除的node的"child"

    if (delete_node->leftchild == neel || delete_node->rightchild == neel){
        y = delete_node;
    }
    else{
        y = Successor(delete_node);         // 將y設成delete_node的Successor
    }                                       // 經過這組if-else, y調整成至多只有一個child
    

    if (y->leftchild != neel){              // 將x設成y的child, 可能有實際資料, 也有可能是NIL
        x = y->leftchild;
    }
    else{
        x = y->rightchild;
    }

    x->parent = y->parent;                 // 即使x是NIL也要把x的parent指向有效的記憶體位置
                                           // 因為在FixUp時需要藉由x->parent判斷x為leftchild或是rightchild

    if (y->parent == neel){                // 接著再把要被釋放記憶體的node之"parent"指向新的child
        this->root = x;                    // 若刪除的是原先的root, 就把x當成新的root 
    }
    else if (y == y->parent->leftchild){   // 若y原本是其parent之left child
        y->parent->leftchild = x;          // 便把x皆在y的parent的left child, 取代y
    }
    else{                                  // 若y原本是其parent之right child
        y->parent->rightchild = x;         // 便把x皆在y的parent的right child, 取代y
    }

    if (y != delete_node) {                // 針對case3
        delete_node->key = y->key;         // 若y是delete_node的替身, 最後要再將y的資料
        delete_node->element = y->element; // 放回delete_node的記憶體位置, 並將y的記憶體位置釋放
    }

    if (y->color == 1) {                   // 若刪除的node是黑色, 要從x進行修正, 以符合RBT的顏色規則
        DeleteFixedUpRBT(x);
    }
}
void RBT::DeleteFixedUpRBT(TreeNode *current){
    // Case0:(i)  current是紅色的, 直接把它塗黑
    //       (ii) current是root, 直接把它塗黑
    while (current != root && current->color == 1) {
        // current是leftchild
        if (current == current->parent->leftchild) {    
            TreeNode *sibling = current->parent->rightchild;
            // Case1: 如果sibling是紅色
            if (sibling->color == 0) {
                sibling->color = 1;
                current->parent->color = 0;
                LeftRotation(current->parent);
                sibling = current->parent->rightchild;
            }
            // 進入 Case2、3、4: sibling是黑色
            // Case2: sibling的兩個child都是黑色
            if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
                sibling->color = 0;
                current = current->parent;           // 若current更新到root, 即跳出迴圈
            }
            // Case3 & 4: current只有其中一個child是黑色
            else {
            	// case3: sibling的right child是黑的, left child是紅色
                if (sibling->rightchild->color == 1){
                    sibling->leftchild->color = 1;
                    sibling->color = 0;
                    RightRotation(sibling);
                    sibling = current->parent->rightchild;
                }
                // 經過Case3後, 一定會變成Case4
                // Case 4: sibling的right child 是紅色的, left child是黑色
                sibling->color = current->parent->color;
                current->parent->color = 1;
                sibling->rightchild->color = 1;
                LeftRotation(current->parent);
                current = root;     // 將current移動到root, 一定跳出迴圈
            }
        }
        // current是rightchild
        else {  
            TreeNode *sibling = current->parent->leftchild;
            // Case1: 如果sibling是紅色
            if (sibling->color == 0) {
                sibling->color = 1;
                current->parent->color = 0;
                RightRotation(current->parent);
                sibling = current->parent->leftchild;
            }
            // 進入 Case2、3、4: sibling是黑色
            // Case2: sibling的兩個child都是黑色
            if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
                sibling->color = 0;
                current = current->parent;             // 若current更新到root, 即跳出迴圈
            }
            // Case3 & 4: current只有其中一個child是黑色
            else {
            	// case3: sibling的left child是黑的, right child是紅色
                if (sibling->leftchild->color == 1){
                    sibling->rightchild->color = 1;
                    sibling->color = 0;
                    LeftRotation(sibling);
                    sibling = current->parent->leftchild;
                }
                // 經過Case3後, 一定會變成Case4
                // Case 4: sibling的left child 是紅色的, rightt child是黑色
                sibling->color = current->parent->color;
                current->parent->color = 1;
                sibling->leftchild->color = 1;
                RightRotation(current->parent);
                current = root;     // 將current移動到root, 一定跳出迴圈
            }
        }
    }
    current->color = 1;
}
void RBT::InsertRBT(int key, string str){
    // 前半部與 InsertBST()之邏輯完全相同, 僅僅需要修改 NULL <-> NIL
    TreeNode *y = neel;
    TreeNode *x = root;
    TreeNode *insert_node = new TreeNode(key, str);

    while (x != neel) {     // 把root初始化成neel, 這裡就可以用neel來做判斷
        y = x;
        if (insert_node->key < x->key){
            x = x->leftchild;
        }
        else{
            x = x->rightchild;
        }
    }

    insert_node->parent = y;

    if (y == neel){
        this->root = insert_node;
    }
    else if (insert_node->key < y->key){
        y->leftchild = insert_node;
    }
    else{
        y->rightchild = insert_node;
    }
    
    // 以下是對RBT之node的設定, 將child pointer指向NIL, 顏色設為紅色
    insert_node->leftchild = neel;
    insert_node->rightchild = neel;
    insert_node->color = 0;             // 顏色可以在constructor中預設
        
    InsertFixedUpRBT(insert_node);      // 對可能出現紅色與紅色node相連之情形做修正
}

void RBT::InsertFixedUpRBT(TreeNode *current){

    // case0: parent是黑色, 就不用進回圈
    while (current->parent->color == 0) {   // 若parent是紅色即進入迴圈

        // 上半部：parent是grandparent的left child
        if (current->parent == current->parent->parent->leftchild) { 
            TreeNode *uncle = current->parent->parent->rightchild;
            // case1: 若uncle是紅色
            if (uncle->color == 0) {
                current->parent->color = 1;
                uncle->color = 1;
                current->parent->parent->color = 0;              //grandparent改成紅色
                current = current->parent->parent;
            }
            // case2 & 3: uncle是黑色
            else {  
                if (current == current->parent->rightchild){     // case2
                    current = current->parent;
                    LeftRotation(current);
                }
                // case3
                current->parent->color = 1;                      //把parent塗黑
                current->parent->parent->color = 0;              // grandparent塗紅
                RightRotation(current->parent->parent);
            }
        }
        // 下半部：parent是grandparent的right child, 與上半部對稱
        else {  
            TreeNode *uncle = current->parent->parent->leftchild;
            // case1: 若uncle是紅色
            if (uncle->color == 0) {
                current->parent->color = 1;
                uncle->color = 1;
                current->parent->parent->color = 0;              //grandparent改成紅色
                current = current->parent->parent;
            }
            // case2 & 3: uncle是黑色
            else {
                if (current == current->parent->leftchild) {     // case2
                    current = current->parent;
                    RightRotation(current);
                }
                // case3
                current->parent->color = 1;
                current->parent->parent->color = 0;
                LeftRotation(current->parent->parent);
            }
        }
    }
    root->color = 1;    // 確保root是黑色
}

void RBT::LeftRotation(TreeNode *x){

    TreeNode *y = x->rightchild;         // 把y指向x的rightchild, 最後y會變成x的parent

    x->rightchild = y->leftchild;        // 圖二(c)左, 把y的leftchild托在x的rightchild

    if (y->leftchild != neel){           // 圖二(c)右, 若node(j)為NIL則忽略
        y->leftchild->parent = x;        // 將原先y的leftchild的parent改成x
    }

    y->parent = x->parent;               // 圖二(d)左, 更新y的parent

    if (x->parent == neel){              // 圖二(d)右, 若原先x是root, 旋轉後y變成新的root
        root = y;          
    }         
    else if (x == x->parent->leftchild){ // 若原先x是其parent的leftchild
        x->parent->leftchild = y;        // 更新後y也是其parent的leftchild
    }
    else{                                // 若原先x是其parent的rightchild
        x->parent->rightchild = y;       // 更新後y也是其parent的rightchild
    }
    y->leftchild = x;                    // 圖二(e)左, 最後才修改y與x
    x->parent = y;                       // 圖二(e)右
}
void RBT::RightRotation(TreeNode *y){

    TreeNode *x = y->leftchild;           // 把x設成y的leftchild

    y->leftchild = x->rightchild;         // 把x的rightchild放到y的leftchild    
    if (x->rightchild != neel){           // 若x的rightchild不為NIL, 將其parent指向y
        x->rightchild->parent = y;
    }
    x->parent = y->parent;                // 將x的parent指向原先y的parent
                                          // 以下一組if-else將修改原先y的parent之child
    if (y->parent == neel){               // 若y原先是root, x將成為新的root
        root = x;          
    }              
    else if (y == y->parent->leftchild){  // 若原先y是其parent之leftchild, 
        y->parent->leftchild = x;         // x就成為其新的parent之leftchild
    }
    else{                                 // 若原先y是其parent之rightchild, 
        y->parent->rightchild = x;        // x就成為其新的parent之rightchild
    }
    x->rightchild = y;                    // 將y設為x之rightchild
    y->parent = x;                        // 將x設為y之parent
}

TreeNode* RBT::Leftmost(TreeNode *current){
    
    while (current->leftchild != NULL){
        current = current->leftchild;
    }
    return current;
}
TreeNode* RBT::Successor(TreeNode *current){
    
    if (current->rightchild != NULL){
        return Leftmost(current->rightchild);
    }
    
    TreeNode *new_node = current->parent;
    
    while (new_node != NULL && current == new_node->rightchild) {
        current = new_node;
        new_node = new_node->parent;
    }
    
    return new_node;
}
TreeNode* RBT::Search(int KEY){

    TreeNode *current = root;               // 將curent指向root作為traversal起點
         
    while (current != NULL && KEY != current->key) {  // 兩種情況跳出迴圈：
    	                                              // 1.沒找到 2.有找到
        if (KEY < current->key){                      
            current = current->leftchild;   // 向左走
        }
        else {
            current = current->rightchild;  // 向右走
        }
    }
    return current;
}