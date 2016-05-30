Title: Comparison Sort: Heap Sort(堆積排序法)    
Date: 2016-4-10 16:30  
Category: 演算法與資料結構  
Tags: C++, Comparison Sort, Sort, Heap  
Summary: 介紹Heap Sort(堆積排序法)。


</br>
###先備知識與注意事項

Sorting(排序)是基本的資料處理，舉例來說，進入圖書館的查詢系統，不論是想按照「出版日期」或是「相關程度」找書，都會得到「排序過」的結果。

常見的**Comparison Sort**及其時間複雜度如表一，假設問題有$N$筆資料：

<center>

||Quick Sort &nbsp;&nbsp;|Merge Sort &nbsp;&nbsp;| Heap Sort&nbsp;&nbsp;| Insertion Sort&nbsp;&nbsp;| Selection Sort&nbsp;&nbsp;|
|---|---|---|---|---|---|
|best case     |&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$ &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$ &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$ &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$|
|average case &nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$|
|worst case|&nbsp;&nbsp;&nbsp;&nbsp;$N^2$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;$N\log N$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$N^2$|

**表一：五種排序法之時間複雜度比較**

</center>

</br>  
**Binary Heap**可以分為Min Heap與Max Heap兩種。兩者用在排序上，僅僅是順序「由大到小」和「由小到大」的差別。  

本篇文章將介紹以Max Heap實現**Heap Sort(堆積排序法)**的方法。

