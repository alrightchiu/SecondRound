Title: Graph: 利用DFS和BFS尋找Connected Component    
Date: 2016-2-12 22:55  
Category: 演算法與資料結構  
Tags: C++, Graph, BFS, DFS  
Summary: 利用DFS和BFS尋找undiredted graph中的Connected Component。


</br>
####**先備知識與注意事項**

若一個undirected graph中，若有任兩個vertex之間沒有path連結，那麼此undirected graph就不是connected，裡面一定包含了兩個以上的connected component。  

如圖一(a)，vertex(0)與vertex(1)不論經過Graph中其他任何vertex都沒有辦法產生一條path連結，則此Graph就不是connected。  
並且觀察，vertex(0)、vertex(2)、vertex(4)彼此皆有path能夠互相連結，因此subgraph：$G(V_1,E_1)$，其中$V_1$:{$0,2,4$}與$E_1$:{$(0,2),(0,4)$}即為一個connected component；subgraph：$G(V_2,E_2)$，其中$V_2$:{$1,3$}與$E_2$:{$(1,3)$}是另一個connected component。


<center>
![cc][f1]

**圖一(a)。**  
</center>

再看圖一(b)，Graph中的任何vertex皆能經由一條path通往其餘vertex，因此，整個Graph所形成的集合即為一個connected component。


<center>
![cc][f2]

**圖一(b)。**  
</center>


