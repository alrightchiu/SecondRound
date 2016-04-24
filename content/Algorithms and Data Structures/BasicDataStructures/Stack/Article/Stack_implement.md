Title: Stack: 以Array與Linked list實作  
Date: 2016-4-24 10:28   
Category: 演算法與資料結構  
Tags: C++, Intro, Stack, Linked list, Array     
Summary: 以Array與Linked list實作Stack。


</br>
###先備知識與注意事項

本篇文章接續[Stack: Intro(簡介)](http://alrightchiu.github.io/SecondRound/stack-introjian-jie.html)，介紹以Array與Linked list實作Stack的方法。

如果對Linked list不太熟悉，建議讀者可以先閱讀以下連結之內容做簡單複習：

* [Linked List: Intro(簡介)](http://alrightchiu.github.io/SecondRound/linked-list-introjian-jie.html)
* [Linked List: 新增資料、刪除資料、反轉](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao-fan-zhuan.html)



***

##目錄


* [以Array實作Stack](#array)
* [以Linked list實作Stack](#ll)
* [參考資料](#ref)
* [Stack系列文章](#series)



<a name="array"></a>

##以Array實作Stack


以下是以Array(陣列)實作Stack的程式範例，`class StackArray`的`private data`有三項：

* `int top`：記錄於stack中，最上面資料的index。
* `int capacity`：即為Array的size，也就是實際配置的記憶體大小。
* `int *stack`：這裡以`int`作為資料的形別(type)做示範，所以就以`int`的Array來表示Stack。

以及Stack的基本處理資料的函式：`Push()`、`Pop()`、`IsEmpty()`、`Top()`、`getSize()`。  

比較需要注意的有：

`DoubleCapacity()`：因為利用Array來存放資料，所以有可能在不斷新增資料(`Push()`)時，碰上一開始分配給Array的記憶體空間(`capacity`)不夠的情況，可以透過重新配置一個`capacity`為兩倍大的Array來解決。

* 更理想的做法是，能夠先對欲放進Stack處理的資料數量有個底，在初始化時，就把`capacity`設定在差不多的範圍，如此一來，不需要重複多次`DoubleCapacity()`，也不會過分浪費記憶體空間。

`Pop()`：最常見的做法，是「想像地」把資料從Array中移除，意思是，當呼叫`Pop()`時，其實只是把`int top`減一，並沒有真的把資料從Array中移除，這麼做的原因是，等到下次`Push()`新增資料時，自然會把該記憶體位置覆寫(overwrite)，見圖二(a)：

* 當`Pop()`時，並沒有把$7$刪除，只是把`top`移回到$3$，讓Stack看起來已經把$7$移除，此時「最上面」資料是$3$。
* 也有些寫法比較謹慎，會把原先是$7$的記憶體位置，以$0$覆寫，見圖二(b)。
    * 參考：[小殘的程式光廊：堆疊(Stack)](http://emn178.pixnet.net/blog/post/93426076-%E5%A0%86%E7%96%8A%28stack%29)
* 還有ㄧ些寫法會真的把$7$移除，此時就需要呼叫`class`的`destructor`。但是這種寫法並不推薦，原因是「向系統要記憶體」需要成本，如果接下來Stack還會新增資料，那麼只要確實更新`top`使得Stack「看起來」有`Pop()`就好。
    * 參考：[Stack Overflow：Freeing last element of a dynamic array](http://stackoverflow.com/questions/13215645/freeing-last-element-of-a-dynamic-array)

<center>
![cc][f2]

**圖二(a)。**
</center>

<center>
![cc][f3]

**圖二(b)。**
</center>


完整程式範例如下：


```cpp
// C++ code
#include <iostream>

class StackArray{
private:
    int top;                 // index of top element
    int capacity;            // allocated memory space of array
    int *stack;              // array representing stack
    void DoubleCapacity();   // double the capacity of stack
public:
    StackArray():top(-1),capacity(1){    // constructor
        stack = new int[capacity];       // initial state: top=-1, capacity=1
    }
    void Push(int x);
    void Pop();
    bool IsEmpty();
    int Top();
    int getSize();
};
void StackArray::DoubleCapacity(){
    
    capacity *= 2;                            // double capacity
    int *newStack = new int[capacity];        // create newStack
    
    for (int i = 0 ; i < capacity/2; i++) {   // copy element to newStack
        newStack[i] = stack[i];
    }
    
    delete [] stack;              // release the memory of stack
    stack = newStack;             // redirect stack to newStack
}

void StackArray::Push(int x){
    
    if (top == capacity - 1) {    // if stack is full, double the capacity
        DoubleCapacity();
    }
    stack[++top] = x;             // update top and put x into stack
}

void StackArray::Pop(){
    
    if (IsEmpty()) {          // if stack is empty, there is nothing to pop
        std::cout << "Stack is empty.\n";
        return;
    }
    top--;                    // update top
//    stack[top] = 0;         // (*1)
//    stack[top].~T();        // (*2)
}

bool StackArray::IsEmpty(){
    
    //    if (top == -1) {
    //        return true;
    //    }
    //    else {
    //        return false;
    //    }
    return (top == -1);
}

int StackArray::Top(){
    
    if (IsEmpty()) {     // check if stack is empty
        std::cout << "Stack is empty.\n";
        return -1;
    }
    return stack[top];   // return the top element
}

int StackArray::getSize(){
    
    return top+1;        // return the number of elements in stack
}

int main(){
    
    StackArray s;
    s.Pop();
    
    s.Push(14);
    s.Push(9);
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    s.Push(7);
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    s.Pop();
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    
    return 0;
}
```
output:

```cpp
Stack is empty.

top: 9
size: 2

top: 7
size: 3

top: 14
size: 1

top: Stack is empty.
-1
size: 0
```

`main()`測試的結果與圖二(c)相同：

<center>
![cc][f4]

**圖二(c)。**
</center>


<a name="ll"></a>

##以Linked list實作Stack

Stack也可以用Linked list實現，以圖三(a)的Linked list為例，原先用來記錄第一個node的指標`first`，在Stack的實作上，就是用來記錄Stack「最上面」資料的指標`top`。

<center>
![cc][f6]

**圖三(a)。**
</center>

這裡延續[Linked List: 新增資料、刪除資料、反轉](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao-fan-zhuan.html)對Linked list的實作方法，採用兩個`class`，一個是`class StackNode`，表示Linked list的node，一個是`class StackList`，其記錄Stack的「最上面node」，作為整個Stack的存取入口(如同Linked list是以「第一個node」作為存取入口)。

* 有些寫法是利用`struct`來建立node，雖然`struct StackNode`的資料成員(data member)一定是**public**，不過只要把`class StackList`的`StackNode *top`設成**private**，那麼在`main()`同樣無法更改每個node的資料。  
參考：[Code Review：C++ Stack using template](http://codereview.stackexchange.com/questions/102393/c-stack-using-template)


</br>  
特別注意`Push()`函式：

* 因為要保持`StackNode *top`一直在Linked list的第一個位置，所以Stack在`Push()`新增資料時，採用Linked list的`Push_front()`。
* 在程式範例中，有這麼一行`StackNode *newnode = new StackNode(x,top);`(目前是註解的狀態)是利用`class StackNode`的第三個**constructor**，直接把新增的node之`next pointer`指向`top`。功能等同於「先利用`class StackNode`的第二個**constructor**，再手動更新`newNode`的`next pointer`」。
    * 小結論：好好利用**constructor**會更有效率。



完整程式範例如下：

```cpp
// C++ code
#include <iostream>
class StackList;

class StackNode{
private:
    int data;
    StackNode *next;
public:
    StackNode():data(0){
        next = 0;
    }
    StackNode(int x):data(x){
        next = 0;
    }
    StackNode(int x, StackNode *nextNode):data(x),next(nextNode){};
    friend class StackList;
};

class StackList{
private:
    StackNode *top;     // remember the address of top element 
    int size;           // number of elements in Stack
public:                 
    StackList():size(0),top(0){};
    void Push(int x);
    void Pop();
    bool IsEmpty();
    int Top();
    int getSize();
};

void StackList::Push(int x){
    
    if (IsEmpty()) {
        top = new StackNode(x);
        size++;
        return;
    }
    
    StackNode *newnode = new StackNode(x);  // Push_front() in Linked list
    newnode->next = top;                    
//    StackNode *newnode = new StackNode(x,top);
    top = newnode;
    size++;
}

void StackList::Pop(){
    
    if (IsEmpty()) {
        std::cout << "Stack is empty.\n";
        return;
    }
    
    StackNode *deletenode = top;
    top = top->next;
    delete deletenode;
    deletenode = 0;
    size--;
}

bool StackList::IsEmpty(){
    
    return (size == 0);     // if size==0, return true
}

int StackList::Top(){
    
    if (IsEmpty()) {
        std::cout << "Stack is empty.\n";
        return -1;
    }
    return top->data;
}

int StackList::getSize(){
    
    return size;
}

int main(){
    
    StackList s;
    s.Pop();
    s.Push(32);
    s.Push(4);
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    s.Push(15);
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    s.Pop();
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nsize: " << s.getSize() << std::endl;
    
    return 0;
}
```
output:

```cpp
Stack is empty.

top: 4
size: 2

top: 15
size: 3

top: 32
size: 1

top: Stack is empty.
-1
size: 0
```

`main()`測試的結果與圖三(b)相同：

<center>
![cc][f5]

**圖三(b)。**
</center>




[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/implement/f2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/implement/f3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/implement/f4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/implement/f5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/implement/f6.png?raw=true




</br>  

以上便是分別以Array和Linked list實作Stack的方法。  
兩者的優劣，可以參考[Linked List: Intro(簡介)](http://alrightchiu.github.io/SecondRound/linked-list-introjian-jie.html#comp)中Array與Linked list的比較。

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch10](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch3](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [小殘的程式光廊：堆疊(Stack)](http://emn178.pixnet.net/blog/post/93426076-%E5%A0%86%E7%96%8A%28stack%29)
* [Stack Overflow：Freeing last element of a dynamic array](http://stackoverflow.com/questions/13215645/freeing-last-element-of-a-dynamic-array)
* [Code Review：C++ Stack using template](http://codereview.stackexchange.com/questions/102393/c-stack-using-template)
* [Linked List: Intro(簡介)](http://alrightchiu.github.io/SecondRound/linked-list-introjian-jie.html)
* [Linked List: 新增資料、刪除資料、反轉](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao-fan-zhuan.html)




<a name="series"></a>

</br>

###Stack系列文章

[Stack: Intro(簡介)](http://alrightchiu.github.io/SecondRound/stack-introjian-jie.html)    
[Stack: 以Array與Linked list實作](http://alrightchiu.github.io/SecondRound/stack-yi-arrayyu-linked-listshi-zuo.html)  





回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


