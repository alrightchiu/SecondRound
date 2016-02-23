Title: Grpah: 利用DFS尋找Strongly Connected Component(SCC)    
Date: 2016-2-17 21:49  
Category: 演算法與資料結構  
Tags: C++, Graph, DFS, DAG  
Summary: 利用DFS尋找diredted graph中的strongly connected component(SCC)。



</br>
###先備知識與注意事項

在一個directed graph中，若對於任意兩個vertex(A)與vertex(B)之間，不是同時存在「從vertex(A)走到vertex(B)」以及「從vertex(B)走到vertex(A)」的path，那麼此directed graph就不是strongly connected，裡面一定包含了兩個以上的strongly connected component(SCC)。  

如圖一(a)，經由path:0-1-2-5，可以從vertex(0)走到vertex(5)，但是無論經過任何vertex，都沒有辦法從vertex(5)走到vertex(0)，因此，圖一(a)的directed graph並不是strongly connected，其中包含了兩個以上的SCC(答案是三個)。

<center>
![cc][f1]

**圖一(a)。**  
</center>


那麼，要如何分辨一個directed graph中的strongly connected component(SCC)，並列出每一個SCC中的所有vertex呢？

本篇文章的目的就是要回應此問題。

演算法將會用到**Transpose of Graph**，如圖一(b)，把G中所有vertex維持不變，**edge的方向顛倒**，就得到G<sup>T</sup>，例如，原本的edge(0,1)改為edge(1,0)，edge(5,6)改為edge(6,5)。  


<center>
![cc][f2]

**圖一(b)。**  
</center>

最重要的是：**G與G<sup>T</sup>的SCC完全相同**。  

原因在於，觀察G中包含在同一個SCC裡的vertex(2)與vertex(3)。G中同時存在「從vertex(2)走到vertex(3)」的path，以及「從vertex(3)走到vertex(2)」的path。在G進行「Transpose」得到G<sup>T</sup>後，這兩條path分別變成與原方向之相反方向，但是存在於vertex(2)與vertex(3)之間的**cycle**仍然存在。  
因此，在G裡面屬於同一個SCC的vertex，在G<sup>T</sup>裡將形成相同的SCC。 

