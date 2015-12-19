Title: 樹(Tree)  
Date: 2015-12-14 9:27  
Category: Algorithms and Data Structures  
Tags: 今天不寫明天就忘了  
Summary: haha  

先備知識：指標(pointer)與連結串列(Linked List)


樹(Tree)是用以描述具有階層結構(hierarchical structure)的問題的首選，階層結構意味著明確的先後次序，例如，若要印出ABC三個字母的所有排列組合(permutation)，直覺反射的圖像會是：


<center> ![圖一][fig1] </center>
<center>圖一：ABC字母排列組合 

|![圖一][fig1]      |
| ---               |
| 圖一：ABC字母排列組合| 

</center>

[fig1]: https://github.com/alrightchiu/SecondRound/tree/master/content/fig/tree/abc_permu1.png


圖一的邏輯為：從起點(R)開始，先決定第一個字母，再依序決定第二、第三個字母，並且，在每一次選擇字母時，可能有不止一個可供選擇的字母。最後一共走出六條路徑，得到六種排列組合。若將起點(R)視為樹根(root)，每一個字母選擇的狀態(例如：A、C、BC、CAB)都視為一個節點(node)，這樣的結構便能夠視為一棵樹。

更精確一點，以下列出兩種樹的定義：  
A. **樹**是由一個或多個節點所組成的有限集合，並且滿足：  

1. 存在且只有一個稱為樹根(root)的節點；
2. 其餘的節點可以分割成任意正整數個(包含零個)互斥(disjoint)的集合：T<sub>1</sub>、...、T<sub>n</sub>，其中每一個集合也都滿足樹的定義，這些集合又稱為這棵樹的**子樹**(subtree)。

B. **樹**是由一個或多個節點(nodes/vertices)以及邊緣(edge)所組成，而且沒有循環(cycle)的資料結構。

根據以上定義，便能判斷下列四種結構中，a、b可以視為樹，而c、d則否：

| 圖二 |
|---             |
|![圖二.a][fig2]|
|a：樹的節點只有指向左子樹與右子樹時，又稱為二元樹(Binary Tree)。|
|![圖二.b][fig3]|
|b：當樹退化成連結串列(linked list)時，仍滿足樹的定義。|          
|![圖二.c][fig4] |
|c：在F出現循環(cycle)；C->B->D->E出現無向循環(undirected cycle)。後者等價於：D有兩個parent node。|
| ![圖二.d][fig5]|
|d：一棵樹只能有一個樹根(root)。此圖像又稱為樹林(forest)。|

[fig2]: is_Tree_a.png
[fig3]: is_Tree_b.png
[fig4]: is_Not_Tree_c.png
[fig5]: is_Not_Tree_d.png

// 接著： 眾多名詞定義，再一張圖

// 樹的特徵：
樹中找特定節點一定只有一條路徑

// 集合關係

// 圖要再做張比較小的, 不然就學怎麼調size(後者比較實際)