Title: Linked List: Traversal 
Date: 2015-12-23 20:00 
Category: Algorithms and Data Structures  
Tags: 今天不寫明天就忘了  
Summary: 介紹Linked List中的Traversal(尋訪)。
 


</br>
######**先備知識與注意事項**

traversal(尋訪)有「站在A地，往所有與A地相連的地方移動」的意思：  

* 以Graph(圖)的語言來說，站在vertex A上，有一條edge連結A與B，若能夠由A往B移動，此即可視為traversal；
* 在以pointer實現之Linked list和Tree中，站在node A上，A具有指向B之pointer，因此能夠由A往B移動，此即可視為traversal。

移動到特定的node之後，通常伴隨著其他行為，例如print out(顯示資料)、assign(賦值)等等，這些行為又稱作Visiting，

這篇文章將介紹在Linked list中的traversal。  
</br>  

##**Traversal in Linked List**

若有一個Linked list如圖一：

</br> 
<center>
![linked list][ll]

**圖一：Linked list: A->B->C->D->NULL。**  
</center>   
[ll]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Linked%20List/LLtraversal.png?raw=true


現要求Visiting為print(顯示資料)，欲列印出Linked list中的所有字母，也就是從A開始，依序印出B、C、D，程式範例如下：

```cpp
// C++ code
#include <iostream>
#include <string>

class Node{
public:
    Node *next;			// pointer to next node
    std::string str;	// data
    
    Node(){};						// default constructor
    Node(std::string s):str(s){};	// constructor
};
void traversal(Node *CurrentNode){
    if (CurrentNode != NULL){
        std::cout << CurrentNode->str << " ";
        traversal(CurrentNode->next);
    }
}
int main() {
    // initialize nodes
    Node *A = new Node("A");
    Node *B = new Node("B");
    Node *C = new Node("C");
    Node *D = new Node("D");
    
    // construct linked list
    A->next = B;
    B->next = C;
    C->next = D;
    
    // representing the front of linked list by CurrentNode
    Node *CurrentNode = A;
    
    // traversal by iteration 
    while (CurrentNode != NULL){
        std::cout << CurrentNode->str << " ";
        CurrentNode = CurrentNode->next;
    }
    std::cout << std::endl;
    
    // traversal by recursion
    traversal(A);
    std::cout << std::endl;
    
    return 0;
}
```
output：

```
A B C D		// from while loop
A B C D		// from traversal()
```
**line5 - line12：**定義`class Node`，其中包含了指向下一個node的pointer，以`string`攜帶字母，以及`Node`的constructor。(為了在main()中能夠取用，全部寫成public member。)

**line13 - line18：**此為recursion(遞迴)形式的traversal，與**line42**一併討論。

**line21 - line24：**定義帶有字母A、B、C、D的node。

**line27 - line29：**示範如何使用最暴力的方式建立出如圖一的Linked list。(管理Linked list之過程時常是動態的(今天新增一筆資料，明天刪除兩筆資料)，因此建議將「新增node」寫成函式形式，例如：insert())。

**line32：** 定義`CurrentNode`表示「當前所位於的node」。在Linked list(及其衍生資料結構)的操作中，時常使用CurrentNode，目的是為了讓函式能夠有效地重複利用。  
舉例來說，若要用最暴力的方式依序印出A、B、C、D：

```cpp
// C++ code
std::cout << A->str << " " << A->next->str << " " << A->next->next->str << " " << A->next->next->next->str << std::endl;
```
output：  

```cpp
A B C D
```
然而，這樣的寫法除了[太hardcore](https://www.youtube.com/watch?v=Qdcfxk-e_iw)之外，還有幾點缺陷：

* 只有當資料量小，並且站在全知視角(已經知道Linked list中A、B、C、D之相對位置)時，才能土法煉鋼。假設Linked list中有一百個node，欲依序列出資料，又或者Node裡不只有定義pointer next，而是有pointer next1、next2、next3，就只好關閉視窗。
* 資料隱蔽問題：一般情況，在定義`class Node`時，不會把`Node *next`與`string str`裸露在外，而是寫成private member讓使用者無法任意修改。

隱蔽性問題這裡不多談，大致上的方向是使用member function存取private member。  
接著要處理的是程式的重複使用性。  

**line35 - line38：** traversal主體。  

分解步驟如下：

* **line35**：在**line32**定義之`CurrentNode`目前為node A，不是NULL，因此進入`While`迴圈。  
如圖二所示，在第一次迴圈中，可觸及的Node只有`CurrentNode`與`CurrentNode->next`，即為A與B。 
 

<center>
![linked list_1][traversal_1]  
**圖二：第一次迴圈，可觸及的node只有A與B。**
</center>   
[traversal_1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Linked%20List/LLtraversal1.png?raw=true
</br>
  
* **line36**：列印出`CurrentNode`所帶的字母。  
* **line37**：將`CurrentNode`移動至`CurrentNode`所指向的下一個node，亦即，從A移動到B。  
* **line38**：跳出迴圈後，若更新過後的`CurrentNode`仍不為NULL(在A之後的是B)，將進入第二次`while`迴圈，重複**line35 - line38**。  

依此類推，第二至第四次迴圈之示意圖如下：  
</br>
<center>
![linked list_2][traversal_2]  
**圖三：第二次迴圈，可觸及的node為B與C。**  

![linked list_3][traversal_3]  
**圖四：第三次迴圈，可觸及的node為C與D。**  

![linked list_4][traversal_4]  
**圖五：第四次迴圈，可觸及的node只有D。**
</center>   
[traversal_2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Linked%20List/LLtraversal2.png?raw=true 
[traversal_3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Linked%20List/LLtraversal3.png?raw=true 
[traversal_4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Linked%20List/LLtraversal4.png?raw=true
</br>

直到第四次迴圈之**line37**，`CurrentNode`更新為NULL，表示已經抵達Linked list的尾端，同時，不滿足`while`之條件式，便結束迴圈。

**line42：** 函式traversal()之主體(**line13 - line18**)與**line35 - line38**之`while`迴圈具有相同的作用。兩者皆能列印出Linked list中的所有資料。

</br>  

以上是在Linked list中，單向(single direction)的traversal之介紹，只要把握pointer對記憶體位置(memory address)的操作原則，應該能輕鬆掌握。

</br>

######**參考資料**：

* [Wikipedia：Linked List](https://en.wikipedia.org/wiki/Linked_list)
* [太hardcore了](https://www.youtube.com/watch?v=Qdcfxk-e_iw)
  
</br>  






