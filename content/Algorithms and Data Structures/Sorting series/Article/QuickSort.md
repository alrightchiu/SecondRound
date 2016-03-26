Title: Comparison Sort: Quick Sort(快速排序法)    
Date: 2016-3-26 17:51  
Category: 演算法與資料結構  
Tags: C++, Comparison Sort, Sort, Divide and Conquer  
Summary: 介紹Quick Sort(快速排序法)。
 


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



本篇文章將介紹**Quick Sort(快速排序法)**。

***


##目錄

* [Quick Sort(快速排序法)](#insertionsort)
    * [介紹：Partition](#partition)
* [程式碼](#code)
* [參考資料](#ref)
* [Comparison Sort系列文章](#series)



</br>

<a name="insertionsort"></a>

##Quick Sort(快速排序法)

**Quick Sort**是一種「把大問題分成小問題處理」的**Divide and Conquer**方法，概念如下：

* 在數列中任意挑選一個數，稱為**pivot**，然後**調整數列**，使得「所有在**pivot**左邊的數，都比**pivot**還小」，而「在**pivot**右邊的數都比**pivot**大」。
* 接著，將所有在**pivot**左邊的數視為「新的數列」，所有在**pivot**右邊的數視為「另一個新的數列」，「分別」重複上述步驟(選**pivot**、調整數列)，直到分不出「新的數列」為止。

<center>
![cc][f1]

**圖一(a)。**  
</center>

以圖一(a)為例，考慮數列{$9、4、1、6、7、3、8、2、5$}，先任意選定$5$為**pivot**，接著調整數列，使得**pivot**左邊的數{$4、1、3、2$}皆小於$5$，而**pivot**右邊的數{$9、8、6、7$}皆大於$5$。

* 目前為止，{$4、1、3、2$}之間的大小順序還不必理會，{$9、8、6、7$}之間的大小順序也不必理會。



接著，分別將**pivot**左邊的數列{$4、1、3、2$}與**pivot**右邊的數列{$9、8、6、7$}視為新的數列處理，重複上述步驟(選**pivot**、調整數列)，直到分不出「新的數列」為止，見圖一(b)。

<center>
![cc][f2]

**圖一(b)。**  
</center>

以上的步驟中：

* **pivot**可以任意挑選，在此是固定挑選數列(矩陣)的最後一個元素。
* 在「新的數列」上只是重複相同的步驟(選**pivot**、調整數列)，可以利用**遞迴(recursion)**處理。


所以，最關鍵的就是如何「調整數列」，江湖上尊稱其為：**Partition**。


</br>

<a name="partition"></a>

###介紹：Partition()

如同圖一(a)，**Partition**的功能就是把數列「區分」成「小於**pivot**」與「大於**pivot**」兩半。

<center>
![cc][f1]

**圖一(a)。**  
</center>


詳細步驟如下：

定義變數(variable)，見圖二(a)：

* `int front`為數列的「最前端」index。
    * 此例，`front`為index($0$)。
* `int end`為數列的「最尾端」index。
    * 此例，`end`為index($8$)。
* `int i`為所有小於**pivot**的數所形成的數列的「最後位置」。
    * 一開始將index(`i`)初始化為`front-1`，因為有可能數列中，所有數都比**pivot**大。
    * 一旦發現有數比**pivot**小，index(`i`)便往後移動(`i++`)，表示「所有小於**pivot**的數所形成的數列」又增加了一名成員。
    * 當演算法結束時，所有在index(`i`)左邊的數，都比**pivot**小，所有在index(`i`)右邊的數，都比**pivot**大。
* `int j`是讓**pivot**與其餘數值逐一比較的index，從`front`檢查到`end-1`(因為`end`是**pivot**自己)。
* `int pivot=array[end]`，以數列最後一個數做為**pivot**。
    * 此例，`pivot`為$5$。
    * **pivot**的「數值」可以是任意的，挑選矩陣的最後一個位置是為了方便index(`j`)的移動，也可以挑選任意位置。



<center>
![cc][f3]

**圖二(a)。**  
</center>


接著，開始移動index(`j`)，從index($0$)到index($7$)，將數列(矩陣)元素逐一與`pivot`比較，並進行調整(用`swap()`調整)。


見圖二(b)，一開始，`j`$=0$，`i`$=-1$：

* 比較`pivot`與`array[j=0]`，發現`pivot`$=5<$`array[0]`$=9$，所以不需要移動index(`i`)。
* 移動index(`j`)，`j++`，繼續往後比較。


<center>
![cc][f4]

**圖二(b)。**  
</center>

見圖二(c)，此時`j`$=1$，`i`$=-1$：

* 比較`pivot`與`array[j=1]`，發現`pivot`$=5>$`array[1]`$=4$，便執行：
    * `i++`：移動index(`i`)，表示又找到一個比**pivot**小的數。此時，`i`$=0$。
    * `swap(array[i=0],array[j=1])`：透過這個`swap()`，便能把比**pivot**小的數，放到比**pivot**大的數的「前面」(也就是矩陣的左邊)。
* 移動index(`j`)，`j++`，繼續往後比較。


<center>
![cc][f5]

**圖二(c)。**  
</center>

見圖二(d)，此時`j`$=2$，`i`$=0$：

* 比較`pivot`與`array[j=2]`，發現`pivot`$=5>$`array[2]`$=1$，便執行：
    * `i++`：移動index(`i`)，表示又找到一個比**pivot**小的數。此時，`i`$=1$。
    * `swap(array[i=1],array[j=2])`：透過這個`swap()`，便能把比**pivot**小的數({$4、1$})，放到比**pivot**大的數({$9$})的「前面」。
* 移動index(`j`)，`j++`，繼續往後比較。




<center>
![cc][f6]

**圖二(d)。**  
</center>


見圖二(e)，此時`j`$=3$，`i`$=1$：

* 比較`pivot`與`array[j=3]`，發現`pivot`$=5<$`array[3]`$=6$，所以不需要移動index(`i`)。
* 移動index(`j`)，`j++`，繼續往後比較。



<center>
![cc][f7]

**圖二(e)。**  
</center>

見圖二(f)，此時`j`$=4$，`i`$=1$：

* 比較`pivot`與`array[j=4]`，發現`pivot`$=5<$`array[4]`$=7$，所以不需要移動index(`i`)。
    * 到目前為止，漸漸可以看出，「比**pivot**小」的數形成一個數列({$4、1$})，「比**pivot**大」的數形成另一個數列({$9、6、7$})，最後只要把**pivot**插入這兩個數列中間，就完成的`Partition()`。
* 移動index(`j`)，`j++`，繼續往後比較。



<center>
![cc][f8]

**圖二(f)。**  
</center>


見圖二(g)，此時`j`$=5$，`i`$=1$：

* 比較`pivot`與`array[j=5]`，發現`pivot`$=5>$`array[5]`$=3$，便執行：
    * `i++`：移動index(`i`)，表示又找到一個比**pivot**小的數。此時，`i`$=2$。
    * `swap(array[i=2],array[j=5])`：透過這個`swap()`，便能把比**pivot**小的數({$4、1、3$})，放到比**pivot**大的數({$6、7、9$})的「前面」。
* 移動index(`j`)，`j++`，繼續往後比較。


<center>
![cc][f9]

**圖二(g)。**  
</center>


見圖二(h)，此時`j`$=6$，`i`$=2$：

* 比較`pivot`與`array[j=6]`，發現`pivot`$=5<$`array[6]`$=8$，所以不需要移動index(`i`)。
* 移動index(`j`)，`j++`，繼續往後比較。



<center>
![cc][f10]

**圖二(h)。**  
</center>


見圖二(i)，此時`j`$=7$，`i`$=2$：

* 比較`pivot`與`array[j=7]`，發現`pivot`$=5>$`array[7]`$=2$，便執行：
    * `i++`：移動index(`i`)，表示又找到一個比**pivot**小的數。此時，`i`$=3$。
    * `swap(array[i=3],array[j=7])`：透過這個`swap()`，便能把比**pivot**小的數({$4、1、3、2$})，放到比**pivot**大的數({$6、7、9、8$})的「前面」。
* 移動index(`j`)，`j++`，繼續往後比較。



<center>
![cc][f11]

**圖二(i)。**  
</center>

見圖二(j)，此時`j`$=8$，`i`$=3$。  
因為index(`j`)只需要從`front`移動到`end-1`即可。當index(`j`)走到`end`時，便結束此**迴圈**，表示數列中的所有數都已經和**pivot**比較過了。

* `i++`，把index(`i`)從「所有比**pivot**小的數列」的最後一個位置，移動到「所有比**pivot**大的數列」的第一個位置，此時`i`$=4$。
* 接著執行`swap(array[i=4],array[end])`：便成功把**pivot**插入兩個數列之間了。



<center>
![cc][f12]

**圖二(j)。**  
</center>

經過以上步驟，便把數列分成三部分，如圖一(a)：

1. 比**pivot**小的數所形成的數列；
2. **pivot**；
3. 比**pivot**大的數所形成的數列。

<center>
![cc][f1]

**圖一(a)。**  
</center>

接著，只要再對「左數列」與「右數列」，分別重複上述的「選**pivot**、調整數列」步驟，如圖三，直到新數列的`front`$\geq$`end`，便能完成對數列的排序。  
(回頭看一眼圖一(b)。)

<center>
![cc][f13]

**圖三：以遞迴的方式，對新的數列做排序。**  
</center>



</br>

<a name="code"></a>

##程式碼

程式碼很直觀：

* `swap()`：交換矩陣元素之位置，使用時機：
    * 當`Partition()`中條件式`if(pivot<array[j])`時；
    * 當index(`j`)檢查完除了`pivot=array[end]`之外的元素時。 
* `Partition()`：將數列調整成「比**pivot**小」、「**pivot**」、「比**pivot**大」的主要函式。
* `QuickSort()`：進行**Quick Sort**的主要函式，以**遞迴(recursion)**的形式，將數列(矩陣)不斷拆解成更小的數列並以此排序。

以及`main()`，以矩陣表示如圖一(a)的數列，進行`QuickSort()`，並將矩陣元素以`PrintArray()`印出。

完整程式範例如下：

```cpp
// C++ code

#include <iostream>
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
int Partition(int *arr, int front, int end){
    int pivot = arr[end];
    int i = front -1;
    for (int j = front; j < end; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[end]);
    return i+1;
}
void QuickSort(int *arr, int front, int end){
    if (front < end) {
        int pivot = Partition(arr, front, end);
        QuickSort(arr, front, pivot - 1);
        QuickSort(arr, pivot + 1, end);
    }
}
void PrintArray(int *arr, int size){
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
int main() {
    
    int n = 9;
    int arr_quick[] = {9, 4, 1, 6, 7, 3, 8, 2, 5};
    std::cout << "original:\n";
    PrintArray(arr_quick, n);

    QuickSort(arr_quick, 0, n-1);
    
    std::cout << "sorted:\n";
    PrintArray(arr_quick, n);
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


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Sorting%20series/ComparisonSort_fig/QuickSort/f13.png?raw=true



</br>


以上便是**Quick Sort**的介紹。  
因為不需要額外的記憶體空間，因此，只要能避免**worst case**，那麼**Quick Sort**會非常有效率。

關於時間複雜度的部分，請參考：

* [Khan Academy：Analysis of quicksort](https://www.khanacademy.org/computing/computer-science/algorithms/quick-sort/a/analysis-of-quicksort)。

優化的方法，請參考：

* [Stackoverflow：How to optimize quicksort](http://stackoverflow.com/questions/12454866/how-to-optimize-quicksort)



</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch7](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch7](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Wikipedia：Comparison sort](https://en.wikipedia.org/wiki/Comparison_sort)
* [Wikipedia：Sorting algorithm](https://en.wikipedia.org/wiki/Sorting_algorithm)
* [Stackoverflow：How to optimize quicksort](http://stackoverflow.com/questions/12454866/how-to-optimize-quicksort)


<a name="series"></a>

</br>

###Comparison Sort系列文章

[Comparison Sort: Insertion Sort(插入排序法)](http://alrightchiu.github.io/SecondRound/comparison-sort-insertion-sortcha-ru-pai-xu-fa.html)   
[Comparison Sort: Quick Sort(快速排序法)](http://alrightchiu.github.io/SecondRound/comparison-sort-quick-sortkuai-su-pai-xu-fa.html)  



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>