有關Min Heap的內容，請參考[Priority Queue：Binary Heap](http://alrightchiu.github.io/SecondRound/priority-queuebinary-heap.html)。

***

##目錄

* [Binary Heap(二元堆積)](#bh)
    * [函式：MaxHeapify](#heapify)
    * [函式：BuildMaxHeap](#build)
* [Heap Sort(堆積排序法)](#hs)
* [程式碼](#code)
* [參考資料](#ref)
* [Comparison Sort系列文章](#series)



</br>

<a name="bh"></a>

##Binary Heap(二元堆積)

Binary Heap有兩項基本特徵：

**特徵一**：Binary Heap之結構可以視作**Complete Binary Tree**。

* 如圖一(a)，數值$1$~$9$，一共有9個元素，按照**Complete Binary Tree**之順序規則，填滿位置$1$~$9$，以index($1$)~index($9$)表示。

這樣的優點是方便尋找「parent-child」之關係，以index($i$)的node為例：

* 其**left child**必定位在**index($2i$)**；
* 其**right child**必定位在**index($2i+1$)**；
* 其**parent**必定位在**index($\lfloor i/2 \rfloor$)**。

以圖一(a)中位於index($3$)之node($8$)為例：

* 其**left child**為index($6$)之node($2$)；
* 其**right child**為index($7$)之node($3$)；
* 其**parent**為index($1$)之node($9$)。

<center>
![cc][f1]

**圖一(a)：黑色數字是index，藍色數字是value。**
</center>

**特徵二**：若將位於index($i$)之node視為subtree之**root**，那麼，可將此Binary Heap分為兩類：

* **Max Heap**：在每一個subtree中，**root**之「數值」要比兩個**child**之「數值」還要大，見圖一(a)：
    * $value(i)>value(2i)$
    * $value(i)>value(2i+1)$

* **Min Heap**：在每一個subtree中，**root**之「數值」要比兩個**child**之「數值」還要小，見圖一(b)：
    * $value(i)<value(2i)$
    * $value(i)<value(2i+1)$

<center>
![cc][f2]

**圖一(b)。**
</center>

特別注意：在同一個subtree裡，**leftchild**(index($2i$))與**rightchild**(index($2i+1$))的「數值」大小順序不重要，只要和**root**(index($i$))比較即可。  
這也是Binary Heap與Binary Search Tree其中一項區別。

要滿足Binary Heap特有的「parent-child」之關係，只要讓矩陣中index($0$)的位置閒置，從index($1$)開始存放資料，即可使用矩陣(array)來表示Binary Heap。


接著介紹兩個函式，把任意矩陣轉換成Max Heap。

</br>

<a name="heapify"></a>

###函式：MaxHeapify

`MaxHeapify()`的功能，是要「由上而下」地，以Max Heap的規則(**root**的數值「大於」兩個**child**的數值)，調整矩陣。

以圖二(a)為例，觀察subtree「index($2$)-index($4$)-index($5$)」之「數值」：

* root：index($i=2$)為$1$
* leftchild：index($2i=4$)為$9$
* rightchild：index($2i+1=5$)為$4$

不符合Max Heap規則，所以要想辦法把這三個數值中的「最大值」，調整到index($i=2$)，也就是這棵subtree的**root**。

方法如下：

* 找到這三者的最大值，並以`int largest`記錄該最大值的index。
    * 圖二(a)中，把`largest`記錄為index($4$)。
* 將index(root)與index(largest)這兩個node互換位置，如此一來，當前的subtree必定能滿足Max Heap規則。
    * 圖二(b)中，將index($2$)與index($4$)的node互換。
    * subtree「index($2$)-index($4$)-index($5$)」的數值分別為「$9-1-4$」，符合Max Heap。
* 繼續以index(largest)當作新的subtree的root，檢查新的subtree是否符合Max Heap規則。
    * 圖二(b)中，subtree「index($4$)-index($8$)-index($9$)」再次不滿足Max Heap，便重複上述步驟，得到圖二(c)。


<center>
![cc][f3]

**圖二(a)。**
</center>

<center>
![cc][f4]

**圖二(b)。**
</center>

<center>
![cc][f5]

**圖二(c)。**
</center>


如此一來，有被`MaxHeapify()`檢查過的subtree，都會符合Max Heap規則。

因此，只要對所有「具有child的node」檢查一次`MaxHeapify()`，便能夠把一個任意矩陣調整成Max Heap，這就是下一個函式`BuildMaxHeap()`的功能。


</br>

<a name="build"></a>

###函式：BuildMaxHeap


有了`MaxHeapify()`之後，只要對所有「具有child的node」進行`MaxHeapify()`，也就是位於index($1$)$~$index($\lfloor N/2 \rfloor$)的node，就能夠將任意矩陣調整成Max Heap。

* 以圖三為例，Binary Heap共有$9$個node，便從index($4$)往index($1$)，逐一對node進行`MaxHeapify()`，即可得到Max Heap。

為什麼只要對「具有child的node」調整呢？  
因為Max Heap的規則是「比較root的數值與child的數值」，如果是沒有child的node(屬於**leaf node**)，就一定不會違反Max Heap的規則。


<center>
![cc][BuildMaxHeap]

**圖三。**
</center>


</br>

<a name="hs"></a>

##Heap Sort(堆積排序法)

經過`BuildMaxHeap()`之後，便能將任意矩陣調整成Max Heap。

那麼要如何將此Max Heap做排序呢？

Max Heap的特徵是「第一個node具有最大值」，如果要將資料「由小到大」排序，步驟如下：

1. 把「第一個node」和「最後一個node」互換位置。
2. **假裝heap的「最後一個node」從此消失不見**。
3. 對「第一個node」進行`MaxHeapify()`。

重複以上步驟，從heap的最後一個node開始，一路往上，直到root，便能得到排好序的矩陣。

以圖四(a)的Max Heap為例，現要將其調整成「由小到大」排好序的矩陣。

<center>
![cc][f17]

**圖四(a)。**
</center>

首先，從「最後一個位置」開始，將index($9$)的node與index($1$)的node互換位置，見圖四(b)。  
如此，便把「最大值」的node($9$)給放到最後一個位置，以及「不是最大值」的node($1$)換到第一個位置。

<center>
![cc][f18]

**圖四(b)。**
</center>

接著，最重要的概念就是「假裝最後一個node消失不見」：

* 因為此時，heap的「最後一個node」一定是「最大值」的node，也就表示，如果要得到「由小到大」的排序，那麼，此時便已經完成「最大值node」的調整。

同時，目前的index($1$)的node一定不是「最大值」，所以要利用`MaxHeapify()`重新調整「矩陣」，使其符合Max Heap規則。

又因為「假裝最後一個node消失不見」，所以，接下來要調整的「矩陣」，是「忽略index($9$)」的矩陣，因此只要考慮由「index($1$)~index($8$)」所形成的矩陣即可。  

* 圖四(c)中的`size`，即表示`MaxHeapify()`要處理的矩陣之**size**。
* 此次的`MaxHeapify()`將會碰到subtree(index($1$)-index($2$)-index($3$))與subtree(index($3$)-index($6$)-index($7$))。


<center>
![cc][f19]

**圖四(c)。**
</center>

經過圖四(c)的`MaxHeapify()`調整，由「index($1$)~index($8$)」形成的矩陣，又再次滿足Max Heap，見圖四(d)。


<center>
![cc][f20]

**圖四(d)。**
</center>

接著，再繼續重複上述步驟：

* 交換「目前考慮的矩陣」的「最後一個位置node」與「第一個node」；
* 以`MaxHeapify()`調整「目前考慮的矩陣」。

便能得到「由小到大」排好序的矩陣，見圖四(e)。

<center>
![cc][HeapSort]

**圖四(e)。**
</center>


</br>

<a name="code"></a>

##程式碼

範例程式碼包含幾個部分：

`swap()`：執行「交換node」。因為要實質交換`array`中的資料，所以必須使用**reference**或使**pointer**作為函式的參數(argument)。

`MaxHeapify()`：「由上而下」，逐一檢查subtree，使subtree滿足Max Heap規則。函式的參數分別為

* `array`：記錄heap的矩陣。
* `root`：當前的subtree之root的index。
* `length`：這是「目前要處理的矩陣」的長度，同時也用來判斷**child**是否超過矩陣長度。

`BuildMaxHeap()`：對所有「具有**child**的node」進行`MaxHeapify()`。

`HeapSort()`：將Max Heap轉換成「由小到大」排好序的矩陣。小細節：

* 因為heap是從index($1$)開始儲存資料，所以要先把index($0$)給空下來，最後再將其刪除。
* 這裡是利用`std::vector<int>`的成員函式(member function)「`insert()`」與「`erase()`」處理。
關於`std::vector`，請參考：[Cplusplus：std::vector](http://www.cplusplus.com/reference/vector/vector/)。

以及`main()`，建立矩陣，並執行`HeapSort()`，驗證結果。


```cpp
// C++ code
#include <iostream>
#include <vector>

void swap(int &p1, int &p2){
    int temp = p1;
    p1 = p2;
    p2 = temp;
}

void MaxHeapify(std::vector<int> &array, int root, int length){
    
    int left = 2*root,      // 取得left child
    right = 2*root + 1,     // 取得right child
    largest;                // largest用來記錄包含root與child, 三者之中Key最大的node
    
    if (left <= length && array[left] > array[root])
        largest = left;
    else
        largest = root;
    
    if (right <= length && array[right] > array[largest])
        largest = right;
    
    if (largest != root) {                         // 如果目前root的Key不是三者中的最大
        swap(array[largest], array[root]);         // 就調換root與三者中Key最大的node之位置
        MaxHeapify(array, largest, length);        // 調整新的subtree成Max Heap
    }
}

void BuildMaxHeap(std::vector<int> &array){
    
    for (int i = (int)array.size()/2; i >= 1 ; i--) {
        MaxHeapify(array, i, (int)array.size() - 1);     // length要減一, 因為heap從1開始存放資料
    }
}

void HeapSort(std::vector<int> &array){
    
    array.insert(array.begin(), 0);                     // 將index(0)閒置
    
    BuildMaxHeap(array);                                // 將array調整成Max Heap
    
    int size = (int)array.size() -1;                    // size用來記錄「目前要處理的矩陣」之長度
    for (int i = (int)array.size() -1; i >= 2; i--) {
        swap(array[1], array[i]);                       // 將最大值放到array的最後一個位置
        size--;
        MaxHeapify(array, 1, size);                     // 調整「忽略最後一個位置」的矩陣
    }
    
    array.erase(array.begin());                         // 將index(0)刪除
}

void PrintArray(std::vector<int> &array){
    for (int i = 0; i < array.size(); i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main() {

    int arr = {9, 4, 1, 6, 7, 3, 8, 2, 5};
    std::vector<int> array(arr, arr+sizeof(arr)/sizeof(int));
    
    std::cout << "original:\n";
    PrintArray(array);

    HeapSort(array);
    
    std::cout << "sorted:\n";
    PrintArray(array);

    return 0;
}

```
output:

```cpp
original:
9 4 1 6 7 3 8 2 5
sorted:
1 2 3 4 5 6 7 8 9
```



[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f6.png?raw=true
[BuildMaxHeap]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/BuildMaxHeap.gif?raw=true
[HeapSort]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/HeapSort.gif?raw=true
[f17]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f17.png?raw=true
[f18]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f18.png?raw=true
[f19]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f19.png?raw=true
[f20]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/HeapSort/f20.png?raw=true



</br>  

以上便是以Max Heap實現**Heap Sort**之介紹。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch6](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch5, Ch7](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Priority Queue：Intro(簡介) & Binary Heap](http://alrightchiu.github.io/SecondRound/priority-queueintrojian-jie-binary-heap.html)
* [Cplusplus：std::vector](http://www.cplusplus.com/reference/vector/vector/)


<a name="series"></a>

</br>

###Comparison Sort系列文章

[Comparison Sort: Insertion Sort(插入排序法)](http://alrightchiu.github.io/SecondRound/comparison-sort-insertion-sortcha-ru-pai-xu-fa.html)   
[Comparison Sort: Quick Sort(快速排序法)](http://alrightchiu.github.io/SecondRound/comparison-sort-quick-sortkuai-su-pai-xu-fa.html)  
[Comparison Sort: Heap Sort(堆積排序法)](http://alrightchiu.github.io/SecondRound/comparison-sort-heap-sortdui-ji-pai-xu-fa.html)  
[Comparison Sort: Merge Sort(合併排序法)](http://alrightchiu.github.io/SecondRound/comparison-sort-merge-sorthe-bing-pai-xu-fa.html)   


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


