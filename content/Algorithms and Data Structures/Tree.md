Title: Tree(樹)  
Date: 2015-12-19 20:27  
Category: Algorithms and Data Structures  
Tags: 今天不寫明天就忘了  
Summary: 介紹資料結構中樹(Tree)的概念。


</br>
######**先備知識與注意事項**

若熟悉連結串列(Linked List)將會更容易理解樹：Linked list是一維的線性結構(不是往前、就是往後)，而樹(與圖)則推廣成多維的結構。  
</br>
<center>![linkedlist][linked_list]

**圖一：A、B、C、D稱為node(節點)，用以代表資料(data)、狀態(state)。  
連結各個node之間的連結(link)稱為edge，可能是單方向，或者雙向。**  
</center>   

[linked_list]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/linked_list_size.png?raw=true


另外則是用詞翻譯的選擇，為響應兩性平權(feminism)以及使用的頻繁程度，名詞如node/vertex(節點)、edge(邊緣？)、parent(父還是母？)、child(孩子？)、ancestor(祖先？)...皆視為專有名詞，不做翻譯。因此文句中會出現中英夾雜的情況，敬請見諒。  
</br>  
[OK Go](https://www.youtube.com/watch?v=u1ZB_rGFyeU).  
</br>

##目錄

* [隨處可見的樹](#example)  
* [那麼，樹最根本的特徵是什麼？](#essence)   
* [還有哪些結構是樹？](#tree_not_tree)  
* [用以描述一棵樹的元素](#element)  
* [樹的定義](#definition)  
* [程式碼](#code)  
* [集合關係](#set)  
* [參考資料](#ref)


 <a name="example"></a>
 
##**隨處可見的樹**
**樹**(Tree)是用以描述具有**階層結構**(hierarchical structure)的問題的首選，階層結構意味著明確的先後次序，例如，若要印出ABC三個字母的所有排列組合(permutation)，直覺反射的圖像會是：  

<center>  
![fig1.a][abc_permu-a]
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
![fig1.b][abc_permu-b]  

**圖二：ABC字母排列組合。  
視R為樹根(root)，每一個選擇狀態視為node，此即為樹的結構。**
</center>
</br>
[abc_permu-a]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/abc_permu_size.png?raw=true
[abc_permu-b]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/abc_permu_vertical_size.png?raw=true

圖二的邏輯為：從起點(R)開始，先決定第一個字母，再依序決定第二、第三個字母，並且，在每一次選擇字母時，可能有不止一個可供選擇的字母。最後一共走出六條路徑，得到六種排列組合，而且這六種排列方式只能經由一種唯一的選擇方式(唯一的路徑)產生。若將起點(R)視為樹根(root)，每一個字母選擇的狀態(例如：A、C、BC、CAB)都視為一個node，這樣的結構便能夠視為一棵樹。  

另外如一本書的目錄、族譜、官僚企業的職位關係，甚至是更廣義的「從家門口為起點尋找方圓一公里以內的便利商店」都能夠建立出樹的模型。  
</br>

 <a name="essence"></a> 
##**那麼，樹最根本的特徵是什麼？**
以族譜為例，若包龍星是宋世傑的爸爸，那麼包龍星就絕對不能同時又是宋世傑的兒子。圖三以node與edge描述此關係，並定義箭頭是從父指向子，則包龍星指向宋世傑的箭頭表示包為父、宋為子，而宋世傑指向包龍星的箭頭表示宋為父、包為子，這一個箭頭即違反了最初「包龍星是宋世傑的爸爸」的命題，此即稱為cycle，也就是著名的「雞生蛋」與「蛋生雞」。  

而樹的最根本特徵就是：**在樹的結構裡，只有一個樹根(root)，並且不存在cycle**。
此特徵將衍生出另外兩項等價的性質：

1. **在樹中若要從root尋找特定node，一定只存在一條路徑(path)。**  
2. **每個node只會有一個parent。**

</br> 
<center>
![cycle][cycle]  

**圖三：包龍星若是宋世傑的parent(父)，又同時為其child(子)，即形成cycle。**
</center>
[cycle]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/cycle_size.png?raw=true
</br> 

<a name="tree_not_tree"></a> 
##**還有哪些結構是樹？**
(若不是樹，皆稱為圖(graph)。)

下列四種結構中，a、b可以視為樹，而c、d則否：
</br>
<center>![fig2.a][example-a]  
**圖三.a：若樹的node只有指向左子樹(left subtree)與右子樹(right subtree)時，又稱為Binary Tree(二元樹)。**  
</br> 
![fig2.b][example-b]  
**圖三.b：若樹退化成連結串列(linked list)，仍滿足樹的定義。**    
</br>
![fig2.c][example-c]  
**圖三.c：在F出現cycle；C->B->D->E出現undirected cycle，詳見圖論(graph theory)。  
後者等價於：D有兩個parent node。**  
</br>
![fig2.d][example-d]  
**圖三.d：一棵樹只能有一個樹根(root)。此圖像又稱為樹林(forest)。**
</center>
</br>
[example-a]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/is_Tree_a_size.png?raw=true
[example-b]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/is_Tree_b_size.png?raw=true
[example-c]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/is_Not_Tree_c_size.png?raw=true
[example-d]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/is_Not_Tree_d_size.png?raw=true

 <a name="element"></a>
##**用以描述一棵樹的元素**
 
<center>
![fig3][generalTree]   

**圖四：這是一棵普通的樹。**  
</center>  
[generalTree]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/general_Tree.png?raw=true  

配合圖四，以下將介紹在樹中常見的元素，可以用來計算時間複雜度(time complexity)、與同好交流感情，好處不勝枚舉。   

針對**node / vertex**：  

* **degree(分歧度)**：一個node擁有的subtree(子樹)的個數。例如：A的degree為3，F的degree為2，N的degree為0。  
* **root(樹根)**：樹中最上層的node，也是唯一一個其parent為**NULL**的node。圖四中，A即為root。
* **leaf**：沒有child/subtree的node稱為leaf node。圖四中，G、H、J、K、L、M、N皆為leaf node。
* **external node**：沒有child的node。因此，leaf node與external node同義。
* **internal node**：至少有一個child的node，稱為internal node。圖四中，A、B、C、D、E、F、I皆為internal node。

針對**樹**：

* **parent** <--> **child**：圖四中，A為C的parent，C為A的child；E為K的parent，K為E的child。若以pointer說明，被指向者(pointed)為child，指向者(point to)為parent。
* **siblings**：擁有相同parent的node們，互相稱兄道弟。例如：B、C、D共同的parent為A。
* **descendant(子嗣)**：圖四中，站在A，所有能夠以「尋找child」的方式找到的node，皆稱為A的descendant，因此整棵樹除了A以外皆為A的descendant。站在F，能夠以「parent指向child」找到的node有L、M，則稱L、M為F的descendant。
* **ancestor(祖先)**：圖四中，站在K，所有能夠以「尋找parent」的方式找到的node，皆稱為K的ancestor，因此，E、B、A皆為K的ancestor。
* **path(路徑)**：由descendant與ancestor關係連結成的edge，例如A-B-E-K、A-C-F-N。
* **level**：定義root的level為1，其餘node的level為其parent的level加一。
* **height of node**：某一node與其最長path上之descendant leaf node之間的edge數。例如F的height為1，D的height為2，leaf node的height為0。
* **height of tree**：樹的height即為root的height，圖四中，樹的height為A的height，等於3。
* **depth**：某一node與root之間的edge數。例如，F的depth為2，L的depth為3。  
</br>

<a name="definition"></a>
##**定義**

根據以上範例說明，再配合樹的定義，還不飛上天？  
以下列出兩種樹的定義：  

A. **樹**是由一個或多個節點所組成的有限集合，並且滿足：  

1. 存在且只有一個稱為樹根(root)的節點；
2. 其餘的節點可以分割成任意正整數個(包含零個)互斥(disjoint)的集合：$T_1、...、T_n$，其中每一個集合也都滿足樹的定義，這些集合又稱為這棵樹的**子樹**(subtree)。

B. **樹**是由一個或多個nodes/vertices以及edge所組成，而且沒有cycle的集合(set)。  
</br>

 <a name="code"></a>
##**程式碼**

實務上，要以程式碼實作一棵樹，常用的手法為：先以class TreeNode(或是struct)定義出每顆node能夠指向多少subtree、攜帶哪些資料形態，再以另一個class Tree表示整棵樹，並以root作為樹的存取點：

```cpp
// 以C++為例
class Tree;
class TreeNode{
	TreeNode *leftchild;		// 以下表示每一個node有四個pointer指向child
	TreeNode *rightchild;
	TreeNode *whatever;
	TreeNode *works;
	int data1;					// node所攜帶的info
	double data2;
	...
	friend class Tree;
};
class Tree{
	TreeNode *root;				// 以root作為存取整棵樹的起點
};
```

 <a name="set"></a>
##**集合關係**

本篇介紹的樹(Tree)位居承先啟後的戰略位置，圖五展示了與樹有關的資料結構的集合關係圖：

<center>
![fig4][set]  
**圖五：與樹相關的資料結構之集合關係。**
</center>
</br>
[set]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree/Set_Graph_Tree_size.png?raw=true

本篇介紹的樹並沒有限制child/ subtree的個數，理論上可以有多到超過記憶體空間的child node。  
然而在實務上，較常使用每個node至多只有兩個child的樹，稱為**Binary Tree**(二元樹)。  
從Binary Tree再增加「鍵值大小規則」，即得到**Binary Search Tree**(BST，二元搜尋樹)。  
以BST為基礎，在每個node上添加顏色(紅與黑)用以平衡樹的height，以減短搜尋時間，此種樹稱為**Red Black Tree**(RB Tree，紅黑樹)。  
另一個方向，若打破「不能存在cycle」的限制，則從樹推廣至**圖(graph)**。  

在接下來的文章將先以縮小集合的方向依序介紹：Binary Tree、BST、RB Tree，再進入圖(graph)這個更複雜的主題。

 <a name="ref"></a>
</br>
######**參考資料**：
* [Wikipedia：Tree(data structure)](https://en.wikipedia.org/wiki/Tree_%28data_structure%29)
* [Fundamentals of Data Structures in C++](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Infinite Loop：【演算】樹 - Tree](http://program-lover.blogspot.tw/2008/12/tree.html)
* [Wikipedia：九品芝麻官](https://zh.wikipedia.org/wiki/%E4%B9%9D%E5%93%81%E8%8A%9D%E9%BA%BB%E5%AE%98)
* [Wikipedia：威龍闖天關](https://zh.wikipedia.org/wiki/%E5%AF%A9%E6%AD%BB%E5%AE%98_%281992%E5%B9%B4%E9%9B%BB%E5%BD%B1%29)  

</br>