最後一點溫馨小提醒：一如往常，本篇文章將不會有嚴謹證明，不過在[參考資料](#ref)會附上內有嚴謹證明的網站連結，請讀者務必前往一窺究竟。


***

##目錄

* [如果只有一次DFS()不行嗎？](#if_dfs_only)
* [演算法](#algorithm)
* [程式碼](#code)
* [參考資料](#ref)
* [BFS/DFS系列文章](#series)

</br>

<a name="if_dfs_only"></a>

##如果只有一次DFS()不行嗎？

在[上一篇文章](http://alrightchiu.github.io/SecondRound/graph-li-yong-dfshe-bfsxun-zhao-connected-component.html)提到，只用一次`DFS()`或`BFS()`得到`predecessor`後，便能夠找到undirected graph中的connected component。

那如果用來找SCC？

以下示範兩次`DFS()`來說明，一次是按照「$8、7、...、2、1$」之順序把vertex設為搜尋起點，另一次則是按照「$1、2、...、7、8$」之順序。

<center>
![dfs][f3]

**圖二(a)：以vertex(8)作為起點，接著是vertex(5)、vertex(3)。**
</center>

圖二(a)，首先以vertex(8)作為`DFS()`的第一次起點，在搜尋完以vertex(8)作為`root`的Depth-First Tree後，再以vertex(5)作為新的起點。同樣的，在搜尋完以vertex(5)作為`root`的Depth-First Tree後，再以vertex(3)作為新的起點。  
從圖二(a)的「時間軸」可以看出，此次`DFS()`找到了一個Depth-First Forest，其中包含三棵Depth-First Tree，而這三棵Depth-First Tree分別就是Graph中的三個SCC。

問題不就解決了嗎？  
透過一次`DFS()`就找到了directed graph中的SCC。

再接著看圖二(b)，以vertex(0)作為`DFS()`的起點。  
很遺憾，「時間軸」裡形成了一整棵Depth-First Tree，directed graph中的三個SCC沒有被分開。


<center>
![dfs][f4]

**圖二(b)：以vertex(0)作為起點。**  
</center>


由於SCC需要兩個方向的path(「vertex(X)到vertex(Y)」和「vertex(Y)到vertex(X)」)都成立，但是`DFS()`只在意「單方向」的edge，只要存在edge(X,Y)，便把`predecessor[Y]`更新成vertex(X)，在Predecessor Subgraph裡，vertex(X)與vertex(Y)便在同一棵Depth-First Tree中。  

因此，只有一次`DFS()`的`predecessor`是不夠的，圖二(a)只是運氣好。

</br>
以下便開始本次的「拒絕運氣好大作戰」。  
</br>



<a name="algorithm"></a>

##**演算法**

若考慮具有多個SCC的directed graph，為了方便起見，定義其**Component Graph**為$G^{SCC}=(V^{SCC},E^{SCC})$，其中：

* $V^{SCC}$：把每個SCC視為一個元素，並以該元素作為$V^{SCC}$的vertex。
    * 例如圖三(a)，令$C_1=${$0,1,2,3$}，$C_2=${$4,5$}，$C_3=${$6,7,8$}，則$V^{SCC}=${$C_1,C_2,C_3$}。
* $E^{SCC}$：考慮vertex(X)屬於$C_1$，vertex(Y)屬於$C_2$，若存在「連結兩個不同SCC」的edge(X,Y)，則edge(X,Y)便屬於$E^{SCC}$。
    * 以圖三(a)為例，vertex(1)屬於$C_1$，vertex(4)屬於$C_2$，則edge(1,4)屬於$E^{SCC}$，依此類推，便得到$E^{SCC}=${$(1,4),(2,5),(4,6),(5,6),(5,7)$}。


<center>
![dfs][f5]

**圖三(a)：每一個directed graph，只要「以SCC作為基本元素(vertex)」，都會有其相對應的component graph。**  
</center>


由上述定義可以觀察出，每一個directed graph，只要「以SCC作為基本元素(vertex)」，都會有其相對應的component graph。

而使用component graph的優點是：「component graph一定是**directed acyclic graph(DAG)**」。  

* 因為SCC的定義[(請參閱：Graph: Intro(簡介))](http://alrightchiu.github.io/SecondRound/graph-introjian-jie.html)，若C<sub>1</sub>與C<sub>2</sub>之前存在**cycle**，那就表示C<sub>1</sub>和C<sub>2</sub>都不應該自稱為SCC，而要合併C<sub>1</sub>與C<sub>2</sub>成為一個更大集合的SCC。因此，不同的SCC之間，一定不存在**cycle**。  
* 等價的性質：若directed graph中存在兩個SCC，分別為C<sub>1</sub>與C<sub>2</sub>，若存在一條path從C<sub>1</sub>中的vertex(X)走到C<sub>2</sub>中的vertex(Y)，就不可能同時存在一條path從C<sub>2</sub>中的vertex(Z)走到C<sub>1</sub>中的vertex(W)，否則即出現**cycle**，應該合併成更大的SCC(C<sub>3</sub>)，如圖三(b)。


<center>
![dfs][f6]

**圖三(b)：SCC。**  
</center>


考慮如圖三(c)的DAG(directed acyclic graph)，若`DFS()`在每次尋找「新的搜尋起點時」，能夠按照「一條path上，從尾端至開頭」的vertex順序，那麼**Predecessor Subgraph**就能長成「能夠分辨出SCC」的**Depth-First Forest**。

* 圖三(c)中，由於從C<sub>3</sub>無法往回走到C<sub>2</sub>，從C<sub>2</sub>無法往回走到C<sub>1</sub>，因此，`DFS()`的起點順序若為：C<sub>3</sub>、C<sub>2</sub>、C<sub>1</sub>，就能夠把這三個component graph中的vertex(也就是directed graph的SCC)給分開。


<center>
![dfs][f7]

**圖三(c)：。**  
</center>


那麼，該如何確保每一次都能找到「一條path上，從尾端至開頭的vertex順序」？

<center>
![dfs][f8]

**圖三(d)：。**  
</center>


再觀察圖三(d)，分別以起點順序「C<sub>2</sub>、C<sub>3</sub>、C<sub>1</sub>」與起點順序「C<sub>1</sub>、C<sub>2</sub>、C<sub>3</sub>」進行`DFS()`，配合圖三(c)，將發現，不論以哪個vertex作為起點，「`finish`的大小順序一定是C<sub>1</sub>、C<sub>2</sub>、C<sub>3</sub>」。

更廣義地，假設現有C<sub>1</sub>與C<sub>2</sub>分別為directed graph中兩個互斥(disjoint)的SCC，並且vertex(X)屬於C<sub>1</sub>，vertex(Y)屬於C<sub>2</sub>：

* 若directed graph中存在edge(X,Y)，那麼，C<sub>1</sub>集合中所有vertex的「最大`finish`」一定比C<sub>2</sub>集合中所有vertex的「最大`finish`」還要大。

以圖三(e)為例，component graph的$E^{SCC}$存在「從C<sub>1</sub>指向C<sub>2</sub>」以及「從C<sub>2</sub>指向C<sub>3</sub>」的edge，因此，若以SCC中vertex的「最大`finish`」代表`finish[SCC]`，`finish`的大小順序應為：`finish[C1]`>`finish[C2]`>`finish[C3]`，其中：

* $C_1=${$0,1,2,3$}，`finish[C1]`$=$`finish[3]`$=18$。
* $C_2=${$4,5$}，`finish[C2]`$=$`finish[5]`$=10$。
* $C_3=${$6,7,8$}，`finish[C3]`$=$`finish[8]`$=6$。  


<center>
![dfs][f9]

**圖三(e)：。**
</center>

考慮圖三(f)，仍然符合：`finish[C1]`>`finish[C2]`>`finish[C3]`：

* `finish[C1]`$=$`finish[0]`$=18$。
* `finish[C2]`$=$`finish[5]`$=15$。
* `finish[C3]`$=$`finish[6]`$=13$。


<center>
![dfs][f10]

**圖三(f)：。** 
</center>


由以上推論，可以更新在圖三(c)時的說明至更廣義的結論：

* 只要按照「`finish`小到大」的順序選取SCC中的vertex作為`DFS()`的起點，就能夠在**Predecessor Subgraph**中以**Depth-First Forest**分辨出所有SCC。

到這裡為止，可以確認：

1. 需要第一次`DFS()`先取得`finish`。
2. 再根據剛取得的`finish`之「順序」來判斷「第二次`DFS()`」的起點順序。
3. 進行第二次`DFS()`來取得`predecessor`，並利用Predecessor Subgraph分辨出SCC。

但是問題又來了。  
如果真的是按照「第一次`finish`由小到大」的順序選取SCC中的vertex作為第二次`DFS()`的起點，還是有可能失敗，因為第一次`DFS()`在選取起點時，並沒有對SCC的先備知識，可以視為隨機選取：

* 若第一次`DFS()`結果如圖三(e)，則按照「`finish`由小到大」的順序選取起點，將依序選中「vertex(7)、vertex(4)、vertex(1)」作為起點進行第二次`DFS()`，那麼將得到如圖三(g)之結果，順利區分三個SCC。

<center>
![dfs][f11]

**圖三(g)：。** 
</center>

* 但是如果第一次`DFS()`結果如圖三(f)，按照「`finish`由小到大」的順序選取起點，只會選中vertex(3)作為起點，便把整個Graph搜尋完畢，最後Predecessor Subgraph又形成一整棵Depth-First Tree，如圖三(h)。

<center>
![dfs][f12]

**圖三(h)：。** 
</center>



所以，先在Graph:G上執行第一次`DFS()`，得到`finish`後，按照`finish`由小到大的順序，作為第二次在Graph:G上執行`DFS()`的起點之方法，宣告失敗。

不過，還好有**Transpose of Graph: G<sup>T</sup>**。

<center>
![dfs][f13]

**圖三(i)：。** 
</center>

觀察圖三(i)中的G與G<sup>T</sup>，發現：

- 兩個Graph的SCC完全相同，皆為C<sub>1</sub>、C<sub>2</sub>、C<sub>3</sub>。
- SCC的`finish`順序完全相反。
    * 在G上，從C<sub>3</sub>無法往回走到C<sub>2</sub>，從C<sub>2</sub>無法往回走到C<sub>1</sub>。
    * 在G<sup>T</sup>上，從C<sub>1</sub>無法往回走到C<sub>2</sub>，從C<sub>2</sub>無法往回走到C<sub>3</sub>。

根據以上特徵，若先在Graph:G上執行第一次`DFS()`，得到`finish`後，按照`finish`由大到小的順序，會是「C<sub>1</sub>、C<sub>2</sub>、C<sub>3</sub>」，而**這個順序在Transpose of Graph: G<sup>T</sup>，就正好是「**`finish`**小到大」的順序**。

因此，以「第一次`DFS()`所得到的`finish`之由大到小順序」選取起點，在G<sup>T</sup>上進行第二次`DFS()`，就可以先選到C<sub>1</sub>，由於無法從C<sub>1</sub>走回C<sub>2</sub>，因此`DFS()`在搜尋完C<sub>1</sub>內的所有vertex後，便形成自己的Depth-First Tree。接著再依序挑選C<sub>2</sub>、C<sub>3</sub>為起點進行搜尋，並且建立起各自SCC的Depth-First Tree。

如此一來，便找到了directed graph中的SCC。

</br>

<a name="code"></a>

##程式碼

根據以上說明，演算法分成四個步驟：

1. 對G執行`DFS()`。
2. 產生G<sup>T</sup>。
3. 按照第一次`DFS()`所得到的`finish`由大到小的順序選取起點，對G<sup>T</sup>執行`DFS()`。
4. 從第二次`DFS()`的`predecessor`找到Predecessor Subgraph。若directed graph有多個SCC，那麼Predecessor Subgraph就會是Depth-First Forest，其中的每一棵Depth-First Tree都是一個SCC。


範例程式碼延續上一篇文章定義的`class Graph`，主要多了幾個函式：

1. `GetColor()`、`GetFinish()`、`GetPredecessor`：用來取得`color`、`finish`、`predecessor`。
2. `GraphTranspose()`：產生G<sup>T</sup>。
3. `VariableInitializeDFS()`：把原先`DFS()`主函式中，「配置記憶體」與「初始化」資料的部分獨立出來。
4. `QuickSort()`等三個函式：用來得到`finish`由大致小的vertex順序。
    * 若共有6個vertex，經過一次`DFS()`後得到`finish`如圖四，那麼`QuickSort()`將會對`finish`進行排序，並且在排序的過程，一併將`finish`原先對應的vertex排序後，放入`finishLargetoSmall`。之後再利用`finishLargetoSmall`的順序，進行第二次`DFS()`。
5. `PrintSCCs()`：尋找SCC最主要的函式，主要包含上述的四個步驟。(其中有許多用以顯示資料項的指令，與尋找SCC無關)


<center>
![dfs][f18]

**圖四：。** 
</center>


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
    
    int GetColor(int i){return color[i];};              // 取得private data: color
    int GetFinish(int i){return finish[i];};            // 取得private data: finish
    int GetPredecessor(int i){return predecessor[i];};  // 取得private data: predecessor
    
    void AddEdgeList(int from, int to);
    
    void BFS(int Start);
    void DFS(int Start);
    void DFSVisit(int vertex, int &time);
    void VariableInitializeDFS();     // 對DFS()需要的資料：color, discover, finish, predecessor
                                      // 進行「配置記憶體」與「初始化」
    
    void CCDFS(int vertex);           // 吃一個int, 表示起點vertex, 若沒給就從0開始
    void CCBFS(int vertex = 0);
    void SetCollapsing(int vertex);
    void PrintDataArray(int *array);  // 列印出array[]
    void PrintFinish();               // 列印出 finish[]
    void PrintPredecessor();          // 列印出 predecessor[]
    
    Graph GraphTranspose();           // 產生Transpose of Graph
    void PrintSCCs(int Start = 0);    // 吃一個int, 表示起點vertex, 若沒給就從0開始
    
    // 利用QuickSort()得到 finish[] 由大致小的vertex順序
    friend void QuickSort(int *vec, int front, int end, int *vec2);
    friend int Partition(int *vec, int front, int end, int *vec2);
    friend void swap(int *x, int *y);
};

void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
int Partition(int *vec, int front, int end, int *vec2){
    int pivot = vec[end];
    int i = front - 1;
    for (int j = front; j < end; j++) {
        if (vec[j] > pivot) {
            i++;
            swap(&vec[i], &vec[j]);
            swap(&vec2[i], &vec2[j]);
        }
    }
    swap(&vec[i + 1], &vec[end]);
    swap(&vec2[i + 1], &vec2[end]);
    
    return i + 1;   // 把 i + 1 當成下一個 recurrsive call 的 中間斷點
}
void QuickSort(int *vec, int front, int end, int *vec2){
    if (front < end) {
        int pivot = Partition(vec, front, end, vec2);
        QuickSort(vec, front, pivot - 1, vec2);
        QuickSort(vec, pivot + 1, end, vec2);
    }
}

void Graph::PrintSCCs(int Start){
    // 第一次DFS(), 目的是取得finish[]
    DFS(Start);
    
    // 顯示 第一次DFS()後的finish[]
    std::cout << "First DFS() on G, finish time:" << std::endl;
    PrintFinish();
    
    // gT代表Transpose of Graph
    Graph gT(num_vertex);
    gT = GraphTranspose();
    
    // 矩陣 finishLargetoSmall[] 用來儲存 finish[] 由大至小的vertex順序
    int finishLargetoSmall[num_vertex];
    for (int i = 0; i < num_vertex; i++) {
        finishLargetoSmall[i] = i;
    }
    // QuickSort()會更新 finishLargetoSmall[] 成 finish[] 由大至小的vertex順序
    QuickSort(finish, 0, num_vertex-1, finishLargetoSmall);
    
    // 列印出 finish[] 由大至小的vertex順序
    std::cout << "finish time Large to Small:" << std::endl;
    PrintDataArray(finishLargetoSmall);
    
    // 第二次DFS(), 執行在gT上, 先對四個資料「配置記憶體」且「初始化」
    gT.VariableInitializeDFS();
    int time = 0;
    for (int i = 0; i < num_vertex; i++){
        if (gT.GetColor(finishLargetoSmall[i]) == 0) {
            gT.DFSVisit(finishLargetoSmall[i], time);
        }
    }
    
    // 顯示 第二次DFS()後的finish[]
    std::cout << "Second DFS() on gT, finish time:\n";
    gT.PrintFinish();
    // 顯示 第二次DFS()後的predecessor[]
    std::cout << "predecessor[] before SetCollapsing:\n";
    gT.PrintPredecessor();
    
    for (int i = 0; i< num_vertex; i++)
        gT.SetCollapsing(i);
    
    // 顯示 SetCollapsing後的predecessor[]
    std::cout << "predecessor after SetCollapsing:\n";
    gT.PrintPredecessor();
    
    // 如同在undirected graph中尋找connected component
    int num_cc = 0;
    for (int i = 0; i < num_vertex; i++) {
        if (gT.GetPredecessor(i) < 0) {
            std::cout << "SCC#" << ++num_cc << ": " << i << " ";
            for (int j = 0; j < num_vertex; j++) {
                if (gT.GetPredecessor(j) == i) {
                    std::cout << j << " ";
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
void Graph::VariableInitializeDFS(){
    color = new int[num_vertex];
    discover = new int[num_vertex];
    finish = new int[num_vertex];
    predecessor = new int[num_vertex];
    
    for (int i = 0; i < num_vertex; i++) {
        color[i] = 0;
        discover[i] = 0;
        finish[i] = 0;
        predecessor[i] = -1;
    }
}
Graph Graph::GraphTranspose(){
    Graph gT(num_vertex);
    for (int i = 0; i < num_vertex; i++) {
        for (std::list<int>::iterator itr = AdjList[i].begin();itr != AdjList[i].end(); itr++) {
            gT.AddEdgeList(*itr, i);
        }
    }
    return gT;
}
void Graph::PrintDataArray(int *array){
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << array[i];
    std::cout << std::endl;
}
void Graph::PrintFinish(){
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << finish[i];
    std::cout << std::endl;
}
void Graph::PrintPredecessor(){
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << predecessor[i];
    std::cout << std::endl;
}

int main(){
    Graph g4(9);
    g4.AddEdgeList(0, 1);
    g4.AddEdgeList(1, 2);g4.AddEdgeList(1, 4);
    g4.AddEdgeList(2, 0);g4.AddEdgeList(2, 3);g4.AddEdgeList(2, 5);
    g4.AddEdgeList(3, 2);
    g4.AddEdgeList(4, 5);g4.AddEdgeList(4, 6);
    g4.AddEdgeList(5, 4);g4.AddEdgeList(5, 6);g4.AddEdgeList(5, 7);
    g4.AddEdgeList(6, 7);
    g4.AddEdgeList(7, 8);
    g4.AddEdgeList(8, 6);
    
    std::cout << "Vertex(0) as starting point for the First DFS():\n\n";
    g4.PrintSCCs();
    std::cout << "Vertex(3) as starting point for the First DFS():\n\n";
    g4.PrintSCCs(3);
     
    return 0;
}
```
output:

```cpp
Vertex(0) as starting point for the First DFS():

First DFS() on G, finish time:
   0   1   2   3   4   5   6   7   8
  18  17  16   5  14  15  13  12  11
finish time Large to Small:
   0   1   2   3   4   5   6   7   8
   0   1   2   5   4   6   7   8   3
Second DFS() on gT, finish time:
   0   1   2   3   4   5   6   7   8
   8   4   7   6  11  12  18  16  17
predecessor[] before SetCollapsing:
   0   1   2   3   4   5   6   7   8
  -1   2   0   2   5  -1  -1   8   6
predecessor after SetCollapsing:
   0   1   2   3   4   5   6   7   8
  -1   0   0   0   5  -1  -1   6   6
SCC#1: 0 1 2 3 
SCC#2: 5 4 
SCC#3: 6 7 8 

Vertex(3) as starting point for the First DFS():

First DFS() on G, finish time:
   0   1   2   3   4   5   6   7   8
  16  15  17  18  14  13  12  11  10
finish time Large to Small:
   0   1   2   3   4   5   6   7   8
   3   2   0   1   4   5   6   7   8
Second DFS() on gT, finish time:
   0   1   2   3   4   5   6   7   8
   5   6   7   8  12  11  18  16  17
predecessor[] before SetCollapsing:
   0   1   2   3   4   5   6   7   8
   1   2   3  -1  -1   4  -1   8   6
predecessor after SetCollapsing:
   0   1   2   3   4   5   6   7   8
   3   3   3  -1  -1   4  -1   6   6
SCC#1: 3 0 1 2 
SCC#2: 4 5 
SCC#3: 6 7 8  
```

結果如圖五(a)與圖五(b)：

<center>
![scc][f14]  

![scc][f15]

**圖五(a)：第一次**`DFS()`**以vertex(0)作為起點。** 
</center>


<center>
![scc][f16]  

![scc][f17]

**圖五(b)：第一次**`DFS()`**以vertex(3)作為起點。** 
</center>


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f13.png?raw=true
[f14]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f14.png?raw=true
[f15]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f15.png?raw=true
[f16]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f16.png?raw=true
[f17]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f17.png?raw=true
[f18]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/SCC_fig/f18.png?raw=true


</br>

以上便是利用`DFS()`來尋找directed graph中的strongly connected component之應用。  
其中，`finish`之順序在DAG(directed acyclic graph)中扮演了關鍵角色。

下一篇將介紹如何利用`DFS()`尋找DAG的Topological sort(拓撲排序)，敬請期待。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch22](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Rashid Bin Muhammad：Strongly Connected Components](http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/GraphAlgor/strongComponent.htm)
* [GeeksforGeeks：Strongly Connected Components](http://www.geeksforgeeks.org/strongly-connected-components/)

<a name="series"></a>

</br>

###BFS/DFS系列文章


[Graph: Breadth-First Search(BFS，廣度優先搜尋)](http://alrightchiu.github.io/SecondRound/graph-breadth-first-searchbfsguang-du-you-xian-sou-xun.html)  
[Graph: Depth-First Search(DFS，深度優先搜尋)](http://alrightchiu.github.io/SecondRound/graph-depth-first-searchdfsshen-du-you-xian-sou-xun.html)  
[Graph: 利用DFS和BFS尋找Connected Component](http://alrightchiu.github.io/SecondRound/graph-li-yong-dfshe-bfsxun-zhao-connected-component.html)  
[Grpah: 利用DFS尋找Strongly Connected Component(SCC)](http://alrightchiu.github.io/SecondRound/grpah-li-yong-dfsxun-zhao-strongly-connected-componentscc.html)  
[Grpah: 利用DFS尋找DAG的Topological Sort(拓撲排序)](http://alrightchiu.github.io/SecondRound/grpah-li-yong-dfsxun-zhao-dagde-topological-sorttuo-pu-pai-xu.html)  



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)



</br>






