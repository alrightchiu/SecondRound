Title: Grpah: 利用DFS尋找DAG的Topological Sort(拓撲排序)    
Date: 2016-2-18 18:01  
Category: 演算法與資料結構  
Tags: C++, Graph, DFS, DAG  
Summary: 利用DFS尋找directed acyclic graph(DAG)的Topological Sort(拓撲排序)。


</br>
###先備知識與注意事項

有些事件具有絕對的「先後關係」，例如，襪子要在鞋子之前穿上，否則穿上鞋子後要再穿襪子需要一點奇蹟。

若以Graph來表示，vertex(穿襪子)、vertex(穿鞋子)與edge(先後關係)如圖一(a)：

<center>
![cc][f1]

**圖一(a)。**  
</center>

在[Graph: Intro(簡介)](http://alrightchiu.github.io/SecondRound/graph-introjian-jie.html)曾經提過的「課程與其先修課程」亦具有這樣的「先後關係」。  
例如，學生一定要先修過「資料結構」，才能選修「演算法分析」，否則選課系統會生氣。

那麼要如何確保在選修「演算法分析」之前，已經先選修「資料結構」，而在選修「資料結構」之前，已經先修完「程式(一)」與「離散數學」？

本篇文章將要介紹的Topological Sort(拓撲排序)就是要解決這項煩惱。  
如果找到了圖一(b)的Topological Sort(拓撲排序)，就能知道可行的修課順序，以確保在選修「演算法分析」之前，一定已經修過「資料結構」與「線性代數」。


<center>
![cc][f2]

**圖一(b)。**  
</center>

***

##目錄

* [Topological Sort(拓撲排序)](#ts)
* [演算法](#algorithm)
* [程式碼](#code)
* [參考資料](#ref)
* [Graph系列文章](#series)



<a name="ts"></a>

##Topological Sort(拓撲排序)

所謂的Topological Sort(拓撲排序)要求，若**directed acyclic graph(DAG)**中存在一條edge(X,Y)，那麼序列中，vertex(X)一定要在vertex(Y)之前出現。

* 以圖二(a)為例，存在edge(2,6)、edge(6,9)，那麼Topological Sort中，vertex(2)一定要出現在vertex(6)之前，vertex(6)一定要在vertex(9)之前。


<center>
![cc][f3]

**圖二(a)：將圖一(b)以符號表示。**  
</center>

根據圖一(b)，選修「資料結構」與「線性代數」的先後順序顯然無所謂，「數值分析」與「離散數學」的修課順序也互相沒有影響，因為兩者之間沒有必然的先後關係。  
因此，正確的Topological Sort可能不止一種，以下兩種排序皆為圖二(a)的可能結果：

```cpp
Topological Sort:
  3  4  5 14  1  0  2  7  6 12 13 11  9 10  8
Topological Sort:
  3  1  0  2  7  4  5 14  6 12 13 11  9 10  8
```

最重要的一點：只有**directed acyclic graph(DAG)**的Topological Sort(拓撲排序)才有意義。

* 以圖二(b)為例，若根據Topological Sort的定義：「若存在一條edge(X,Y)，則序列中，vertex(X)一定要在vertex(Y)之前出現」，那麼，存在edge(fish,rice)，序列可能是「魚、飯、肉、菜」，但是卻也同時存在edge(rice,pork)，序列可能是「飯、肉、菜、魚」，而第二個序列卻違反「存在edge(fish,rice)，魚要在飯之前吃」的限制。  
因此，若directed graph中存在cycle，討論Topological Sort其實沒有什麼幫助。


<center>
![cc][f4]

**圖二(b)。**  
</center>


<a name="algorithm"></a>

##演算法

在[Grpah: 利用DFS尋找Strongly Connected Component(SCC)](http://alrightchiu.github.io/SecondRound/grpah-li-yong-dfsxun-zhao-strongly-connected-componentscc.html)曾經提過DAG的性質：

* 在DAG上執行一次`DFS()`，若存在一條path從vertex(X)到vertex(Y)，那麼`finish[X]`一定比`finish[Y]`還大。([證明請點這裡](http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/GraphAlgor/topoSort.htm))

因此，只要進行一次`DFS()`，並且依照`finish[]`由大到小印出vertex，就是Topological Sort(拓撲排序)了。

<center>
![cc][f5]

**圖三。**  
</center>

<a name="code"></a>

##程式碼

是的，只要把[上一篇文章](http://alrightchiu.github.io/SecondRound/grpah-li-yong-dfsxun-zhao-strongly-connected-componentscc.html)介紹過的`PrintSCCs()`的前半部照抄，就能夠找到`finish`由大到小的順序。


```cpp
// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <iomanip>      // for setw()

class Graph{
private:
    int num_vertex;
    std::vector< std::list<int> > AdjList;
    int *color,             // 0:white, 1:gray, 2:black
        *distance,
        *predecessor,
        *discover,
        *finish;
    
public:
    Graph():num_vertex(0){};
    Graph(int N):num_vertex(N){
        // initialize Adj List
        AdjList.resize(num_vertex);
    };
    
    int GetColor(int i){return color[i];};
    int GetFinish(int i){return finish[i];};
    int GetPredecessor(int i){return predecessor[i];};
    
    void AddEdgeList(int from, int to);
    
    void BFS(int Start);
    void DFS(int Start);
    void DFSVisit(int vertex, int &time);
    void VariableInitializeDFS();
    
    void CCDFS(int vertex);     // 吃一個int, 表示起點vertex, 若沒給就從0開始
    void CCBFS(int vertex = 0);
    void SetCollapsing(int vertex);
    void PrintDataArray(int *array);
    void PrintFinish();
    void PrintPredecessor();
    
    Graph GraphTranspose();
    void PrintSCCs(int Start = 0);
    
    friend void QuickSort(int *vec, int front, int end, int *vec2);
    friend int Partition(int *vec, int front, int end, int *vec2);
    friend void swap(int *x, int *y);
    
    void TopologicalSort(int Start = 0);
};

void Graph::TopologicalSort(int Start){
    
    DFS(Start);         // 進行一次DFS用來取得 finish[]
    
    // 矩陣 finishLargetoSmall[] 用來儲存 finish[] 由大至小的vertex順序
    int finishLargetoSmall[num_vertex];
    for (int i = 0; i < num_vertex; i++) {
        finishLargetoSmall[i] = i;
    }
    // QuickSort()會更新 finishLargetoSmall[] 成 finish[] 由大至小的vertex順序
    QuickSort(finish, 0, num_vertex-1, finishLargetoSmall);
    
    std::cout << "Topological Sort:\n";
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(3) << finishLargetoSmall[i];
    std::cout << std::endl;
}

int main(){
    Graph g5(15);            // 建立如圖二(a)的DAG
    g5.AddEdgeList(0, 2);
    g5.AddEdgeList(1, 2);
    g5.AddEdgeList(2, 6);g5.AddEdgeList(2, 7);
    g5.AddEdgeList(3, 4);
    g5.AddEdgeList(4, 5);
    g5.AddEdgeList(5, 6);g5.AddEdgeList(5, 14);
    g5.AddEdgeList(6, 8);g5.AddEdgeList(6, 9);g5.AddEdgeList(6, 11);g5.AddEdgeList(6, 12);
    g5.AddEdgeList(7, 8);
    g5.AddEdgeList(9, 10);
    g5.AddEdgeList(12, 13);
    
    g5.TopologicalSort();
    g5.TopologicalSort(4);
     
    return 0;
}
```
output:

```cpp
Topological Sort:
  3  4  5 14  1  0  2  7  6 12 13 11  9 10  8
Topological Sort:
  3  1  0  2  7  4  5 14  6 12 13 11  9 10  8
```

</br>

以上的做法是另外呼叫一個修改過的`QuickSort()`，對額外的矩陣`finishLargetoSmall[]`進行排序，優點是不需要更動`DFS()`。

還有些常見的方法就是修改`DFS()`，主要有兩種，發生在當vertex要被標記為「已讀(visited)」或者「塗黑」時：

1. 把剛剛塗黑的vertex放進`stack`中，那麼按照順序，最先被塗黑的vertex就最先被放入`stack`的vertex，也就最後被`pop()`出`stack`。  
因此，對`stack`依序進行`pop()`便能夠維持`finish`由大到小的順序。([詳見GeeksforGeeks：Topological Sorting](http://www.geeksforgeeks.org/topological-sorting/))
2. 或者，把剛剛塗黑的vertex推進(push)一串Linked list，那麼，只要每次都是在Linked list的前端(front)加入vertex，當有下一個vertex被推入Linked list時，先前`finish`較小的vertex就被往後挪。  
最後，對Linked list進行一次traversal，得到的vertex順序就會是`finish`由大到小。



[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/TS_fig/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/TS_fig/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/TS_fig/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/TS_fig/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/TS_fig/f5.png?raw=true


</br>

以上便是利用`DFS()`來尋找DAG的Topological sort(拓撲排序)的介紹。  
基本上是`DFS()`的變形/延伸，再一次`finish`又扮演關鍵角色拯救了世界。


</br>

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch22](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Rashid Bin Muhammad：Topological Sort](http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/GraphAlgor/topoSort.htm)
* [GeeksforGeeks：Topological Sorting](http://www.geeksforgeeks.org/topological-sorting/)

<a name="series"></a>

</br>

##Graph系列文章

* [Graph: Intro(簡介)](http://alrightchiu.github.io/SecondRound/graph-introjian-jie.html)
* [Graph: Breadth-First Search(BFS，廣度優先搜尋)](http://alrightchiu.github.io/SecondRound/graph-breadth-first-searchbfsguang-du-you-xian-sou-xun.html)
* [Graph: Depth-First Search(DFS，深度優先搜尋)](http://alrightchiu.github.io/SecondRound/graph-depth-first-searchdfsshen-du-you-xian-sou-xun.html)
* [Graph: 利用DFS和BFS尋找Connected Component](http://alrightchiu.github.io/SecondRound/graph-li-yong-dfshe-bfsxun-zhao-connected-component.html)
* [Grpah: 利用DFS尋找Strongly Connected Component(SCC)](http://alrightchiu.github.io/SecondRound/grpah-li-yong-dfsxun-zhao-strongly-connected-componentscc.html)
* [Grpah: 利用DFS尋找DAG的Topological Sort(拓撲排序)](http://alrightchiu.github.io/SecondRound/grpah-li-yong-dfsxun-zhao-dagde-topological-sorttuo-pu-pai-xu.html)



</br>






