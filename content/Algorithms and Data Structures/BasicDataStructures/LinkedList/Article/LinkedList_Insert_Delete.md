Title: Linked List: 新增資料、刪除資料    
Date: 2016-4-19 22:39   
Category: 演算法與資料結構  
Tags: C++, Intro, Linked List     
Summary: 介紹於Linked List(連結串列)中新增資料與刪除資料的方法。


</br>
###先備知識與注意事項

本篇文章將延續[Linked List: Intro(簡介)](http://alrightchiu.github.io/SecondRound/linked-list-introjian-jie.html)，繼續介紹如何在Linked List中新增資料與刪除資料。

<center>
![cc][f0]

</center>


`class ListNode`與`class LinkedList`的定義如下：


```cpp
// C++ code
class LinkedList;    // 為了將class LinkedList設成class ListNode的friend,
                     // 需要先宣告
class ListNode{
private:
    int data;
    ListNode *next;
public:
    ListNode():data(0){ next = 0 ;};
    ListNode(int a):data(a){ next = 0;};
    friend class LinkedList;
};

class LinkedList{
private:
    // int size;                   // size是用來記錄Linked List的長度, 非必要
    ListNode *first;            // list的第一個node
public:
    LinkedList(){first = 0;};
    void PrintList();           // 印出list的所有資料
    void Push_front(int x);     // 在list的開頭新增node
    void Push_back(int x);      // 在list的尾巴新增node
    void Delete(int x);         // 刪除list中的 int x
    void Clear();               // 把整串list刪除
    void Reverse();             // 將list反轉: 7->3->14 => 14->3->7
    bool HasCycle();            // 檢查list中是否有cycle
};
```


***

##目錄

* [函式：PrintList](#print)
* [函式：Push_front](#front)
* [函式：Push_back](#back)
* [函式：Delete](#delete)
* [函式：Clear](#clearall)
* [測試](#test)
* [參考資料](#ref)
* [Linked list系列文章](#series)



</br>

<a name="print"></a>

##函式：PrintList

第一個要介紹的是`PrintList()`，功能就是把Linked List中的所有資料依序印出。要印出所有的資料，就必須「逐一訪問(**Visiting**)」Linked List中的每一個node，這樣的操作又稱為**Traversal(尋訪)**。

能夠完成這樣的操作，要歸功於node中記錄了「下一個node的記憶體位置」，如此，才能在訪問完當前的node之後，知道要繼續往哪一個記憶體位置上的node前進。

<center>
![cc][f1]

**圖一。**
</center>

以圖一為例：

* 建立`ListNode *current`來表示「目前走到哪一個node」。
* 若要對Linked List存取資料，必定是從第一個node開始，所以把`current`指向`first`所代表的記憶體位置，`current=first`。
    * 目前`first`即為node($7$)。
    * 同時，還能夠知道「下一個node」是指向node($3$)。
* 在印出`current->data`，也就是$7$後，便把`current`移動到「下一個node」。
    * 透過`current=current->next`，即可把`current`指向node($3$)所在的記憶體位置。
* 重複上述步驟，直到`current`指向Linked List的終點`NULL`為止，便能印出所有資料。

由此可見，所有需要在Linked List中尋找特定資料的操作，都會用上**Traversal**。

程式範例如下：

```cpp
// C++ code
void LinkedList::PrintList(){
    
    if (first == 0) {                      // 如果first node指向NULL, 表示list沒有資料
        std::cout << "List is empty.\n";
        return;
    }
    
    ListNode *current = first;
    while (current != 0) {                 // Traversal
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}
```


</br>

<a name="front"></a>

##函式：Push_front

`Push_front()`的功能是在Linked List的開頭新增資料。  

若考慮在Linked list($3$-$>14$)的開頭加入$23$，方法如下：

* 先建立一個新的節點`ListNode *newNode`，帶有欲新增的資料($23$)，如圖二(a)。
* 將`newNode`中的**pointer**，`ListNode *next`，指向Linked List的第一個node`first`，如圖二(b)。
* 接著，把`first`更新成`newNode`。

經過以上步驟(時間複雜度為O($1$))便得到新的Linked List：$23$-$>3$-$>14$。

<center>
![cc][f2]

**圖二(a)。**
</center>

<center>
![cc][f3]

**圖二(b)。**
</center>

程式範例如下：

```cpp
// C++ code
void LinkedList::Push_front(int x){
    
    ListNode *newNode = new ListNode(x);
    newNode->next = first;        // 先把first接在newNode後面
    first = newNode;              // 再把first指向newNode所指向的記憶體位置
}
```



</br>

<a name="back"></a>

##函式：Push_back

`Push_back()`的功能是在Linked List的尾巴新增資料。

若考慮在Linked list($7$-$>3$-$>14$)的尾巴加入$23$，方法如下：

* 先建立一個新的節點`ListNode *newNode`，帶有欲新增的資料($23$)。
* 先利用如同`PrintList()`中提過的**Traversal**，把新建立的`ListNode *current`移動到Linked List的尾端，node($14$)，如圖三(a)。
    * 有些資料結構會在`class LinkedList`中新增一項`ListNode *last`，記錄Linked List的最後一個node，那麼，`Push_back()`就不需要**Traversal**，可以在O($1$)時間內完成。
    * 若沒有`ListNode *last`，就需要O($N$)的**Traversal**。
* 接著把`current`的`next pointer`指向newNode，如圖三(b)。

即可得到新的Linked List：$7$-$>3$-$>14$-$>23$。


<center>
![cc][f4]

**圖三(a)。**
</center>

<center>
![cc][f5]

**圖三(b)。**
</center>

程式範例如下：


```cpp
// C++ code
void LinkedList::Push_back(int x){
    
    ListNode *newNode = new ListNode(x);
    
    if (first == 0) {             // 若list沒有node, 令newNode為first
        first = newNode;
        return;
    }
    
    ListNode *current = first;
    while (current->next != 0) {  // Traversal
        current = current->next;
    }
    current->next = newNode;
}
```




</br>

<a name="delete"></a>

##函式：Delete

`Delete(int x)`的功能是要刪除Linked List中，資料為`int x`的node。  
會有兩種情形，第一種是Linked List中確實有`int x`，第二種是沒有。在第一種情況中，要再把`int x`位於`first`的情況分開。

**case1-1**：要在Linked List($7$-$>3$-$>14$)中刪除具有$3$的node，見圖四(a)：

* 利用**Traversal**的概念，以`ListNode *current`指向node($3$)，同時有`ListNode *previous`指向node($3$)的「前一個node」，node($7$)。
* 接著，把`previsou`的`next pointer`指向`current`的`next pointer`。
* 再把`current`的記憶體釋放(若是使用`new`進行動態配置，就使用`delete`釋放)，還給**heap**。

關鍵就是，在整個`Delete()`的過程，只有node($3$)知道node($14$)的記憶體位置，所以在把node($3$)刪除之前，必須先透過node($3$)的**pointer**找到node($14$)，把node($14$)接到node($7$)上(也可以說是，換成用node($7$)的**pointer**記住node($14$)的記憶體位置)。

<center>
![cc][f6]

**圖四(a)。**
</center>


**case1-2**：若要刪除具有$7$的node，而且Linked List只有一個node，見圖四(b)：

* 需要把這個情況獨立出來的原因是，這個情況不會進行**Traversal**，所以`ListNode *previous`始終指向`NULL`，便不能呼叫其private data，若進行`previous->next`將會因為意圖對「無效的」記憶體位置進行存取，而產生像是「EXC_BAD_ACCESS」的錯誤(error)。
* 所以只要直接釋放`first`的記憶體位置即可。



<center>
![cc][f7]

**圖四(b)。**
</center>

**case2**：若Linked List中沒有要刪除的node，包含Linked List本來就沒有任何node，見圖四(c)：

* 若想要刪除$8$，但是Linked List($7$-$>3$-$>14$)沒有$8$，那麼在**Traversal**後，`ListNode *current`會一路走到Linked List的結尾，也就是`NULL`。
* 若Linked List本來就是空的，那麼建立的`ListNode *current = first`，`current`也會指向`NULL`。
* 以上這兩種情況，直接結束`Delete()`函式。


<center>
![cc][f8]

**圖四(c)。**
</center>

程式範例如下：


```cpp
// C++ code
void LinkedList::Delete(int x){
    
    ListNode *current = first,      
             *previous = 0;
    while (current != 0 && current->data != x) {  // Traversal
        previous = current;                       // 如果current指向NULL
        current = current->next;                  // 或是current->data == x
    }                                             // 即結束while loop
    
    if (current == 0) {             // list沒有要刪的node, 或是list為empty
        std::cout << "There is no " << x << " in list.\n";
        return;
    }
    else if (current == first) {    // list只有一個node
        delete first;               
        first = 0;                  // 當指標被delete後, 將其指向NULL, 
        current = 0;                // 可以避免不必要bug
        return;                     
    }
    else {                          // 其餘情況, previous不為NULL
        previous->next = current->next;
        delete current;
        current = 0;
    }
}
```



</br>

<a name="clear"></a>

##函式：Clear

`Clear()`的功能是清除整個Linked List。方法如下：

* 從Linked List的「第一個node」`first`開始，進行**Traversal**。
    * 利用`first=first->next`即可不斷移動`first`。
* 建立一個`ListNode *current`記錄「要刪除的node」之記憶體位置。
* 重複上述步驟，直到`first`指向Linked List的尾巴`NULL`為止。

見圖五(a)：

* 原先`first`記錄的是node($7$)。
* 建立`ListNode *current`記錄`first`，也就是node($7$)。
* 將`first`移動到node($3$)。
* 刪除`current`指向的node($7$)。

如此，便把node($7$)從Linked List移除。

<center>
![cc][f9]

**圖五(a)。**
</center>

見圖五(b)：

* 目前`first`記錄的是node($3$)。
* 建立`ListNode *current`記錄`first`，也就是node($3$)。
* 將`first`移動到node($14$)。
* 刪除`current`指向的node($3$)。

如此，便把node($3$)從Linked List移除。


<center>
![cc][f10]

**圖五(b)。**
</center>

見圖五(c)：

* 目前`first`記錄的是node($14$)。
* 建立`ListNode *current`記錄`first`，也就是node($14$)。
* 將`first`移動到`NULL`。
* 刪除`current`指向的node($14$)。

這樣便把Linked List的node刪除完畢。

<center>
![cc][f11]

**圖五(c)。**
</center>


程式範例如下：


```cpp
// C++ code
void LinkedList::Clear(){
    
    while (first != 0) {
        ListNode *current = first;
        first = first->next;
        delete current;
        current = 0;
    }
}
```


</br>

<a name="test"></a>

##測試

在`main()`測試前面所介紹的各個函式。

```cpp
//C++ code
int main() {

    LinkedList list;     // 建立LinkedList的object
    list.PrintList();    // 目前list是空的
    list.Delete(4);      // list是空的, 沒有4
    list.Push_back(5);   // list: 5
    list.Push_back(3);   // list: 5 3
    list.Push_front(9);  // list: 9 5 3
    list.PrintList();    // 印出: 9 5 3
    list.Delete(5);      // list: 9 3
    list.Push_back(4);   // list: 9 3 4
    list.Push_front(8);  // list: 8 9 3 4
    list.PrintList();    // 印出: 8 9 3 4
    list.Clear();        // 清空list
    list.PrintList();    // 印出: List is empty.

    return 0;
}
```
output:

```cpp
List is empty.
There is no 4 in list.
9 5 3 
8 9 3 4 
List is empty.
```

[f0]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f0.png?raw=true
[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/LinkedList/Insert_Delete/f11.png?raw=true



</br>  

以上是在**Linked List**中新增資料、刪除資料的方法介紹。  

程式的實作方式根據`class LinkedList`的建立方式會有所不同，不過使用**pointer**的邏輯應該是大同小異的。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch10](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch4](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [小殘的程式光廊：連結串列(Linked List)](http://emn178.pixnet.net/blog/post/93557502-%E9%80%A3%E7%B5%90%E4%B8%B2%E5%88%97%28linked-list%29)



<a name="series"></a>

</br>

###Linked List系列文章

[Linked List: Intro(簡介)](http://alrightchiu.github.io/SecondRound/linked-list-introjian-jie.html)  
[Linked List: 新增資料、刪除資料](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao.html)  


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


