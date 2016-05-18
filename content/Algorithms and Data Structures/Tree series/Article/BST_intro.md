Title: Binary Search Tree: Intro(簡介)    
Date: 2016-1-2 12:15  
Category: 演算法與資料結構  
Tags: C++, Binary Search Tree(二元搜尋樹), Intro, Dictionary,  
Summary: 謹以Binary Search Tree(二元搜尋樹)向拯救90後的童年的鉅作《七龍珠》以及鳥山明老師致敬。

</br>
###先備知識與注意事項

先前的文章介紹過廣義的Tree(樹)、Binary Tree(二元樹)，這篇文章將繼續增加限制條件，使Binary Tree晉升成Binary Search Tree(BST，二元搜尋樹)。  

這裡要處理的資料是日本漫畫的曠世鉅作《七龍珠》中各角色的戰鬥力。  
七龍珠的劇情(正篇有七龍珠、七龍珠Z、七龍珠改、七龍珠GT、七龍珠超，劇場版還有[七龍珠劇場版(太多了請參閱維基百科)](https://zh.wikipedia.org/wiki/%E4%B8%83%E9%BE%99%E7%8F%A0#.E5.8A.87.E5.A0.B4.E7.89.88))時而前後連貫，時而交錯，為了維持每個系列之間的角色設定，就需要對角色的戰鬥力進行管理，避免劇情不合邏輯變成鬧劇，在此，筆者推薦鳥山明老師可以使用先進如BST的資料結構來整理角色的資料(當然，也是可以用excel或是國小生字簿，[Whatever Works](http://www.imdb.com/title/tt1178663/))。  

熱血沸騰了。  

*** 

##目錄

* [引入Dictionary](#dictionary)
* [Binary Search Tree的特徵](#bst)
* [在Binary Search Tree中管理資料](#operation)
    * [insert(新增資料)](#insert) 
    * [search(搜尋資料)](#search)
    * [sort(排序)](#sort)
    * [delete(刪除資料)](#delete)
* [參考資料](#ref)
* [BST系列文章](#tree_series)

</br>

<a name="dictionary"></a>

##引入Dictionary

搜尋與排序都需要「比大小」，欲執行「比大小」，就要使用「能夠比大小」的資料形態(亦即：兩個比較之物只能唯一滿足於「大於」、「小於」或「等於」之關係)，最直觀的便是使用整數(integer)。  

在先前的文章中，`class TreeNode`包含了指向child的pointer、指向parent的pointer，以及一個`char data`來儲存字母。  

而更多時候，node所攜帶的資料項目(在此為`char data`)可能更複雜，也許是一個姓名、一組帳戶資料、一本照片集、一組科學資料等等，然而，要使用這些資料進行「比大小」必須而外自行定義規則，例如：「姓氏照字母順序排序，字母越前面值越大；若第一個字相同，則依序往下比較；若姓名中所有字母之順序皆相同則...」，非常不實際，因此，不用再則了，變通的方法便是直接在資料上加上「編號」(也可以想成，把資料對應(mapping)到特定編號)，以編號做排序，並且能夠以特定編號搜尋其所對應之資料項目，即可避開上述麻煩。  

如此概念便是**Dictionary**，稱上述的「編號」為「Key(鍵值)」，稱「資料項目」為「Element(元素)」，則稱一組「Key-Element pairs」的集合為**Dictionary**。  

如圖一所示，若將先前的字母視為「Element」並加上「Key」，則(Key, Element)可以表示成(編號, A)，若處理學生資料，將編號視為學號，資料視為姓名，則能夠將(Key, Element)可以表示成(學號, 姓名)。

 
<center>
![dictionary1][dictionary1]

**圖一：。**  
</center> 

在接下來的篇幅裡，將使用七龍珠的角色(悟空)作為Element，角色的戰鬥力視為Key：


<center>
![dictionary2][dictionary2]

**圖二：。**  
</center>

修正後的`class TreeNode`可能長這樣： 

```cpp
// C++ code
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    std::string data;       // Element
    int Key;                // Key, used for comparison 
	 ...
};
```
  
 
備註： 

1. Dictionary的概念也出現在**Hash Table**(有天才翻譯成"哈希表"也很酷)、C/C++標準函式庫(Standard Library)中的**container**:`map`等等，有非常多應用。
2. 以下角色戰鬥力的絕對值是捏造的，不過相對值盡力維持正確(除了撒旦)，若有疑問，歡迎龍珠粉來信討論。 
3. 由於故事的角色眾多，以下將挑選具代表性之角色用來說明BST。

</br>

<a name="bst"></a>

##Binary Search Tree的特徵

有了加裝Dictionary後的`TreeNode`，便能夠說明BST的特徵：

* 任何CurrnetNode之Key若與其left child、right child之Key有以下關係(若pointer指向`NULL`則忽略)：$Key(L)<Key(Current)<Key(R)$，則可稱這棵樹為Binary Search Tree(BST)。

以圖三為例，樹中有三個node，悟空的戰鬥力為1000，龜仙人的戰鬥力為8，克林的戰鬥力為2，若將龜仙人設為`root`，則克林的戰鬥力較小，因此成為龜仙人的left child，悟空的戰鬥力較大，便成為龜仙人的right child，如此便滿足$Key(L)<Key(Current)<Key(R)$，即可稱圖三為一棵BST。

<center>
![bst][bst]

**圖三：。**  
</center>

有了BST後，便能夠替鳥山明老師處理角色之間的戰鬥力關係了。

</br>

<a name="operation"></a>

##在Binary Search Tree中管理資料

故事一開始的主要角色有悟空(1000)、龜仙人(8)和克林(2)，以龜仙人為`root`能夠建立出一棵BST如圖四：

<center>
![bst1][bst1]

**圖四：。**  
</center>
</br>


<a name="insert"></a>

###insert(新增資料)

隨著故事劇情推進，角色也會跟著增加，因此，要在BST中**新增node(新增資料)**。  
在此，比克大魔王現身地球危害人間，其戰鬥力為513，欲將其放進BST，根據BST的規則判斷出，比克的戰鬥力比龜仙人高，因此要將比克放在龜仙人的right subtree(右子樹)；接著，再和悟空比較，比克的戰鬥力比悟空低，因此將比克建立在悟空的left child上，如圖五(a)所示：

<center>
![insert][insert1]

**圖五(a)：。**  
</center>

接著，賽亞人王子達爾登場，其戰鬥力為524，根據BST的規則，判斷出其應在「龜仙人的right subtree」、「悟空的left subtree」與比克的「right child」，如圖五(b)所示。  
在下一篇文章將會看到，實際上的演算法正是以這種方式實現：移動`CurrentNode`，並將`CurrentNode`之Key與「欲新增之node的Key」比較，找到符合BST規則的位置後，置入新的node。

<center>
![insert][insert2]

**圖五(b)：。**  
</center>


<a name="search"></a>

###search(搜尋資料)

在處理資料時，時常需要尋找某特定資料，是否存在資料結構中。以BST處理資料，最簡單的方式便是用Key尋找。  
以圖六為例，故事推進到納美克星弗力札大王篇，若想要確認基紐隊長的資料是否已經建立完成，只要記住隊長的戰鬥力為「627」，進入BST中，便能夠找到隊長，必且回傳(return)隊長的node。  
有時會出現欲搜尋的資料尚未被建立進BST中、或者已經從BST中移除的情況，例如，若要在悟空變成超級賽亞人之前找克林，以克林的戰鬥力「2」來搜尋，但是發現找不到，便回傳`NULL`。

因為克林被弗力札大王給殺死了啊啊阿啊(變身超級賽亞人)。

<center>
![search][search]

**圖六：。**  
</center>

（界王神的聲音：為什麼`root`從龜仙人變成比克？不會違反BST規則嗎？詳見[Red Black Tree系列之Rotation(旋轉)](http://alrightchiu.github.io/SecondRound/red-black-tree-rotationxuan-zhuan.html)。）

<a name="sort"></a>

###sort(排序)

故事來到了魔人普烏篇，因為角色有點多，有點混亂，此時，若想要知道各角色戰鬥力的大小排序，只要按照**Inorder Traversal**即可按照戰鬥力(Key)高低列出所有資料：

<center>
![sort][sort]

**圖七：藍色數字為戰鬥力(key)，紅色數字表示「戰鬥力由小到大」之順序。**  
</center>


<a name="delete"></a>

###delete(刪除資料)

最後，當角色死掉去領便當，就需要從BST刪除資料，而根據欲刪除資料之「**child個數**」可以分成三種情況：

1. 刪除撒旦：撒旦**沒有child**，因此，直接把撒旦的parent(普烏)之left child指向`NULL`即可。
2. 刪除弗力札：弗力札有**一個child**(left child)，因此刪除弗力札之前，需要先把弗力札的left child(基紐)接到弗力札的parent(龜仙人)上，又因為弗力札原本是龜仙人的right child，因此基紐將遞補弗力札，成為龜仙人的right child。
3. 刪除西魯：西魯有**兩個child**，稍微麻煩一點，需要「多一個步驟」，將留待之後的文章做詳細說明。


<center>
![delete][delete]

**圖八：。**  
</center>

[dictionary1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f1.png?raw=true
[dictionary2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f2.png?raw=true
[bst]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f3.png?raw=true
[bst1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f14.png?raw=true 
[insert1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f4.png?raw=true
[insert2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f5.png?raw=true
[search]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f16.png?raw=true
[sort]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f17.png?raw=true
[delete]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BST_fig/Intro/f15.png?raw=true

</br>



以上便是BST與BST中的基本操作之介紹，所有操作都是以BST之特徵：$Key(L)<Key(Current)<Key(R)$為出發點，尤其是新增node與刪除node，務必要滿足BST之規則。

接下來，將以兩篇文章的篇幅，說明上述四種資料處理操作的演算法。

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch12](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch5](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Wikipedia：七龍珠](https://zh.wikipedia.org/wiki/%E4%B8%83%E9%BE%99%E7%8F%A0#.E5.8A.87.E5.A0.B4.E7.89.88)
* [IMDB：Whatever Works](http://www.imdb.com/title/tt1178663/)


</br>

<a name="tree_series"></a>

###BST系列文章


[Binary Search Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/binary-search-tree-introjian-jie.html)  
[Binary Search Tree: Search(搜尋資料)、Insert(新增資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-searchsou-xun-zi-liao-insertxin-zeng-zi-liao.html)  
[Binary Search Tree: Sort(排序)、Delete(刪除資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-sortpai-xu-deleteshan-chu-zi-liao.html)

回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)




</br>  






