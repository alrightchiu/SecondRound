Title: Grpah: Breadth First Search(BFS，廣度優先搜尋)    
Date: 2016-2-6 14:15  
Category: 演算法與資料結構  
Tags: C++, Graph  
Summary: 介紹Graph(圖)中的基本操作：Breadth First Search(BFS，廣度優先搜尋) 。
 


</br>
####**先備知識與注意事項**


在[Binary Tree: Traversal(尋訪)](http://alrightchiu.github.io/SecondRound/binary-tree-traversalxun-fang.html#level)與[Binary Tree: 建立一棵Binary Tree](http://alrightchiu.github.io/SecondRound/binary-tree-jian-li-yi-ke-binary-tree.html)兩篇文章裡，介紹了如何利用`queue`在Binary Tree中進行**Level-Order Traversal**，其概念便是：各個node相對於`root`有其相對應的level，按照level由小到大依序對node進行Visiting。  
而level便代表了node與`root`之「距離」，以Graph的語言來說，「距離」便是path的**length(長度)/distance(距離)**。如圖一：

* Level=2：path(A-B)、path(A-C)之length為$1$。
* Level=3：path(A-B-D)、path(A-B-E)、path(A-C-F)之length為$2$。
* Level=4：path(A-B-E-G)、path(A-B-E-H)之length為$3$。

而Breadth First Search(BFS，廣度優先搜尋)便是廣義的Level-Order Traversal，將試用情境從Tree推廣至Graph。

<center>
![level-order][f1]

**圖一。**  
</center>


另外，在解釋演算法時，可能會用到Graph中的專有名詞，如**undirected**、**connected component**、**weight**等等，若覺得這些名詞像被打了馬賽克糊糊的，可以先回到[Grpah: Intro(簡介)](http://alrightchiu.github.io/SecondRound/grpah-introjian-jie.html)狠狠回憶一番。


***


##目錄

* [Breadth First Search(BFS，廣度優先搜尋)](#bfs)
* [演算法](#algorithm)
* [程式碼](#code)
* [討論](#discuss)
* [參考資料](#ref)



<a name="bfs"></a>

##**Breadth First Search(BFS，廣度優先搜尋)**

所以`BFS()`的功能有哪些呢？

考慮圖二(a)的Graph(沒有weight、是connected的undirected graph)：

<center>
![bfs][f2]

**圖二(a)。**  
</center>


若選定以vertex(A)作為起點，對圖二(a)的G進行`BFS()`，可以得到：

1. 從vertex(A)抵達在G裡所有「與vertex(A)在同一個connected component裡」的vertex的最短距離(shortest path)。  
(由於圖二(a)的G是connected undirected graph，所以從G中任何一點出發進行`BFS()`皆能抵達其餘所有vertex。)
2. 不僅僅能夠得到vertex(I)與vertex(A)的最短距離為$3$，還能夠指出一條可能的path，說明要如何從vertex(A)走到vertex(I)，例如path:A-C-F-I，或者path:A-C-G-I。  



<a name="algorithm"></a>

##**演算法**


在正式開始之前，需要先準備三項武器：

1. `color` array：利用color標記哪些vertex已經被「找到」，哪些還沒有。
    * 白色表示該vertex還沒有被找過；
    * 灰色表示該vertex已經被某個vertex找過；
    * 黑色表示該vertex已經從`queue`的隊伍中移除。
2. `distance` array：記錄每一個vertex與起點vertex之距離。
3. `predecessor` array：記錄某個vertex是被哪一個vertex找到的，如此便能回溯路徑。

`BFS()`的方法如下：

初始化(initialization)，如圖二(b)：

* 把所有vertex塗成白色；
* 把所有vertex的`distance`設為無限大，表示從起點vertex走不到，或者還沒有走到；
* 把所有vertex的`predecessor`清除(或者設成`NULL`、`-1`，可以辨識即可)。
* 建立空的`queue`。

<center>
![bfs][f3]

**圖二(b)。**  
</center>


把起點vertex(A)推進`queue`，如圖二(c)：

* 將vertex(A)塗成灰色；
* `distance[A]`設為$0$；
* `predecessor[A]`不更動(只要能辨識出vertex(A)沒有predecesor即可。)

<center>
![bfs][f4]

**圖二(c)。**  
</center>


接著，以`queue`的`front`當作新的起點搜尋。  
新的起點是vertex(A)，便檢查所有與vertex(A)相鄰的vertex(見圖二(a)的`Adjacency List`)，修改其`color`、`distance`、`predecessor`。  
如圖二(d)，vertex(B)、vertex(C)、vertex(D)與vertex(A)相鄰，如果vertex的顏色是白色，表示還沒有被其他vertex「找到」，便執行以下步驟：

* 將三個vertex的`color`塗成灰色；
* 將`distance[B]、distance[C]、distance[D]`設成`distance[A]`$+1=1$，
* 將`predecessor[B]、predecessor[C]、predecessor[D]`設成vertex(A)。
* 把三個vertex按照「找到」的順序，依序推進`queue`裡；
* 最後，把vertex(A)塗黑，移出`queue`。

經過以上步驟，vertex(B)、vertex(C)、vertex(D)便被vertex(A)「找到」，並把`predecessor`設成vertex(A)，所以回溯路徑時，任何經過vertex(B)的path，必定是由vertex(A)來。同理，vertex(C)與vertex(D)也是。

而`distance[B]`是vertex(A)的`distance[A]`加一，如此一來，只要到達vertex(A)是經由最短路徑，那麼從vertex(A)走到vertex(B)的路徑，也會是最短路徑。  
由於vertex(A)是起點，`distance[A]`$=0$，因此，`distance[B]`$=1$一定是從vertex(A)走到vertex(B)的最短距離。

由於推進`queue`的順序正好是vertex被「找到」的順序，因此，之後要取得`queue`的`front`作為新的起點做搜尋時，便能確保按照先前被「找到」的順序(如同Tree的Level-Order Traversal)。


<center>
![bfs][f5]

**圖二(d)：以vertex(A)作為搜尋起點。**  
</center>


接著，繼續以`queue`的`front`當作新的起點搜尋。  
新的起點是vertex(B)，檢查所有與其相鄰的vertex，共有vertex(A)與vertex(E)。由於vertex(A)已經被「找到」過(顏色為灰色或黑色)，因此，vertex(B)只能找到vertex(E)，便進行以下步驟：

* 將vertex(E)的`color`塗成灰色；
* 將`distance[E]`設成`distance[B]`$+1=2$；
* 將`predecessor[E]`設成vertex(B)；
* 將vertex(E)推進`queue`；
* 最後，把vertex(B)塗黑，移出`queue`。


<center>
![bfs][f6]

**圖二(e)：以vertex(B)作為搜尋起點。**  
</center>

由於vertex(B)是第一個`distance`為$1$且被視為搜尋起點的vertex，這就表示，所有`distance`為$0$的vertex都已經

1. 被當作搜尋起點；
2. 搜尋過其相鄰之vertex；
3. 被塗成黑色。

往後`queue`裡面只會出現`distance`$\geq 1$的vertex。
</br>

接下來，繼續以`queue`的`front`，得到vertex(C)作為新的起點，檢查其相鄰的vertex的顏色，如果是灰色或黑色(vertex(A)與vertex(E)已經被「找到」)則忽略，若是白色(vertex(F)、vertex(G)、vertex(H))，見圖二(f)：

* 將`color`修改成灰色；
* 將`distance`修改成`distance[C]`$+1$；
* 將`predecessor`修改成vertex(C)；
* 將vertex按照被「找到」的順序，推進`queue`；
* 將vertex(C)塗黑，移出`queue`。

<center>
![bfs][f7]

**圖二(f)：以vertex(C)作為搜尋起點。**  
</center>

接著，從`queue`的`front`找到vertex(D)，可惜所有與vertex(D)相鄰的vertex(A)與vertex(H)不是灰色就是黑色，都已經被「找到」，見圖二(g)：

* 將vertex(D)塗黑，移出`queue`。


<center>
![bfs][f8]

**圖二(g)：以vertex(D)作為搜尋起點。**  
</center>

接著，重複上述之步驟，直到`queue`被清空，即結束`BFS()`搜尋，見圖二(h)-(l)。

<center>
![bfs][f9]

**圖二(h)：以vertex(E)作為搜尋起點。**  

![bfs][f10]

**圖二(i)：以vertex(F)作為搜尋起點，將vertex(I)推進`queue`。** 

![bfs][f11]

**圖二(j)：以vertex(G)作為搜尋起點。** 

![bfs][f12]

**圖二(k)：以vertex(H)作為搜尋起點。** 

![bfs][f13]

**圖二(l)：以vertex(I)作為搜尋起點。**

</center>


當`queue`中的vertex都被移除(`pop()`)，表示`Adjacency List`中的所有vertex都被當作起點搜尋過其相鄰的vertex，此時`BFS()`便完成，得到以vertex(A)為起點，所有其餘vertex之相對應`distance`與`predecessor`，如圖二(m)。


<center>
![bfs][f14]

**圖二(m)：。**  
</center>



<a name="code"></a>

##**程式碼**

(為了簡化程式，以下程式將使用`int`處理資料，把$9$個vertex`char A~I`依序對應到`int 0~8`)


範例程式碼的主要功能為：

1. 建立如圖二(a)的`Adjacency List`；
2. 進行`BFS()`。

共包含幾個部分：

* `class Graph`：定義方式有非常多種，這裡提供的`class Graph`的主要目標有二：
    * private data member：
        * `num_vertex`：需要在定義`Graph`的object(物件)時，給定vertex的數目，以便建立`Adjacency List`(或者`Adjacency Matrix`)。
        * `std::vector< std::list<int> > AdjList`：利用C++標準函式庫(STL)提供的container(容器):`std::vector`與`std::list`來實現。這樣的寫法的優點是：不需要使用到`new operator`，便能夠將記憶體控管交給STL，不需要自行處理`delete operator`，以避免記憶體遺漏(memory leak)，詳細討論請參考[Code Review：Depth First Search and Breadth First Search in C++](http://codereview.stackexchange.com/questions/82476/depth-first-search-and-breadth-first-search-in-c)。
        * `color`、`distance`、`predecessor`：將在`BFS()`中使用，功能如上述。
    * public member function：
        * `Constructor:Graph(int num_vertex)`：在定義Graph的object(物件)時，需要知道vertex的數目，並在`constructor`中定義好`AdjList`。
        * `AddEdgeList(int from, in to)`：功能便是在`AdjList`新增從`from`到`to`的edge。
        * `BFS(int Start)`：需要知道起點vertex。

* `main()`：用`AddEdgeList()`建立起如圖二(a)的Graph之`AdjList()`，並在Graph上執行`BFS()`。


```cpp
// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <queue>

class Graph{
private:
    int num_vertex;
    std::vector< std::list<int> > AdjList;
    int *color,             // 0:白色, 1:灰色, 2:黑色
        *distance,          // 0:起點, 無限大:從起點走不到的vertex
        *predecessor;       // -1:沒有predecessor, 表示為起點vertex
public:
    Graph():num_vertex(0){};           // default constructor
    Graph(int N):num_vertex(N){        // constructor with input: number of vertex
        // initialize Adjacency List
        AdjList.resize(num_vertex);
    };
    void AddEdgeList(int from, int to);
    void BFS(int Start);
};

void Graph::AddEdgeList(int from, int to){
    AdjList[from].push_back(to);
}

void Graph::BFS(int Start){
    
    color = new int[num_vertex];
    predecessor = new int[num_vertex];
    distance = new int[num_vertex];
    
    for (int i = 0; i < num_vertex; i++) {  // 初始化，如圖二(b)
        color[i] = 0;                       // 0:白色;
        predecessor[i] = -1;                // -1表示沒有predecessor
        distance[i] = num_vertex+1;         // num_vertex個vertex, 
    }                                       // 最長距離 distance = num_vertex -1條edge
    
    // 先處理起點vertex，如圖二(c)
    color[Start] = 1;   // 1:gray
    distance[Start] = 0;
    predecessor[Start] = -1;
    
    std::queue<int> q;
    int i = Start;
    q.push(i);
    
    while (!q.empty()) {
        int u = q.front();                  // u 為新的搜尋起點
        for (std::list<int>::iterator itr = AdjList[u].begin(); itr != AdjList[u].end(); itr++) {
            if (color[*itr] == 0) {                // 若被「找到」的vertex是白色
                color[*itr] = 1;                   // 塗成灰色, 表示已經被「找到」
                distance[*itr] = distance[u] + 1;  // 距離是predecessor之距離加一
                predecessor[*itr] = u;             // 更新被「找到」的vertex的predecessor
                q.push(*itr);                      // 把vertex推進queue            
            }
        }
        q.pop();        // 把u移出queue
        color[u] = 2;   // 並且把u塗成黑色
    }
}

int main(){
    Graph g1(9);    
    // 建立出圖二(a)的Adjacency List
    g1.AddEdgeList(0, 1);g1.AddEdgeList(0, 2);g1.AddEdgeList(0, 3);
    g1.AddEdgeList(1, 0);g1.AddEdgeList(1, 4);
    g1.AddEdgeList(2, 0);g1.AddEdgeList(2, 4);g1.AddEdgeList(2, 5);g1.AddEdgeList(2, 6);g1.AddEdgeList(2, 7);
    g1.AddEdgeList(3, 0);g1.AddEdgeList(3, 7);
    g1.AddEdgeList(4, 1);g1.AddEdgeList(4, 2);g1.AddEdgeList(4, 5);
    g1.AddEdgeList(5, 2);g1.AddEdgeList(5, 4);g1.AddEdgeList(5, 8);
    g1.AddEdgeList(6, 2);g1.AddEdgeList(6, 7);g1.AddEdgeList(6, 8);
    g1.AddEdgeList(7, 2);g1.AddEdgeList(7, 3);g1.AddEdgeList(7, 6);
    g1.AddEdgeList(8, 5);g1.AddEdgeList(8, 6);
    
    g1.BFS(0);    
     
    return 0;
}
```

在`g1.BFS(0)`後，若觀察`distance`與`predecessor`，應該能看到如圖二(n)的結果：

<center>
![bfs][f15]

**圖二(n)。**  
</center>


<a name="discuss"></a>

##**討論**

* 由於`BFS()`是用`AdjList`來判斷edge的連結狀況，因此，`BFS()`對undirected graph或directed graph皆適用。

* 若將`predecessor array`中，所有vertex的「前後關係」以edge連結，可以得到**Predecessor Subgraph**，因為其connected與acyclic的性質，使得**Predecessor Subgraph**會是一棵以起點vertex為`root`的Tree，又稱為**Breadth-First Tree**，而所有**Predecessor Subgraph**出現的edge稱為**tree edge**，見圖三(a)：

<center>
![bfs][f16]

**圖三(a)。**  
</center>

* 如圖三(a)的**Breadth-First Tree**可能不止有一種可能，原因在於「建立`Adjacency List`時的順序」，將會影響到`BFS()`中的`for loop`在「找到」vertex時的順序。
    * 如圖三(b)，若更改圖二(a)之`AdjList`，把vertex(A)之Linked list中，vertex(B)與vertex(C)之順序對調，使得`BFS()`在搜尋vertex時，vertex(C)會比vertex(B)更早進入`queue`，也就更早成為搜尋的新起點，那麼最後得到的**Breadth-First Tree**就會不同。
    * 不過，雖然**Breadth-First Tree**不一樣，但是每個vertex相對於起點vertex的`distance`保證相同。

<center>
![bfs][f17]

**圖三(b)。**  
</center>


* 本篇文章所提供的`BFS()`演算法，其`color`之灰色與黑色可以合併，換句話說，若只使用白色與黑色，同樣能完成`BFS()`。不過在下一篇文章將介紹的`DFS()`演算法中，白色、灰色與黑色將分別具有不同功能。

* 上述之`BFS()`的時間複雜度為$O(|V|+|E|)$，在所有與Graph有關的演算法中算是非常有效率。


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f13.png?raw=true
[f14]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f14.png?raw=true
[f15]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f15.png?raw=true
[f16]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f16.png?raw=true
[f17]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/BFS_fig/f17.png?raw=true

</br>

以上便是Breadth First Search(BFS，廣度優先搜尋)之介紹。  
下一篇將介紹另一種在Graph同樣常見的搜尋方法：Depth First Search(DFS，深度優先搜尋)。


</br>


<a name="ref"></a>

####**參考資料**：

* [Introduction to Algorithms, Ch22](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Code Review：Depth First Search and Breadth First Search in C++](http://codereview.stackexchange.com/questions/82476/depth-first-search-and-breadth-first-search-in-c)
* [Theory of Programming：Adjacency List using C++ STL](http://theoryofprogramming.com/adjacency-list-using-cpp-stl/)
* [GeeksforGeeks：Detect cycle in an undirected graph](http://www.geeksforgeeks.org/detect-cycle-undirected-graph/)



</br>






