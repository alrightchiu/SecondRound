Title: Comparison Sort: Insertion Sort(插入排序法)    
Date: 2016-3-22 23:49  
Category: 演算法與資料結構  
Tags: C++, Comparison Sort, Sort  
Summary: 介紹Insertion Sort(插入排序法)。
 


</br>
###先備知識與注意事項

Sorting(排序)是非常基本的資料處理操作，舉例來說，進入圖書館的查詢系統，不論是想按照「出版日期」或是「相關程度」找書，都會得到「排序過」的結果。

常見的**Comparison Sort**及其時間複雜度如表一，假設問題有$N$筆資料：

<center>

||Quick Sort &nbsp;&nbsp;|Merge Sort &nbsp;&nbsp;| Heap Sort&nbsp;&nbsp;| Insertion Sort&nbsp;&nbsp;| Selection Sort&nbsp;&nbsp;|
|---|---|---|---|---|---|
|best case     |&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$ &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$ &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$ &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$|
|average case &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$|
|worst case|&nbsp;&nbsp;&nbsp;&nbsp;$N^2$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$|

**表一：五種排序法之時間複雜度比較**

</center>



本篇文章將介紹**Insertion Sort(插入排序法)**。

***


##目錄

* [Insertion Sort(插入排序法)](#insertionsort)
* [程式碼](#code)
* [參考資料](#ref)
* [Comparison Sort系列文章](#series)

</br>

<a name="insertionsort"></a>

##Insertion Sort(插入排序法)

想像手上有一副撲克牌，若想要將紙牌從左到右按照「小到大」排序。

**Insertion Sort**的方法為：將第$i$張紙牌加入「前$i-1$張排序過」的紙牌組合，得到$i$張排序過的紙牌組合。

以圖一為例，從左邊數來的前三張紙牌已經排好序：index(0)、index(1)、index(2)分別為$1、3、5$，現在要將第四張紙牌(index(3)，數值為$2$)加入前三張牌，想要得到「前四張排好序」的紙牌組合。  
經由觀察，最終結果的紙牌順序為$1、2、3、5$，可以透過以下步驟完成：

* 原先index(2)的$5$搬到index(3)；
* 原先index(1)的$3$搬到index(2)；
* 原先index(3)的$2$搬到index(1)；

如此便能把第$4$張紙牌加入(**insert**)「前$3$張排序過」的紙牌組合，得到「前$4$張排序過」的紙牌組合。

<center>
![cc][f1]

**圖一：想像這是撲克牌。**  
</center>

由以上說明可以看出，**Insertion Sort**要求，在處理第$i$筆資料時，第$1$筆至第$i-1$筆資料必須先排好序。  
那麼，只要按照順序，從第$1$筆資料開始處理，便能確保「處理第$2$筆資料時，第$1$筆資料已經排好序」，「處理第$3$筆資料時，第$1$筆、第$2$筆資料已經排好序」，依此類推，若共有$N$筆資料，必定能夠在處理第$N$筆資料時，將第$1$筆至第$N-1$筆資料排序過。

<center>
![cc][f2]

**圖二。**  
</center>

以下將以圖二的矩陣資料為例，進行**Insertion Sort**，見圖三。  


<center>
![cc][f3]

**圖三。**  
</center>

</br>
那麼，第$i$筆資料要怎麼知道其在前$1$~$i-1$筆資料之間的順序位置？以圖三為例，當要將`array[3]`的$2$，加入`array[0]`~`array[2]`的數列時，要怎麼得知，其將被換到`array[1]`的位置？    
就是要透過**comparison**(比較)。


詳細步驟如下：

現考慮將`array[1]`的$3$加入`array[1]`之前的數列，也就是與`array[0]`的$5$，一起形成「排好序」的`array[0]`~`array[1]`，見圖四(a)：

* 定義變數`int i`為「目前欲處理」的資料的index。
    * 在此`i=1`，`array[i]=3`。
* 定義變數`int j`來表示「已經排好序」的數列的「最後一個」元素之index。
    * 在此`j=0`，`array[j]=5`。
    * `int j`會不斷遞減，`j--`，來檢查`array[j]`是否比「目前欲處理」的資料還大。
* 定義變數`int key=array[i]`，把「目前欲處理」的`array[i]`以`key`儲存，避免`array[i]`被覆蓋掉。


<center>
![cc][f4]

**圖四(a)。**  
</center>

接著，比較`key`與`array[j]`的大小，同時確認index(j)沒有超出矩陣範圍。  

若`key<array[j]`(並且`j>-1`)，就表示「目前欲處理」的資料(原先位在index(i)的資料)，比`array[j]`(也就是index(i-1)的資料)還要小，於是將`array[j]`「往後移」，見圖四(b)：

* 換位置的方式類似`swap()`，先執行`array[j+1]=array[j]`，也就是將原先的`array[j]`「往後移一個位置」。
    * 在此，把$5$放進`array[1]`。
    * 可以預期，原先$5$在的位置array[0]會被`key`給補上。如此便完成`array[0]`~`array[1]`的排序。
* 接著，因為不確定前$1$~$i-1$筆資料中，是否還有資料比`key`大，所以執行`j--`，繼續「往前」比較。
    * 在此例，因為`j--`後，`j`等於$-1$，已經超過矩陣範圍，所以便結束程序。


<center>
![cc][f5]

**圖四(b)。**  
</center>


當兩項條件：`j>-1`與`key<array[j]`中，有任何一項條件不滿足時，便表示「已經檢查到前$1$~$i-1$筆資料的盡頭」，或是「已經沒有比`key`還小的資料」，於是便執行`array[j+1]=key`，把`key`放回矩陣裡。

* 當不滿足上述兩項條件時，`j+1`就會是`key`的位置。  
在此，`j`等於$-1$，`j+1`等於$0$，表示`key`是$1$~$i$筆資料中，最小的資料。

以上步驟，便完成`array[0]`~`array[1]`的排序。

<center>
![cc][f6]

**圖四(c)。**  
</center>


目前為止，可以確定`array[0]`~`array[1]`已經排好序，便能夠再接著將`array[2]`加入前面兩項，完成`array[0]`~`array[2]`的排序，見圖四(d)。

<center>
![cc][f7]

**圖四(d)。**  
</center>

再看一次，要把`array[3]`放入`array[0]`~`array[2]`的排序中，形成`array[0]`~`array[3]`的排序。

其中，在比較`key`是否比`array[0]`還小時，發現`key>array[j]`(`key`為$2$，`array[j]`為$1$)，便不需要把`array[0]`「往後移」，並把`key`放進`array[j+1]`，完成`array[0]`~`array[3]`的排序。


<center>
![cc][f8]

**圖四(e)。**  
</center>

重複上述步驟，便可完成`array[0]`~`array[5]`的排序，如圖三所示。


</br>

<a name="code"></a>

##程式碼

程式碼很直觀：

* `int i`是「目前要處理」的資料index(i)；
* `int j=i-1`是用來指出前$1$~$i-1$筆資料的index；
* 第一個`for`迴圈，用意是把每一個矩陣元素都視為「目前要處理」的資料一次；
* 在`for`迴圈裡的`while`迴圈，用意是把「目前要處理」的資料與前$1$~$i-1$筆資料做比較，並找到適當的位置，將$1$~$i$筆資料做排序。  

以及`main()`，建立如圖二的矩陣，進行`InsertionSort()`，並將矩陣元素以`PrintArray()`印出。

```cpp
// C++ code

#include <iostream>
void InsertionSort(int *arr, int size){
    
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (key < arr[j] && j >= 0) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}
void PrintArray(int *arr, int size){
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
int main() {
    
    int array[6] = { 5, 3, 1, 2, 6, 4 };
    std::cout << "original:\n";
    PrintArray(array, 6);
    
    InsertionSort(array, 6);
    
    std::cout << "sorted:\n";
    PrintArray(array, 6);
    return 0;
}
```

output:

```cpp
original:
5 4 1 2 6 4 
sorted:
1 2 4 4 5 6 
```


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/InsertionSort/f8.png?raw=true



</br>


以上便是**Insertion Sort**的介紹。

關於時間複雜度的部分：  
(欲將資料由左至右以「小到大」排序)

* **best case**：若要處理的序列是$1、2、...、N$，那麼只要比較$N$次即可完成排序，時間複雜度為$O(N)$，此即為。
    * 依此推論，當問題已經「接近完成排序」的狀態時，使用**Insertion Sort**會非常有效率。
* **worst case**：若要處理的序列恰好是顛倒順序，$N、N-1、...、2、1$，那麼位於index(i)的元素，需要比較「$i-1$次」，完成演算法總共要比較$frac{N(N-1)}{2}$次，時間複雜度為$O(N^2)$。
* **average case**：時間複雜度也是$O(N^2)$。
    * 詳情請參考：[Mordecai Golin：Average Case Analysis of Insertionsort](http://home.cse.ust.hk/faculty/golin/COMP271Sp03/Notes/Ins_Sort_Average_Case.pdf)。

當問題的資料量較小時(欲排序的元素之數目較小)，使用**Insertion Sort**會很有效率，這是因為和**Quick Sort**、**Merge Sort**、**Heap Sort**相比，**Insertion Sort**不具有「遞迴」形式，因此不需要**系統的stack**，詳情請參考：

* [Stackoverflow：Is recursion ever faster than looping?](http://stackoverflow.com/questions/2651112/is-recursion-ever-faster-than-looping)
* [Stackoverflow：Recursion or Iteration?](http://stackoverflow.com/questions/72209/recursion-or-iteration)

再加上前面提到的**best case**特徵，有些演算法會在**Quick Sort**中加入**Insertion Sort**，讓剩下的「接近完成排序」的資料以**Insertion Sort**處理，使排序更有效率，詳情請參考：

* [Stackoverflow：How to optimize quicksort](http://stackoverflow.com/questions/12454866/how-to-optimize-quicksort)

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch1](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch7](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Wikipedia：Comparison sort](https://en.wikipedia.org/wiki/Comparison_sort)
* [Wikipedia：Sorting algorithm](https://en.wikipedia.org/wiki/Sorting_algorithm)
* [Mordecai Golin：Average Case Analysis of Insertionsort](http://home.cse.ust.hk/faculty/golin/COMP271Sp03/Notes/Ins_Sort_Average_Case.pdf)
* [Stackoverflow：Is recursion ever faster than looping?](http://stackoverflow.com/questions/2651112/is-recursion-ever-faster-than-looping)
* [Stackoverflow：Recursion or Iteration?](http://stackoverflow.com/questions/72209/recursion-or-iteration)
* [Stackoverflow：How to optimize quicksort](http://stackoverflow.com/questions/12454866/how-to-optimize-quicksort)


<a name="series"></a>

</br>

###Comparison Sort系列文章

[Comparison Sort: Insertion Sort(插入排序法)](http://alrightchiu.github.io/SecondRound/comparison-sort-insertion-sortcha-ru-pai-xu-fa.html)   




回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>






