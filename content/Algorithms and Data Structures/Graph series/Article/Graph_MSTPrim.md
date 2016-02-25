Title: Minimum Spanning Tree：Prim's Algorithm     
Date: 2016-2-25 20:30  
Category: 演算法與資料結構  
Tags: C++, Graph, MST, Greedy Algorithm, Cut(partition)  
Summary: 介紹於Graph中，利用Prim's Algorithm求得Minimum Spanning Tree(MST，最小生成樹)。


</br>
###先備知識與注意事項

本篇文章要接著介紹另一個尋找MST的演算法：**Prim's Algorithm**。

建議讀者可以先閱讀在[Minimum Spanning Tree：Intro(簡介)](http://alrightchiu.github.io/SecondRound/minimum-spanning-treeintrojian-jie.html)介紹過的「演算法概念」與**Theorem1**與**Corollary2**，對理解接下來的內容會有些幫助。


***

##目錄

* [Prim's Algorithm](#algorithm)
* [程式碼](#code)
* [參考資料](#ref)
* [MST系列文章](#series)



</br>

<a name="algorithm"></a>

##Prim's Algorithm

考慮圖一(a)的Graph，目標是要在此Graph上找到MST。

<center>
![cc][f1]

**圖一(a)。**
</center>


**Prim's Algorithm**之演算法將使用三個資料項目：

* `predecessor[]`：記錄最後在MST中，各個vertex之間的「parent-child」關係。
* `key[]`：此為「挑選edge」的關鍵，`key[]`記錄的會是edge的weight，但是會將weight「暫存在vertex上」，表示「到達該vertex的成本」，演算法的最後將挑選出能夠以「最低成本」抵達該vertex的edge，此edge上的兩個vertex即形成「parent-child」關係，能夠記錄進`predecessor[]`。
* `visited[]`：用來標記出，哪些vertex已經在MST裡面，哪些還沒有。已經是MST之vertex標記為$1$，其餘為$0$。

有了`key[]`與`visited[]`，就能正確挑選出edge，形成最後的MST。


演算法步驟如下：

首先，初始化所有vertex的資料項目，見圖一(b)：

* `predecessor[]`設成$-1$，表示vertex沒有predecessor。
* `key[]`設成「無限大」，表示所有vertex都沒有辦法經由與其餘vertex相連之edge抵達。
* `visited[]`設成$0$，表示MST裡面還沒有vertex。  
(程式範例中，`visited[]`的資料型態是用`bool`，因此設為`false`)


<center>
![cc][f2]

**圖一(b)。**
</center>

初始化之後，選定其中一個vertex作為起點。  
這個起點會是MST的root，如果不要求MST之起點為何，那麼起點可以任意挑選。  
在此選定vertex(2)作為起點，便將`key[2]`設成$0$。

* 更新`key[]`是關鍵，原因在於，演算法會根據`key[]`的大小，每次挑選出`key[]`值最小的vertex放進MST。

接著便開始演算法的主要迴圈。  

* 先從`key[]`中挑選出數值最小(可以想成「抵達成本」最小)的vertex，放進MST。  
在此挑選到的是vertex(2)，見圖一(c)。
* 由於vertex(2)已經屬於MST，便更新`visited[2]=1`。


<center>
![cc][f3]

**圖一(c)。**
</center>


接著，找到所有與vertex(2)具有edge相連的vertex(X)，比較所有edge(2,X)之weight與vertex(X)的`key[]`，如果weight比`key[X]`小，便更新`predecessor[X]`成vertex(2)，並且把`key[X]`更新成weight(2,X)：

* 表示從vertex(2)經由edge(2,X)抵達vertex(X)，只要花費weight(2,X)的成本，而到最後，若Graph上沒有從其他vertex經由「成本更小的edge」能夠抵達vertex(X)，那麼MST中，就會存在edge(2,X)，因為在所有連結vertex(2)與vertex(X)的edge中，edge(2,X)的成本最低。
* 另外一種情況，若weight(2,X)比`key[X]`大，那就不更新`key[X]`，表示從Graph中某個vertex(Y)到達vertex(X)，會比從vertex(2)到達vertex(X)的成本更低。也不更新`predecessor[X]`，維持vertex(Y)作為vertex(X)的predecessor。

根據圖一(c)，與vertex(2)相連的vertex分別是vertex(1)、vertex(3)、vertex(6)，經比較後發現： 
 
* $weight(2,1)<$`key[1]`；
* $weight(2,3)<$`key[3]`；
* $weight(2,6)<$`key[6]`；  
    
此時便更新`key[]`與`predecessor[]`，見圖一(d)：

* `key[1]=weight(2,1)=10`，`predecessor[1]=2`；
* `key[3]=weight(2,3)=5`，`predecessor[3]=2`；
* `key[6]=weight(2,6)=3`，`predecessor[6]=2`。



<center>
![cc][f4]

**圖一(d)。**
</center>


總結圖一(c)與圖一(d)的步驟：

1. 選取`key[]`值最小的vertex放進MST，假設為vertex(X)；
2. 將vertex(X)之`visited[]`標記為$1$(或是`true`)；
3. 比較weight與`key[]`，判斷是否需要更新所有與vertex(X)具有edge相連之vertex的`key[]`與`predecessor[]`。

以上便完成一次**Prim's Algorithm**的主要迴圈。  
若Graph上有$V$個vertex，接下來只要繼續上述步驟「$V-1$」次，就能得到Graph上的MST。

接著，繼續從`key[]`挑選出數值最小的vertex(3)放進MST，更新`visited[3]=1`，見圖一(e)。

<center>
![cc][f5]

**圖一(e)。**
</center>

再比較所有與vertex(3)具有edge相連之vertex的`key[]`，根據圖一(e)：

* vertex(2)已經進入MST中(`visited[2]=1`)，忽略不計；
* vertex(6)與vertex(3)相連，不過weight(6,3)比`key[6]=8`還要大，表示從vertex(3)走到vertex(6)之成本高於從vertex(2)走到vertex(6)，因此不更新`key[6]`與`predecessor[6]`；
* vertex(4)與vertex(3)相連，並且weight(4,3)$<$`key[4]=infinity`，於是更新`key[4]=weight(4,3)`，`predecessor[4]=3`，見圖一(f)。

<center>
![cc][f6]

**圖一(f)。**
</center>

接著重複上述步驟，直到Graph上所有vertex都被放進MST中(`visited[]`全部變成$1$)，即可找到MST，見圖一(g)至圖一(n)。


<center>
![cc][f7]

**圖一(g)：將vertex(4)放進MST中**，`visited[4]=1`**。**
</center>



<center>
![cc][f8]

**圖一(h)：從vertex(4)探索vertex(1)、vertex(5)、vertex(6)。**
</center>


<center>
![cc][f9]

**圖一(i)：將vertex(1)放進MST中**，`visited[1]=1`**。**
</center>



<center>
![cc][f10]

**圖一(j)：從vertex(1)探索vertex(0)、vertex(2)、vertex(4)、vertex(6)。**
</center>



<center>
![cc][f11]

**圖一(k)：將vertex(6)放進MST中**，`visited[6]=1`**。**
</center>



<center>
![cc][f12]

**圖一(l)：將vertex(0)放進MST中**，`visited[0]=1`**。**
</center>


<center>
![cc][f13]

**圖一(m)：從vertex(0)探索vertex(1)、vertex(5)。**
</center>


<center>
![cc][f14]

**圖一(n)：將vertex(5)放進MST中**，`visited[5]=1`，**MST建立完畢。**
</center>


見圖一(n)，當Graph上所有vertex的`visited[]`都被設為$1$(或是`true`)後，MST便建立完畢。


那麼**Prim's Algorithm**的運作原則是什麼？

觀察圖一(d)至圖一(j)，被放進MST的vertex恰好都與vertex(6)有edge相連，因此，vertex(6)總共被「檢查了四次」(比較weight與`key[6]`四次)，並且「更新了兩次」(`key[6]`從$\infty$更新成$8$，最後更新成$2$)，可以肯定的是，若vertex(6)一定要與Graph中的其中一個vertex相連(這是MST的要求)，那麼**Prim's Algorithm**一定能夠替vertex(6)挑選出最小weight的edge：

* 以圖一(n)為例，vertex(6)共有四條edge連結至其他vertex，而最後是透過weight為$2$的edge(4,6)與vertex(4)相連。

再看圖二，當vertex(3)被放進MST時，vertex(2)已經被放進MST，因此可以把這兩個vertex視為一個connected component，並且找到一個Cut $(C,V-C)$，其中$C=${$2,3$}，$V-C=${$0,1,4,5,6$}，而在Cut上的crossing edge總共有：

* edge(1,2)，weight$=10$；
* edge(2,6)，weight$=8$；
* edge(3,6)，weight$=9$；
* edge(3,4)，weight$=7$；

顯然，light edge為edge(3,4)，因此edge(3,4)必定會是MST中的edge。

這便是**Theorem1**所證明的事。

<center>
![cc][f15]

**圖二。**
</center>




</br>

<a name="code"></a>

##程式碼

以下的程式範例包含了`class Graph`與`main()`。

`class Graph`：使用`Adjacency Matrix`而不是`Adjacency List`。   

`PrimMST()`為主要演算法，內容如前一節所介紹。  

`MinKeyExtract()`根據`visited`找到，所有尚未放進MST的vertex中，`key[]`最小值的vertex。

最後，`main()`利用`AddEdge()`建立出Graph的`AdjMatrix`，並執行`PrimMST()`。



```cpp
// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <iomanip>      // for setw()

const int Max_Weight = 1000;
struct Edge{
    int from, to, weight;
    Edge(){};
    Edge(int u, int v, int w):from(u), to(v), weight(w){};
};

class GraphMST{
private:
    int num_vertex;
    std::vector<std::vector<int>> AdjMatrix;
public:
    GraphMST():num_vertex(0){};
    GraphMST(int n):num_vertex(n){
        AdjMatrix.resize(num_vertex);
        for (int i = 0; i < num_vertex; i++) {
            AdjMatrix[i].resize(num_vertex);
        }
    }
    void AddEdge(int from, int to, int weight);
 
    void PrimMST(int Start = 0);        // 可以指定起點Start, 若沒有指定, 則從vertex(0)作為MST的root
    friend int MinKeyExtract(int *key, bool *visited, int size);
};

int MinKeyExtract(int *key, bool *visited, int size){

    int min = Max_Weight, min_idx = 0;
    for (int i = 0; i < size; i++) {
        if (visited[i]==false && key[i]<min) {
            min = key[i];
            min_idx = i;
        }
    }
    return min_idx;
}
void GraphMST::PrimMST(int Start){

    int key[num_vertex], predecessor[num_vertex];
    bool visited[num_vertex];
    
    for (int i = 0; i < num_vertex; i++) {
        key[i] = Max_Weight;
        predecessor[i] = -1;
        visited[i] = false;     // false表示vertex還沒有被visited
    }
    
    key[Start] = 0;
    for (int i = 0; i < num_vertex; i++) {
        int u = MinKeyExtract(key, visited, num_vertex);
        visited[u] = true;
        for (int i = 0; i < num_vertex; i++) {
            if ( visited[i]==false && AdjMatrix[u][i]!=0 && AdjMatrix[u][i]<key[i]) {
                predecessor[i] = u;
                key[i] = AdjMatrix[u][i];
            }
        }
    }
    // print MST
    std::cout << std::setw(3) << "v1" << " - " << std::setw(3) << "v2"<< " : weight\n";
    int i = (Start+1)%num_vertex;   // 若從4開始, i依序為5,6,0,1,2,3
    while (i != Start) {
        std::cout << std::setw(3) << predecessor[i] << " - " << std::setw(3) << i
                  << " : " << std::setw(4) << AdjMatrix[predecessor[i]][i] <<"\n";
        i = (++i)%num_vertex;       // 到了6之後, 6+1 = 7, error:bad_access, 透過mod把7喬回0
    }
}
void GraphMST::AddEdge(int from, int to, int weight){
    AdjMatrix[from][to] = weight;
}

int main(){

    GraphMST g6(7);
    g6.AddEdge(0, 1, 5);g6.AddEdge(0, 5, 3);
    g6.AddEdge(1, 0, 5);g6.AddEdge(1, 2, 10);g6.AddEdge(1, 4, 1);g6.AddEdge(1, 6, 4);
    g6.AddEdge(2, 1, 10);g6.AddEdge(2, 3, 5);g6.AddEdge(2, 6, 8);
    g6.AddEdge(3, 2, 5);g6.AddEdge(3, 4, 7);g6.AddEdge(3, 6, 9);
    g6.AddEdge(4, 1, 1);g6.AddEdge(4, 3, 7);g6.AddEdge(4, 5, 6);g6.AddEdge(4, 6, 2);
    g6.AddEdge(5, 0, 3);g6.AddEdge(5, 4, 6);
    g6.AddEdge(6, 1, 4);g6.AddEdge(6, 2, 8);g6.AddEdge(6, 3, 9);g6.AddEdge(6, 4, 2);
    
    std::cout << "MST found by Prim:\n";
    g6.PrimMST(2);

    return 0;
}
```
output:

```cpp
MST found by Prim:
 v1 -  v2 : weight
  2 -   3 :    5
  3 -   4 :    7
  0 -   5 :    3
  4 -   6 :    2
  1 -   0 :    5
  4 -   1 :    1
```

結果如同圖一(n)：

<center>
![cc][f14]

**圖一(n)。**
</center>


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f13.png?raw=true
[f14]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f14.png?raw=true
[f15]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/MST_Prim_fig/f15.png?raw=true

</br>  


若參考[Introduction to Algorithms, Ch23](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844)所提供的Prim's Algorithm之虛擬碼(pseudo code)，將會發現其中有個「Min-Priority Queue」的資料結構，如果讀者有興趣，可以參考以下連結(筆者還在磨劍，請見諒)：


* [Theory of Programming：Prim's Algorithm](http://theoryofprogramming.com/2015/03/27/prims-algorithm/)
* [Rashid Bin Muhammad：Jarnik's (Prim's) Algorithm](http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/GraphAlgor/primAlgor.htm)


 
以上便是利用**Prim's Algorithm**尋找MST之介紹。

若讀者能夠花些時間比較這兩種種樹的策略，將會發現，兩邊都是天才啊。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch23](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Rashid Bin Muhammad：Jarnik's (Prim's) Algorithm](http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/GraphAlgor/primAlgor.htm)
* [Theory of Programming：Prim's Algorithm](http://theoryofprogramming.com/2015/03/27/prims-algorithm/)
* [Greedy Algorithms | Set 5 (Prim’s Minimum Spanning Tree (MST))](http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/)



<a name="series"></a>

</br>

###MST系列文章

[Minimum Spanning Tree：Intro(簡介)](http://alrightchiu.github.io/SecondRound/minimum-spanning-treeintrojian-jie.html)  
[Minimum Spanning Tree：Kruskal's Algorithm](http://alrightchiu.github.io/SecondRound/minimum-spanning-treekruskals-algorithm.html)



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


