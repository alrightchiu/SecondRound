Title: Single-Source Shortest Path：Bellman-Ford Algorithm    
Date: 2016-3-1 20:12  
Category: 演算法與資料結構  
Tags: C++, Graph, Shortest Path, Relaxation 
Summary: 介紹Bellman-Ford Algorithm。


</br>
###先備知識與注意事項

本篇文章將介紹**Bellman-Ford Algorithm**來回應上一篇[Single-Source Shortest Path：Intro(簡介)](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathintrojian-jie.html)的問題，演算法的概念主要圍繞在：

* **Relaxation** 
* **Convergence property**
* **Path-relaxation property**

建議讀者可以先進[傳送門](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathintrojian-jie.html#relax)精彩回顧一下。



***

##目錄

* [Graph之表示法(representation)](#representation)
* [Bellman-Ford Algorithm](#bf)
* [程式碼](#code)
    * [檢查Graph中是否存在negative cycle](#nc)
* [參考資料](#ref)
* [Shortest Path系列文章](#series)


</br>

<a name="representation"></a>

##Graph之表示法(representation)

在[BFS/DFS系列文章]()使用**Adjacency List**代表Graph，不過edge上沒有weight，可以使用如下資料結構：

* `std::vector<std::list<int>> AdjList;`
  
[MST系列文章]()的Graph利用**Adjacency Matrix**，因此可以使用如下資料結構，把weight放進矩陣之元素：

* `std::vector<std::vector<int>> AdjMatrix;`

本篇文章將利用**Adjacency List**來代表weighted edge，方法是使用C++標準函式庫(STL)的`std::pair<int,int>`，使得`AdjList`的資料結構更新為：

* `std::vector<std::list<std::pair<int,int>>> AdjList;`

若有一條從vertex(0)(`from`)指向vertex(1)(`to`)的edge(0,1)之weight為$5$，若利用新的`AdjList`表示，如圖一：

* 將`from`以`std::vector`的**index**表示；
* 每一個`std::pair<int,int>`都是一個`std::list`的node；
* 將`to`放在`std::pair<int,int>`的第一個`int`資料項；
* 將**weight**放在`std::pair<int,int>`的第二個`int`資料項，

關於`std::pair<int,int>`的用法，可以參考：[Cplusplus：std::pair](http://www.cplusplus.com/reference/utility/pair/?kw=pair)。

<center>
![cc][f1]

**圖一。**
</center>



</br>

<a name="bf"></a>

##Bellman-Ford Algorithm

根據**Path-Relaxation Property**，考慮一條從vertex(0)到vertex(K)之路徑$P:v_0-v_1-...-v_K$，如果在對path之edge進行`Relax()`的順序中，**曾經出現**edge(v<sub>0</sub>,v<sub>1</sub>)、edge(v<sub>1</sub>,v<sub>2</sub>)、...、edge(v<sub>K-1</sub>,v<sub>K</sub>)的順序，那麼這條path一定是最短路徑，滿足`distance[K]`$=\delta(v_0,v_K)$。

* 在對edge(v<sub>1</sub>,v<sub>2</sub>)進行`Relax()`之前，只要已經對edge(v<sub>0</sub>,v<sub>1</sub>)進行過`Relax()`，那麼，不管還有其餘哪一條edge已經進行過`Relax()`，`distance[2]`必定會等於$\delta(0,2)$，因為**Convergence property**。


**Bellman-Ford Algorihm**就用最直覺的方式滿足**Path-Relaxation Property**：

* 一共執行$|V|-1$次迴圈。
* 在每一次迴圈裡，對「**所有的edge**」進行`Relax()`。
* 經過$|V|-1$次的「所有edge之`Relax()`」後，必定能夠產生「按照最短路徑上之edge順序的`Relax()`順序」，也就能夠得到最短路徑。
    * 由於從起點走到任一vertex之最短路徑，最多只會有$|V|-1$條edge，因此，執行$|V|-1$次迴圈必定能夠滿足「最壞情況」(worst case)。


</br>
考慮如圖二(a)之Graph，以vertex(0)作為起點。  
並且根據圖二(a)之**Adjacency List**，得到在**Bellman-Ford Algorihm**中對所有edge進行`Relax()`之順序如圖二(b)。

<center>
![cc][f2]

**圖二(a)。**

![cc][f3]

**圖二(b)：根據圖二(a)之**`AdjList`**，對所有edge進行**`Relax()`**之順序。**
</center>

</br>
首先，對Graph的資料項目`distance`與`predecessor`進行初始化，見圖二(c)：

* `distance`：將起點vertex之`distance`設為$0$，並將其餘vertex之`distance`設為無限大($\infty$)。
    * 如果在演算法結束後，某個vertex之`distance`仍然無限大($\infty$)，則表示Graph中沒有一條path能夠從起點vertex走到該vertex。
    * 回顧`Relax()`，因為只有起點vertex(0)之`distance`設為$0$，其餘`distance`都是無限大，因此，在進行「所有edge之`Relax()`」時，一定是從與起點vertex(0)相連之edge先開始。
* `predecessor`：將所有vertex之`predecessor`設為$-1$，表示目前還沒有`predecessor`。對其餘vertex來說，可以想成還沒有被起點vertex找到。

<center>
![cc][f4]

**圖二(c)。**
</center>

</br>
接著開始$|V|-1$次的迴圈，並在每次迴圈，都對「所有edge」進行`Relax()`。  
(此例，共$5$次迴圈，每次按照圖二(b)之順序處理$10$條edge)。

###第一次迴圈(iteration#1)

對第一條edge(0,1)進行`Relax()`，見圖三(a)：

* 比較`distance[1]`與`distance[0]+w(0,1)`，發現」從vertex(0)走到vertex(1)」之成本較原先的成本低(原先的成本即為`distance[1]`)，因此更新`distance[1]`為`distance[0]+w(0,1)`。
* 同時更新`predecessor[1]=0`，表示vertex(1)是從vertex(0)走過去的。

<center>
![cc][f5]

**圖三(a)。**
</center>

對第二、三條edge(1,2)、edge(1,4)進行`Relax()`，見圖三(b)：

* 由於`distance[2]`與`distance[4]`皆大於從vertex(1)走過去的成本，因此更新：
    * `distance[2]=distance[1]+w(1,2)`；
    * `distance[4]=distance[1]+w(1,4)`；


<center>
![cc][f6]

**圖三(b)。**
</center>

對第四、五條edge(2,4)、edge(2,5)進行`Relax()`，見圖三(c)：

* 由於`distance[4]`小於「從vertex(2)走過去的成本(`distance[2]+w(2,4)`)，因此，仍維持從vertex(1)走到vertex(4)之路徑。
* 而`distance[5]`大於`distance[2]+w(2,5)`，因此更新：
    * `distance[5]=distance[2]+w(2,5)`；
    * `predecessor[5]=2`；
    

<center>
![cc][f7]

**圖三(c)。**
</center>

對第六條edge(3,2)進行`Relax()`，見圖三(d)：

* 顯然，「從vertex(1)走到vertex(2)」之成本`distance[2]`遠小於「從vertex(3)走到vertex(2)」之成本`distance[3]+w(2,3)`，因此，不更新抵達vertex(2)之路徑。

<center>
![cc][f8]

**圖三(d)。**
</center>

對第七、八條edge(4,3)、edge(4,5)進行`Relax()`，見圖三(e)：

* 由於`distance[3]`大於從vertex(4)走到vertex(3)的成本(`distance[4]+w(4,3)`)，因此更新：
    * `distance[3]=distance[4]+w(4,3)`；
    * `predecessor[3]=4`；
* 而且`distance[5]`也大於`distance[4]+w(4,5)`，表示「從vertex(4)走到vertex(5)」會比「從vertex(2)走到vertex(5)」還近，便更新：
    * `distance[5]=distance[4]+w(4,5)`；
    * `predecessor[5]=4`；


<center>
![cc][f9]

**圖三(e)。**
</center>

對第九、十條edge(5,0)、edge(5,1)進行`Relax()`，見圖三(f)：

* 比較`distance`及edge之**weight**後發現，從vertex(5)走到vertex(0)與vertex(1)之成本皆高於當前走到此二vertex之路徑，因此不更新。

<center>
![cc][f10]

**圖三(f)。**
</center>

經過了第一次迴圈的洗禮，大致能夠掌握`Relax()`的精髓：如果有比目前的成本更低的路徑，就選擇那條路徑。  
例如，圖三(c)中，原本從vertex(0)走到vertex(5)的路徑是$Path:0-1-2-5$，但是在圖三(e)時，發現了更短的路徑$Path:0-1-4-5$，便更新其成為目前「從vertex(0)走到vertex(5)」的最短路徑。

還有沒有可能更新出成本更低的路徑？  

當然有，因為第一次迴圈的`Relax()`明顯沒有更新到和edge(3,2)有關路徑，其原因在於，`Relax()`**進行的edge順序**。  
在第一次迴圈中，是先對edge(3,2)進行`Relax()`，不過此時的`distance[3]`還是無限大，因此沒有作用，之後才更新出「從vertex(4)走到vertex(3)」的路徑。  
就圖三(f)看來，`distance[2]`為$11$，而$Path:0-1-4-3-2$之成本只有$6$，顯然是低於當前的路徑$Path:0-1-2$。

因此需要$|V|-1$次迴圈，以確保演算法一定能找到最短路徑。

</br>
第二次直到第五次迴圈的邏輯同上，在此就不再贅述。

最後得到的最短路徑之**Predecessor Subgraph**如圖三(g)：

<center>
![cc][f11]

**圖三(g)。**
</center>

根據圖三(g)，從vertex(0)走到Graph中其餘vertex之最短路徑為$Path:0-1-4-3-2-5$，若把**Bellman-Ford Algorithm**中，每一次迴圈對所有edge進行`Relax()`之順序攤開，如圖三(h)，可以驗證**Bellman-Ford Algorithm**確實滿足**Path-Relaxation Property**，按照最短路徑之順序對edge進行`Relax()`。  
(**Bellman-Ford Algorithm**就是暴力解決，當然會滿足。)


<center>
![cc][f12]

**圖三(h)。**
</center>



</br>

<a name="code"></a>

##程式碼

程式碼包含幾個部分：

`class Graph_SP`：

* 使用`AdjList`，並利用`std::pair<int,int>`儲存edge的weight。
* `InitializeSingleSource(int Start)`：對資料項目`distance`與`predecessor`進行初始化，並以`int Start`作為最短路徑之起點。
* `Relax()`：對edge進行Relaxation的主要函式。
* `BellmanFord()`：進行**Bellman-Ford Algorithm**的主要函式，內容如前一小節所述。

以及`main()`：建立如圖二(a)之`AdjList`，並進行`BellmenFord()`。


```cpp
// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <utility>          // for std::pair<>
#include <iomanip>          // for std::setw()

const int Max_Distance = 100;
class Graph_SP{             // SP serves as Shortest Path
private:
    int num_vertex;
    std::vector<std::list<std::pair<int,int>>> AdjList;
    std::vector<int> predecessor, distance;
public:
    Graph_SP():num_vertex(0){};
    Graph_SP(int n):num_vertex(n){
        AdjList.resize(num_vertex);
    }
    void AddEdge(int from, int to, int weight);
    void PrintDataArray(std::vector<int> array);
    
    void InitializeSingleSource(int Start);     // 以Start作為起點
    void Relax(int X, int Y, int weight);       // 對edge(X,Y)進行Relax
    bool BellmanFord(int Start = 0);            // 以Start作為起點
                                                // if there is negative cycle, return false
};

bool Graph_SP::BellmanFord(int Start){
    
    InitializeSingleSource(Start);
    
    for (int i = 0; i < num_vertex-1; i++) {                // |V-1|次的iteration
        // for each edge belonging to E(G)
        for (int j = 0 ; j < num_vertex; j++) {             // 把AdjList最外層的vector走一遍
            for (std::list<std::pair<int,int> >::iterator itr = AdjList[j].begin();
                 itr != AdjList[j].end(); itr++) {          // 各個vector中, 所有edge走一遍
                Relax(j, (*itr).first, (*itr).second);
            }
        }
    }
    
    // check if there is negative cycle
    for (int i = 0; i < num_vertex; i++) {
        for (std::list<std::pair<int,int> >::iterator itr = AdjList[i].begin();
             itr != AdjList[i].end(); itr++) {
            if (distance[(*itr).first] > distance[i]+(*itr).second) {   // i是from, *itr是to
                return false;
            }
        }
    }
    
    // print predecessor[] & distance[]
    std::cout << "predecessor[]:\n";
    PrintDataArray(predecessor);
    std::cout << "distance[]:\n";
    PrintDataArray(distance);
    
    return true;
}
void Graph_SP::PrintDataArray(std::vector<int> array){
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << array[i];
    std::cout << std::endl << std::endl;
}
void Graph_SP::InitializeSingleSource(int Start){
    
    distance.resize(num_vertex);
    predecessor.resize(num_vertex);
    
    for (int i = 0; i < num_vertex; i++) {
        distance[i] = Max_Distance;
        predecessor[i] = -1;
    }
    distance[Start] = 0;
}
void Graph_SP::Relax(int from, int to, int weight){
    
    if (distance[to] > distance[from] + weight) {
        distance[to] = distance[from] + weight;
        predecessor[to] = from;
    }
}
void Graph_SP::AddEdge(int from, int to, int weight){
    AdjList[from].push_back(std::make_pair(to,weight));
}

int main(){
    
    Graph_SP g7(6);
    g7.AddEdge(0, 1, 5);
    g7.AddEdge(1, 4, -4);g7.AddEdge(1, 2, 6);
    g7.AddEdge(2, 4, -3);g7.AddEdge(2, 5, -2);
    g7.AddEdge(3, 2, 4);
    g7.AddEdge(4, 3, 1);g7.AddEdge(4, 5, 6);
    g7.AddEdge(5, 0, 3);g7.AddEdge(5, 1, 7);
    
    if (g7.BellmanFord(0))
        std::cout << "There is no negative cycle.\n";
    else
        std::cout << "There is negative cycle.\n";
        
    return 0;
}
```
output:

```cpp
predecessor[]:
   0   1   2   3   4   5
  -1   0   3   4   1   2

distance[]:
   0   1   2   3   4   5
   0   5   6   2   1   4

There is no negative cycle.
```


</br> 
<a name="nc"></a>

###檢查Graph中是否存在negative cycle

聰明的讀者一定已經發現了，`BellmanFord()`還有個功能，可以檢查Graph中是否存在**negative cycle**。  
上一篇文章[Single-Source Shortest Path：Intro(簡介)](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathintrojian-jie.html#limit)曾經提到，塵世間最痛苦的事情莫過於在處理Single-Source Shortest Path問題時碰上**negative cycle**，因為**negative cycle**存在會使得最短路徑出現$-\infty$。


若存在從vertex(from)指向vertex(to)的edge(from,to)，並具有weight，根據`Relax()`的運作原則，：

* 若`distance[to]`$>$`distance[from]+weight`，便更新`distance[to]=distance[from]+weight`」。

因此，在對Graph中所有edge進行`Relax()`後，所有edge(from,to)所連結的兩個vertex之`distance`必定滿足：

* `distance[to]`$\leq$`distance[from]+weight`

而`BellmanFord()`的檢查方法，便是在$|V|-1$次「對所有edge進行`Relax()`」後，檢查，若存在任何一條edge(from,to)所連結的兩個vertex之`distance`關係為：

* `distance[to]`$>$`distance[from]+weight`

就表示存在某一條edge之weight永遠可以讓該edge連結之vertex的`distance`下降，因此Graph中必定存在negative cycle。


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_BellmanFord/f12.png?raw=true




</br>  

以上便是**Bellman-Ford Algorithm**之介紹。  
只要了解：

* **Relaxation**
* **Convergence property**
* **Path-relaxation property**

之概念，即可掌握**Bellman-Ford Algorithm**的運作邏輯。

下一篇文章將介紹的是，**在DAG(directed acyclic graph)中**處理Single-Source Shortest Path之演算法。

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch24](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Wikipedia：Shortest path problem](https://en.wikipedia.org/wiki/Shortest_path_problem)
* [Cplusplus：std::pair](http://www.cplusplus.com/reference/utility/pair/?kw=pair)


<a name="series"></a>

</br>

###Shortest Path系列文章

[Single-Source Shortest Path：Intro(簡介)](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathintrojian-jie.html)  
[Single-Source Shortest Path：Bellman-Ford Algorithm](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathbellman-ford-algorithm.html)  
[Single-Source Shortest Path：on DAG(directed acyclic graph)](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathon-dagdirected-acyclic-graph.html)  

回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


