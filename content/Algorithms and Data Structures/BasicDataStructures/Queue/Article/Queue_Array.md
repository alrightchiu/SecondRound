Title: Queue: 以Array實作Queue    
Date: 2016-4-29 21:42   
Category: 演算法與資料結構  
Tags: C++, Queue, Array   
Summary: 以Array實作Queue(佇列)之介紹。


</br>
###先備知識與注意事項

本篇文章將接續[Queue: Intro(簡介)，並以Linked list實作](http://alrightchiu.github.io/SecondRound/queue-introjian-jie-bing-yi-linked-listshi-zuo.html)，介紹以Array實作Queue的方法。

關於Queue對資料的基本處理方法如`Push()`、`Pop()`等等，請參考[Intro篇](http://alrightchiu.github.io/SecondRound/queue-introjian-jie-bing-yi-linked-listshi-zuo.html)的介紹。


***

##目錄

* [浪費記憶體空間的Array實作：Sequential Queue](#sequential)
* [節省記憶體空間的Array實作：Circular Queue](#circular)
* [參考資料](#ref)
* [Queue系列文章](#series)



</br>

<a name="sequential"></a>

##浪費記憶體空間的Array實作：Sequential Queue

以圖一為例，將Array的開頭(index為$0$)設為Queue的**front**，反方向為**back**。

* **Push**：若要新增資料，按照Array的index順序：$0、1、2...$，將Queue的**back**新增資料。
* **Pop**：若要刪除資料，按照Array的index順序：$0、1、2...$，從Queue的**front**刪除資料。
    * **Pop**並沒有真正把Array的記憶體位置釋放，只是調整**front**，使得Queue「看起來」有刪除資料。

<center>
![cc][SequentialQueue]

**圖一。**
</center>


以上介紹的方法會有記憶體空間浪費的問題。  
由於圖一的**Push**是不斷往Queue的**back**新增資料，因此Array的前兩個位置一旦被「想像地**Pop**」後，會一直閒置到整個Array的記憶體被釋放為止，無法有效利用。

下一小節將提出另一種以Array實作Queue的方法，將能夠有效利用圖二中Array的前兩個位置。

<center>
![cc][f60]

**圖二。**
</center>

另外，圖二的Array顯然已經填滿，若要在此Queue繼續增加資料，就需要重新配置一個容量更大的Array，方法如圖三，將資料複製到新的Array上，並且調整**front**與**back**。

<center>
![cc][f61]

**圖三。**
</center>

完整的程式範例如下：


```cpp
// C++ code

#include <iostream>
using std::cout;

class QueueArraySequential{
private:
    int capacity, front, back;
    int *queue;
    void DoubleCapacity();
public:
    QueueArraySequential():capacity(5),front(-1),back(-1){
        queue = new int[capacity];
    };
    void Push(int x);
    void Pop();
    bool IsEmpty();
    bool IsFull();
    int getFront();
    int getBack();
    int getSize();
    int getCapacity();    // 驗證用, 可有可無
};

void QueueArraySequential::DoubleCapacity(){
    
    capacity *= 2;
    int *newQueue = new int[capacity];
    
    int j = -1;
    for (int i = front+1; i <= back; i++) {
        j++;
        newQueue[j] = queue[i];
    }
    front = -1;       // 新的array從0開始, 把舊的array"整段平移", front跟back要更新
    back = j;
    delete [] queue;
    queue = newQueue;
}
void QueueArraySequential::Push(int x){
    
    if (IsFull()) {
        DoubleCapacity();
    }
    queue[++back] = x;
}
void QueueArraySequential::Pop(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return;
    }
    front++;        
}
bool QueueArraySequential::IsFull(){
    
    return (back + 1 == capacity);
}
bool QueueArraySequential::IsEmpty(){
    
    return (front  == back);
}
int QueueArraySequential::getFront(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return -1;
    }
    
    return queue[front+1];
}
int QueueArraySequential::getBack(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return -1;
    }
    
    return queue[back];
}
int QueueArraySequential::getSize(){
    
    return (back - front);
}
int QueueArraySequential::getCapacity(){
    
    return capacity;
}

int main(){

    QueueArraySequential q;
    if (q.IsEmpty()) {
        cout << "Queue is empty.\n";
    }
    q.Push(24);
    cout << "\nAfter push 24: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(8);
    q.Push(23);
    cout << "\nAfter push 8, 23: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Pop();
    cout << "\nAfter pop 24: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(13);
    cout << "\nAfter push 13: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Pop();
    cout << "\nAfter pop 8: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(35);
    cout << "\nAfter push 35: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(9);
    cout << "\nAfter push 9: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    
    return 0;
}
```

output:


```cpp
Queue is empty.

After push 24: 
front: 24    back: 24
capacity: 5  number of elements: 1

After push 8, 23: 
front: 24    back: 23
capacity: 5  number of elements: 3

After pop 24: 
front: 8    back: 23
capacity: 5  number of elements: 2

After push 13: 
front: 8    back: 13
capacity: 5  number of elements: 3

After pop 8: 
front: 23    back: 13
capacity: 5  number of elements: 2

After push 35: 
front: 23    back: 35
capacity: 5  number of elements: 3

After push 9: 
front: 23    back: 9
capacity: 10  number of elements: 4
```

在`main()`的測試中，最重要的是執行`Push(9)`的前後。

在`Push(9)`之前，Array的狀況如圖四(a)：

* 此時，Queue其實只有$3$筆資料，理論上，具有大小為$5$的Array應該要能夠再新增一筆資料。
* 但是因為上述的`Push()`與`Pop()`，Array的前兩個記憶體位置無法重複利用。

<center>
![cc][f60]

**圖四(a)。**
</center>

因此，若要`Push(9)`，就需要重新配置容量更大的Array，才能在Queue中新增$9$，見圖四(b)：

* 此時，Queue其實只有$4$筆資料，但是使用了`capacity`為$10$的Array。
    * 這裡的`capacity`更新成$10$是因為程式範例中`DoubleCapacity()`的寫法。
    * 根據不同「重新配置Array」的寫法，更新後的`capacity`大小可能會有所不同。


<center>
![cc][f62]

**圖四(b)。**
</center>

根據以上說明，第一種方法不只是記憶體空間的浪費，還有「重新配置Array」的運算成本浪費。

接下來便介紹第二種以Array實作Queue的方法，能夠較為有效地利用Array的記憶體位置。

</br>

<a name="circular"></a>

##節省記憶體空間的Array實作：Circular Queue

第二種方法做出來的Queue又稱為**Circular Queue**，環狀(circular)的意思就是能夠「繞回Array的前端」，重複利用Array的記憶體空間，見圖五：



<center>
![cc][CircularQueue]

**圖五。**
</center>


比較圖一的**Sequential Queue**與圖五的**Circular Queue**之差異：

* **Sequential Queue**：
    * **front**與**back**之初始值為$-1$。
    * 一但被**Pop**後的記憶體空間將閒置到Queue的物件(object)被釋放。

* **Circular Queue**：
    * **front**與**back**之初始值為$0$，因此會犧牲Array中的其中一個記憶體位置。
    * 被**Pop**後的記憶體空間將會重複使用，將會用上餘數(mod)來計算index。

完整的程式範例如下：


```cpp
// C++ code

#include <iostream>
using std::cout;

class QueueArrayCircular{
private:
    int capacity, front, back;
    int *queue;
    void DoubleCapacity();
public:
    QueueArrayCircular():capacity(5),front(0),back(0){     // 從0開始, 第一個位置放掉
        queue = new int[capacity];
    }
    void Push(int x);
    void Pop();
    bool IsEmpty();
    bool IsFull();
    int getFront();
    int getBack();
    int getSize();
    int getCapacity();    // 驗證用, 可有可無
};

void QueueArrayCircular::DoubleCapacity(){
    
    int *newQueue = new int[capacity*2];
    
    int j = front;
    for (int i = 1; i <= getSize(); i++) {
        newQueue[i] = queue[(j+1)%capacity];
    }
    capacity *= 2;
    back = getSize();
    front = 0;
    delete [] queue;
    queue = newQueue;
}
void QueueArrayCircular::Push(int x){
    
    if (IsFull()) {
        DoubleCapacity();
    }
    
    back = (back + 1)%capacity;
    queue[back] = x;
}
void QueueArrayCircular::Pop(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return;
    }
    
    front = (front + 1)%capacity;
}
bool QueueArrayCircular::IsEmpty(){
    
    return (front == back);
}
bool QueueArrayCircular::IsFull(){
    
    return ((back + 1)%capacity == front);
}
int QueueArrayCircular::getFront(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return -1;
    }
    return queue[(front + 1)%capacity];
}
int QueueArrayCircular::getBack(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return -1;
    }

    return queue[back];
}
int QueueArrayCircular::getSize(){
    
    int size;
    if (front < back) {
        size = back - front;
    }
    else {
        size = capacity - (front - back);
    }

    return size;
}
int QueueArrayCircular::getCapacity(){
    
    return capacity;
}

int main(){

    QueueArrayCircular q;
    if (q.IsEmpty()) {
        cout << "Queue is empty.\n";
    }
    q.Push(24);
    cout << "\nAfter push 24: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(8);
    q.Push(23);
    cout << "\nAfter push 8, 23: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Pop();
    cout << "\nAfter pop 24: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(13);
    cout << "\nAfter push 13: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Pop();
    cout << "\nAfter pop 8: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(35);
    cout << "\nAfter push 35: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(9);
    cout << "\nAfter push 9: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    q.Push(64);
    cout << "\nAfter push 10: \n"
         << "front: " << q.getFront() << "    back: " << q.getBack() << "\n"
         << "capacity: " << q.getCapacity()
         << "  number of elements: " << q.getSize() << "\n";
    return 0;
}

```
output:

```cpp
Queue is empty.

After push 24: 
front: 24    back: 24
capacity: 5  number of elements: 1

After push 8, 23: 
front: 24    back: 23
capacity: 5  number of elements: 3

After pop 24: 
front: 8    back: 23
capacity: 5  number of elements: 2

After push 13: 
front: 8    back: 13
capacity: 5  number of elements: 3

After pop 8: 
front: 23    back: 13
capacity: 5  number of elements: 2

After push 35: 
front: 23    back: 35
capacity: 5  number of elements: 3

After push 9: 
front: 23    back: 9
capacity: 5  number of elements: 4

After push 10: 
front: 8    back: 64
capacity: 10  number of elements: 5
```

在`main()`中的測試結果可以看出，**Circular Queue**確實有效地利用了Array的記憶體空間，與**Sequential Queue**比較，在`Push(9)`時，並沒有呼叫`DoubleCapacity()`，因為**Circular Queue**的`Push()`會將資料新增至Array的開頭，如圖六：

<center>
![cc][f80]

**圖六。**
</center>

而接續圖六的狀態後，再繼續`Push(10)`就會呼叫`DoubleCapacity()`，如圖七，要注意的是必須按照**front**到**back**的順序複製資料，並且更新**front**與**back**所代表的index：

<center>
![cc][f81]

**圖七。**
</center>



[SequentialQueue]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/Array/SequentialQueue.gif?raw=true
[CircularQueue]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/Array/CircularQueue.gif?raw=true
[f60]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/Array/f60.png?raw=true
[f61]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/Array/f61.png?raw=true
[f62]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/Array/f62.png?raw=true
[f80]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/Array/f80.png?raw=true
[f81]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/Array/f81.png?raw=true




</br>  

以上是以Array實作**Sequential Queue**與**Circular Queue**的介紹。  



</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch10](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch3](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)






<a name="series"></a>

</br>

###Queue系列文章

[Queue: Intro(簡介)，並以Linked list實作](http://alrightchiu.github.io/SecondRound/queue-introjian-jie-bing-yi-linked-listshi-zuo.html)   
[Queue: 以Array實作Queue](http://alrightchiu.github.io/SecondRound/queue-yi-arrayshi-zuo-queue.html)  



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


