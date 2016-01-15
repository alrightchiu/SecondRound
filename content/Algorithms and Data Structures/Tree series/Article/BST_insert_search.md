Title: Binary Search Tree: Search(搜尋資料)、Insert(新增資料)    
Date: 2016-1-3 20:17  
Category: 演算法與資料結構  
Tags: C++  
Summary: 介紹Binary Search Tree的基本操作：Search(搜尋資料)與Insert(新增資料)。
 

</br>
######**先備知識與注意事項**

在開始介紹search(搜尋資料)與insert(新增資料)之前，先定義好`class TreeNode`與`class BST`，順便對未來將介紹的其他member function(成員函式)留下美好的第一印象：

```cpp
// C++ code
#include <iostream>
#include <string>
class BST;
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    int key;
    std::string element;
public:
    // constructor
    TreeNode():key(0), element(""){
        leftchild = 0; rightchild = 0; parent = 0;
    }
    TreeNode(int a, std::string b):key(a), element(b){
        leftchild = 0; rightchild = 0; parent = 0;
    }
    // default copy constructor
    // default destructor
    
    int GetKey() const{ return key;};
    std::string GetElement() const{ return element;};
    
    void SetKey(int k){ key = k;};
    void SetElement(std::string e){ element = e;};   
    
    friend class BST;   // 放在 private 或 public 都可以 
};

// class BST
class BST{
private:
    TreeNode *root;
    TreeNode* Rightmost(TreeNode *current);
    TreeNode* Leftmost(TreeNode *current);
    TreeNode* Copy(const TreeNode *origNode);    // preorder traversal, 用在 copy constructor和 operator=
    void PostorderDelete(TreeNode *current);
public:
    BST(){ root = 0; };		// default constructor
    BST(const BST &p);		// copy constructor
    BST& operator = (const BST &p);
    ~BST();  // destructor
    
    TreeNode* Search(int key);
    void InsertBST(TreeNode &new_node);
    
    TreeNode* Successor(TreeNode *current);
    TreeNode* Predecessor(TreeNode *current);
    void InorderPrint();
    void DeleteBST(int KEY);
    
    bool IsEmpty() const{return (root==NULL);};    // 確認BST是否存有資料
};

```

