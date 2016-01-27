Title: Red Black Tree: Intro(簡介)    
Date: 2016-1-23 16:09  
Category: 演算法與資料結構  
Tags: C++, Red Black Tree  
Summary: 介紹Red Black Tree(紅黑樹)的基本性質。
 


</br>
######**先備知識與注意事項**
 
在閱讀過BST系列文章後可以發現，在BST中的操作，不論是Insert(新增資料)或是Delete(刪除資料)，皆需要先做Search(搜尋)，而Search(搜尋)的效率，取決於BST的height(樹高)，如果一棵樹越矮、越平衡(balanced)，則在此BST中搜尋資料的速度較快，理想狀況為Complete Binary Tree(時間複雜度：$O(\log N)$)。反之，若由於輸入資料的順序使得BST沒長好、偏一邊，則在此BST中搜尋資料的最壞情況將有可能如同在Linked List做搜尋(時間複雜度：$O(N)$)。  

本篇文章將介紹BST的進階版Red Black Tree(RBT，紅黑樹)之基本性質，會說是進階版，原因在於RBT其實也是BST(滿足$Key(L)<Key(Current)<Key(R)$)，不過RBT的node比BST多加了「顏色」(紅色或黑色)，而正因為多了「顏色」，便能修正BST有可能退化成Linked list(連結串列)的潛在缺陷。 

<center>
![anarchy][f1]

**圖一：不平衡的BST。**  
</center>

另外，本篇文章將不進行「RBT之時間複雜度如何能視同Complete Binary Tree」之論證(筆者火侯不夠，請見諒)，建議讀者可以參考[Introduction to Algorithms，第13章](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844)，不過結論就是，RBT可以被視為如同Complete Binary Tree的BST，所有與Search(搜尋)有關的操作(Leftmost、Successor、Insert、Delete等等)，都能夠在$O(\log N)$內完成。


***



