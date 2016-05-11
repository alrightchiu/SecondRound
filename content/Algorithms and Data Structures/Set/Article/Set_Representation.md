Title: Set：表示法(representation)     
Date: 2016-5-11 20:50  
Category: 演算法與資料結構  
Tags: C++, Set(Find and Union)  
Summary: 介紹Set的表示方法。


</br>
###先備知識與注意事項

// 比較 Set 與 Sequence

Sequense(序列)是

例如某堂課的期末報告要分組時，組員之間不會有「順序」的差別，

***

##目錄

* [Set表示法](#set)
    * [函式：FindSet](#find)
    * [函式：Collapsing](#Collapsing)
    * [函式：UnionSet](#union)
* [程式碼](#code)
* [參考資料](#ref)
* [Set系列文章](#series)


</br>

<a name="set"></a>

##Set表示法


Set是不講求順序(order)的資料彙集(collection)，其概念可以使用類似Tree的資料結構來實現：

* 以**root**來代表不同的Set；
* Set中的其餘element之**parent/predecessor**指向**root**。

本篇文章介紹的表示法將以矩陣(array)處理與Set有關的兩件事：

1. 判斷哪些element屬於同一個subset；
2. 判斷每個subset中各有幾個element。

<center>
![cc][f2]

**圖一(a)：共有三個Set。**
</center>

配合圖一(a)：

* 若element(X)之「矩陣值」為負值(negative value)，即表示element(X)「沒有predecessor」，element(X)就是其所屬於的subset的root。
    * root就像每個subset的門牌，可以用來分辨某兩個element是否屬於同一個subset。
    * 見圖一(a)，element(1)、element(4)、element(7)皆為各自所代表的Set之root。
* 若element(Y)之「矩陣值」為正值(positive value)，該數值即為element(Y)的predecessor。
    * 見圖一(a)，element(0)之predecessor為element(7)，即表示element(0)與element(7)屬於同一個subset。
    * 從element(5)一路往predecessor方向找，會先找到element(3)，再找到element(1)，而element(2)也會找到element(1)，即表示element$:1、2、3、5$皆屬於同一個subset。
    * 同理，element$:0、6、7$屬於同一個subset。而element(4)自成一個subset。
* 若element(X)是某個subset的root，其矩陣數值的絕對值(absolute value)即代表該subset中的「element數目」。
    * 見圖一(a)，element(1)之矩陣數值為$-4$，取絕對值為$4$，即表示element(1)所在的subset中共有4個element。
    * 同理，element(7)所在的subset中共有$3$個element。


</br>  
在處理**Disjoint Set**的問題時，經常要處理兩個問題：

* `FindSet(element)`：確認某個element所隸屬的subset為何。
* `UnionSet(X,Y)`：將element(X)與element(Y)合併放進同一個Set。

</br>  

<a name="find"></a>

###FindSet

若利用上述的Set表示法，當進行`FindSet(element)`時，便以每個subset之root作為判斷的依據，只要在矩陣中一路回溯`predecessor`，直到root(矩陣值為負值)後，回傳root，就代表了該element所在的subset。



<center>
![cc][f2]

**圖一(a)：共有三個Set。**
</center>


以圖一(a)為例，觀察`FindSet(6)`：

* `Array[6]`為$0$
* `Array[0]`為$7$
* `Array[7]`為$-3$


由此可以確定：element(6)屬於「以element(7)作為代表」的subset。

再看`FindSet(5)`：

* `Array[5]`為$3$
* `Array[3]`為$1$
* `Array[1]`為$-4$

也就表示，element(5)屬於「以element(1)作為代表」的subset。

所以element(5)與element(6)屬於不同subset。  
</br>    

<a name="Collapsing"></a>

###Collapsing

另外，有個提升演算法效率的概念稱為**Set Collapsing**，目的是要讓`FindSet()`能夠在時間複雜度O($1$)完成。

本篇文章的[範例程式](#code)中，將「執行`FindSet()`時，順便進行**Set Collapsing**」定義成`FindSetCollapsing()`：

* 如圖一(b)，`FindSetCollapsing(5)`會將element(5)的predecessor調整成element(1)，那麼下次想要知道element(5)屬於哪個subset時，就不需要經過element(3)，便能夠以時間複雜度：$O(1)$完成。
* 參考[Graph: 利用DFS和BFS尋找Connected Component](http://alrightchiu.github.io/SecondRound/graph-li-yong-dfshe-bfsxun-zhao-connected-component.html#algorithm)對`SetCollapsing()`的介紹。



<center>
![cc][f3]

**圖一(b)。**
</center>


</br>  

<a name="union"></a>

###UnionSet

進行`UnionSet(X,Y)`時，只要將element(X)所在的Set之root指向element(Y)所在的Set之root即可(或者相反指向，將Set(Y)之root指向Set(X)之root)，見圖二。

<center>
![cc][f6]

**圖二。**
</center>


一般而言，`UnionSet(X,Y)`會利用「Set中element的個數」來判斷要以Set(X)的root還是Set(Y)的root作為合併後的Set之代表root。

* 通常「Set中element個數越多」，那麼該Set之**height**會比較大(此處的height表示Set中，root與距離其最遠的vertex之間的edge數)，進行`FindSet(element)`時的時間複雜度也較大。  
* 因此，若把「Set中element個數較少」的Set併進「Set中element個數較多」的Set，一般情況下，最後合併好的Set之**height**不會變，但是相反的情況，會使**height**增加，而**height**增加會影響`FindSet()`的效率。
* 試比較圖二中的兩種合併準則：
    * 將Set(4)併進Set(1)後，新的Set之**height**不變。
    * 反之，將Set(1)併進Set(4)時，新的Set之**height**比原先的Set(1)還大。




</br>

<a name="code"></a>

##程式碼

範例程式碼包含：

* 函式`FindSetCollapsing()`與`UnionSet()`如上一小節介紹。
* `PrintArray()`印出Array元素。
* 在`main()`中，進行簡單的測試。


```cpp
// C++ code
#include <iostream>
#include <iomanip>      // for std::setw()

int FindSetCollapsing(int *subset, int i){      
    
    int root;  
    for (root = i; subset[root] >= 0; root = subset[root]);  // 找到root
    
    while (i != root) {             // 進行collapsing
        int parent = subset[i];
        subset[i] = root;
        i = parent;
    }
    
    return root;
}

void UnionSet(int *subset, int x, int y){
    
    int xroot = FindSetCollapsing(subset, x),
        yroot = FindSetCollapsing(subset, y);
    
    // 用rank比較, 負越多表示set越多element, 所以是值比較小的element比較多
    // xroot, yroot的subset[]一定都是負值
    
    // x比較多element或是一樣多的時候, 以x作為root
    if (subset[xroot] <= subset[yroot]) {     
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    }
    else {    //  subset[xroot] > subset[yroot], 表示y比較多element
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot;
    }
}

void PrintArray(int *array, int size){

    for (int i = 0; i < size; i++){   
        std::cout << std::setw(3) << i;
    }
    std::cout << std::endl;
    for (int i = 0; i < size; i++){   
        std::cout << std::setw(3) << array[i];
    }
    std::cout << std::endl;
}

int main(){

    const int size = 6;
    int array[size] = {-1, -1, -1, -1, -1, -1};
    PrintArray(array, size);

    UnionSet(array, 1, 2);
    std::cout << "After union(1,2):\n";
    PrintArray(array, size);
    
    UnionSet(array, 0, 2);
    std::cout << "After union(0,2):\n";
    PrintArray(array, size);
    
    UnionSet(array, 3, 5);
    std::cout << "After union(3,5):\n";
    PrintArray(array, size);
    
    UnionSet(array, 2, 5);
    std::cout << "After union(2,5):\n";
    PrintArray(array, size);


    std::cout << "element(5) belongs to Set(" << FindSetCollapsing(array, 5) << ").\n";
    std::cout << "After collapsing:\n";
    PrintArray(array, size);

    return 0;
}

```
output:

```cpp
  0  1  2  3  4  5
 -1 -1 -1 -1 -1 -1
After union(1,2):
  0  1  2  3  4  5
 -1 -2  1 -1 -1 -1
After union(0,2):
  0  1  2  3  4  5
  1 -3  1 -1 -1 -1
After union(3,5):
  0  1  2  3  4  5
  1 -3  1 -2 -1  3
After union(2,5):
  0  1  2  3  4  5
  1 -5  1  1 -1  3
element(5) belongs to Set(1).
After collapsing:
  0  1  2  3  4  5
  1 -5  1  1 -1  1
```

`main()`中測試結果如圖三：

<center>
![cc][SetUnion]

**圖三。**
</center>


[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Kruskal_fig/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Kruskal_fig/f3.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Set/Representation/f6.png?raw=true
[SetUnion]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Set/Representation/SetUnion.gif?raw=true



</br>  


 
以上便是其中一種Set表示法之介紹。

其他表示法可以參考以下連結：

* [HackerEarth：Disjoint Set Union (Union Find)](https://www.hackerearth.com/notes/disjoint-set-union-union-find/)




</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch23](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Dickson Tsai：Disjoint Sets - Data Structures in 5 Minutes](https://www.youtube.com/watch?v=gcmjC-OcWpI)
* [HackerEarth：Disjoint Set Union (Union Find)](https://www.hackerearth.com/notes/disjoint-set-union-union-find/)
* [Graph: 利用DFS和BFS尋找Connected Component](http://alrightchiu.github.io/SecondRound/graph-li-yong-dfshe-bfsxun-zhao-connected-component.html#algorithm)


<a name="series"></a>

</br>

###Set系列文章

[Set：表示法(representation)]()


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


