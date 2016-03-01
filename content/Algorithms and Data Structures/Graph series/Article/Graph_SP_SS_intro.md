Title: Single-Source Shortest Path：Intro(簡介)    
Date: 2016-2-29 20:12  
Category: 演算法與資料結構  
Tags: C++, Graph, Intro, Shortest Path, Relaxation 
Summary: 介紹於Graph中，以單一Vertex作為起點，抵達其餘Vertex之最短路徑(Single-Source Shortest Path)之問題情境與基本性質。

 


</br>
###先備知識與注意事項

如果非要在這個章節加上一份先備知識，我會說是[Google Map](https://www.google.com.tw/maps)。


***

##目錄

* [最短路徑問題(Shortest Path)](#sp)
    * [限制](#limit)
    * [特徵](#property)
* [觀念：Relaxation](#relax)
    * [Triangle inequality](#triangle)
    * [Upper-Bound property](#upper-bound)
    * [Convergence property](#convergence)
    * [Path-relaxation property](#path-relaxation)
* [參考資料](#ref)
* [Shortest Path系列文章](#series)


</br>

<a name="sp"></a>

##最短路徑問題(Shortest Path)

為了解決較為廣義的情境，接下來討論的最短路徑問題將考慮的是一個**weighted directed graph**，以weight總和表示path之成本，並以具有方向性之edge表示兩個vertex之間的關係。  

* undirected graph的問題能夠以directed graph的模型解決，反之則無法。
* 不具weight的edge也能夠以weighted edge模擬(將全部weight設為相同數值即可)，反之則無法。
* 可以視為只能處理unweighted graph之BFS/DFS的擴充包。

定義名詞：

* path之weight：若有一條$Path:0-1-2-...-K$，定義其weight為所有edge之weight總和，$w(path)=\sum_{i=1}^{K}w(i-1,i)$，如圖一。
* 最短路徑$w(path)=\delta(X,Y)$：在Graph上，所有從vertex(X)出發抵達vertex(Y)的path中，具有最小weight之path，稱為最短路徑，其weight滿足：
    * $\delta(X,Y)=\min${$w(path)|path from X to Y$}。  
    (可能有多條path之weight皆為極小值，那麼這些path都是最短路徑。)
* 若$\delta(X,Y)=\infty$，則表示無法從vertex(X)走到vertex(Y)。

<center>
![cc][f1]

**圖一。**
</center>

根據出發的vertex(稱為source)與終點vertex(稱為destination)之數量選擇，可以將最短路徑問題分成四類：

1. **Single-Pair Shortest Path**：從單一vertex，抵達某個特定vertex之最短路徑，此為第二種問題的子問題；
2. **Single-Source Shortest Path**：從單一vertex，抵達Graph中其餘所有vertex之最短路徑；
3. **Single-Destination Shortest Path**：從Graph中的每一個vertex抵達某個特定vertex之最短路徑：
    * 此為第二種問題之變形，只要把edge的方向相反，也就是在**G<sup>T</sup>**上，執行第二種問題之演算法即可。
4. **All-Pairs Shortest Path**：Graph中的所有vertex抵達其餘所有vertex之最短路徑。
    * 若把每一個vertex都當作起點，即可利用第二種問題之方法解決。
    * 不過之後將介紹的**Floyd-Warshall Algorithm**有更好的答案。


綜合以上，先學第二種問題的演算法：以單一vertex為起點，抵達Graph中的其餘所有vertex之最短路徑，就是最明智的選擇。(推銷成功)

先來看個簡單的範例：

<center>
![cc][f2]

**圖二(a)。**
</center>

如圖二(a)，考慮一個weighted directed graph，若以vertex(0)為起點，要找到從vertex(0)抵達Graph中其餘vertex的最短路徑，結果便如圖二(b)。  

* 考慮從vertex(0)走到vertex(2)：path$:0-1-2$的edge數最少，但是weight總和是$11$，而edge數較多的$Path:0-1-4-3-2$的weight總和只有$6$，才是從vertex(0)走到vertex(2)的最短路徑。
* 原因在於weight有負值(negative value)，所以edge越多的path，weight總和不見得越大(經過某些路徑使得整體成本降低)。  

<center>
![cc][f3]

**圖二(b)：path上之edge越少，不見得weight總和越小。**
</center>

在處理最短路徑問題時，最基本需要用到兩個資料項目：

* `distance[]`：記錄從起點vertex，經過具有weight之edge，走到其餘vertex之「距離」，也就是該條path之weight。若有一條$Path:0-1-2$，則path之weight總和即記錄在`distance[2]`。
* `predecessor[]`：除了距離之外，還希望能夠回溯路徑，因此需要記錄vertex之`predecessor`，並以此得到**Predecessor Subgraph**。

由於最短路徑一定**不包含cycle**，**Predecessor Subgraph**會是一棵**Shortest-Path Tree**。

 * 起點vertex即為**root**；
 * 從**root**到其餘vertex的path必定是唯一的最短路徑。

(若Graph上出現$\delta(X,Y)=\infty$的情況，**Predecessor Subgraph**會是**Shortest-Path Forest**。)

</br>
<a name="limit"></a>

###限制

為什麼最短路徑一定**不包含cycle**？  

由於weight只要求是實數(real number)，weight可正可負，因此Graph中可能出現**weight總和為正**的cycle與**weight總和為負**的cycle。

考慮圖三(a)，cycle$:1-2-3$之weight為$10$，若path經過此cycle，weight之總和必定會增加，此path不會是最短路徑。考慮從vertex(0)走到vertex(4)：

* $Path:0-1-2-3-4$之weight為$10$；
* 經過一次cycle，$Path:0-1-2-3-1-2-3-4$之weight為$20$。

<center>
![cc][f4]

**圖三(a)。**
</center>

考慮圖三(b)，cycle$:1-2-3$之weight為$-6$，表示path經過此cycle，weight之總和就會減少，而且經過越多次，weight總和越少，所以永遠不會有最短路徑，$\delta(X,Y)=-\infty$。考慮從vertex(0)走到vertex(4)：

* $Path:0-1-2-3-4$之weight為$4$；
* 經過一次cycle，$Path:0-1-2-3-1-2-3-4$之weight為$-2$；
* 經過兩次cycle，$Path:0-1-2-3-1-2-3-1-2-3-4$之weight為$-8$；



<center>
![cc][f5]

**圖三(b)。**
</center>

因此，在考慮最短路徑問題時，問題之Graph可以有總和為正值的cycle，但是不能有總和為負值的cycle。  

而演算法所挑選出來的最短路徑之Predecessor Subgraph，一定不包含cycle。

<a name="property"></a>

###特徵

最短路徑除了**不會有cycle**外，還有兩點特徵值得一提。

**特徵一**：因為最短路徑不存在cycle，在共有$V$個vertex之Graph中，從vertex(X)走到vertex(Y)的最短路徑，至多只會包含$|V|-1$條edge。

* 如圖二(b)，從vertex(0)走到vertex(5)的paht上，共有$5$條edge。

此種情形發生在**Shortest-Path Tree**退化成Linked list的時候。

**特徵二**：。若在Graph上，存在一條從vertex(0)走到vertex(K)的最短路徑，$P:v_0-v_1-v_2-...-v_K$，並定義所有path上的vertex所形成的**subpath**為$P_{ij}:v_i-v_{i+1}-...-v_j$，其中$1\leq i\leq j\leq K$，那麼每一條**subpath**都會是最短路徑。  
白話文：最短路徑必定是由較小段的最短路徑所連結起來的。換句話說，每個最短路徑接起來的路徑必定仍然是最短路徑。

圖二(b)中，從vertex(0)走到vertex(5)之最短路徑為：$P:v_0-v_1-v_4-v_3-v_2-v_5$，那麼其中的任何一條path，都是兩個vertex之間的最短距離：

* 從vertex(1)走到vertex(2)之最短路徑：$P:v_1-v_4-v_3-v_2$，weight總和為$1$。
* 從vertex(4)走到vertex(5)之最短路徑：$P:v_4-v_3-v_2-v_5$，weight總和為$3$。

<center>
![cc][f3]

**圖二(b)。**
</center>

以上將「能夠考慮最短路徑問題」的Graph形式，以及「最短路徑之**Predecessor Subgraph**」的特徵介紹完畢。

接著要介紹尋找最短路徑之演算法核心概念：**Relaxation**。


</br>

<a name="relax"></a>

##觀念：Relaxation

見圖四(a)，現從vertex(S)作為起點，欲尋找抵達vertex(X)與vertex(Y)之最短路徑。

<center>
![cc][f6]

**圖四(a)。**
</center>

其中，vertex(X)之最短路徑只有一種可能，$\delta(S,X)=w(S,X)$。  
而vertex(Y)可以從vertex(S)或者vertex(X)抵達，因此最短路徑$\delta(S,Y)$有兩種可能：

* $\delta(S,Y)=w(S,Y)$：vertex(Y)經由vertex(S)直接抵達。
* $\delta(S,Y)=w(S,X)+w(X,Y)$：vertex(S)先經過vertex(X)，再到vertex(Y)。

這兩條path中，weight最小的path即是最短路徑。  
找到最短路徑的方法很直觀，只要比較這兩條路徑的weight，挑weight小的path即可。

<center>
![cc][f7]

**圖四(b)。**
</center>

以圖四(b)為例，首先從vertex(S)出發，發現其與vertex(X)、vertex(Y)皆有edge相連，便更新兩個vertex的`distance`為兩條相連edge之weight：

* `distance[Y]=w(S,Y)`；
* `distance[X]=w(S,X)`。

接著考慮經過vertex(X)再到vertex(Y)之path有沒有可能成本較低，便比較`distance[Y]`與`distance[X]+w(X,Y)`：

* 若`distance[Y]`$>$`distance[X]+w(X,Y)`，則表示$P:S-X-Y$之成本小於$P:S-Y$，因此從vertex(S)經過vertex(X)再到vertex(Y)才是最短路徑，便更新
    * `distance[Y]=distance[X]+w(X,Y)`；
    * `predecessor[Y]=X`。
* 反之，若`distance[Y]`$<$`distance[X]+w(X,Y)`，表示$P:S-Y$之成本小於$P:S-X-Y$，則不需要更新兩個資料項目，目前的路徑已經是最短路徑。

以上的「比較`distance`後決定是否更新`distance`(與`predecessor`)」步驟，就是**Relaxation**。

`Relax()`之範例程式碼如下：

```cpp
// C++ code
// Relax() as a member function of class Graph_SP

void Graph_SP::Relax(int from, int to, int weight){
    
    if (distance[to] > distance[from] + weight) {
        distance[to] = distance[from] + weight;
        predecessor[to] = from;
    }
}
```


根據**Relaxation**可以衍生出幾個性質：

<a name="triangle"></a>

###Triangle inequality

經過**Relaxation**，並得到最短路徑後，任意兩個在Grpah上之vertex(X)與vertex(Y)，若存在edge(X,Y)從vertex(X)指向vertex(Y)，必定滿足以下關係(假設vertex(S)為起點)：

* $\delta(S,Y)\leq \delta(S,X)+w(X,Y)$。


<a name="upper-bound"></a>

###Upper-Bound property

一旦從vertex(S)抵達vertex(X)之path已經更新至最短路徑$\delta(S,X)$，那麼`distance[X]`就不會再更新了。

* 觀察`Relax()`的`if`條件式，若新的path之weight比原有的path之weight還大，就不更新`distance`與`predecessor`。


<a name="convergence"></a>

###Convergence property

假定Graph上存在從vertex(X)指向vertex(Y)之edge(X,Y)，並且從起點vertex走到vertex(Y)之最短路徑包含這條edge。  
若在對edge(X,Y)進行`Relax()`之前，從vertex(S)到達vertex(X)的path就已經滿足最短路徑，`distance[X]`$=\delta(S,X)$，那麼在對edge(X,Y)進行`Relax()`後，必定得到從vertex(S)走到vertex(Y)之最短路徑，並更新`distance[Y]`$=\delta(S,Y)$，而且至此之後，`distance[Y]`將不會再被更新。

如圖五(a)，$Path=S-X-Y$是從vertex(S)走到vertex(Y)的最短路徑，並且在對edge(X,Y)進行`Relax()`之前，從vertex(S)走到vertex(X)之path已經是最短路徑，`distance[X]`$=\delta(S,X)$，那麼，此時對edge(X,Y)進行`Relax()`，必定能得到從vertex(S)走到vertex(Y)之最短路徑，`distance[Y]`$=\delta(S,Y)$。

<center>
![cc][f8]

**圖五(a)。**
</center>

但是若從vertex(S)走到vertex(Y)之最短路徑不包含edge(X,Y)，那麼即使在對edge(X,Y)做`Relax()`之前，`distance[X]`已經等於$\delta(S,X)$，`distance[Y]`仍然不更新。

這表示，`distance[Y]`已經等於$\delta(S,Y)$，如圖五(b)。或者，從vertex(S)走到vertex(Y)之最短路徑會從其他vertex走到vertex(Y)。


<center>
![cc][f9]

**圖五(b)。**
</center>


<a name="path-relaxation"></a>

###Path-relaxation property

考慮一條從vertex(0)到vertex(K)之路徑$P:v_0-v_1-...-v_K$，如果在對path之edge進行`Relax()`的順序中，**曾經出現**edge(v<sub>0</sub>,v<sub>1</sub>)、edge(v<sub>1</sub>,v<sub>2</sub>)、...、edge(v<sub>K-1</sub>,v<sub>K</sub>)的順序，那麼這條path一定是最短路徑，滿足`distance[K]`$=\delta(v_0,v_K)$。

* 在對edge(v<sub>1</sub>,v<sub>2</sub>)進行`Relax()`之前，只要已經對edge(v<sub>0</sub>,v<sub>1</sub>)進行過`Relax()`，那麼，不管還有對其餘哪一條edge進行`Relax()`，`distance[2]`必定會等於$\delta(0,2)$，因為**Convergence property**。

例如，現有一條從vertex(0)走到vertex(3)之最短路徑為$Path:0-1-2-3$，根據**Convergence property**，只要在對edge(1,2)進行`Relax()`之前，已經對edge(0,1)進行`Relax()`，如此便保證$Path:0-1-2$一定是最短路徑，此時再對edge(2,3)進行`Relax()`，便能找到$Path:0-1-2-3$。  
換言之，只要確保`Relax()`的過程曾經出現「edge(0,1)->edge(1,2)->edge(2,3)」的順序，不需理會中間是否有其他edge進行`Relax()`，即使有也不影響最後結果。

* 例如，`Relax()`順序：「edge(2,3)->**edge(0,1)**->edge(2,3)->**edge(1,2)**->**edge(2,3)**」仍可以得到最短路徑，`distance[3]`$=\delta(0,3)$。



[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Graph%20series/ShortestPath_fig/SingleSource_intro/f9.png?raw=true





</br>  

以上便是稍微繁瑣的**Single-Source最短路徑**之問題情境的介紹。  

接下來將依序介紹三個演算法來找到Single-Source問題的最短路徑：

1. **Bellmem-Ford Algorithm**
2. **Shortest Path in DAG**
3. **Dijkstra's Algorithm**

只要掌握**Relaxation**、**Convergence property**與**Path-relaxation property**之概念就沒問題了。

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch24](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch6](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Wikipedia：Shortest path problem](https://en.wikipedia.org/wiki/Shortest_path_problem)
* [大話西遊之仙履奇緣](https://www.youtube.com/watch?v=UHd1jHjPL7E)

<a name="series"></a>

</br>

###Shortest Path系列文章

[Single-Source Shortest Path：Intro(簡介)](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathintrojian-jie.html)  
[Single-Source Shortest Path：Bellman-Ford Algorithm](http://alrightchiu.github.io/SecondRound/single-source-shortest-pathbellman-ford-algorithm.html)  


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


