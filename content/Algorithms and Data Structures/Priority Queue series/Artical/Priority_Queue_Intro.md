Title: Priority Queue：Intro(簡介)
Date: 2016-3-5 21:25  
Category: 演算法與資料結構  
Tags: C++, Intro, Priority Queue, Heap, Binary Tree,  
Summary: 簡介Priority Queue(優先權佇列)之基本概念。



</br>
###先備知識與注意事項

本篇文章將介紹抽象的資料結構：Priority Queue(優先權佇列)的基本概念。  


***

##目錄

* [簡介：Priority Queue(優先權佇列)](#pq)
* [參考資料](#ref)
* [Priority Queue系列文章](#series)

</br>

<a name="pq"></a>

##簡介：Priority Queue(優先權佇列)

「待辦事項」通常是表面上看起來沒有順序、但其實具有「執行時的優先順序」的一堆事情。

Priority Queue(優先權佇列)就像在處理「待辦事項」，能夠在眾多各自具有優先順序之資料堆中，取出「最重要/最不重要」的項目：

**Priority**：對資料項目賦予「權重/優先權」，用以表示資料的重要程度。  
**Queue**：隊伍、佇列、順序，意味著把資料整理成「某種順序」的資料結構。

* 所謂的「某種順序」，可能是「先進先出(First-In-First-Out)」順序：每次要從此資料結構讀取資料時，必定是拿到「先進入」該資料結構的資料。  
    (請參考：[Queue: Intro(簡介)](http://alrightchiu.github.io/SecondRound/queue-introjian-jie-bing-yi-linked-listshi-zuo.html))
* 也有可能是額外賦予資料的「權重/優先權」順序：每次要從此資料結構讀取資料時，必定會拿到具有「最大值/最小值權重」的資料。

如果每次要從Priority Queue讀取資料時，都拿到「權重最大」的資料，則稱此為**Max-Priority Queue**。  
反之，若每次讀取資料，都拿到「權重最小」的資料，則稱此為**Min-Priority Queue**。

舉例來說，如果每天起床固定要：

* 削鉛筆，重要性$2$分；
* 梳洗，重要性$10$分；
* 剪指甲，重要性$4$分；

那麼，以一個Max-Priority Queue來管理「每天起床的待辦事項」，第一件事情要先刷牙上廁所，然後是剪指甲，最後再削一些無關緊要的鉛筆。  
反之，若以一個Min-Priority Queue來管理「每天起床的待辦事項」，第一件事情要先削鉛筆，然後是剪指甲，最後才能去刷牙。

一個Max-Priority Queue，最基本會有三種操作：

1. **Insert**：將資料加入Queue中。
    * 例如，把「削鉛筆」、「梳洗」、「剪指甲」三件事情寫進Queue裡。 
2. **IncreaseKey**：當某項資料的「重要性提高」時，需要在Queue中改變資料的權重，以下將以Key代表權重。
    * 例如，最近突變出某種藉由指甲垢傳遞的流感病毒，使得「剪指甲」的重要性提高，便需要增加「剪指甲」的Key，假設Key提高到$11$分，那麼每天起床的工作順序就變成：「剪指甲」、「梳洗」、「削鉛筆」。 
3. **ExtractMax**：取得最重要(Key最大)的資料，並將其從Queue中移除。
    * 每天起床後，先從「待辦事項」得知，最重要的事情是「梳洗」，並在完成之後，將其從「待辦事項」移除，避免重複執行。 

與之對應的Min-Priority Queue之基本操作則是：

1. **Insert**：將資料加入Queue中。 
2. **DecreaseKey**：當某項資料的「重要性降低」時，需要改變在Queue中資料的Key。
3. **ExtractMin**：取得重要性最低(Key最小)的資料，並將其從Queue中移除。

</br>  
為了替**Prim's Algorithm**以及**Dijkstra's Algorithm**鋪路，本系列文章將以Min-Priority Queue作為代表。  
不過只要掌握Priority Queue的概念，Max-Priority Queue就只是桌上的一塊小蛋糕。

稍微困難的是實現方法，[Fundamentals of Data Structures in C++, Ch9](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)一共列出了六種實現方法(可能還有更多)：

1. Leftist Tree(左傾樹)
2. Binomial Heap(二項式堆積)
3. Fibonacci Heap(費式堆積)
4. Pairing Heap(成對堆積)
5. Symmetric Min-Max Heap(對稱式最小-最大堆積)
6. Interval Heap(區間堆積)

前四種資料結構稱為**Single-End Priority Queue(SEPQ)**，亦即，該資料結構只能取得「最大」或是「最小」權重的資料。  
後兩種資料結構稱為**Double-End Priority Queue(DEPQ)**，可以同時取得「權重最大」以及「權重最小」的資料。

下ㄧ篇文章，將介紹比上述六種資料結構更初級的**Binary Heap**來實現Min-Priority Queue。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch6](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch5, Ch9](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Ashley Montanaro：Priority queues and Dijkstra’s algorithm](https://www.cs.bris.ac.uk/~montanar/teaching/dsa/dijkstra-handout.pdf)
* [禪心劍氣相思骨：Priority Queue 解析1 - 從binary heap開始](http://blog.yam.com/rockmanray/article/44952434)
* [Single-Source Shortest Path：Dijkstra's Algorithm](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathdijkstras-algorithm.html)
* [Minimum Spanning Tree：Prim's Algorithm](http://alrightchiu.github.io/SecondRound/minimum-spanning-treeprims-algorithm.html)
* [Wikipedia：Priority Queue](https://en.wikipedia.org/wiki/Priority_queue)



<a name="series"></a>

</br>

###Priority Queue系列文章

[Priority Queue：Intro(簡介)](http://alrightchiu.github.io/SecondRound/priority-queueintrojian-jie.html)  
[Priority Queue：Binary Heap](http://alrightchiu.github.io/SecondRound/priority-queuebinary-heap.html)  



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


