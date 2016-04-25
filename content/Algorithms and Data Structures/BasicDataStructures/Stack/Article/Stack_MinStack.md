Title: Stack: 能夠在O(1)取得最小值的MinStack  
Date: 2016-4-25 12:08   
Category: 演算法與資料結構  
Tags: C++, Intro, Stack,     
Summary: 實作出能夠在O(1)取得最小值的Stack。


</br>
###先備知識與注意事項

眾所皆知，若手上處理的資料結構是Stack，除了「最上面」的資料可以輕易讀取(利用`top()`)，若要知道Stack中的其餘資料，就只能透過從Stack的最上方把資料一個一個`pop()`後，才能檢視，時間複雜度為O($N$)。

如果想要在O($1$)的時間複雜度取得Stack中的「最小值」資料，該如何設計呢？

<center>
![cc][f11]

**圖一。**
</center>

以圖一為例，因為圖一是Stack的剖面圖，所以很容易可以看出，目前Stack中的最小值為$4$。但是一般的Stack並沒有提供剖面圖這種東西，只能透過函式`top()`來得知Stack「最上面」的$9$。

本篇文章要介紹的便是能夠在O($1$)的時間複雜度，得知目前Stack裡的「最小值」的進化版Stack，以下稱為**MinStack**。

在程式範例中將使用標準模板函式庫(STL)的`std::stack<int>`，其基本函式功能可以參考[Stack: Intro(簡介)](http://alrightchiu.github.io/SecondRound/stack-introjian-jie.html)，更多內容請參考：[Cplusplus：std::stack](http://www.cplusplus.com/reference/stack/stack/)。

***

##目錄

* [以兩個Stack來實作MinStack](#minstack)
* [程式碼](#code)
* [參考資料](#ref)
* [Stack系列文章](#series)



</br>

<a name="minstack"></a>

##以兩個Stack來實作MinStack

要實作出**MinStack**的關鍵就是在一個`class MinStack`中使用「兩個Stack」，其中一個Stack用來存放資料(稱為`datastack`)，另一個用來記錄「目前最小值」(稱為`minstack`)。

在`Push()`新增資料時：

* `datastack`一如往常以`push()`將資料放進Stack中。
* `minstack`就必須判斷「欲新增的資料」是否有比「目前最上面」還要小。
    * 如果有，就把「欲新增的資料」`push()`進`minstack`，此時該資料會位在Stack的「最上面」。  
        * 特例：如果原先`minstack`裏面沒有資料，那麼就直接將「欲新增的資料」`push()`進`minstack`。
        * 透過這個步驟，可以保證`minstack`的「最上面」資料就是`datastack`中的「最小值」。
    * 如果沒有，就把`minstack`「目前最上面」的資料，再`push()`進`minstack`一次，表示在新增資料後，「最小值」不變。

</br>  
以圖二(a)為例，先後進行了「新增$6$」與「新增$13$」：

* 在新增$6$時：
    * 以`push(6)`將$6$放進`datastack`。
    * 因為`minstack`目前是空的，便同樣以`push(6)`將$6$放進`minstack`。
    * 從`minstack`的「最上面」資料可以得知，目前`datastack`中的「最小值」是$6$。
* 在新增$13$時：
    * 以`push(13)`將$13$放進`datastack`。
    * 判斷出$13$與目前`minstack`的「最上面」資料何者較小，並將較小者`push()`進`minstack`。
    * 同樣地，透過`minsatck`的「最上面」資料，可以得知目前`datastack`中的「最小值」仍然是$6$。


<center>
![cc][f7]

**圖二(a)。**
</center>

接著又新增了$4$、$9$、$1$，觀察圖二(b)，`minstack`的「最上面」資料，一定記錄著`datastack`的「最小值」。

<center>
![cc][f8]

**圖二(b)。**
</center>


</br>   
由於`Push()`新增資料時對`minstack`的處理，在`Pop()`刪除資料時，`datastack`和`minstack`只要同步進行`pop()`即可，觀察圖三，每一次之後，`minstack`的「最上面」資料仍然是`datastack`的「最小值」。


<center>
![cc][f9]

![cc][f10]

**圖三。**
</center>


</br>

<a name="code"></a>

##程式碼


完整程式範例如下：


```cpp
// C++ code
#include <iostream>
#include <stack>        // std::stack<>

class MinStack{
private:
    std::stack<int> data;
    std::stack<int> minstack;
public:
    MinStack(){};
    void Push(int x);
    void Pop();
    bool IsEmpty();
    int Top();
    int getSize();
    int getMin();
};
void MinStack::Push(int x){
    
    data.push(x);
    if (minstack.empty() || x < minstack.top()) {
        minstack.push(x);
    }
    else {
        minstack.push(minstack.top());
    }
}

void MinStack::Pop(){
    
    if (data.empty()) {
        std::cout << "Stack is empty.\n";
        return;
    }
    data.pop();
    minstack.pop();
}

int MinStack::getMin(){
    
    if (data.empty()) {
        std::cout << "Stack is empty.\n";
        return -1;
    }
    
    return minstack.top();
}

bool MinStack::IsEmpty(){
    
    return data.empty();
}

int MinStack::Top(){
    
    if (data.empty()) {
        std::cout << "Stack is empty.\n";   // sorry for the bad exception handling
        return -1;                          
    }
    
    return data.top();
}

int MinStack::getSize(){
    
    if (data.empty()) {
        std::cout << "Stack is empty.\n";
        return -1;
    }
    
    return (int)data.size();
}

int main(){
    
    MinStack s;
    s.Pop();
    s.Push(6);
    std::cout << "\ntop: " << s.Top() << "\nmin: " << s.getMin() << std::endl;
              
    s.Push(13);
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Push(4);
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Push(9);
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Push(1);
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    s.Pop();
    std::cout << "\ntop: " << s.Top()
              << "\nmin: " << s.getMin() << std::endl;
    
    return 0;
}
```
output:

```cpp
Stack is empty.

top: 6
min: 6

top: 13
min: 6

top: 4
min: 4

top: 9
min: 4

top: 1
min: 1

top: 9
min: 4

top: 4
min: 4

top: 13
min: 6

top: 6
min: 6

top: Stack is empty.
-1
min: Stack is empty.
-1
```

`main()`測試的結果與圖二、圖三相同：




[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/MinStack/f7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/MinStack/f8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/MinStack/f9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/MinStack/f10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/BasicDataStructures/Stack/MinStack/f11.png?raw=true



</br>  

以上便是分別以兩個`std::stack<>`實作**MinStack**的方法。  

更多相關的討論，包括更節省記憶體空間的做法，請參考：[Stack Overflow：design a stack such that getMinimum( ) should be O(1)](http://stackoverflow.com/questions/685060/design-a-stack-such-that-getminimum-should-be-o1)。

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch10](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch3](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Stack Overflow：design a stack such that getMinimum( ) should be O(1)](http://stackoverflow.com/questions/685060/design-a-stack-such-that-getminimum-should-be-o1)
* [Cplusplus：std::stack](http://www.cplusplus.com/reference/stack/stack/)






<a name="series"></a>

</br>

###Stack系列文章

[Stack: Intro(簡介)](http://alrightchiu.github.io/SecondRound/stack-introjian-jie.html)    
[Stack: 以Array與Linked list實作](http://alrightchiu.github.io/SecondRound/stack-yi-arrayyu-linked-listshi-zuo.html)  
[Stack: 能夠在O(1)取得最小值的MinStack](http://alrightchiu.github.io/SecondRound/stack-neng-gou-zai-o1qu-de-zui-xiao-zhi-de-minstack.html)  




回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