本篇文章要示範強大的`DFS()`與`BFS()`的小小應用：尋找undirected graph中的**connected component**。  
若不太熟悉**connected**的定義，可以先閱讀[Graph: Intro(簡介)](alrightchiu.github.io/SecondRound/graph-introjian-jie.html)稍稍複習。  
此外，為了集中火力，`DFS()`與`BFS()`的程式碼就留在[Graph: Depth-First Search(DFS，深度優先搜尋)](http://alrightchiu.github.io/SecondRound/graph-depth-first-searchdfsshen-du-you-xian-sou-xun.html)與[Graph: Breadth-First Search(BFS，廣度優先搜尋)](http://alrightchiu.github.io/SecondRound/graph-breadth-first-searchbfsguang-du-you-xian-sou-xun.html)裡面，如果還沒有對兩個演算法送出交友邀請，千萬不要客氣。

最後一點溫馨小提醒：Graph與connected component的本質上是Set(集合)，因此，以下將會用到Set的觀點做說明。如果不熟悉Set也沒關係，只要知道Set是「一團不在意次序(order)的資料」就可以了。


***

##目錄

* [DFS與BFS何德何能？](#how)
* [演算法](#algorithm)
* [程式碼](#code)
* [如果是Strongly Connected Component呢](#scc)
* [參考資料](#ref)



<a name="how"></a>

##**DFS與BFS何德何能？**

關鍵就是，兩個演算法都能產生`predecessor`。

若vertex(Y)的`predecessor[Y]`是vertex(X)，表示：

* vertex(Y)是被vertex(X)給找到。
* 也就表示，在undirected graph中，存在edge(X,Y)。
* 又因為是undirected graph，若存在edge(X,Y)，就能從vertex(X)走到vertex(Y)，也能從vertex(Y)走到vertex(X)。

因此，若vertex(Y)的`predecessor[Y]`是vertex(X)，就表示vertex(X)與vertex(Y)一定在同一個connected component裡面。

這也就是為什麼`DFS()`與`BFS()`都能夠用來尋找undirected graph中的connected component的原因。


<a name="algorithm"></a>

##**演算法**

尋找connected component的演算法很直觀，只要三步驟：

1. 在Graph上執行`DFS()`或`BFS()`得到`predecessor`。
2. 進行`SetCollapsing()`。（蛤？）
3. 印出共有幾個connected component，以及各個connected component中的vertex。

以上步驟，顯然有個傢伙很突兀。

<center>
![sc][f3]

**圖二。**  
</center>

`SetCollapsing()`是稍後會用到的函式，其功能可以拆成兩個部分理解：一個是「Set」，一個是「Collapsing」：

* Set(集合)：`SetCollapsing()`處理的對象是Set，也就是不具「次序(order)」的資料，如圖二左，共有三個Set，分別是$S_1$:{$0,1,4,5,7$}、$S_2$:{$3,6,8$}、$S_3$:{$2$}。
    * 在Set上，時常要做的操作便是「查看某個元素(element)在哪一個Set裡面」，而Set通常是用`root`代表，因此，若如圖二左的方式，以element(0)作為「存取(access)點」(也就是這個Set的`root`)，那麼要判斷element(7)是在element(0)所代表的Set內(而不是element(3)所代表的Set)，就需要$O(N)$的搜尋時間($N$為Set內的元素數量)，從element(7)一路找到element(0)，才能判斷。
    * 如果能夠以圖二右的資料結構表示Set，那麼以element(0)、element(2)、element(3)代表Set，要判斷任何一個元素(element)是屬於哪一個Set，便只要$O(1)$的時間。
* Collapsing(塌陷)：讓Set「塌陷」，使得所有element皆能直接指向其所在的Set之`root`。

  
(以下先以`DFS()`為例，稍後在程式碼的部分將會同步展示以`BFS()`進行之結果)

考慮圖三(a)的undirected graph：

<center>
![dfs][f4]

**圖三(a)。**  
</center>

首先，對Graph進行`DFS()`，取得`predecessor`，以及圖三(b)的Depth-First Forest：  

* 由於是undirected graph，由任意vertex作為`DFS()`起點，結果都會相同。此處以vertex(0)作為起點。
* 圖三(b)中，vertex(0)、vertex(2)、vertex(3)的`predecessor`皆為$-1$，表示這三者皆為Depth-First Tree的`root`，也就是Set的`root`。
* 而其餘的vertex，皆能透過`predecessor`回溯到`root`。


<center>
![sc][f5]

**圖三(b)。**  
</center>


接下來要進行`SetCollapsing()`。  
在此先以$Set$:{$0,1,4,5,7$}為例作說明，目標是從圖三(c)左轉換成圖三(c)右。


<center>
![sc][f6]

**圖三(c)。**  
</center>

先觀察：`SetCollapsing()`什麼時候完成？  
就是當Set中，除了`root`之外的所有vertex(等義於element)的`predecessor`都指向`root`時，也就是當要被「塌陷」的vertex剩下`root`的時候，`SetCollapsing()`便完成。

若現在要對vertex(7)進行`SetCollapsing()`，見圖三(d)：

* 把vertex(7)標記上`current`。
* 找到vertex(7)所在的Set的`root`，也就是vertex(0)，標記上`root`。
* 找到vertex(7)的`predecessor`，也就是vertex(5)，標記上`parent`。

接著判斷，若`current`不等於`root`(表示除了`root`之外，還有vertex還沒有「塌陷」，因此`SetCollapsing()`還沒有完成)，那麼就把：

* `current`的`predecessor`更新成`root`：`predecessor[7]`更新成vertex(0)。
* 並且把原先的`parent`當成新的`current`：`current`移動到vertex(5)。

<center>
![sc][f7]

**圖三(d)。**  
</center>


現在`current`變成了vertex(5)，不等於`root`vertex(0)，因此繼續：

* 把`parent`標記成vertex(4)。
* `current`的`predecessor`更新成`root`：`predecessor[5]`更新成vertex(0)。
* 並且把原先的`parent`當成新的`current`：`current`移動到vertex(4)。


<center>
![sc][f8]

**圖三(e)。**  
</center>


接下來的`current`依序會是vertex(4)、vertex(1)，因為`current`還不是vertex(0)，便按照上述步驟「塌陷」。直到`current`移到vertex(0)後，`SetCollapsing()`便完成，見圖三(f)。


<center>
![sc][f9]

**圖三(f9)。**  
</center>


比較一下經過`SetCollapsing()`處立前後的`predecessor`，如圖四。


<center>
![sc][f10]

**圖四。**  
</center>

只要先找到`predecessor`為「負值」的vertex，就找到了代表某一connected component的Set之`root`，再尋找哪些vertex的`predecessor`指向該`root`，便能找出該connected component所包含的所有vertex。

大功告成。


<a name="code"></a>

##**程式碼**

範例程式碼除了已經介紹過的`class Graph`、`DFS()`以及`BFS()`之外，還多了四個函式。  
其中，在undirected graph中尋找connected component的主要函式：`CCDFS(int vertex)`與`CCBFS(int vertex)`共包含三個部分：

* `DFS()`或者`BFS()`，取得`predecessor`。
* `SetCollapsing`是為了讓尋找connected component變得更簡單。
* 最後，是利用「塌陷」過後的`predecessor`找到connected component。  
(`PrintPredecessor()`是為了印出`predecessor`以驗證`SetCollapsing`的正確性，非必要。)


```cpp
// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <iomanip>      // for std::setw()

class Graph{
private:
    int num_vertex;
    std::vector< std::list<int> > AdjList;
    int *color,             // 0:white, 1:gray, 2:black
        *predecessor,
        *distance,          // for BFS()
        *discover,          // for DFS()
        *finish;
public:
    Graph():num_vertex(0){};
    Graph(int N):num_vertex(N){
        // initialize Adj List
        AdjList.resize(num_vertex);
    };
    void AddEdgeList(int from, int to);
    void BFS(int Start);    
    void DFS(int Start);
    void DFSVisit(int vertex, int &time);
    
    void CCDFS(int vertex);                // 利用DFS 
    void CCBFS(int vertex = 0);            // 利用BFS, 兩者邏輯完全相同
    void SetCollapsing(int vertex);
    void PrintPredecessor();               // 印出predecessor, 供驗証用, 非必要
};

void Graph::SetCollapsing(int current){
    int root;  // root
    for (root = current; predecessor[root] >= 0; root = predecessor[root]);
    
    while (current != root) {
        int parent = predecessor[current];
        predecessor[current] = root;
        current = parent;
    }
}

void Graph::CCDFS(int vertex = 0){
    
    DFS(vertex);      // 
    PrintPredecessor();
    for (int i = 0; i< num_vertex; i++){
        SetCollapsing(i);
    }
    PrintPredecessor();
    
    int num_cc = 0;
    for (int i = 0; i < num_vertex; i++) {
        if (predecessor[i] < 0) {
            std::cout << "Component#" << ++num_cc << ": " << i << " ";
            for (int j = 0; j < num_vertex; j++) {
                if (predecessor[j] == i) {
                    std::cout << j << " ";
                }
            }
            std::cout << std::endl;
        }
    }
}

void Graph::CCBFS(int vertex){
    
    BFS(vertex);
    PrintPredecessor();
    for (int i = 0; i< num_vertex; i++){
        SetCollapsing(i);
    }
    PrintPredecessor();
    
    int num_cc = 0;
    for (int i = 0; i < num_vertex; i++) {
        if (predecessor[i] < 0) {
            std::cout << "Component#" << ++num_cc << ": " << i << " ";
            for (int j = 0; j < num_vertex; j++) {
                if (predecessor[j] == i) {
                    std::cout << j << " ";
                }
            }
            std::cout << std::endl;
        }
    }
}
void Graph::PrintPredecessor(){
    std::cout << "predecessor:" << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << predecessor[i];
    std::cout << std::endl;
}

int main(){
    Graph g3(9);
    g3.AddEdgeList(0, 1);
    g3.AddEdgeList(1, 0);g3.AddEdgeList(1, 4);g3.AddEdgeList(1, 5);
    // AdjList[2] empty
    g3.AddEdgeList(3, 6);
    g3.AddEdgeList(4, 1);g3.AddEdgeList(4, 5);
    g3.AddEdgeList(5, 1);g3.AddEdgeList(5, 4);g3.AddEdgeList(5, 7);
    g3.AddEdgeList(6, 3);g3.AddEdgeList(6, 8);
    g3.AddEdgeList(7, 5);
    g3.AddEdgeList(8, 6);
    
    g3.CCDFS();
    std::cout << std::endl;
    g3.CCBFS();
    std::cout << std::endl;
    
    return 0;
}
```
output:

```cpp
predecessor:
   0   1   2   3   4   5   6   7   8
  -1   0  -1  -1   1   4   3   5   6
predecessor:
   0   1   2   3   4   5   6   7   8
  -1   0  -1  -1   0   0   3   0   3
Component#1: 0 1 4 5 7 
Component#2: 2 
Component#3: 3 6 8 

predecessor:
   0   1   2   3   4   5   6   7   8
  -1   0  -1  -1   1   1   3   5   6
predecessor:
   0   1   2   3   4   5   6   7   8
  -1   0  -1  -1   0   0   3   0   3
Component#1: 0 1 4 5 7 
Component#2: 2 
Component#3: 3 6 8 
```


聰明的讀者們一定已經發現了，不進行`SetCollapsing()`，單單只靠`predecessor`一樣可以找出connected component。  

不過筆者認為使用`SetCollapsing()`處理`predecessor`，雖然不見得是最有效率的方法，但是概念上比較直觀，因為connected component就是Set。

事實上，這兩種Set表示法(「塌陷」前後的`predecessor`)，正好是在Set問題中永恆的兩難(trade-off)：「要**Union**方便」還是「**Find**方便」，有興趣的讀者可以看看這篇：[Disjoint Set Union (Union Find)](https://www.hackerearth.com/notes/disjoint-set-union-union-find/)。

<a name="scc"></a>

##**如果是Strongly Connected Component呢**

以上處理的是「在undirected graph中尋找connected component」，那如果要在「directed graph中尋找strongly connected component」呢？

顯然，光是只有`predecessor`是不夠的，因為在directed graph中，`predecessor`能夠保證的是一個「有方向性」的edge，例如edge(X,Y)表示，從vertex(X)能夠走到vertex(Y)，卻不能打包票從vertex(Y)也能走回vertex(X)。因此，要找到strongly connected component需要更高級的方法才行。

不過到這裡，是時候[該說再見了朋友們](https://www.youtube.com/watch?v=NrGxru6nqeQ)。  
更高級的方法，請待下回分曉。


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/CC_fig/f10.png?raw=true


</br>


<a name="ref"></a>

####**參考資料**：

* [Introduction to Algorithms, Ch22](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Hackerearth：Disjoint Set Union(Union Find)](https://www.hackerearth.com/notes/disjoint-set-union-union-find/)
* [Wikipedia：Set (abstract data type)](https://en.wikipedia.org/wiki/Set_%28abstract_data_type%29)



</br>






