Title: Stack: Intro(簡介)    
Date: 2016-4-22 20:23   
Category: 演算法與資料結構  
Tags: C++, Intro, Stack,   
Summary: 介紹Stack(堆疊)。


</br>
###先備知識與注意事項

**Stack(堆疊)**是一種概念性的抽象資料結構，可以分別使用Array(陣列)與Linked list(連結串列)來實作。

本篇文章將介紹Stack的基本概念與應用，程式實作將留在下一篇。


***

##目錄

* [簡介：Stack](#intro)
* [Stack的應用](#app)
* [參考資料](#ref)
* [Linked list系列文章](#series)



</br>

<a name="intro"></a>

##簡介：Stack

**Stack**是具有「Last-In-First-Out」的資料結構(可以想像成一種裝資料的容器)，「最晚進入Stack」的資料會「最先被取出」，「最早進入Stack」的資料則「最晚被取出」。  

就像搬家的時候要把書(資料)裝進箱子(Stack)，假設箱子的開口大小剛剛好只能平放一本書，如果先放入《灌籃高手》，再放《笑傲江湖》，再放《搶救國文大作戰》，那麼到了新家之後，因為箱子底下沒有破洞，所以要拿到最底下的《灌籃高手》，就一定得先把上面的《搶救國文大作戰》與《笑傲江湖》拿出箱子才行。  

觀察以上的敘述，《灌籃高手》符合「最早放入箱子」而且「最晚拿出來」，而《搶救國文大作戰》符合「最晚放入箱子」且「最早拿出來」，其餘的資料也勢必符合此規則(每個資料至少會位於「最上面」一次)，便能夠將這個箱子視為一個Stack。  
由此可見，Stack是一個抽象的概念，只要符合「Last-In-First-Out」的資料結構，都可以視為Stack。


一般的**Stack**，會有以下幾個處理資料結構的功能：

* **Push(data)**：把資料放進Stack。
    * 把書放進箱子。
* **Pop**：把「最上面」的資料從Stack中移除。
    * 把位於箱子「最上面」的書拿出來。
* **Top**：回傳「最上面」的資料，不影響資料結構本身。
    * 確認目前位於箱子「最上面」的是哪本書。
* **IsEmpty**：確認Stack裡是否有資料，不影響資料結構本身。
    * 確認箱子裡面有沒有書。
* **getSize**：回傳Stack裡的資料個數，不影響資料結構本身。
    * 記錄目前箱子已經裝了多少本書。

<center>
![cc][f1]

**圖一。**
</center>

以圖一為例，一開始Stack是空的：

* 當`Push(6)`後，便把$6$放入Stack。並用一個稱為`top`的變數，記錄Stack最上面的資料為何，在此即為$6$。
* 當`Push(3)`、`Push(7)`後，便把$3、7$放入Stack。並更新`top`，使其記錄$7$。
* 當`Pop()`後，便把Stack中「最上面」的資料($7$)給移除，所以Stack中剩下$6、3$，並更新`top`記錄$3$。
* 當`Push(14)`、`Push(8)`後，便把$14、8$放入Stack。並更新`top`，使其記錄$8$。
* 在以上的任何階段，只要Stack有資料，使用函式`Top()`會回傳變數`top`所記錄的資料。
* 在以上的任何階段，使用`IsEmpty()`便能判斷Stack裡是否有存放資料。
* 在以上的任何階段，使用`getSize()`會回傳Stack中的資料個數。


Stack還有一項重要的特徵：

* 除了最上面的資料可以使用`Top()`來讀取，無法得知Stack裡面還有其餘哪些資料。要知道Stack裡的所有資料，只能靠`Pop()`，把資料一個一個拿出來檢查。



</br>

<a name="app"></a>

##Stack的應用




Stack最大的功能是「記得先前的資訊」，所以時常用來處理需要「回復到先前的狀態」的問題，也稱為**Back-Tracking**問題，例如：

* 編輯器(如word、sublime等等)中的**undo**。
* 網頁瀏覽器的「回到前一頁」功能。
* 編譯器(compiler)中的**Parsing**。
    * 參考：[Compiler Design - Top-Down Parser](http://www.tutorialspoint.com/compiler_design/compiler_design_top_down_parser.htm)。
* 任何**遞迴(recursion)**形式的演算法，都可以用Stack改寫，例如Depth-First Search(DFS，深度優先搜尋)。
    * 參考：[GeeksforGeeks:Iterative Depth First Traversal of Graph](http://www.geeksforgeeks.org/iterative-depth-first-traversal/)。
    * 因為**遞迴(recursion)**使用了系統的**Call Stack**。
* 於記憶體管理(memory management)中的**Call Stack**。
    * 參考：[Wikipedia：Call stack](https://en.wikipedia.org/wiki/Call_stack)。
    * 參考：[Stack Overflow：What and where are the stack and heap?](http://stackoverflow.com/questions/79923/what-and-where-are-the-stack-and-heap)。







[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/intro/f1.png?raw=true




</br>  

以上是**Stack**的初步介紹。  

下一篇文章將介紹以Array與Linked list實作Stack的方法。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch10](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch3](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Compiler Design - Top-Down Parser](http://www.tutorialspoint.com/compiler_design/compiler_design_top_down_parser.htm)
* [GeeksforGeeks:Iterative Depth First Traversal of Graph](http://www.geeksforgeeks.org/iterative-depth-first-traversal/)
* [Wikipedia：Call stack](https://en.wikipedia.org/wiki/Call_stack)
* [Stack Overflow：What and where are the stack and heap?](http://stackoverflow.com/questions/79923/what-and-where-are-the-stack-and-heap)
* [Gerald W. Kruse：Stack Applications](http://jcsites.juniata.edu/faculty/kruse/cs240/stackapps.htm)
* [Quora：What are the applications of queues and stacks in C++?](https://www.quora.com/What-are-the-applications-of-queues-and-stacks-in-C++)





<a name="series"></a>

</br>

###Stack系列文章

[Stack: Intro(簡介)](http://alrightchiu.github.io/SecondRound/stack-introjian-jie.html)  
   


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


