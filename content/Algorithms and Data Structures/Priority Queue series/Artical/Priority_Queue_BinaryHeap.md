Title: Priority Queue：Intro(簡介) & Binary Heap
Date: 2016-3-5 21:25  
Category: 演算法與資料結構  
Tags: C++, Intro, Priority Queue, Heap, Binary Tree,  
Summary: 簡介Priority Queue(優先權佇列)，並以Binary Heap(二元堆積)實現Min-Priority Queue。



</br>
###先備知識與注意事項

本篇文章將介紹抽象的資料結構：Priority Queue(優先權佇列)，並且以Binary Heap(二元堆積)來實現Min-Priority Queue。  

Binary Heap的概念源自Binary Tree，若讀者有興趣，不妨回顧一下何謂**Complete Binary Tree**暖暖身。  
(請參考：[Binary Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/binary-tree-introjian-jie.html))


***

##目錄

* [簡介：Priority Queue(優先權佇列)](#pq)
* [Binary Heap(二元堆積)](#bh)
* [Binary Heap之Operation(函式)](#operation) 
    * [函式：MinHeapify](#MinHeapify)
    * [函式：BuildMinHeap](#BuildMinHeap)
    * [函式：其他](#other)
    * [函式：Minimum](#Minimum)
    * [函式：ExtractMin](#ExtractMin)
    * [函式：DecreaseKey](#DecreaseKey)
    * [函式：MinHeapInsert](#MinHeapInsert)
* [參考資料](#ref)
* [Priority Queue系列文章](#series)




</br>

<a name="pq"></a>

##簡介：Priority Queue(優先權佇列)

「待辦事項」通常是表面上看起來沒有順序、但其實具有「執行時的優先順序」的一堆事情。

Priority Queue(優先權佇列)就像在處理「待辦事項」，能夠在眾多各自具有優先順序之資料堆中，取出「最重要/最不重要」的項目：

**Priority**：對資料項目賦予「權重/優先權」，用以表示資料的重要程度。  
**Queue**：隊伍、佇列、順序，意味著把資料整理成「某種順序」的資料結構。

* 所謂的「某種順序」，可能是「先進先出(First-In-First-Out)」順序：每次要從此資料結構讀取資料時，必定是拿到「先進入」的資料。  
    (請參考：[Wikipedia：Queue](https://en.wikipedia.org/wiki/Queue_%28abstract_data_type%29))
* 也有可能是額外賦予資料的「權重/優先權」順序：每次要從此資料結構讀取資料時，必定會拿到具有「最大值/最小值權重」的資料。

如果每次要從Priority Queue讀取資料時，都拿到「權重最大」的資料，則稱此為**Max-Priority Queue**。  
反之，若每次讀取資料，都拿到「權重最小」的資料，則稱此為**Min-Priority Queue**。

舉例來說，如果每天起床固定要：

* 削鉛筆，重要性$2$分；
* 梳洗，重要性$10$分；
* 剪指甲，重要性$4$分；

那麼，以一個Max-Priority Queue來管理「每天起床的待辦事項」，第一件事情要先刷牙上廁所，然後是剪指甲，最後再削一些無關緊要的鉛筆。  
反之，若以一個Min-Priority Queue來管理「每天起床的待辦事項」，第一件事情要先削鉛筆，然後是剪指甲，最後才能去刷牙。

一個Max-Priority Queue，最基本會有三種操作：

1. **Insert**：將資料加入Queue中。
    * 例如，把「削鉛筆」、「梳洗」、「剪指甲」三件事情寫進Queue裡。 
2. **IncreaseKey**：當某項資料的「重要性提高」時，需要在Queue中改變資料的權重，以下將以Key代表權重。
    * 例如，最近突變出某種藉由指甲垢傳遞的流感病毒，使得「剪指甲」的重要性提高，便需要增加「剪指甲」的Key，假設Key提高到$11$分，那麼每天起床的工作順序就變成：「剪指甲」、「梳洗」、「削鉛筆」。 
3. **ExtractMax**：取得最重要(Key最大)的資料，並將其從Queue中移除。
    * 每天起床後，先從「待辦事項」得知，最重要的事情是「梳洗」，並在完成之後，將其從「待辦事項」移除，避免重複執行。 

與之對應的Min-Priority Queue之基本操作則是：

1. **Insert**：將資料加入Queue中。 
2. **DecreaseKey**：當某項資料的「重要性降低」時，需要改變在Queue中資料的Key。
3. **ExtractMin**：取得重要性最低(Key最小)的資料，並將其從Queue中移除。

</br>
為了替**Prim's Algorithm**以及**Dijkstra's Algorithm**鋪路，以下將介紹Min-Priority Queue做代表。  
不過只要掌握Priority Queue的概念，Max-Priority Queue就只是桌上的一塊小蛋糕。

稍微困難的是實現方法。  
[Fundamentals of Data Structures in C++, Ch9](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)一共列出了六種實現方法(可能還有更多)：

1. Leftist Tree(左傾樹)
2. Binomial Heap(二項式堆積)
3. Fibonacci Heap(費式堆積)
4. Pairing Heap(成對堆積)
5. Symmetric Min-Max Heap(對稱式最小-最大堆積)
6. Interval Heap(區間堆積)

前四種資料結構稱為**Single-End Priority Queue(SEPQ)**，亦即，該資料結構只能取得「最大」或是「最小」權重的資料。  
後兩種資料結構稱為**Double-End Priority Queue(DEPQ)**，可以同時取得「權重最大」以及「權重最小」的資料。

而下ㄧ小節，將介紹比上述六種資料結構更初級的**Binary Heap**來實現Min-Priority Queue。

</br>

<a name="bh"></a>

##Binary Heap(二元堆積)

為處理廣義情形，建議將Binary Heap中的元素定義為**Dictionary**，每個資料項目皆有其對應的**Key**值，也就是Priority Queue將使用的**Key**。  
(關於Dictionary，請參考：[Binary Search Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/binary-search-tree-introjian-jie.html#dictionary))

Binary Heap有兩項基本特徵：

特徵一：Binary Heap之結構可以視作**Complete Binary Tree**。

* 如圖一(a)，A$\sim$I共9個元素，便按照**Complete Binary Tree**之順序規則，填滿位置$1\sim9$，以index($1$)$\sim$index($9$)表示。

這樣的優點是易於尋找「parent-child」之關係，以index($i$)的node為例：

* 其**left child**必定位在**index($2i$)**；
* 其**right child**必定位在**index($2i+1$)**；
* 其**parent**必定位在**index($\lfloor i/2 \rfloor$)**。

以圖一(a)中位於index($3$)之node(F)為例：

* 其**left child**為index($6$)之node(E)；
* 其**right child**為index($7$)之node(I)；
* 其**parent**為index($1$)之node(D)。


<center>
![cc][f1]

**圖一(a)。**
</center>

特徵二，若將位於index($i$)之node視為subtree之**root**，那麼，可將此Binary Heap分為兩類：

* **Max Heap**：在每一個subtree中，**root**之「key」要比兩個**child**之「key」還要大，滿足$Key(i)>Key(2i)$以及$Key(i)>Key(2i+1)$。
* **Min Heap**：在每一個subtree中，**root**之「key」要比兩個**child**之「key」還要小，滿足$Key(i)<Key(2i)$以及$Key(i)<Key(2i+1)$。

以圖一(b)之Min Heap為例，每個node下方的藍色數字表示其**Key**值，檢查Min-Heap中任何一個subtree，皆滿足$Key(i)<Key(2i)$以及$Key(i)<Key(2i+1)$。

<center>
![cc][f2]

**圖一(b)。**
</center>


由於Binary Heap特有的「parent-child」之關係，只要讓矩陣中index($0$)的位置閒置，從index($1$)開始存放資料之Dictionary，便能夠使用矩陣(array)來表示Binary Heap，如圖一(b)。

Binary Heap中的每個資料(node)之Dictionary將以`struct`實現：  
(亦可使用`std::pair<int,int>`或其他)

```cpp
// C++ code
struct HeapNode{
    int element, key;
    HeapNode():element(0),key(0){};
    HeapNode(int node, int key):element(node), key(key){};
};
```

備註：為了區別`element`與`key`，圖示中的`element`是以「英文字母」表示，而`key`用`int`。不過這裡定義的`struct HeapNode`之`element`是以`int`表示，主要是為了使這篇文章定義的**Min-Priority Queue**可以在[Single-Source Shortest Path：Dijkstra's Algorithm](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathdijkstras-algorithm.html)直接複製貼上使用。

</br>
`class BinaryHeap`之定義，以及所有成員函式(member function)之宣告如下：

```cpp
// C++ code
class BinaryHeap{
private:
    std::vector<HeapNode> heap;        // 存放HeapNode資料的矩陣
    void swap(struct HeapNode &p1, struct HeapNode &p2);
    int FindPosition(int node);
    int GetParentNode(int node){return std::floor(node/2);};
public:
    BinaryHeap(){				// default constructor會把heap[0]給預留 
        heap.resize(1);   // 之後若新增HeapNode, 會從heap[1]開始新增
    }
    BinaryHeap(int n){
        heap.resize(n + 1);
    }
    bool IsHeapEmpty(){return (heap.size()<1);};
    
    // Min-Priority Queue
    void MinHeapify(int node, int length);
    void BuildMinHeap(std::vector<int> array);
    void DecreaseKey(int node, int newKey);
    void MinHeapInsert(int node, int key);
    int Minimum();                  // 回傳vertex的位置index
    int ExtractMin();               // 回傳vertex的位置index
    
//    void HeapSortSmalltoLarge();
    
    // Max-Priority Queue
    ...
};

```


</br>

<a name="operation"></a>

##Binary Heap之Operation(函式)

接著要介紹和Min Heap與Min-Priority Queue有關的函式。

小小提醒：為避免混淆，在圖示介紹中，資料的「element」是以「英文字母」表示，而實際的程式碼，資料的「element」仍是用`int`，例如，使用$A-1$、$B-2$做對應。

<a name="MinHeapify"></a>

###函式：MinHeapify

`MinHeapify()`是一種「由上至下」(由**root**往**leaf**)，按照Min Heap之規則逐一調整subtree的方法，步驟如下：

* 選定某個index($i$)之node($i$)，視為subtree之**root**；
* 比較**root**的Key與兩個**child**(node($2i$)與node($2i+1$))之Key；
    * 如果**left child**之Key最小，則將node($i$)與node($2i$)對調位置，使原先的**left child**成為**root**；
    * 如果**right child**之Key最小，則將node($i$)與node($2i+1$)對調位置，使原先的**right child**成為**root**；
* 並在對調之後，繼續檢查「新的subtree」是否滿足Min Heap的規則。

  
以圖二(a)為例，node(K)、node(Y)、node(Z)所形成的subtree不符合Min Heap的規則，因此：

* 比較三者的Key，以變數`int smallest`記錄具有最小Key值的node。
* `smallest`是Key為$17$的node(Y)，則將node(K)與node(Y)互換位置，如圖二(b)。


<center>
![cc][f3]

**圖二(a)。**

![cc][f4]

**圖二(b)。**
</center>

在經過以上步驟之後，node(K)、node(Y)、node(Z)所形成的subtree已經滿足Min Heap的規則。  

不過，由於node(K)之Key值比node(Y)之Key值大，因此，即使原先由node(Y)、node(B)、node(G)形成之subtree滿足Min Heap規則(以圖二(a)的情形為例，原先已經不滿足Min Heap)，仍不能保證node(K)取代node(Y)後，node(K)、node(B)、node(G)所形成之subtree也滿足Min Heap規則，所以需要重複上述步驟，再次以node(K)作為subtree之**root**，檢查並調整subtree成Min Heap，如圖二(c)與圖二(d)。

<center>
![cc][f5]

**圖二(c)。**

![cc][f6]

**圖二(d)。**
</center>

因為`smallest`挑的是node($i$)、node($2i$)、node($2i+1$)中Key值最小的node，使之成為subtree之`root`，因此「所有被檢查過的」subtree，必定滿足Min Heap之規則，如圖二(d)中的「node(Y)、node(G)、node(Z)」與「node(G)、node(B)、node(K)」。

但是，Binary Heap中仍然可能有某些subtree不符合Min Heap規則，如圖二(d)中的「node(E)、node(F)、node(I)」，因此，會需要一個迴圈對「所有具有**child**的node」進行檢查(利用`MinHeapify()`檢查)，這就是下一個函式`BuildMinHeap()`的任務。

`MinHeapify()`之範例程式碼如下：

```cpp
// C++ code
void BinaryHeap::MinHeapify(int node, int length){
    
    int left = 2*node,          // 取得left child
        right = 2*node + 1,     // 取得right child
        smallest;               // smallest用來記錄包含root與child, 三者之中Key最小的node
    
    if (left <= length && heap[left].key < heap[node].key)
        smallest = left;
    else
        smallest = node;
    
    if (right <= length && heap[right].key < heap[smallest].key)
        smallest = right;
    
    if (smallest != node) {                 // 如果目前node的Key不是三者中的最小
        swap(heap[smallest], heap[node]);   // 就調換node與三者中Key最小的node之位置
        MinHeapify(smallest, length);       // 調整新的subtree成Min Heap
    }
}
```



<a name="BuildMinHeap"></a>

###函式：BuildMinHeap


`BuildMinHeap()`的任務很簡單，把每一個「具有**child**」的node都進行過一次`MinHeapify()`，如此便能保證Binary Heap中的所有subtree皆滿足Min Heap規則，便能將一個由任意矩陣代表的Binary Heap轉換成Min Heap。

根據Binary Heap的index($i$)特徵：

* **left child**在index($2i$)；
* **right child**在index($2i+1$)；
* **parent**在index($\lfloor i/2 \rfloor$)；

若Binary Heap共有$N$個node，那麼所有「具有**child**」的node，必定位在index($i$)到index($\lfloor N/2 \rfloor$)。

以圖三(a)中的任意Binary Heap(還不是Min Heap)為例，共有$9$個node，因此，必定只有index($1$)到index($4$)的node具有**child**。  

<center>
![cc][f7]

**圖三(a)。**
</center>

因此，`BuildMinHeap()`只要從index($4$)之node，一路往index($1$)之node進行`MinHeapify()`，便能將此Binary Heap轉換成Min Heap，見圖三(b)。

<center>
![cc][f7_1]

**圖三(b)。**
</center>

`BuildMinHeap()`之範例程式碼如下：

* input：給定一個任意矩陣`array[]`。
    * 此處給定`std::vector<int> array`，把`array`的**index**視為`element`，把`array`的**數值**視為`key`。
        * 若array[] = {100, 27, 34, 56,...}，那麼key(100)就對應到element(0)，key(27)對應到element(1)，key(34)對應到element(2)，依此類推。
        * 例如在**Graph**問題中，`array`的**index**時常對應到「特定的vertex」，例如`BFS()`的`distance[]`，`distance[1]`即表示「從起點vertex走到vertex(1)」的距離，因此不需要特別使用`struct HeapNode`表示`array`的矩陣元素。
        
    * 也可以把input令成`std::vector<HeapNode> array`，那麼每一個矩陣元素都有各自的`element`與`key`，依序放進`std::vector<HeapNode> heap`即可。


* `heap[]`初始化：先把`array[]`的資料放進`heap[]`，並將`heap[0]`閒置。
* 接著對index($\lfloor N/2 \rfloor$)到index($1$)進行`MinHeapify()`。  
</br>

```cpp
// C++ code
void BinaryHeap::BuildMinHeap(std::vector<int> array){

    // 將array[]的資料放進 heap之矩陣中, 並預留 heap[0] 不做使用
    for (int i = 0; i < array.size(); i++) {     
        heap[i + 1].element = i;                 // 把array[]的idx視為element
        heap[i + 1].key = array[i];              // 把array[]的數值視為key
    }
    for (int i = (int)heap.size()/2; i >= 1 ; i--) {
        MinHeapify(i, (int)heap.size()-1);     // length要減一, 因為heap從從1開始存放資料
    }
}
```



<a name="other"></a>

###函式：其他

在進入與Min-Priority Queue有關的函式之前，先介紹些其他(雜項)放鬆心情。

首先是在`MinHeapify()`出現過的`swap()`，單純地利用**reference**作為函式的參數，對Heap中的node進行位置調換：

```cpp
// C++ code
void BinaryHeap::swap(struct HeapNode &p1, struct HeapNode &p2){
    
    struct HeapNode temp = p1;
    p1 = p2;
    p2 = temp;
}
```

`FindPosition()`是為了確認特定元素所在的位置(index)，這會用在Min-Priority Queue中的`DecreaseKey()`，因為其需要先「找到資料在Heap中的位置」，再調整該資料之Key。

不過由於`DecreaseKey()`的時間複雜度只有$O(\log N)$，若使用以下的直覺寫法，缺點是會把時間複雜度提高到$O(N)$(其中$N$為node總數)。  
還有一些替代方法，例如以空間換取時間，設立一個矩陣變數，記錄每一筆資料的位置，便能維持`DecreaseKey()`的效率。  

這部分就留給讀者自行斟酌。


```cpp
// C++ code
int BinaryHeap::FindPosition(int node){
    
    int idx = 0;
    for (int i = 1; i < heap.size(); i++) {
        if (heap[i].element == node) {
            idx = i;
        }
    }
    return idx;
}
```
以及定義在`class BinaryHeap`裡面的`IsHeapEmpty()`與`GetParentNode()`，分別檢查Heap是否有資料，和回傳node(i)之**parent**的index($\lfloor i/2 \rfloor$)：

```cpp
// C++ code
class BinaryHeap{
    ...
    bool IsHeapEmpty(){return (heap.size()<1);};
    int GetParentNode(int node){return std::floor(node/2);};
    ...
};
```


<a name="Minimum"></a>

###函式：Minimum

因為Min Queue的規則，**root**一定是所有node中，具有最小Key值的node，因此，若要得到最小值，只要讀取`heap[1]`即可：

```cpp
// C++ code
int BinaryHeap::Minimum(){
    return heap[1].element;
}

```


<a name="ExtractMin"></a>

###函式：ExtractMin

`ExtractMin()`的目的是「回傳具有最小Key的node之index」，並且將其從Heap中移除，步驟如下：

* 確認Heap是否有資料，若沒有的話，便回傳**error：巧婦難為無米之炊**。
* 若Heap中有資料，先以變數`min`讀取Min Heap中的**root**之資料內容，**root**即為Heap中具有最小Key值之node；
* 接著把Heap中「最後一個node」之資料放進「第一個index位置」裡面，如此便從Heap中移除原先的「最小Key值node」；
* 由於在上個步驟已經把原先位於「最後位置index」之node放進**root**之位置，便能夠直接刪除最後一個位置的記憶體位置，調整存放資料的`heap`；
    * 在此，因為使用了C++標準函式庫(STL)的`std::vector`，若要刪除`heap`的最後一個元素，只要只用成員函式(member function)：`std::vector::erase()`即可。  
    (關於`std::vector::erase`，請參考：[Cplusplus：std::vector::erase](http://www.cplusplus.com/reference/vector/vector/erase/))
* 此時，**root**位置的node之Key極有可能比其兩個**child**之Key值還要大，有可能違反Min Heap規則，因此需要對其執行`MinHeapify()`。

以圖四(a)為例，要取出Min Heap的**root**，也就是Key值為2的node(D)，並且將Min Heap中，位在最後一個index之node(C)放進**root**，然後利用`MinHeapify()`重新將Heap調整成Min heap，如圖四(b)。


<center>
![cc][f8]

**圖四(a)。**
</center>

<center>
![cc][f8_1]

**圖四(b)。**
</center>


`ExtractMin()`之範例程式碼如下：


```cpp
// C++ code
int BinaryHeap::ExtractMin(){
    
    if (IsHeapEmpty()) {
        std::cout << "error: heap is empty\n";
        exit(-1);
    }
    int min = heap[1].element;
    // delete the first element/vertex
    heap[1] = heap[heap.size()-1];            // 把最後一個element放到第一個位置,
    heap.erase(heap.begin()+heap.size()-1);   // 再刪除最後一個element
    MinHeapify(1, (int)heap.size());          // 目前, heap[1]具有最大Key, 需要進行調整
    
    return min;
}
```


<a name="DecreaseKey"></a>

###函式：DecreaseKey

`DecreaseKey()`的目的是調整Min Heap中的node之Key值，因為Key值改變，極有可能違反Min Heap規則，因此也需要對Heap進行調整，步驟如下：

* 由於函式的參數(argument)是`struct`結構中的`element`，若以圖五(a)為例，資料的`element`就是「英文字母」(A、B、C等等)，因此，先利用`FindPosition()`找到該資料在Heap中的位置index；
* 再判斷，若參數中的`newKey`沒有比原先的Key還小，就直接結束函式(可以想成`DecreaseKey()`只有把資料之Key降低，沒有調高的功能)；
* 若沒有在上個步驟結束函式，便把資料之Key更新成`newKey`；
    * 因為使用矩陣存放資料，所以只要有資料在Heap中的index，即可靠index對資料進行存取。
* 因為是把資料的Key「降低」，因此，有可能使得原先資料所位於的subtree違反Min Heap規則，需要調整：
    * 假設被修改的資料是位於index($i$)的node($i$)，便比較node($i$)與其**parent**(也就是node($\lfloor i/2 \rfloor$))之Key值，
    * 如果node($i$)之Key值較小，便交換index($i$)與index($\lfloor i/2 \rfloor$)上的資料(如同在`Minheapify()`中的交換`swap()`)。
    * 若node($i$)之Key值仍然比其**parent**之Key值大，表示，node($i$)所在之subtree仍滿足Min Heap規則，即可結束函式。
    * 還有，由於Heap的**root**是從index($1$)開始存放資料，若一路回溯**parent**直到index小於$1$，表示Heap中所有與「被修改的資料」有關之subtree都被檢查過了，可以結束函式。


<center>
![cc][f9]

**圖五(a)。**
</center>

以圖五(a)為例，若將node(H)之Key值從原先的$15$更改成$3$，將使得subtree「node(A)、node(G)、node(H)」違反Min Heap規則，如圖五(b)，必須利用上述方法修正。  
修正流程見圖五(c)。


<center>
![cc][f10]

**圖五(b)。**
</center>

<center>
![cc][f10_1]

**圖五(c)。**
</center>

`DecreaseKey()`之範例程式碼如下：

```cpp
// C++ code
void BinaryHeap::DecreaseKey(int node, int newKey){
    
    int posi = FindPosition(node);      // 找到node所在的位置index
    
    if (newKey > heap[posi].key) {      // 如果不是把node的Key下修, 便終止此函式
        std::cout << "new key is larger than current key\n";
        return;
    }
    heap[posi].key = newKey;            // 更新node之Key後, 需要檢查是否新的subtree滿足Min Heap
    while (posi > 1 && heap[GetParentNode(posi)].key > heap[posi].key) {
        swap(heap[posi], heap[GetParentNode(posi)]);
        posi = GetParentNode(posi);
    }
}
```


<a name="MinHeapInsert"></a>

###函式：MinHeapInsert

有了`DecreaseKey()`後，要在Min Heap中新增資料會容易許多容易：  
(若恰好以`std::vector`建立`heap[]`，簡直只要兩行程式碼)

* 在Heap中多配置一塊新的記憶體位置，也就是把用來儲存Heap之矩陣`heap[]`拉長，存放新的資料；
* 現在新的資料已經位於Heap中的最後一個位置index，只要利用`DecreaseKey()`，即可將新的Heap調整成Min Heap。
    * 因為`DecreaseKey()`並不會阻止「`newKey`等於node現有Key」的情形，所以，即使在`DecreaseKey()`的輸入參數(input argument)上，`newKey`若等於已經存在Heap中的node之Key也是可行的。


以圖六(a)為例，若要在Heap中新增Key為$6$之node(J)，便將其加入`heap[]`的最後位置，再利用`DecreaseKey()`比較node(J)與其**parent**之Key值，檢查是否符合Min Heap規則，若不符合即進行修正，見圖六(a)-圖六(c)。

<center>
![cc][f11]

**圖六(a)。**

![cc][f12]

**圖六(b)。**

![cc][f13]

**圖六(c)。**
</center>


`MinHeapInsert()`之範例程式碼如下：

```cpp
// C++ code
void BinaryHeap::MinHeapInsert(int node, int key){
    
    heap.push_back(HeapNode(node,key));    // 在heap[]尾巴新增一個node
    DecreaseKey(node, key);
}
```

</br>


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f7.png?raw=true
[f7_1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/BuildMinHeap_v2.gif?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f8.png?raw=true
[f8_1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/ExtractMin_v2.gif?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f10.png?raw=true
[f10_1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/DecreaseKey.gif?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Priority%20Queue%20series/Binary%20Heap%20fig/f13.png?raw=true



由以上說明可以發現，在`MinHeapify()`中，是「由上往下」對subtree進行修正，有的寫法會將此操作獨立成函式：**SiftDown**；  
在`DecreaseKey()`中，則是「由下往上」進行修正，此則稱為**SiftUp**，範例請參考：[Code Review：Implementation of binary heap in C++](http://codereview.stackexchange.com/questions/42999/implementation-of-binary-heap-in-c)。

</br>  

以上便是以Binary Heap實現Min-Priority Queue之說明，後者將在許多應用出現，包括與Graph相關的**Prim's Algorithm**和[Dijkstra's Algorithm](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathdijkstras-algorithm.html)。

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch6](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch5, Ch9](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Ashley Montanaro：Priority queues and Dijkstra’s algorithm](https://www.cs.bris.ac.uk/~montanar/teaching/dsa/dijkstra-handout.pdf)
* [禪心劍氣相思骨：Priority Queue 解析1 - 從binary heap開始](http://blog.yam.com/rockmanray/article/44952434)
* [Binary Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/binary-tree-introjian-jie.html)
* [Code Review：Implementation of binary heap in C++](http://codereview.stackexchange.com/questions/42999/implementation-of-binary-heap-in-c)
* [Single-Source Shortest Path：Dijkstra's Algorithm](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathdijkstras-algorithm.html)
* [Wikipedia：Priority Queue](https://en.wikipedia.org/wiki/Priority_queue)
* [Wikipedia：Binary Heap](https://en.wikipedia.org/wiki/Binary_heap)


<a name="series"></a>

</br>

###Priority Queue系列文章

[Priority Queue：Intro(簡介) & Binary Heap](http://alrightchiu.github.io/SecondRound/priority-queueintrojian-jie-binary-heap.html)  



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


