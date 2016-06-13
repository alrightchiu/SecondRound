Title: Binary Tree: Traversal(尋訪)    
Date: 2015-12-24 22:41  
Category: 演算法與資料結構  
Tags: C++, Binary Tree  
Summary: 介紹Binary Tree(二元樹)中的Traversal(尋訪)。
 


</br>
###先備知識與注意事項


在Linked list與Tree中的traversal中對於pointer的操作，在概念上完全相同，不過由於Node的pointer增加了，於是從一維的移動拓展到二維的移動。  
建議讀者可以先閱讀[Linked List: 新增資料、刪除資料、反轉](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao-fan-zhuan.html#print)作簡單複習。  

本篇文章將介紹在Binary Tree中的四種traversal方法。  

程式實作的部分，除了**遞迴(recursion)**，還有可能會使用上Stack(堆疊)與Queue(佇列)，如果不太熟悉，請參考：

* [Stack: Intro(簡介)](http://alrightchiu.github.io/SecondRound/stack-introjian-jie.html)
* [Queue: Intro(簡介)，並以Linked list實作](http://alrightchiu.github.io/SecondRound/queue-introjian-jie-bing-yi-linked-listshi-zuo.html)


***  
  
##目錄

* [Traversal in Binary Tree](#bttraversal)
* [Example with Code](#ex_code)
    * [Pre-Order Traversal](#pre)
    * [In-Order Traversal](#in)
    * [Post-Order Traversal](#post)
    * [Level-Order Traversal](#level)
* [In-Order Traversal by Parent Field](#in_parent)
    * [Successor、leftmost](#successor)
    * [Predecessor、rightmost](#predecessor)
* [參考資料](#ref)
* [Binary Tree系列文章](#tree_series)

</br>

<a name="bttraversal"></a>

##Traversal in Binary Tree


Traversal(尋訪)有「站在A地，往所有與A地相連的地方移動」的意思：  

* 以Graph(圖)的語言來說，站在vertex(A)上，有一條edge連結vertex(A)與vertex(B)，若能夠由A往B移動，此即可視為traversal；
* 在以pointer實現之Linked list和Tree中，站在node(A)上，並且node(A)具有指向node(B)之pointer，便能夠由A往B移動，此即可視為traversal。

移動到特定的node之後，通常伴隨著其他行為，例如print out(顯示資料)、assign(賦值)、刪除資料等等，這些操作又稱作**Visiting**。

Binary Tree的Node具有兩個指向child的pointer，traversal以「當前所在的node」為參考點，所能夠進行的行為有三種：

* **V**：Visiting，對當前所在的node進行print、assign或其他操作。
* **L**：移動到left child。
* **R**：移動到right child。


<center>
![VLR][f1]

**圖一：CurrentNode位在A，leftchild與rightchild分別為B與C。**  
</center>   



以圖一為例，假設現在CurrentNode位在A，leftchild與rightchild分別為B與C，並加上一項限制：「L一定在R之前」，便能產生三種相對關係：


<center>
![VLR_pre][f2]  

 **圖二(a)-(c) 依序為： (a)pre-order：VLR、(b)in-order：LVR、(c)post-order：LRV**
</center>

**pre-order(VLR)**：當CurrentNode移動到A時，會先對A進行Visiting，接著前往left child進行Visiting，再前往right child進行Visiting。(若child指向NULL則忽略。)

**in-order(LVR)**：當CurrentNode移動到A時，會先對A的left child(B)進行Visiting，接著回到A進行Visiting，再前往right child(C)進行Visiting。(若child指向NULL則忽略。)

**post-order(LRV)**：當CurrentNode移動到A時，會先對A的left child(B)進行Visiting，再前往right child(C)進行Visiting，接著回到A進行Visiting。(若child指向NULL則忽略。)

</br>  
小小備註：

1. 以下圖例中，V表示CurrentNode所在的node，標上數字後表示已經Visiting完成，以print(顯示資料)為例，標上「$1$」表示該node第一個被印出。
2. 以下文字說明，將使用scope(視野範圍)的概念，用來表示以每個CurrentNode(也就是V)為中心，與其所能夠指向之pointer所構成的範圍(等同於「迴圈」或者「函式呼叫」的scope)。因為每個迴圈都會改變CurrentNode(V)的位置，因此scope會以CurrentNode(V)為中心不停移動，直到迴圈/函式結束。

現有一棵樹如圖三(a)，欲進行post-order traversal，並將Visiting用作print(顯示資料)，流程如下：

<center>
![bt_a][f5]

**圖三(a)**  
</center>   


一開始，CurrentNode進到A(root)，按照post-order的順序規則(LRV)，先檢查left child：B是否為NULL，若不是，則先將CurrentNode移動到B(L)：

<center>
![bt_b][f6]

**圖三(b)：scope內：A(V)、B(L)、C(R)。**  
</center>

當CurrentNode移動到B，再一次執行post-order的順序規則(LRV)，檢查left child：D是否為NULL，若不是，則將CurrentNode移動到D(L)：

<center>
![bt_c][f7]

**圖三(c)：scope內：B(V)、D(L)、E(R)。**  
</center>

當CurrentNode移動到D，再一次執行post-order的順序規則(LRV)，檢查出D的left child與right child皆為NULL，表示「LRV」的「L」與「R」都已經執行完畢，便「回到D」做Visiting，在此即印出D(print)。

接著，由於「以D為CurrentNode」形成的scope內之node已經全數Visiting完畢，便可回到「以D之parent作為CurrentNode之scope」，於是將CurrentNode移回B。

回到B的動作發生，即表示：以D為CurrentNode之迴圈或函式已經結束。

<center>
![bt_d][f8]

**圖三(d)：scope內：D(V)。**  
</center>

D已經進行過Visiting，便標上數字「$1$」，表示D為post-order traversal的第一站。  
接著，在「以B為CurrentNode」的scope中，根據post-order規則，繼續往E(R)移動。

<center>
![bt_e][f9]

**圖三(e)：scope內：B(V)、D(L)、E(R)。**  
</center>

進入E後，因為E為leaf node，因此過程如圖三(d)，不會進入NULL。  
在D(L)與E(R)都Visiting過後，便回到B(V)進行Visiting，並標上數字。如此便完成「以B為CurrentNode之scope」內的所有node之Visiting。

接著回到「以A為CurrentNode」的scope。

<center>
![bt_f][f10]

**圖三(f)：scope內：B(V)、D(L)、E(R)。**  
</center>

回到「以A為CurrentNode」的scope後，按照post-order的規則，接著往C(R)移動。

<center>
![bt_g][f11]

**圖三(g)：scope內：A(V)、B(L)、C(R)。**  
</center>

同樣地步驟，再從C移動至F(L)，並發現F為leaf node，於是對F進行Visiting，並標上數字。

<center>
![bt_h][f12]

**圖三(h)：scope內：C(V)、F(L)。**  
</center>

列出F後，發現C的right child指向NULL，於是略過right child(R)，回到C(V)，並對C進行Visiting，標上數字。

<center>
![bt_i][f13]![bt_j][f14]

**圖三(i)-(j)：scope內：C(V)、F(L)。**  
</center>

最後回到「以A為CurrentNode」的scope，對A(V)進行Visiting，便完成了此次post-order traversal，並依序印出`D E B F C A`。

<center>
![bt_k][f15]![bt_l][f16]

**圖三(k)-(l)：scope內：A(V)、B(L)、C(R)。**  
</center>

以上說明了post-order traversal之過程，另外兩種pre-order與in-order在概念上皆相同，只要把握順序規則即可。



</br>

<a name="ex_code"></a>

##Example with Code

接下來，再以一棵稍微複雜的Binary Tree作為範例，展示pre-order、in-order、post-order及level-order之traversal。

現有一棵樹如圖四(a)：

<center>
![ex][f17]

**圖四(a)：。**  
</center>

並以最暴力的方式建立`TreeNode`與`BinaryTree`之物件(object)：

```cpp
// C++ code
#include <iostream>
#include <string>
#include <queue>

class BinaryTree;
class TreeNode{
public:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    std::string str;
    
    TreeNode():leftchild(0),rightchild(0),parent(0){};
    TreeNode(std::string s):str(s),leftchild(0),rightchild(0),parent(0){};
    
    friend class BinaryTree;
};
class BinaryTree{
public:
    TreeNode *root;			// 以root作為存取整棵樹的起點
    BinaryTree():root(0){};
    BinaryTree(TreeNode *node):root(node){};
    
    void Preorder(TreeNode *current);
    void Inorder(TreeNode *current);
    void Postorder(TreeNode *current);
    void Levelorder();
};
// definition of BinaryTree::Preorder()
// definition of BinaryTree::Inorder()
// definition of BinaryTree::Postorder()
// definition of BinaryTree::Levelorder()
int main() {
    // TreeNode instantiation
    TreeNode *nodeA = new TreeNode("A"); TreeNode *nodeB = new TreeNode("B"); 
    TreeNode *nodeC = new TreeNode("C"); TreeNode *nodeD = new TreeNode("D"); 
    TreeNode *nodeE = new TreeNode("E"); TreeNode *nodeF = new TreeNode("F"); 
    TreeNode *nodeG = new TreeNode("G"); TreeNode *nodeH = new TreeNode("H"); 
    TreeNode *nodeI = new TreeNode("I");
    
    // construct the Binary Tree
    nodeA->leftchild = nodeB; nodeA->rightchild = nodeC; 
    nodeB->leftchild = nodeD; nodeB->rightchild = nodeE; 
    nodeE->leftchild = nodeG; nodeE->rightchild = nodeH; 
    nodeC->leftchild = nodeF; nodeF->rightchild = nodeI;
    
    BinaryTree T(nodeA);
    
    T.Preorder(T.root);
    std::cout << std::endl;
    T.Inorder(T.root);
    std::cout << std::endl;
    T.Postorder(T.root);
    std::cout << std::endl;
    T.Levelorder();
    std::cout << std::endl;    
    
    return 0;
}
```
上面的程式碼包含了幾個部分：

* `class TreeNode`的定義；
    * 這裡把所有的data與pointer全部設成「public」裸露在外其實不太好，不過為了要能在`main()`裡土法煉鋼建立出一棵如圖四(a)的Binary Tree，只好將就。
    * 下一篇文章會用比較文明的方式建立Binary Tree，請參考：[Binary Tree: 建立一棵Binary Tree](http://alrightchiu.github.io/SecondRound/binary-tree-jian-li-yi-ke-binary-tree.html)。
* `class BinaryTree`的定義，其中有四個member function分別為四種traversal；
    * 四個函式的定義請繼續看下去。
* main()中建立如圖四(a)的樹，並依序執行四種traversal。
  
其中，pre-order、in-order、post-order traversal的邏輯就只是「V」、「L」、「R」誰先誰後的差別，以下程式碼是以較直覺的遞迴(recursion)形式完成，不過，換成迭代(iteration)配合**Stack(堆疊)**在概念上完全相同，實作上即是考慮「V」、「L」、「R」誰先push(推)進stack。  


</br>  
<a name="pre"></a>

###Pre-Order Traversal

```cpp
// C++ code
void BinaryTree::Preorder(TreeNode *current){
    if (current) {                          // if current != NULL
        std::cout << current->str << " ";   // V
        Preorder(current->leftchild);       // L
        Preorder(current->rightchild);      // R
    }
}
```
output:

```cpp
A B D E G H C F I 
```


<center>
![ex_pre][f18]

**圖四(b)：。**  
</center>

<a name="in"></a>

###In-Order Traversal

```cpp
// C++ code
void BinaryTree::Inorder(TreeNode *current){
    if (current) {                          // if current != NULL
        Inorder(current->leftchild);        // L
        std::cout << current->str << " ";   // V
        Inorder(current->rightchild);       // R
    }
}
```
output:

```cpp
D B G E H A F I C 
```

<center>
![ex_in][f19]

**圖四(c)：。**  
</center>

<a name="post"></a>

###Post-Order Traversal

```cpp
// C++ code
void BinaryTree::Postorder(TreeNode *current){
    if (current) {                         // if current != NULL
        Postorder(current->leftchild);     // L
        Postorder(current->rightchild);    // R
        std::cout << current->str << " ";  // V
    }
}
```
output:

```cpp
D G H E B I F C A 
```

<center>
![ex_post][f20]

**圖四(d)：。**  
</center>

</br>    
<a name="level"></a>

###Level-Order Traversal

Level-order是照著「level由小到大」的順序，由上而下，並在同一個level「由左至右」依序Visiting每個node。

以下提供以迴圈配合**Queue(佇列)**實現level-order traversal之程式碼，其邏輯也非常直觀：

* 以圖四(e)為例，當CurrentNode站在A時，先對A作Visiting，接著檢查是否有left child與right child，若不為NULL，則「依序」將child pointer 推(push)進queue中，又根據queue「先進先出」(first-in-first-out)的特性，先將B(left child)推入queue，再推入C(right child)，便能確保在下一層level時，是由左至右，先Visiting到B，才Visiting到C。

<center>
![ex_level][f21]

**圖四(e)：。**  
</center>

```cpp
// C++ code
void BinaryTree::Levelorder(){

    std::queue<TreeNode*> q;              // using queue
    TreeNode *current = root;
    
    while (current) {                     // if current != NULL
        std::cout << current->str << " ";
        if (current->leftchild)
            q.push(current->leftchild);
        if (current->rightchild)
            q.push(current->rightchild);
        
        if (q.empty())
            break;
        
        current = q.front();
        q.pop();
    }  
}
```
output:

```cpp
A B C D E F G H I
```




</br>

<a name="in_parent"></a>

##In-Order Traversal by Parent Field

在[Binary Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/binary-tree-introjian-jie.html#code)提到，若在`class TreeNode`加入pointer指向其parent node會非常有幫助，其中一項理由正是接下來要介紹的兩個函式：`InorderSuccessor()`與`InorderPredecessor()`。  

說文解字時間：  

* 字首Inorder-，即是按照inorder traversal之規則(LVR)；
* 字尾Successor/ Predecessor，即是「下一個」與「前一個」。

因此，`InorderSuccessor()`與`InorderPredecessor()`便是用來尋找「**以inorder順序**」進行traversal之下一個與前一個node。  

以圖四(c)為例，若CurrentNode站在H(`CurrentNode = nodeH`)，則：

* `CurrentNode = InorderSuccessor(CurrentNode)`會將CurrentNode移動至A；
* `CurrentNode = InorderPredecessor(CurrentNode)`則會將CurrentNode移動至E。

<center>
![ex_in][f19]

**圖四(c)：。**  
</center>

特別介紹inorder，一大原因是為了Binary Search Tree(BST)鋪路，在BST中，照著inorder順序印出node，就會得到「排好序」的資料。

* 詳見：[Binary Search Tree: Sort(排序)、Delete(刪除資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-sortpai-xu-deleteshan-chu-zi-liao.html#binary-search-tree-sortpai-xu-deleteshan-chu-zi-liao)。

另外，若觀察前面提過的遞迴(recursion)形式之inorder traversal，Visiting被包含在遞迴函式內，這表示若要進行多種不同的Visiting，例如print(顯示資料)、assign(賦值、更新資料)，都需要重新寫一個專門功能的遞迴函式。  
顯然，把Visiting和Traversal獨立開來會更有效率。



在看兩個實用的函式之前，有幾件前置作業：  

* 在`class BinaryTree`的定義中加入六個member function(成員函式)：
* 在main()中，把如圖四之Binary Tree的parent pointer建立起來。

```cpp
// C++ code
// inside main()

int main(){

    ...
    // link parent pointer
    nodeB->parent = nodeA; nodeC->parent = nodeA;
    nodeD->parent = nodeB; nodeE->parent = nodeB;
    nodeG->parent = nodeE; nodeH->parent = nodeE;
    nodeF->parent = nodeC; 
    nodeI->parent = nodeF;
    ...

}

// inside definition of class BinaryTree
class BinaryTree{
public:
    TreeNode *root;			// 以root作為存取整棵樹的起點
    BinaryTree():root(0){};
    BinaryTree(TreeNode *node):root(node){};
    
    void Preorder(TreeNode *current);
    void Inorder(TreeNode *current);
    void Postorder(TreeNode *current);
    void Levelorder();

    TreeNode* leftmost(TreeNode *current);
    TreeNode* rightmost(TreeNode *current);

    TreeNode* InorderSuccessor(TreeNode *current);
    TreeNode* InorderPredecessor(TreeNode *current);

    void Inorder_by_parent(TreeNode *root);
    void Inorder_Reverse(TreeNode *root);
};
```

其中包含：

* `InorderSuccessor()`與`InorderPredecessor()`為找到「下一個」與「上一個」的函式主體；
* `leftmost()`和`rightmost()`即是找到Binary Tree整棵樹中「最左」與「最右」的node；
* `Inorder_by_parent()`與`Inorder_Reverse()`將利用`InorderSuccessor()`與`InorderPredecessor()`進行一次In-Order Tarversal。


看下去。  
</br>

<a name="successor"></a>

###Successor、leftmost

函式`leftmost()`的功能為：尋找以`current`為root之subtree中，最左邊的node。

* 以圖四(c)為例，進入以A為root的Binary Tree，`leftmost()`將一路往leftchild前進，便回傳D。
* 而以inorder的順序來說，會找到該subtree中第一個進行Visiting的node。

<center>
![ex_in][f19]

**圖四(c)：。**  
</center>

以下為`leftmost()`的範例程式碼：

```cpp
// C++ code
TreeNode* BinaryTree::leftmost(TreeNode *current){
    while (current->leftchild != NULL){
        current = current->leftchild;
    }
    return current;
}
```

</br>  
接著，觀察在inorder規則下，某一node的「**Successor**」之所在位置有兩種可能：

1. 若CurrentNode的right child不是NULL，則CurrentNode之下一個順序的node即為以「Current->rightchild為root」之subtree中，最左的node。
    * 如圖五(a)所示，若CurrentNode站在B上，B的下一個node即為「以B的right child(也就是E)」為root之subtree中的最左node，即為G。
2. 若CurrentNode沒有right child，則CurrentNode之下一個順序的node是「以left child的身份尋找到的ancestor」。
    * 以圖五(a)中的H為例，H沒有right child，因此往上(往root方向)找ancestor。  
    首先找到E，但是H是E的right child，因此再繼續往上找，此時CurrentNode移動到E。  
    接著往E的parent找到B，而E是B的right child，再繼續往上找，並更新CurrentNode為B。  
    接著往B的parent找到A，此時，**B為A的left child**，則A即為H的下一個順序的node。
3. 若整棵樹偏一邊(稱為skewed Binary Tree)，root只有left subtree，沒有right subtree，那麼，則回傳NULL，表示root的successor。

<center>
![successor][f22]

**圖五(a)：。**  
</center>


以下為`InorderSuccessor()`的範例程式碼：

```cpp
// C++ code
TreeNode* BinaryTree::InorderSuccessor(TreeNode *current){
    if (current->rightchild != NULL){
        return leftmost(current->rightchild);
    }
    
    TreeNode *new_node = new TreeNode;
    new_node = current->parent;
    
    while (new_node != NULL && current == new_node->rightchild) {
        current = new_node;
        new_node = new_node->parent;
    }
    
    return new_node;
}
```

最後，有了`leftmost()`與`InorderSuccessor()`，即能夠以迴圈的方式進行inorder traversal，相較於遞迴形式的函式，具有更大彈性：

```cpp
// C++ code
void BinaryTree::Inorder_by_parent(TreeNode *root){
    TreeNode *current = new TreeNode;
    current = leftmost(root);

    while(current){
        std::cout << current->str << " ";
        current = InorderSuccessor(current);
    }
}
```

output:

```cpp
D B G E H A F I C
```
</br>
<a name="predecessor"></a>

###Predecessor、rightmost

只要把`InorderSuccessor()`與`leftmost()`中，所有的left與right互換，就得到`InorderPredecessor()`與`rightmost()`，而概念上也確實是如此：

* **rightmost**：從「以CurrentNode為subtree」的root一路向右做Linked list的單向traversal。
* **Predecessor**：某一CurrentNode的「前一個順序的node」之位置有兩種可能：

    1. 若CurrentNode的left child不是NULL，則CurrentNode之前一個順序的node即為以「Current->lefttchild為root」之subtree中，最右的node。  
如圖五(b)所示，若CurrentNode站在C上，C的前一個node即為「以C的right child(也就是F)」為root之subtree中的最右node，即為I。
    2. 若CurrentNode沒有left child，則CurrentNode之前一個順序的node是「以right child的身份尋找到的ancestor」。  
以圖五(b)中的F為例，F沒有left child，因此往上(往root方向)找ancestor，首先找到C，但是F是C的left child，因此再繼續往上找，此時CurrentNode為C，往parent找到A，此時，**C為A的right child**，則A即為F的前一個順序的node。
    3. 同樣地，若整棵樹為skewed Binary Tree，root只有right subtree，沒有left subtree，則回傳NULL，表示root的predecessor。

<center>
![predecessor][f23]

**圖五(b)：。**  
</center>



以下為`rightmost()`與`InorderPredecessor()`的範例程式碼：

```cpp
// C++ code
TreeNode* BinaryTree::rightmost(TreeNode *current){
    while (current->rightchild != NULL)
        current = current->rightchild;
    return current;
}
TreeNode* BinaryTree::InorderPredecessor(TreeNode *current){
    if (current->leftchild != NULL)
        return rightmost(current->leftchild);
    
    TreeNode *new_node = new TreeNode;
    new_node = current->parent;
    
    while (new_node != NULL && current == new_node->leftchild) {
        current = new_node;
        new_node = new_node->parent;
    }
    
    return new_node;
}
```
有了`rightmost()`與`InorderPredecessor()`，便能夠照inorder traversal的相反順序對樹的node做Visiting：

```cpp
// C++ code
void BinaryTree::Inorder_Reverse(TreeNode *root){
    TreeNode *current = new TreeNode;
    current = rightmost(root);

    while(current){
        std::cout << current->str << " ";
        current = InorderPredecessor(current);
    }
}
```

output:

```cpp
C I F A H E G B D
```
</br>  
`InorderSuccessor()`和`InorderPredecessor()`在Binary Search Tree的部分會再次出現，並且出現在基本操作：deletion(刪除node)中，因此學起來不止酷，還很實用的啊。



[f1]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f1.png?raw=true
[f2]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f2.png?raw=true
[f5]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f5.png?raw=true 
[f6]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f6.png?raw=true
[f7]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f7.png?raw=true
[f8]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f8.png?raw=true
[f9]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f9.png?raw=true
[f10]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f10.png?raw=true
[f11]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f11.png?raw=true
[f12]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f12.png?raw=true
[f13]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f13.png?raw=true
[f14]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f14.png?raw=true
[f15]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f15.png?raw=true
[f16]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f16.png?raw=true
[f17]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f17.png?raw=true
[f18]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f18.png?raw=true
[f19]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f19.png?raw=true
[f20]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f20.png?raw=true
[f21]:https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f21.png?raw=true
[f22]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f22.png?raw=true
[f23]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Traversal/f23.png?raw=true



</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch12](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch5](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Linked List: 新增資料、刪除資料、反轉](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao-fan-zhuan.html#print)
* [Stack: Intro(簡介)](http://alrightchiu.github.io/SecondRound/stack-introjian-jie.html)
* [Queue: Intro(簡介)，並以Linked list實作](http://alrightchiu.github.io/SecondRound/queue-introjian-jie-bing-yi-linked-listshi-zuo.html)
* [Binary Search Tree: Sort(排序)、Delete(刪除資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-sortpai-xu-deleteshan-chu-zi-liao.html#binary-search-tree-sortpai-xu-deleteshan-chu-zi-liao)


</br>

<a name="tree_series"></a>

###Binary Tree系列文章


[Binary Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/binary-tree-introjian-jie.html)  
[Binary Tree: Traversal(尋訪)](http://alrightchiu.github.io/SecondRound/binary-tree-traversalxun-fang.html)  
[Binary Tree: 建立一棵Binary Tree](http://alrightchiu.github.io/SecondRound/binary-tree-jian-li-yi-ke-binary-tree.html)  


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>






