//
// BST_Search_Insert.cpp
// C++ code, harshly and successfully compiled by g++
//


#include <iostream>
#include <string>
#include <queue>

using std::string;
using std::cout;
using std::endl;

class BST;
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    int key;
    string element;
public:
    TreeNode():leftchild(0),rightchild(0),parent(0),key(0),element(""){};
    TreeNode(int a, string b):leftchild(0),rightchild(0),parent(0),key(a),element(b){};

    int GetKey(){return key;}              // 為了在main()要能夠檢視node是否正確
    string GetElement(){return element;}   // 才需要這兩個member function讀取private data

    // 其餘情況, 因為class BST是class TreeNode的friend class
    // 在class BST的member function中, 可以直接存取class TreeNode的private data

    friend class BST;   // 放在 private 或 public 都可以 
};

class BST{
private:
    TreeNode *root;
    TreeNode* Leftmost(TreeNode *current);
    TreeNode* Successor(TreeNode *current);
    TreeNode* Rightmost(TreeNode *current);
    TreeNode* Predecessor(TreeNode *current);
public:
    BST():root(0){};

    TreeNode* Search(int key);
    void InsertBST(int key, string element);
    void InorderPrint();        // 可以用來確認BST是否建立成功
    void Levelorder();          // 可以確認BST是否建立成功
    void DeleteBST(int key);
};

TreeNode* BST::Search(int KEY){

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

void BST::InsertBST(int key, string element){

    TreeNode *y = 0;        // 準新手爸媽
    TreeNode *x = 0;        // 哨兵
    TreeNode *insert_node = new TreeNode(key, element); 

    x = root;
    while (x != NULL) {                // 在while中, 以如同Search()的方式尋找適當的位置       
        y = x;                                
        if (insert_node->key < x->key){
            x = x->leftchild;
        }
        else{
            x = x->rightchild;
        }
    }                                  // 跳出迴圈後, x即為NULL
                                       // y即為insert_node的parent
    insert_node->parent = y;           // 將insert_node的parent pointer指向y

    if (y == NULL){                    // 下面一組if-else, 把insert_node接上BST
        this->root = insert_node;
    }
    else if (insert_node->key < y->key){
        y->leftchild = insert_node;
    }
    else{
        y->rightchild = insert_node;
    }
}
TreeNode* BST::Rightmost(TreeNode *current){

    while (current->rightchild != NULL){
        current = current->rightchild;
    }
    return current;
}
TreeNode* BST::Predecessor(TreeNode *current){
    if (current->leftchild != NULL){
        return Rightmost(current->leftchild);
    }

    TreeNode *new_node = current->parent;

    while (new_node != NULL && current == new_node->leftchild) {
        current = new_node;
        new_node = new_node->parent;
    }
    return new_node;
}
TreeNode* BST::Leftmost(TreeNode *current){
    
    while (current->leftchild != NULL){
        current = current->leftchild;
    }
    return current;
}
TreeNode* BST::Successor(TreeNode *current){
    
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
void BST::InorderPrint(){
    TreeNode *current = new TreeNode;
    current = Leftmost(root);
    while(current){
        cout << current->element << "(" << current->key << ")" << " ";
        current = Successor(current);
    }
}

void BST::Levelorder(){
    
    std::queue<TreeNode*> q;
    q.push(this->root);					    // 把root作為level-order traversal之起點
    								        // 推進queue中
    while (!q.empty()){                     // 若queue不是空的, 表示還有node沒有visiting

    	TreeNode *current = q.front();      // 取出先進入queue的node
    	q.pop();                          
    	cout << current->element << "(" << current->key << ")" << " ";   

    	if (current->leftchild != NULL){    // 若leftchild有資料, 將其推進queue
            q.push(current->leftchild);
        }
        if (current->rightchild != NULL){   // 若rightchild有資料, 將其推進queue
            q.push(current->rightchild);
        }
    }
}

void BST::DeleteBST(int KEY){               // 要刪除具有KEY的node

    TreeNode *delete_node = Search(KEY);    // 先確認BST中是否有具有KEY的node
    if (delete_node == NULL) {
        std::cout << "data not found.\n";
        return;
    }
    
    TreeNode *y = 0;      // 真正要被刪除並釋放記憶體的node
    TreeNode *x = 0;      // 要被刪除的node的"child"

    if (delete_node->leftchild == NULL || delete_node->rightchild == NULL){
        y = delete_node;
    }
    else{
        y = Successor(delete_node);        // 將y設成delete_node的Successor                          
    }                                      // 經過這組if-else, y調整成至多只有一個child
                                           // 全部調整成case1或case2來處理
    if (y->leftchild != NULL){              
        x = y->leftchild;                  // 將x設成y的child, 可能是有效記憶體,
    }                                      // 也有可能是NULL
    else{
        x = y->rightchild;
    }
    
    if (x != NULL){                        // 在y被刪除之前, 這個步驟把x接回BST 
        x->parent = y->parent;             // 此即為圖二(c)中, 把基紐接回龜仙人的步驟
    }
                                           // 接著再把要被釋放記憶體的node之"parent"指向新的child
    if (y->parent == NULL){                // 若刪除的是原先的root, 就把x當成新的root 
        this->root = x;
    }
    else if (y == y->parent->leftchild){    // 若y原本是其parent之left child
        y->parent->leftchild = x;           // 便把x皆在y的parent的left child, 取代y
    }
    else{                                   // 若y原本是其parent之right child
        y->parent->rightchild = x;          // 便把x皆在y的parent的right child, 取代y
    }                                       
                                            // 針對case3
    if (y != delete_node) {                 // 若y是delete_node的替身, 最後要再將y的資料
        delete_node->key = y->key;          // 放回delete_node的記憶體位置, 並將y的記憶體位置釋放
        delete_node->element = y->element;  // 圖二(d), y即是達爾, delete_node即是西魯
    }

    delete y;                               // 將y的記憶體位置釋放
    y = 0;
}

int main() {
    
    BST T;
    
    T.InsertBST(8,"龜仙人");
    T.InsertBST(1000,"悟空");
    T.InsertBST(2,"克林");
    T.InsertBST(513,"比克");


    cout << "Inorder Traversal:\n";
    T.InorderPrint();
    cout << endl << endl;
    cout << "Level-order Traversal:\n";
    T.Levelorder();
    cout << endl << endl;
    
    T.DeleteBST(8);       // 刪除龜仙人, 確認比克(513)會成為新的root
    cout << "Level-order Traversal:\n";
    T.Levelorder();
    cout << endl << endl;

    TreeNode *node = T.Search(1000);
    if(node != NULL){
    	cout << "There is " << node->GetElement() << "(" << node->GetKey() << ")" << endl;
    }
    else {
    	cout << "no element with Key(1000)" << endl;
    }
    
    node = T.Search(8);
    if(node != NULL){
    	cout << "There is " << node->GetElement() << "(" << node->GetKey() << ")" << endl;
    }
    else {
    	cout << "no element with Key(8)" << endl;
    }

    return 0;
}



