Title: Queue: Intro(簡介)    
Date: 2016-4-227 18:01   
Category: 演算法與資料結構  
Tags: C++, Intro, Queue, Linked List   
Summary: 介紹Queue(佇列)，並以Linked list實作。


</br>
###先備知識與注意事項



**Queue(佇列)**是一種概念性的抽象資料結構，可以分別使用Linked list(連結串列)與Array(陣列)來實作。

本篇文章將介紹Queue的基本概念，並以Linked list實作。


***

##目錄

* [簡介：Queue](#intro)
* [Queue的應用](#app)
* [以Linked list實作](#ll)
* [參考資料](#ref)
* [Queue系列文章](#series)



</br>

<a name="intro"></a>

##簡介：Queue

**Queue**是具有「First-In-First-Out」的資料結構，如同排隊買車票的隊伍即可視為**Queue**，先進入隊伍的人，可以優先離開隊伍，走向售票窗口買票，而後到的人，就需要等隊伍前面的人都買完票後才能買。  

如同普遍認知的排隊隊伍，Queue也具有以下特徵：

1. 隊伍有前方(以**front**表示)以及後方(以**back**表示)之分。
2. 若要進入隊伍(**Push**)，一定是從**back**進入。
3. 若要離開隊伍(**Pop**)，一定是從**front**離開。

以圖一為例，由**front**(隊伍前方)和**back**(隊伍後方)可以判斷，進入隊伍的順序應該是$23、1、3、35$。


<center>
![cc][f1]

**圖一。**
</center>


一般的**Queue**，會有以下幾個處理資料結構的功能，配合圖二：

* **Push(data)**：把資料從Queue的「後面」放進Queue，並更新成新的**back**。
    * 在Queue中新增資料又稱為**enqueue**。
* **Pop**：把**front**所指向的資料從Queue中移除，並更新**front**。
    * 從Queue刪除資料又稱為**dequeue**。
* **getFront**：回傳**front**所指向的資料。
* **getBack**：回傳**back**所指向的資料。
* **IsEmpty**：確認Queue裡是否有資料。
* **getSize**：回傳Queue裡的資料個數。



<center>
![cc][queue]

**圖二。**
</center>




</br>

<a name="app"></a>

##Queue的應用

因為**Queue**的「First-In-First-Out」特徵，常用於先到先執行、需要排程(**scheduling**)的應用：



* 演算法：[Breadth-First Search(廣度優先搜尋)](http://alrightchiu.github.io/SecondRound/graph-breadth-first-searchbfsguang-du-you-xian-sou-xun.html)與[Tree的Level-Order Traversal](http://alrightchiu.github.io/SecondRound/binary-tree-traversalxun-fang.html#level)會用到**Queue**。
* 作業系統：被多個程式共享的資源(例如CPU、印表機、網站伺服器)，一次只能執行一個需求(例如request、interrupt)，因此需要有個**Queue**來安排多個程式的執行順序(例如device queue、job queue)，請參考：
    * [Tutorialspoint：Operating System - Process Scheduling](http://www.tutorialspoint.com/operating_system/os_process_scheduling.htm)。
    * [Stack Overflow：What are practical applications of Queues?](http://stackoverflow.com/questions/2392824/what-are-practical-applications-of-queues)。
    * [Stack Exchange：Which queue does the long-term scheduler maintain?](http://cs.stackexchange.com/questions/1106/which-queue-does-the-long-term-scheduler-maintain)



</br>

<a name="ll"></a>

##以Linked list實作

以Linked list實作Queue非常直覺，如圖三，把每筆資料視為node，並且以**pointer**前後連結：

* Queue的`Push()`：在list的「尾巴」新增資料。
* Queue的`Pop()`：在list的「開頭」刪除資料。

<center>
![cc][f2]

**圖三。**
</center>

和[Linked List: 新增資料、刪除資料、反轉](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao-fan-zhuan.html)介紹的Linked list的差異在於，因為Queue需要記得**front**和**back**的資料，所以Linked list除了原先記錄「第一個node」的**pointer**之外，要再多一個**pointer**記錄「最後一個node」。

* 有了**back pointer**後，便能在時間複雜度O($1$)完成「在Linked list尾巴新增資料」。




完整的程式範例如下：


```cpp
// C++ code
#include <iostream>

struct QueueNode{
    int data;
    QueueNode *next;
    QueueNode():data(0),next(0){};
    QueueNode(int x):data(x),next(0){};
};

class QueueList{
private:
    QueueNode *front;
    QueueNode *back;
    int size;
public:
    QueueList():front(0),back(0),size(0){};
    void Push(int x);
    void Pop();
    bool IsEmpty();
    int getFront();
    int getBack();
    int getSize();
};

void QueueList::Push(int x){
    
    if (IsEmpty()) {
        front = new QueueNode(x);
        back = front;
        size++;
        return;
    }
    
    QueueNode *newNode = new QueueNode(x);
    back->next = newNode;
    back = newNode;         // update back pointer
    size++;
}

void QueueList::Pop(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return;
    }
    
    QueueNode *deletenode = front;
    front = front->next;    // update front pointer
    delete deletenode;
    deletenode = 0;
    size--;
}

int QueueList::getFront(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return -1;
    }
    
    return front->data;
}

int QueueList::getBack(){
    
    if (IsEmpty()) {
        std::cout << "Queue is empty.\n";
        return -1;
    }
    
    return back->data;
}

bool QueueList::IsEmpty(){
    
//    return (size == 0);
    return ((front && back) == 0);
}

int QueueList::getSize(){
    
    return size;
}

int main(){
    
    QueueList q;
    if (q.IsEmpty()) {
        std::cout << "Queue is empty.\n";
    }
    q.Push(24);
    std::cout<< "\nAfter push 24: \n";
    std::cout << "front: " << q.getFront() << "    " << "back: " << q.getBack() << "\n";
    q.Push(8);
    std::cout<< "\nAfter push 8: \n";
    std::cout << "front: " << q.getFront() << "    " << "back: " << q.getBack() << "\n";
    q.Push(23);
    std::cout<< "\nAfter push 23: \n";
    std::cout << "front: " << q.getFront() << "    " << "back: " << q.getBack() << "\n";
    q.Push(13);
    std::cout<< "\nAfter push 13: \n";
    std::cout << "front: " << q.getFront() << "    " << "back: " << q.getBack() << "\n";
    q.Pop();
    std::cout<< "\nAfter pop the front element: \n";
    q.Push(35);
    std::cout<< "\nAfter push 13: \n";
    std::cout << "front: " << q.getFront() << "     " << "back: " << q.getBack() << "\n";
    q.Pop();
    std::cout<< "\nAfter pop the front element: \n";
    std::cout << "front: " << q.getFront() << "    " << "back: " << q.getBack() << "\n";
    q.Pop();
    std::cout<< "\nAfter pop the front element: \n";
    std::cout << "front: " << q.getFront() << "    " << "back: " << q.getBack() << "\n";
    q.Pop();
    std::cout<< "\nAfter pop the front element: \n";
    std::cout << "front: " << q.getFront() << "    " << "back: " << q.getBack() << "\n";
    q.Pop();
    std::cout<< "\nAfter pop the front element: \n";
    q.Pop();
    
    return 0;
}
```
output:

```cpp
Queue is empty.

After push 24: 
front: 24    back: 24

After push 8: 
front: 24    back: 8

After push 23: 
front: 24    back: 23

After push 13: 
front: 24    back: 13

After pop the front element: 

After push 13: 
front: 8     back: 35

After pop the front element: 
front: 23    back: 35

After pop the front element: 
front: 13    back: 35

After pop the front element: 
front: 35    back: 35

After pop the front element: 
Queue is empty.
```








[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/intro/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/intro/f2.png?raw=true
[queue]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Queue/intro/queue.gif?raw=true



</br>  

以上是**Queue**的基本介紹。  



</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch10](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch3](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Tutorialspoint：Operating System - Process Scheduling](http://www.tutorialspoint.com/operating_system/os_process_scheduling.htm)。
* [Stack Overflow：What are practical applications of Queues?](http://stackoverflow.com/questions/2392824/what-are-practical-applications-of-queues)。
* [Stack Exchange：Which queue does the long-term scheduler maintain?](http://cs.stackexchange.com/questions/1106/which-queue-does-the-long-term-scheduler-maintain)
* [Quora：What are the applications of queues and stacks in C++?](https://www.quora.com/What-are-the-applications-of-queues-and-stacks-in-C++)





<a name="series"></a>

</br>

###Queue系列文章

[Queue: Intro(簡介)]()  
 



回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


