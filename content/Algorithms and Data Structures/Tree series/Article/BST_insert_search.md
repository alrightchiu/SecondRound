Title: Binary Search Tree: Search(搜尋資料)、Insert(新增資料)    
Date: 2016-1-3 20:17  
Category: 演算法與資料結構  
Tags: C++, Binary Search Tree    
Summary: 介紹Binary Search Tree(二元搜尋樹)的基本操作：Search(搜尋資料)與Insert(新增資料)。
 

</br>
###先備知識與注意事項

(完整範例程式碼也可以看這裡：[BST.cpp](https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/ExampleCode/BST_Search_Insert.cpp))


在開始介紹search(搜尋資料)與insert(新增資料)之前，先定義好`class TreeNode`與`class BST`，順便對未來將介紹的其他member function(成員函式)留下美好的第一印象：

```cpp
// C++ code
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
public:
    BST():root(0){};	
    
    TreeNode* Search(int key);
    void InsertBST(int key, string element);
    void InorderPrint();        // 可以用來確認BST是否建立成功
    void Levelorder();          // 可以確認BST是否建立成功
};

```

小小備註：範例程式碼只是其中一種可行的方法，實作方法並不唯一，筆者相信有更優秀的寫法(有效利用記憶體、避免memory leak等議題)，建議讀者可以多多參考例如[Stack Exchange:Code Review](http://codereview.stackexchange.com/)等等眾多優秀的網站，看網友的程式碼的寫法以及由該份程式碼所開啟的討論串，應該會對實際寫作技巧有些幫助。  


***
  
##目錄

* [BST::Search(搜尋)](#search)
* [BST::InsertBST(新增資料)](#insert)
* [參考資料](#ref)
* [BST系列文章](#tree_series)

</br>

<a name="search"></a>

##BST::Search(搜尋)

BST的`Search()`操作，便是根據BST的特徵：Key(L)<Key(Current)<Key(R)，判斷`Current`應該往left subtree走，還是往right subtree走。

現有一棵BST如圖一(a)所示：

<center>
![bst][f1]

**圖一(a)：。**  
</center>   

搜尋結果可能成功，可能失敗，以下便分別以兩個KEY值作說明。

###搜尋成功

若現在要從BST中搜尋基紐隊長，便以基紐隊長的KEY($627$)進入BST。  

進入BST後，便把用來移動的`Current`指向`root`，如圖一(b)。  

<center>
![bst][f2]

**圖一(b)：。**  
</center> 

接著將KEY($627$)和比克(`root`)的戰鬥力($513$)比較，結果是基紐隊長戰勝，因此，基紐隊長如果在BST裡面，應該會長在比克的right subtree裡面，於是便將`Current`往比克的right child(達爾)移動，如圖一(c)。

<center>
![bst][f3]

**圖一(c)：。**  
</center> 

將`Current`移動到達爾之後，再將KEY($627$)與達爾的戰鬥力($524$)比較，結果仍然是基紐隊長大勝，因此步驟同上，繼續將`Current`往達爾的right child(弗力札)移動，如圖一(d)。

<center>
![bst][f4]

**圖一(d)：。**  
</center> 

將`Current`移動到弗力札之後，再將KEY($627$)與弗力札的戰鬥力($709$)比較，結果是弗力札略勝，於是便往弗力札的left child尋找基紐隊長，如圖一(e)。

<center>
![bst][f5]

**圖一(e)：。**  
</center>

此時，`Current`的Key($627$)與傳送進`Search()`的KEY($627$)相同，便確認`Current`即為基紐隊長，於是跳出`while`迴圈，並傳回`Current`。  

宣告搜尋成功。


###搜尋失敗

若現在要從BST中尋找克林，便以克林的戰鬥力($2$)作為KEY($2$)，進入`Search()`。  

進入BST後，同樣把用來移動的`Current`指向`root`，如圖一(b)。

<center>
![bst][f2]

**圖一(b)：。**  
</center> 

接著便將KEY($2$)和比克的戰鬥力($513$)比較，結果是比克勝出，如果克林($2$)在這棵BST中，應該會長在比克的left subtree上，於是將`Currnet`往比克的left child(龜仙人)移動，如圖一(f)。

<center>
![bst][f6]

**圖一(f)：。**  
</center> 

將`Current`移動至龜仙人後，將KEY($2$)和龜仙人的戰鬥力($8$)比較，便判斷出，要將`Current`往龜仙人的left child移動，如圖一(f)。  

然而，由於龜仙人沒有left child，於是`Current`指向`NULL`，便跳出迴圈，並回傳`NULL`，即表示搜尋失敗，克林不在BST中。

以下是`BST::Search()`的範例程式碼，其中，有兩種情況會跳出`while`迴圈：

1. `Current`移動到`NULL`，表示搜尋失敗。
2. KEY與`Current`的key相同，表示搜尋成功；

這兩種情況作為條件式(condition)的先後順序很重要，因為如果`Current`是`NULL`，便不能對其`key`做存取，會產生諸如**BAD_ACCESS**的錯誤(error)。

```cpp
// C++ code
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
```


</br> 

<a name="insert"></a>

##BST::InsertBST(新增資料)

函式`InsertBST()`的概念，可以視為`Search()`的延伸：

1. 根據BST對Key之規則，先找到將要新增之node「適合的位置」；
2. 再將欲新增的node接上BST。

要尋找「對新增node而言的適當位置」，需要召喚一位「哨兵」先行探路，而「將會成為新增node的**parent node**(準新手爸媽)」的那個node，則跟著「哨兵」的腳步，往前推進。 
 
定義「哨兵」為**x**，「準新手爸媽」為**y**，現欲新增「比克(513)」進入圖二(a)的BST。  
(這裡的「哨兵**x**」具有`BST::Search()`中`Current`的功能。)

<center>
![bst][f7]

**圖二(a)：。**  
</center> 

如圖二(a)，剛進入BST時，「哨兵**x**」進到`root`，而「準新手爸媽**y**」設為`root`的parent，即為`NULL`。  

接著，將欲新增node之Key(比克($513$))與「哨兵**x**」之Key(龜仙人($8$))相比，比克的戰鬥力比龜仙人高，所以比克應該要長在龜仙人的right subtree，因此把「哨兵**x**」往龜仙人的right child(悟空)移動，並且更新「準新手爸媽**y**」為龜仙人，如圖二(b)。

* 若這棵BST裡沒有悟空($1000$)長在龜仙人的right child位置，那麼比克($513$)就會變成龜仙人的right child，所以稱龜仙人是「準新手爸媽」。

<center>
![bst][f8]

**圖二(b)：。**  
</center> 

接著，繼續比較欲新增node之Key(比克($513$))與「哨兵**x**」之Key(悟空($1000$))，結果是悟空的戰鬥力較高，比克應該要長在悟空的left subtree，因此，將「哨兵**x**」往悟空的left child(`NULL`)移動，同時更新「準新手爸媽**y**」為悟空，如圖二(c)。

更新後，「準新手爸媽**y**」成為悟空，「哨兵**x**」指向`NULL`壯烈犧牲，即達到跳出迴圈之條件。此時，便找到了「新增node」之適當位置。  

那個「適當位置」在哪裡呢？就是「準新手爸媽**y**」的child pointer。表示比克($513$)一定會是悟空($1000$)的child。

<center>
![bst][f9]

**圖二(c)：。**  
</center> 

下一步，便是比較欲新增node之Key(比克(513))與「準新手爸媽**y**」之Key(悟空(1000))來判斷要接在left child還是right child的位置。  
比較發現悟空戰鬥力較高，因此，比克(513)便成為「準新手爸媽**y**」的left child，如圖二(d)，便成功把比克(513)接到BST上。

如此便完成了於BST中新增資料。

<center>
![bst][f10]

**圖二(d)：。**  
</center> 

以下是`InsertBST()`的範例程式碼，關鍵便是「哨兵**x**」與「準新手爸媽**y**」的冒險之旅：


```cpp
// C++ code
void BST::InsertBST(int key, string element){

    TreeNode *y = 0;        // 準新手爸媽
    TreeNode *x = 0;        // 哨兵
    // call default copy constructor of TreeNode
    TreeNode *insert_node = new TreeNode(key, element);   // insert_node為將要新增的node

    x = root;
    while (x != NULL) {                 // 在while中, 以如同Search()的方式尋找適當的位置       
        y = x;                          // y先更新到原來x的位置
        if (insert_node->key < x->key){ // 判斷x是要往left- 還是right- 前進
            x = x->leftchild;
        }
        else{
            x = x->rightchild;
        }
    }                                   // 跳出迴圈後, x即為NULL
                                        // y即為insert_node的parent
    insert_node->parent = y;            // 將insert_node的parent pointer指向y

    if (y == NULL){                     // 下面一組if-else, 把insert_node接上BST
        this->root = insert_node;
    }
    else if (insert_node->key < y->key){
        y->leftchild = insert_node;
    }
    else{
        y->rightchild = insert_node;
    }
}
```



有了`BST::InsertBST()`後，就可以用土法煉鋼的方式建立一棵如圖二(d)的BST，再以[]()介紹過的Inorder Traversal與Level-Order Traversal檢驗，順便測試BST中是否有Key($1000$)與Key($73$)這兩筆資料：

```cpp
// C++ code
int main() {
    
    BST T;
    
    T.InsertBST(8,"龜仙人");
    T.InsertBST(1000,"悟空");
    T.InsertBST(2,"克林");
    T.InsertBST(513,"比克");


    cout << "Inorder Traversal:\n";
    T.InorderPrint();
    cout << endl;
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
    
    node = T.Search(73);
    if(node != NULL){
    	cout << "There is " << node->GetElement() << "(" << node->GetKey() << ")" << endl;
    }
    else {
    	cout << "no element with Key(73)" << endl;
    }

    return 0;
}
```

output: 

```cpp
Inorder Traversal:
克林(2) 龜仙人(8) 比克(513) 悟空(1000)
Level-order Traversal:
龜仙人(8) 克林(2) 悟空(1000) 比克(513)

There is 悟空(1000)
no element with Key(73)
```


以上便是BST中`Search()`與`InsertBST()`之介紹，只要掌握BST的性質Key(L)<Key(Current)<Key(R)與樹中的Traversal(pointer的移動)即可輕鬆上路。 



[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/search_insert/f10.png?raw=true


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch12](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch5](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
 

</br>

<a name="tree_series"></a>

###BST系列文章


[Binary Search Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/binary-search-tree-introjian-jie.html)  
[Binary Search Tree: Search(搜尋資料)、Insert(新增資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-searchsou-xun-zi-liao-insertxin-zeng-zi-liao.html)  
[Binary Search Tree: Sort(排序)、Delete(刪除資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-sortpai-xu-deleteshan-chu-zi-liao.html)  


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)



</br>