文章內容將著重於BST這個資料結構，並提供此資料結構中可行的演算法，因此，有關C++的實作方法並不唯一，筆者相信有更優秀的寫法(有效利用記憶體、避免memory leak(記憶體洩漏)等議題)，建議讀者可以多多參考例如[Stack Exchange:Code Review](http://codereview.stackexchange.com/)等等眾多優秀的網站，看網友的程式碼的寫法以及由該份程式碼所開啟的討論串，應該會對實際寫作技巧有些幫助。  
(筆者也還在學啊啊啊啊)

另外，用以測試的`main()`將在BST系列的演算法都介紹完後登場。

***
  
##目錄

* [BST::Search(搜尋)](#search)
* [BST::Insert(新增資料)](#insert)
* [參考資料](#ref)


<a name="search"></a>

##**BST::Search(搜尋)**

BST的`Search()`操作，便是根據BST的特徵：$Key(L)<Key(Current)<Key(R)$，判斷`Current`node應該往left subtree走，還是往right subtree走。

現有一棵BST如圖一(a)所示：

  
<center>
![bst][search0]

**圖一(a)：。**  
</center>   

搜尋結果可能成功，可能失敗，以下便分別以兩個KEY值作說明。

####搜尋成功

* 若現在要從BST中搜尋基紐隊長，便以基紐隊長的KEY(627)進入BST。  
進入BST後，便把用來移動的`Current`node指向`root`，如圖一(b)。  

<center>
![bst][search1]

**圖一(b)：。**  
</center> 

* 此時，便將KEY(627)和比克(`root`)的戰鬥力(513)比較，結果是基紐隊長戰勝，因此，基紐隊長如果在BST裡面，應該會長在比克的right subtree，於是便將`Current`往比克的right child(達爾)移動，如圖一(c)。

<center>
![bst][search2]

**圖一(c)：。**  
</center> 

* 將`Current`移動到達爾之後，再將KEY(627)與達爾的戰鬥力(524)比較，結果仍然是基紐隊長大勝，因此步驟同上，繼續將`Current`往達爾的right child(弗力札)移動，如圖一(d)。

<center>
![bst][search3]

**圖一(d)：。**  
</center> 

* 將`Current`移動到弗力札之後，再將KEY(627)與弗力札的戰鬥力(709)比較，結果是弗力札略勝，於是便往弗力札的left child尋找基紐隊長，如圖一(e)。

<center>
![bst][search4]

**圖一(e)：。**  
</center>

* 此時，`Current`的Key(627)與傳送進`Search()`的KEY(627)相同，便確認`Current`即為基紐隊長，於是跳出`while`迴圈，並傳回`Current`。  
即搜尋成功。


####搜尋失敗

* 若現在要從BST中尋找克林，便以克林的戰鬥力(2)為KEY(2)，進入`Search()`。  
進入BST後，同樣把用來移動的`Current`node指向`root`，如圖一(b)。

<center>
![bst][search1]

**圖一(b)：。**  
</center> 

* 接著便將KEY(2)和比克的戰鬥力(513)比較，結果是比克勝出，於是將`Currnet`往比克的left child(龜仙人)移動，如圖一(f)。

<center>
![bst][search5]

**圖一(f)：。**  
</center> 

* 將`Current`移動至龜仙人後，將KEY(2)和龜仙人的戰鬥力(8)比較，便判斷出，要將`Current`往龜仙人的left child移動，如圖一(f)。  
然而，由於龜仙人沒有left child，於是`Current`指向`NULL`，便跳出迴圈，並回傳`NULL`，即表示搜尋失敗，克林不在BST中。

以下是`BST::Search()`的範例程式碼，其中，有兩種情況會跳出`while`迴圈：

1. KEY與`Current`node的key相同，表示搜尋成功；
2. `Current`移動到`NULL`，表示搜尋失敗。

```cpp
// C++ code
TreeNode* BST::Search(int KEY){
    TreeNode *current = new TreeNode;
    current = root;
    while (current != NULL && KEY != current->GetKey()) {
        if (KEY < current->GetKey())
            current = current->leftchild;   // 向左走
        else
            current = current->rightchild;  // 向右走
    }
    return current;
}
```
</br> 
<a name="insert"></a>

##**BST::InsertBST(新增資料)**

函式`InsertBST()`的演算法概念，可以視為`Search()`的延伸：

1. 根據BST對Key之規則，先找到「將要新增之node」適合的位置；
2. 再將欲新增的node接上BST。

要尋找「對新增node而言的適當位置」，需要召喚一位「哨兵」先行探路，而「將會成為新增node的**parent node**(準新手爸媽)」則跟著「哨兵」的腳步，往前推進。 
 
定義「哨兵」為**x**，「準新手爸媽」為**y**，現欲新增「比克，戰鬥力(513)」進入如圖二(a)之BST。  
(這裡的「哨兵**x**」具有`BST::Search()`中`Current`node的功能。)


<center>
![bst][insert1]

**圖二(a)：。**  
</center> 

* 如圖二(a)，剛進入BST時，「哨兵**x**」進到`root`，而「準新手爸媽**y**」即為`root`的parent，即為`NULL`。  
* 接著，將欲新增node之Key(比克(513))與「哨兵**x**」之Key(龜仙人(8))相比，比克的戰鬥力比龜仙人高，所以比克應該要長在龜仙人的right subtree，因此把「哨兵**x**」往龜仙人的right child(悟空)移動，並且更新「準新手爸媽**y**」為龜仙人，如圖二(b)。

<center>
![bst][insert2]

**圖二(b)：。**  
</center> 

* 接著，繼續比較欲新增node之Key(比克(513))與「哨兵**x**」之Key(悟空(1000))，結果是悟空的戰鬥力較高，比克應該要長在悟空的left subtree，因此，將「哨兵**x**」往悟空的left child(`NULL`)移動，同時更新「準新手爸媽**y**」為悟空，如圖二(c)。
* 更新後，「準新手爸媽**y**」成為悟空，「哨兵**x**」指向`NULL`壯烈犧牲，即達到跳出迴圈之條件。此時，便找到了「新增node」之適當位置。  
那個「適當位置」在哪裡呢？就是「準新手爸媽**y**」的child pointer。

<center>
![bst][insert3]

**圖二(c)：。**  
</center> 

* 下一步，便是比較欲新增node之Key(比克(513))與「準新手爸媽**y**」之Key(悟空(1000))，發現悟空戰鬥力較高，因此，比克(513)便成為「準新手爸媽**y**」的left child，如圖二(d)，便成功把比克(513)接到BST上。

<center>
![bst][insert4]

**圖二(d)：。**  
</center> 

以下是`BST::InsertBST()`的範例程式碼，關鍵便是「哨兵**x**」與「準新手爸媽**y**」的冒險之旅：

```cpp
// C++ code
void BST::InsertBST(TreeNode &new_node){
    TreeNode *y = new TreeNode; y = 0;
    TreeNode *x = new TreeNode; x = 0;
    TreeNode *insert_node = new TreeNode(new_node); // call default copy constructor of TreeNode
    
    x = root;
    while (x != NULL) {
        y = x;
        if (insert_node->GetKey() < x->GetKey())
            x = x->leftchild;
        else
            x = x->rightchild;
    }
    insert_node->parent = y;
    if (y == NULL)
        this->root = insert_node;
    else if (insert_node->GetKey() < y->GetKey())
        y->leftchild = insert_node;
    else
        y->rightchild = insert_node;
}
```
備註：  

* 在定義函式`InsertBST()`時，函式的參數(argument)可能會視情境而有所改變，這裡是以一個`TreeNode`的物件(object)之**reference**作為參數，傳進函式`InsertBST()`。
* 在`InsertBST()`特別標示出BST是為了與之後會介紹的Red Black Tree(紅黑樹)之`InsertRBT()`做區別。

[search0]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f18.png?raw=true
[search1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f19.png?raw=true
[search2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f20.png?raw=true
[search3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f21.png?raw=true
[search4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f22.png?raw=true
[search5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f23.png?raw=true
[insert1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f24.png?raw=true
[insert2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f25.png?raw=true
[insert3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f26.png?raw=true
[insert4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f27.png?raw=true

</br>

以上便是BST中`Search()`與`InsertBST()`之介紹，只要掌握BST的性質$Key(L)<Key(Current)<Key(R)$與樹中的Traversal(pointer的移動)即可輕鬆上路。  


</br>
<a name="ref"></a>

######**參考資料**：

* [Introduction to Algorithms](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
 

</br>