##目錄
* [為什麼需要Red Black Tree？](#why)
* [Red Black Tree的特徵](#rbt)
* [程式碼](#code)
* [參考資料](#ref)



<a name="why"></a>

##**為什麼需要Red Black Tree？**

若考慮最壞情況，在建立BST時，輸入資料的Key恰好被排序過(例如：1、2、3、4...)，那麼這顆BST便會退化成Linked List(接續前例，所有leftchild pointer都指向`NULL`，只有rightchild child被使用)：

```cpp
// C++ code
int main{
    BST T;
    T.InsertBST(1);   // 依序加入Key為1、2、3...之資料
    T.InsertBST(2);
    T.InsertBST(3);
    ...
}
```
圖二中，右側的藍色數字表示「搜尋該node需要比較(`if KEY == current->key`)的次數」，也就是迴圈的次數。考慮一串Linked List共有N個node，若要尋找第K個node，則最壞情況即是一路找到最後一個node，需要N次。

<center>
![linked list][f2]

**圖二：BST退化成Linked List。**  
</center>

再比較Complete Binary Tree，如圖三。其中，node裡的數字即為Key，node旁邊的藍色數字代表該node在Complete Binary Tree中的位置順序，右側的藍色數字代表迴圈次數。  
位置順序與迴圈次數有以下關係：

* 在位置$2^3=8$與$2^4=16$之間的node($2^3\leq i<2^4, i=8\sim 15$)，只需要$3+1$次比較(comparison)即可找到。
* 依此類推，若BST中有N個node，則所有node保證能夠在$\lfloor {\log N} \rfloor +1$次 ($2^k\leq N<2^{k+1}, k\leq\log N$)迴圈以內找到。


<center>
![cbt][f3]

**圖三：搜尋BST的理想情況：Complete Binary Tree。**  
</center>

以上兩個範例為一棵具有N個node的BST之height(樹高)提供了邊界：$\log N\leq height\leq N$。

因此，BST越平衡，在樹中搜尋資料的時間就越短，連帶地Insert(新增資料)、Delete(刪除資料)也會變得更有效率。  
這就是RBT值得被介紹的原因。


<a name="rbt"></a>

##**Red Black Tree的特徵**

Red Black Tree(RBT)是node塗了「顏色」的Binary Search Tree(BST)，藉由控制顏色，能夠保證在RBT中，**最長path(路徑)不會超過最短path的兩倍**(若最短的path是5，最長的path至多只能是10)，如此，RBT便能夠近似地視為平衡，如圖四。

<center>
![rbt][f4]

**圖四：最短的path為3(最右path:26-41-47)，其餘path最長只能是6(最左path:26-17-14-10-7-3)。  
若蓋住`NIL`與顏色，此即為BST。**  
</center>

圖四中，所有原本在BST中指向`NULL`的pointer，在RBT中，全部指向了`NIL`。然而什麼是`NIL`？`NIL`是永遠為黑色、並且實際占有記憶體的node，因為有配置記憶體，因此能夠以`Node->color`的方式取得某個node之顏色(若使用`NULL`則無法)，此優勢將在後續介紹如何於RBT中Insert(新增資料)與Delete(刪除資料)時派上用場。

接著來看RBT的五項特徵：

1. RBT中的每一個node不是黑色就是紅色。
2. `root`一定是黑色。
3. 每一個leaf node(也就是`NIL`)一定是黑色。
4. 如果某個node是紅色，那麼其兩個child必定是黑色，不能有兩個紅色node相連，如圖四中的node(17)、node(30)。  
若某個node為黑色，其child之顏色沒有限制，如圖四中的node(38)、node(26)、node(21)。
5. 站在任何一個node上，所有從該node走到其任意descendant的path上之黑色node數必定相同。  
以圖四為例，站在node(14)上，所有從node(14)走向其descendant leaves(也就是`NIL`)的path上之黑色node數必為3： 
    * path1:14(**b**)-10(r)-7(**b**)-3(r)-NIL(**b**)；
    * path2:14(**b**)-10(r)-7(**b**)-NIL(**b**)；
    * path3:14(**b**)-10(r)-12(**b**)-NIL(**b**)；
    * path4:14(**b**)-16(**b**)-15(r)-NIL(**b**)；
    * path5:14(**b**)-16(**b**)-NIL(**b**)；   

根據上述特徵的第四點與第五點，RBT中path可能的長度最小值一定是全部node皆為黑色，而path可能的長度最大值並定是紅色-黑色相間(如圖四)，如此便確保RBT擁有**最長path(路徑)不會超過最短path的兩倍**的特性。


<a name="code"></a>

##**程式碼**

實際的程式實現上，會把所有`NIL`視為同一個`NIL`，並把`root`的parent指向`NIL`，以節省記憶體空間，如圖五。

<center>
![rbt][f5]

**圖五：。**  
</center>

`class TreeNode`與`class RBT`之資料成員(data member)程式範例如下：

```cpp
// C++ code
#include <iostream>
#include <string>
class RBT;
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    std::string element;
    int key;
    int color;         // 0: Red,  1: Black
    friend RBT;
    ...
}
class RBT{
private:
    TreeNode *root;
    TreeNode *neel;    // 此即為NIL, 常被稱為sentinel
    ...
}
```
(為了避開某些IDE將`NIL`設定成保留關鍵字(reserved keywords，例如`template`、`while`、`struct`等等)，因此使用`neel`。)

為求畫面簡潔，往後的篇幅裡將把RBT示意圖中的`NIL`隱藏起來，只顯示RBT中的internal node，如圖六，不過心裡要記得，RBT無時無刻都被`NIL`充滿著。

<center>
![rbt][f6]

**圖六：。**  
</center>


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Intro/biasedtree.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Intro/rbt1.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Intro/rbt2.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Intro/rbt3.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Intro/rbt4.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Intro/rbt5.png?raw=true
  
  
以上便是RBT之初探，最重要的結論為：就時間複雜度而言，RBT能夠被視為平衡的BST，所有操作皆能在時間複雜度為$O(\log N)$內完成。

在接下來的三篇文章中，將依序介紹Rotation(旋轉)、Insert(新增資料)與Delete(刪除資料)。
  

</br>

<a name="ref"></a>

######**參考資料**：

* [Introduction to Algorithms, Ch13](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [GeeksforGeeks：Red Black Tree](http://code.geeksforgeeks.org/NtLnIk)




</br>






