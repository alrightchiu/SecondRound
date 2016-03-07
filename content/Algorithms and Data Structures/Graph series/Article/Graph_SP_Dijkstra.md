Title: Single-Source Shortest Path：Dijkstra's Algorithm    
Date: 2016-3-7 22:30   
Category: 演算法與資料結構  
Tags: C++, Graph, Shortest Path, Relaxation, Priority Queue, Greedy Algorithm   
Summary: 介紹Dijkstra's Algorithm。



</br>
###先備知識與注意事項

本篇文章將介紹另一種解決Single-Source Shortest Path的方法：**Dijkstra's Algorithm**。  
演算法的概念依然需要：

* **Relaxation** 
* **Convergence property**
* **Path-relaxation property**

若有疑惑，[傳送門在這裏](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathintrojian-jie.html#relax)。

在演算法中，將會使用到**Min-Priority Queue**，包含三個基本操作(operation)：

* `ExtractMin()`
* `DecreaseKey()`
* `BuildMinHeap()`，亦可用`MinHeapInsert()`取代；

若讀者需要稍作複習，請參考[Priority Queue：Intro(簡介) & Binary Heap](http://alrightchiu.github.io/SecondRound/priority-queueintrojian-jie-binary-heap.html)。


***

##目錄

* [Dijkstra's Algorithm](#dijkstra)
* [程式碼](#code)
* [參考資料](#ref)
* [Shortest Path系列文章](#series)


</br>

<a name="dijkstra"></a>

##Dijkstra's Algorithm


若某一directed graph中，所有edge的weight皆為**非負實數**($weight\geq 0$)，如圖一(a)，便能夠使用**Dijkstra's Algorithm**處理這個directed graph上的Single-Source Shortest Path問題。


<center>
![cc][f1]

**圖一(a)。**
</center>


**Dijkstra's Algorithm**是一種「每次挑選當前最佳選擇(optimal solution)」的**Greedy Algorithm**：

1. 把vertex分成兩個集合，其中一個集合「**S**」中的vertex屬於「已經找到從起點vertex出發至該vertex的最短路徑」，另一個集合「**Q**」中的vertex則還沒有。
    * 非常重要：這裡的**Q**就是**Min-Priority Queue**，其中每一個**HeapNode**的`element`即為「vertex的index」，`key`即為「vertex的`distance`」，表示從起點走到該vertex的path成本。 
2. 對所有vertex的`predecessor`與`distance`進行初始化(見圖一(b))，並更新起點vertex之`distance`為$0$；
3. 若Graph中有$V$個vertex，便執行$V$次迴圈。  
4. 在每一個迴圈的開始，從**Q**中挑選出「`distance`值最小」的vertex，表示已經找到「從起點vertex抵達該vertex之最短距離」，並將該vertex從**Q**中移除，放進**S**集合。
    * 利用`ExtractMin()`挑選「`distance`值最小」的vertex，即為**Greedy Algorithm**概念。 
5. 每一次迴圈都會藉由`Relax()`更新「從起點vertex到達**仍屬於Q集合**的vertex之path距離」，並將path距離存放於`distance[]`。並且利用`DecreaseKey()`更新**Q**中vertex的**Key**(也就是`distance`)。  
步驟四與步驟五為完整的一次迴圈。
6. 進到下一個迴圈時，會繼續從**Q**中挑選出「`distance`最小」的vertex，放進**S**。
7. 重複上述步驟，直到**Q**中的vertex都被放進**S**為止。  

如此便能得到從起點vertex抵達其餘vertex的最短路徑。

演算法將使用兩項資料項目`predecessor[]`與`distance[]`，其功能如下：

* `predecessor[]`：
* `distance[]`：

</br>
以下以圖一(a)之Graph為例，以vertex(0)為起點，進行**Dijkstra's Algorithm**。

首先，對`predecessor[]`和`distance[]`進行初始化：

* 所有vertex的`predecessor[]`先設為$-1$。
    * 若到演算法結束時，vertex(X)之`predecessor[X]`仍等於$-1$，表示從起點vertex走不到vertex(X)。
* 所有vertex的`distance[]`先設為「無限大($\infty$)」。
    * 若在演算法結束時，`distance[]`仍為「無限大($\infty$)」，表示，從起點vertex走不到vertex(X)。
* 把起點vertex(0)之`distance[0]`設為$0$，如此一來，當挑選Min-Priority Queue中，`distance[]`值最小的vertex，進行路徑探索時，就會從vertex(0)開始。


<center>
![cc][f2]

**圖一(b)。**
</center>

接著便進入演算法的主要迴圈，包含兩個步驟：


1. `ExtractMin()`：從Min-Priority Queue，也就是**Q**中，選出`distance`最小的vertex，並將其從**Q**中移除。
2. `Relax()`以及`DecreaseKey()`：對選出的vertex所連結的edge進行`Relax()`，更新`distance`與`predecessor`，並同步更新**Q**。


**第一次迴圈：**

從**Q**中以`ExtractMin()`，取得目前`distance`值最小的vertex：起點vertex(0)。  
建立一個變數`int u`記住vertex(0)，並將vertex(0)從**Q**移除，表示已經找到從起點vertex走到vertex(0)的最短路徑，見圖二(a)。


<center>
![cc][f3]

**圖二(a)。**
</center>

接著，以vertex(0)為探索起點，尋找所有「從vertex(0)指出去」的edge，進行：

* `Relax()`：以圖二(b)為例，從vertex(0)能夠走到vertex(1)、vertex(5)，便利用`Relax()`更新從起點vertex，經過vertex(0)後抵達vertex(1)、vertex(5)的path之weight總和，存放進`distance[]`，同時更新`predecessor[]`。
    * vertex(1)能夠從vertex(0)以成本「$8$」走到，比原先的「無限大($\infty$)」成本要低，因此，更新：
        * `distance[1]=distance[0]+weight(0,1)`；
        * `predecessor[1]=0`。
    * 對vertex(5)比照辦理。
* `DecreaseKey()`：在更新`distance[]`之後，也要同步更新**Q**中HeapNode的資料，也就是有被`Relax()`的vertex之`distance`。


<center>
![cc][f4]

**圖二(b)。**
</center>

</br>
**第二次迴圈：**

繼續從**Q**中選出`distance`最小的vertex(5)，並將vertex(5)從**Q**中移除，表示已經找到「從起點vertex(0)走到vertex(5)」的最短距離，如圖三(a)。

<center>
![cc][f5]

**圖三(a)。**
</center>

關鍵是，為什麼從**Q**中挑選`distance`最小的vertex(X)，就能肯定已經找到「從起點vertex走到vertex(X)之最短路徑」，`distance[X]`$=\delta(0,X)$？

這裡不進行嚴謹證明(請參考：[Ashley Montanaro：Priority queues and Dijkstra’s algorithm](https://www.cs.bris.ac.uk/~montanar/teaching/dsa/dijkstra-handout.pdf))，只就基本條件推論。

前面提到，**Dijkstra's Algorithm**的使用時機是，當Graph中的weight皆為非負實數($weight\geq 0$)，因此：

* Graph中的所有**cycle**之wieght總和必定是正值(positive value)；
* 亦即，路徑中的edge越多，其weight總和只會增加或持平，不可能減少。

那麼再看圖三(a)，從vertex(0)走到vertex(5)有兩個選擇：

1. 一個是經過edge(0,5)，成本為$1$；
2. 另一個是經過edge(0,1)，再經過其他vertex(例如，$Path:0-1-2-3-5$)，最後抵達vertex(5)，由於edge(0,1)之weight已經是$8$，可以確定的是，經過edge(0,1)的這條path之weight總和必定大於$8$。

顯然，直接經過edge(0,5)會是從vertex(0)走到vertex(5)最短的路徑。  

更廣義地說，從**Q**中挑選`distance`最小之vertex，並將其從**Q**中移除，必定表示，已經找到從起點vertex走到該vertex之最短路徑。

</br>
接著，重複上述步驟：

* `Relax()`：對所有「從vertex(5)指出去」的edge進行**Relaxation**。
* `DecreaseKey()`：同步更新**Q**中的vertex之`distance[]`。


<center>
![cc][f6]

**圖三(b)。**
</center>

接下來，第三次迴圈至第六次迴圈(直到**Q**中的vertex都被移除)的步驟如上述，見圖四(a)-圖七(a)。

**第三次迴圈：**

<center>
![cc][f7]

**圖四(a)。**
</center>

<center>
![cc][f8]

**圖四(b)。**
</center>

**第四次迴圈：**

<center>
![cc][f9]

**圖五(a)。**
</center>

<center>
![cc][f10]

**圖五(b)。**
</center>


**第五次迴圈：**

<center>
![cc][f11]

**圖六(a)。**
</center>

<center>
![cc][f12]

**圖六(b)。**
</center>

**第六次迴圈：**

<center>
![cc][f13]

**圖七(a)。**
</center>


圖七(a)便是從vertex(0)走到Graph中其餘vertex之最短路徑**Predecessor Subgraph**。


</br>

<a name="code"></a>

##程式碼

程式碼包含幾個部分：

`class BinaryHeap`與`struct HeapNode`：以Binary Heap實現Min-Priority Queue，概念與範例程式碼請參考[Priority Queue：Intro(簡介) & Binary Heap](http://alrightchiu.github.io/SecondRound/priority-queueintrojian-jie-binary-heap.html)。

`class Graph_SP`：

* 基本的資料項目：`AdjList`、`num_vertex`、`predecessor`、`distance`。
* `Relax()`、`InitializeSingleSource()`之功能與**Bellman-Ford Algorithm**所使用相同。
* `Dijkstra()`：功能如前一小節所述。
* 把`class BinaryHeap`設為`class Graph_SP`的「**friend**」，才能夠把`distance[]`放進Min Heap中。

以及`main()`：建立如圖一(a)之`AdjList`，並進行`Dijkstra()`。


```cpp
// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <utility>          // for std::pair<>
#include <iomanip>          // for std::setw()
#include <cmath>            // for std::floor

struct HeapNode{
    int element, key;               // element代表vertex, key代表distance
    HeapNode():element(0),key(0){};
    HeapNode(int node, int key):element(node), key(key){};
};
class BinaryHeap{
private:
    std::vector<HeapNode> heap;            // 存放HeapNode之vertex及distance的矩陣
    void swap(struct HeapNode &p1, struct HeapNode &p2);
    int FindPosition(int node);
    int GetParentNode(int node){return std::floor(node/2);};
public:
    BinaryHeap(){heap.resize(1);};
    BinaryHeap(int n){heap.resize(n + 1);}          
    bool IsHeapEmpty(){return (heap.size()<1);};
    
    // Min-Priority Queue
    void MinHeapify(int node, int length);
    void BuildMinHeap(std::vector<int> array);                               
    void DecreaseKey(int node, int newKey);
    void MinHeapInsert(int node, int key);
    int Minimum();         
    int ExtractMin();
};

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
    void PrintIntArray(int *array);

    void InitializeSingleSource(int Start);     // 以Start作為起點
    void Relax(int X, int Y, int weight);       // edge方向：from X to Y
    
    void Dijkstra(int Start = 0);        // 需要Min-Priority Queue
    friend class BinaryHeap;             // 以Binary Heap實現Min-Priority Queue
};
void Graph_SP::Dijkstra(int Start){
    
    InitializeSingleSource(Start);
    
    BinaryHeap minQueue(num_vertex+1);   // object of min queue
    minQueue.BuildMinHeap(distance);
    
    while (!minQueue.IsHeapEmpty()) {
        int u = minQueue.ExtractMin();
        for (std::list<std::pair<int, int>>::iterator itr = AdjList[u].begin();
             itr != AdjList[u].end(); itr++) {
            Relax(u, (*itr).first, (*itr).second);
            minQueue.DecreaseKey((*itr).first, distance[(*itr).first]);
        }
    }
    std::cout << "\nprint predecessor:\n";
    PrintDataArray(predecessor);
    std::cout << "\nprint distance:\n";
    PrintDataArray(distance);
}

int main(){
    
    Graph_SP g9(6);
    g9.AddEdge(0, 1, 8);g9.AddEdge(0, 5, 1);
    g9.AddEdge(1, 0, 3);g9.AddEdge(1, 2, 1);
    g9.AddEdge(2, 0, 5);g9.AddEdge(2, 3, 2);g9.AddEdge(2, 4, 2);
    g9.AddEdge(3, 1, 4);g9.AddEdge(3, 2, 6);g9.AddEdge(3, 4, 7);g9.AddEdge(3, 5, 3);
    g9.AddEdge(5, 3, 2);g9.AddEdge(5, 4, 8);
    
    g9.Dijkstra(0);
        
    return 0;
}
```
output:

```cpp
new key is larger than current key
new key is larger than current key

print predecessor:
   0   1   2   3   4   5
  -1   3   1   5   5   0

print distance:
   0   1   2   3   4   5
   0   7   8   3   9   1
```





最後，關於**Dijkstra's Algorithm**之時間複雜度，會因為Min-Priority Queue所使用的資料結構(例如，Binary Heap或是Fibonacci Heap)而有所差異，可能最差從$O(V^2+E)$到最好$O(V\logV+E)$，請參考以下連結之討論：

* [Ashley Montanaro：Priority queues and Dijkstra’s algorithm](https://www.cs.bris.ac.uk/~montanar/teaching/dsa/dijkstra-handout.pdf)
* [Rashid Bin Muhammad：Dijkstra's Algorithm](http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/GraphAlgor/dijkstraAlgor.htm)
* [Wikipedia：Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra's_algorithm)





[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_Dijkstra/f13.png?raw=true




</br>  

以上便是**Dijkstra's Algorithm**之介紹。  

讀者不妨嘗試比較**Dijkstra's Algorithm**與**Bellman-Ford Algorithm**之差異，一般情況下，**Dijkstra's Algorithm**應該會較有效率，不過限制是Graph之weight必須是非負實數，若遇到weight為負數時，仍需使用**Bellman-Ford Algorithm**。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch24](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Wikipedia：Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra's_algorithm)
* [Priority Queue：Intro(簡介) & Binary Heap](http://alrightchiu.github.io/SecondRound/priority-queueintrojian-jie-binary-heap.html)
* [Ashley Montanaro：Priority queues and Dijkstra’s algorithm](https://www.cs.bris.ac.uk/~montanar/teaching/dsa/dijkstra-handout.pdf)
* [Rashid Bin Muhammad：Dijkstra's Algorithm](http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/GraphAlgor/dijkstraAlgor.htm)


<a name="series"></a>

</br>

###Shortest Path系列文章

[Single-Source Shortest Path：Intro(簡介)](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathintrojian-jie.html)  
[Single-Source Shortest Path：Bellman-Ford Algorithm](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathbellman-ford-algorithm.html)  
[Single-Source Shortest Path：on DAG(directed acyclic graph)](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathon-dagdirected-acyclic-graph.html)  



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


