Title: Hash Table：Intro(簡介)  
Date: 2016-5-19 23:01 
Category: 演算法與資料結構  
Tags: C++, Intro, Dictionary,  
Summary: 介紹Hash Table(雜湊表)的基本概念。


</br>
###先備知識與注意事項

在做資料處理時，常常需要「查詢資料」，譬如線上購物平台有會員登入時，首先確認輸入的帳號密碼是否在資料庫裡，如果是，便從資料庫裡找出此會員的資料，如購物記錄、暫存購物清單等等。

想到「查詢資料」，可能會想到能夠在[時間複雜度為$O(\log{N})$完成查詢的**平衡的**Binary Search Tree(二元搜尋樹)](http://alrightchiu.github.io/SecondRound/binary-search-tree-introjian-jie.html)，如圖一。

* 在圖一的BST中，要找到Key(17)的資料，需要比較$4=\lfloor{\log_{2}{15}\rfloor+1}$次，時間複雜度可以視為**height(樹高)**。

<center>
![cc][f1]

**圖一：若為平衡的BST，查詢資料之時間複雜度為$O(\log{N})$。**
</center>

但是若資料量非常龐大(例如社交平台的註冊會員資料庫)，即使是$O(\log{N})$也非常可觀。

如果能在時間複雜度為常數的$O(1)$完成查詢該有多好。

本篇文章便要介紹能夠在$O(1)$完成查詢的**Hash Table(雜湊表)**。


***

##目錄

* [簡介：Dictionary(字典)](#dict)
* [以Array實現的Direct Access Table](#dat)
* [Hash Table的概念](#ht)
* [出事了：Collision](#collision)
* [參考資料](#ref)
* [Hash Table系列文章](#series)



</br>

<a name="dict"></a>

##簡介：Dictionary(字典)


**Dictionary**是以「鍵值-資料對」(**Key-Value** pair)來描述資料的抽象資料形態(Abstract Data Type)。

舉例來說：

* 電話簿裡的Dictionary即是將「姓名」視為**Key**，「電話號碼」視為**Value**。
* 學校學籍系統的Dictionary將「學號」視為**Key**，「學生資料」(如姓名、修課記錄)視為**Value**。


所以，任何具有辨別功能、可以用在「查詢資料」的「符號」(像是姓名、學號、網址等等)都能夠視為**Key**。  
而**Value**代表著較為廣義的「資料」，例如電話號碼、學籍資料、IP位置等等。

只要在系統輸入**Key**，便能找到相對應的**Value**，這就是**Dictionary**的基本概念。

<center>
![cc][f2]

**圖二：。**
</center>

一般**Dictionary**會支援三個操作：

1. 新增資料(insert)
2. 刪除資料(delete)
3. 查詢資料(search)


以上三個操作：

* 若以**平衡的BST**(例如AVL樹、[Red Black Tree](http://alrightchiu.github.io/SecondRound/red-black-tree-introjian-jie.html))便能在$O(\log{N})$完成。
* 「理想情況」的**Hash Table**希望能夠以$O(1)$完成。



</br>

<a name="dat"></a>

##以Array實現的Direct Access Table

若要在$O(1)$對資料進行「新增」、「刪除」以及「查詢」，沒有比Array(矩陣)更適合的人選了：

* 若直接把**Key**當作Array的**index**，並將**Value**存放進Array，這樣的實作稱為**Direct Access Table**。

但是**Direct Access Table**有兩個重大缺陷：

1. **Key**一定要是「非負整數(non-negative integer)」，才能作為Array的**index**。
2. 若**Key**的「範圍」非常大，可是**Key**的「數量」相對很少，那麼會非常浪費記憶體空間。
    * 以圖三為例，因為**Key**的範圍從$2$到$47$，所以Array的大小(size)至少要「$48$」，因此$15$筆資料用了$48$單位的記憶體空間，也就浪費超過三分之二的記憶體空間。若**Key**的範圍更大，浪費的程度將非常可觀。


<center>
![cc][f3]

**圖三：。**
</center>

關於第一點「**Key**不是非負整數」的缺陷，可以先利用一個「一對一函數(one-to-one function)」將**Key**對應到非負整數，問題即可解決。

例如，若**Key**是英文名字，那便利用「ASCII編碼」將字串轉換成非負整數。現要存放「T-MAC」與「KOBE」兩位球員的資料：

* 「T-MAC」$=84*10^4+45*10^3+77*10^2+65*10+67=893417$
* 「KOBE」$=75*10^3+79*10^2+66*10+69=83629$

以上的範例，至少需要一個大小為$893418$的Array，可是只有存放兩個英文名字，不划算。

**Hash Table**的核心概念就是要解決第二個缺陷：避免記憶體空間浪費。

</br>  

<a name="ht"></a>

##Hash Table的概念

**Hash Table**希望能夠將存放資料的「Table」的大小(size)降到「真正會存放進Table的資料的數量」，也就是「有用到的**Key**的數量」：

* 若有用到的**Key**之數量為$n$，**Table**的大小為$m$，那麼目標就是$m=\Theta (n)$。

要達到這個目標，必須引入**Hash Function**，將**Key**對應到符合**Table**大小$m$的範圍內，$index=h(Key)$，即可成為**Hash Table**的**index**，如圖四。

<center>
![cc][f4]

**圖四：Hash Table和Direct Access Table的差別在於Hash Function。**
</center>

優良的**Hash Function**($h()$)應具備以下特徵：

* $h()$的定義域為整個**Key**的宇集合$U$，值域應小於**Table**的大小$m$：  
$h:U\rightarrow \{\,0,1,...,m-1\,\},\:where \:|U|\gg m$
* 盡可能讓**Key**在經過**Hash Function**後，在值域(domain)能夠平均分佈(uniform distributed)，如此才不會有「兩筆資料存進同一個**Table**空格(稱為**slot**)」的情況。

可惜事與願違，因為$|U|\gg m$，再加上**Hash Function**設計不易，所以一定會發生**Collision**。


</br>    

<a name="collision"></a>

##出事了：Collision

**Collision**就是兩筆資料存進同一個**Table**之**slot**的情形，這將會使得查詢資料失敗(例如：使用item1的**Key**，卻回傳item2的資料)。


假設**Hash Function**為$h(Key)=Key \bmod{m}$，**Table**大小為$m=6$，目前要處理的資料之**Key**有$67,26,50,33,16,71$，那麼各個Key將被對應到的**index**如下，同時參考圖五：

* $h(67)=67 \bmod{6}=1$
* $h(26)=26 \bmod{6}=2$
* $h(50)=50 \bmod{6}=2$
* $h(33)=33 \bmod{6}=3$
* $h(16)=16 \bmod{6}=4$
* $h(71)=71 \bmod{6}=5$

「item26」與「item50」經過**Hash Function**後，同時想要將資料存進`Table[2]`，這就是**Collision**。

<center>
![cc][f5]

**圖三：。**
</center>


**Collision**在「可能使用到的**Key**」之數量遠大於**Table**大小(亦即$|U|\gg m$)的情況下，無可避免。

解決的辦法有二：

1. **Chaining**：使用Linked list把「Hashing到同一個slot」的資料串起來。
2. **Open Addressing**：使用**Probing Method**來尋找**Table**中「空的slot」存放資料。

這兩個方法將分別在後續文章介紹。



[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/HashTable%20series/Intro/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/HashTable%20series/Intro/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/HashTable%20series/Intro/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/HashTable%20series/Intro/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/HashTable%20series/Intro/f5.png?raw=true




</br>  


 
以上是**Hash Table**之基本概念介紹。

接下來兩篇將介紹**Chaining**與**Open Addressing**解決**Collision**。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch11](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch8](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [John W Lockwood：Lecture 6 - Hash Functions](http://www.arl.wustl.edu/projects/fpx/cse535/lecture/cse535_lecture6_Hash_Functions.pdf)
* [MIT 6.006：Lecture 8: Hashing with Chaining](http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-006-introduction-to-algorithms-fall-2011/lecture-videos/lecture-8-hashing-with-chaining/)





<a name="series"></a>

</br>

###Hash Table系列文章

[Hash Table：Intro(簡介)](http://alrightchiu.github.io/SecondRound/hash-tableintrojian-jie.html)   


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


