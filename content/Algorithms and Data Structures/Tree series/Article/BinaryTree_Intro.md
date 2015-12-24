Title: Binary Tree: Intro 
Date: 2015-12-21 22:24  
Category: Algorithms and Data Structures  
Tags: 今天不寫明天就忘了  
Summary: 介紹Binary Tree的基本概念。



</br>
######**先備知識與注意事項**

接續上一篇談到的[Tree(樹)](http://alrightchiu.github.io/SecondRound/treeshu.html)，這篇文章將介紹樹這個大集合裡的其中一支大宗：**Binary Tree**。

</br>
學海無涯，快點跳海。
</br>

##目錄
* [Binary Tree](#bt) 
* [程式碼](#code)   
* [Full & Complete Binary Tree](#fullcomplete) 
* [學習Binary Tree的未來出路](#application)
* [參考資料](#ref)

 <a name="bt"></a>
 
##**Binary Tree**
最廣義的樹(Tree)對於樹上的node之child數目沒有限制，因此，每個node可以有多個child。

<center>  
![general_tree][general_tree]

**圖一：這是一棵樹(Tree)**
</center>  
[general_tree]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Intro/general_tree.png?raw=true


若限制node只能有兩個child，等價於「樹上的每一個node之degree皆為2」，此即稱為**Binary Tree**(二元樹)，並稱兩個child pointer為left child和right-child。

<center>  
![binary_tree][binary_tree] 

**圖二：這是一棵Binary Tree。**
</center>
[binary_tree]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Intro/binary_tree.png?raw=true


 <a name="code"></a>
##**程式碼**

修改在[Tree(樹)](http://alrightchiu.github.io/SecondRound/treeshu.html#code)提供的程式實作方式，將node的child pointer設為left child與right child，以滿足Binary Tree的形式。  
另外，在class TreeNode有個`TreeNode *parent`，顧名思義，即是指向該node之parent的pointer，以圖二為例，B的parent pointer即指向A。  
Binary Tree的node未必需要parent pointer(或稱為parent field)，然而加入parent後，在對樹的操作(operaion)如inorder traversal(中序尋訪)、node deletion(刪除node)、以及任何需要back-tracing(回溯路徑)的操作時，會更加有效率。

```cpp
// 以C++為例
class Tree;
class TreeNode{
	TreeNode *leftchild;		 
	TreeNode *rightchild;		
	TreeNode *parent;			
	int data1;					
	double data2;
	...
	friend class Tree;
};
class Tree{
	TreeNode *root;				// 以root作為存取整棵樹的起點
};
```

 <a name="fullcomplete"></a>

##**Full & Complete Binary Tree**  

有兩類Binary Tree十分常見，分別為**Full Binary Tree**以及**Complete Binary Tree**。  
(完滿二元樹？完整二元樹？我的建議是當作專有名詞不要翻譯。) 
 
#####**A. Full Binary Tree**:  

如圖三所示，若所有internal node都有兩個subtree，leaf node具有相同的level(或height)，則稱此為**Full Binary Tree**(或稱作Perfect Binary Tree)。  
因此，若一棵Full Binary Tree的leaf node之level為$n$，整棵樹共有$2^n-1$個node。leaf node的level為4， 整棵樹共有15個node。  
並且，每個node與其child有以下關係：

* 第$i$個node的left child之index為 $2i$；
* 第$i$個node的right child之index為 $2i+1$；
* 除了root之parent為NULL之外，第$i$個node的parent之index為 $\lfloor {i\over2} \rfloor$ 。

<center>
![Full Binary Tree][fullBT]  

**圖三：若一棵Full Binary Tree的leaf node之level為$n$，整棵樹共有$2^n-1$個node。**  
</center>   
[fullBT]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Intro/fullBT.png?raw=true  


#####**B. Complete Binary Tree**:

若一棵樹的node按照Full Binary Tree的次序排列(由上至下，由左至右)，則稱此樹為**Complete Binary Tree**。  

以圖四及圖五作說明。  
圖四的樹共有10個node，且這十個node正好填滿Full Binary Tree的前十個位置，則此樹為Complete Binary Tree。  

<center>
![Complete Binary Tree][complBT]  

**圖四：這是一棵Complete Binary Tree。**
</center>  
[complBT]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Intro/complBT.png?raw=true

圖五的樹共有11個node，但是第11個node(K)應該要是第5個node(E)的child，因此，此樹並非Complete Binary Tree。

<center>
![Not Complete Binary Tree][non_complBT]  

**圖五：這不是一棵Complete Binary Tree。**
</center>
</br> 
[non_complBT]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Intro/non_complBT.png?raw=true
 
 <a name="application"></a>

##**學習Binary Tree的未來出路**

如果有家長擔心小孩子學了Binary Tree之後對未來的出路沒有幫助，這裡有[網路神人在StackOverFlow](http://stackoverflow.com/questions/2130416/what-are-the-applications-of-binary-trees)開示，以下簡單翻譯幾項Binary Tree的應用：

* Binary Search Tree(BST)：在某些資料經常要增加、刪除的應用中，BST常用來做搜尋，例如許多程式語言的Library中的`map`和`set`。
* Binary Space Partition：應用於幾乎所有的3D電玩遊戲以決定哪些物件需要rendered(呈現)。
* Binary Tries：應用於大多數high-bandwidth router(高頻寬路由器)以儲存router-tables。
* Heaps：用以實現高效率的priority queues(優先權佇列)，許多作業系統用來安排工作程序。
* Huffman Coding Tree：例如.jpeg、.mp3等壓縮技術皆使用Huffman編碼。(在一顆20MB的硬碟要價新台幣一萬元的時代，壓縮技術就是救世主。)  

以及其他應用(記得點進連結瞻仰大神網友風範)。  
大致可以體會到Binary Tree是許多進階應用的基礎，學會了幾乎是治病強身。  
接下來將會介紹Binary Tree(以及往後主題)中最基本的操作：traversal(尋訪)，顧名思義，就是如何在樹中移動，有了traversal之後再進一步探討search(搜尋)、insertion(新增node)、deletion(刪除node)、sorting(排序)會更加容易。

</br>
<a name="ref"></a>

######**參考資料**：
* [Wikipedia：Binary tree](https://en.wikipedia.org/wiki/Binary_tree)
* [Fundamentals of Data Structures in C++](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)  
* [StackOverFlow：What are the applications of binary trees?](http://stackoverflow.com/questions/2130416/what-are-the-applications-of-binary-trees)

</br>






