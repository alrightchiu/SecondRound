Title: Hash Table：Chaining(簡介)  
Date: 2016-5-25 23:01 
Category: 演算法與資料結構  
Tags: C++, Intro, Dictionary, Hash Table,  
Summary: 介紹Chaining解決Collision。


</br>
###先備知識與注意事項

本篇文章將延續[Hash Table：Intro(簡介)](http://alrightchiu.github.io/SecondRound/hash-tableintrojian-jie.html)的議題，介紹**Chaining**來解決**Collision**。

其中將會用到Linked list的概念，若不熟悉請參考：[Linked List: Intro(簡介)](http://alrightchiu.github.io/SecondRound/linked-list-introjian-jie.html)。





***

##目錄

* [Chaining的概念](#chaining)
* [程式碼](#code)
    * [偷懶：使用STL](#stl)
    * [不偷懶：用Node串出Linked list](#ll)
* [參考資料](#ref)
* [Hash Table系列文章](#series)



</br>

<a name="chaining"></a>

##Chaining的概念

如果利用**Division Method**實作Hash Function：

$$
h(Key)=Key\bmod m
$$

若選擇$m=6$，那麼對於**Key**為$14,16,26,36,47,50,71,90$的item，進行**Hashing**後將會有如圖一的**Collision**發生。

解決的辦法，就是將被分配到同一個slot的item用**Linked list**串起來，這就是**Chaining**。

<center>
![cc][f1]

**圖一：。**
</center>

有了Linked list處理被分配到同ㄧ個slot的item，Hash Table的三項資料處理分別修正成：

**Insert**：

* 先利用Hash Function取得**Table**的**index**。
* 接著，只要在每一個slot的list之**front**加入item，即可保證在$O(1)$的時間複雜度完成。
    * 參考：[Linked list:push_front()](http://alrightchiu.github.io/SecondRound/linked-list-xin-zeng-zi-liao-shan-chu-zi-liao-fan-zhuan.html#front)

**Serach**：

* 先利用Hash Function取得**Table**的**index**。
* 再利用Linked list的**traversal**尋找item。

**Delete**：

* 先利用Hash Function取得**Table**的**index**。
* 再利用Linked list的**traversal**尋找欲刪除的item。

關於**Search**與**Delete**的時間複雜度：

**worst case**：$O(n)$，所有item都被很遜的Hash Function分配到同一個slot。

**average case**：$O(1+\alpha)$，其中$\alpha=\frac{n}{m}$稱為**load factor**，其物理意義為：

* 「資料數量($n$)」與「slot個數($m$)」的比例。
* 也會是list的**expected length**(list的平均長度)。

以圖二為例，若$m=3,n=6$，那麼$\alpha=n\m=2$，也就是「平均」每個list會被分配到$\alpha=2$個item，所以**Search**的時間複雜度就是list長度$O(\alpha)$，再加上Hash Function的時間複雜度$O(1)$，便得到$O(1+\alpha)$。

<center>
![cc][f2]

**圖二：以上四種皆為可能的情形，其中右下圖為平均的/期望值結果。**
</center>


若「資料數量($n$)」與「slot個數($m$)」的比例具有$n=O(m)$的關係，再加上一個不會把所有item分配到同一個slot的正常Hash Function，那麼可以想像，**Search**與**Delete**的時間可以在接近

$$
O(1+\alpha)=O(1+constant)=O(1)
$$

的情況下完成。詳細證明請參考：[Adnan Aziz：Hash Tables](http://users.ece.utexas.edu/~adnan/360C/hash.pdf)




</br>    

<a name="code"></a>

##程式碼


// 兩份, 一份用vector<list<pair<>>>, 一份用node+pointer




</br>    

<a name="stl"></a>

###偷懶：STL

// 介紹基本三個操作, 印output講解, 視情況作圖


```cpp
// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <string>

using std::vector;
using std::list;
using std::string;
using std::cout;
using std::endl;

struct dict{
    string key;                 // Name
    string value;               // Team
    dict():key(""),value(""){};
    dict(string Key, string Value):key(Key),value(Value){};
    
};

class HashChain_std{
private:
    int size, count;    // size是table大小, count記錄幾筆資料, count/size=load factor
    vector<list<dict> > table;
    
    int PreHashing(string key_str);                 // turn key_string to key_int
    int HashFunction(string key_str);               // Division method
    
    void TableDoubling(){};                     // load factor
    void TableShrinking(){};                    // load factor
    void Rehashing();                           // change size
public:
    HashChain_std():size(5),count(0){
        table.resize(size);
    }
    HashChain_std(int m):size(m),count(0){
        table.resize(size);
    }
    
    void Insert(dict data);         // consider TableDoubling()
    void Delete(string key);        // consider TableShrinking()
    string Search(string key);
    void DisplayTable();
};

string HashChain_std::Search(string key_str){
    
    int index = HashFunction(key_str);
    for (list<dict>::iterator itr = table[index].begin(); itr != table[index].end(); itr++) {
        if ((*itr).key == key_str) {
            return (*itr).value;
        }
    }
    return "...\nno such data";
}

void HashChain_std::Delete(string key_str){

    int index = HashFunction(key_str);
    for (list<dict>::iterator itr = table[index].begin(); itr != table[index].end(); itr++) {
        if ((*itr).key == key_str) {
            table[index].erase(itr);
        }
    }
}


void HashChain_std::Insert(dict data){

    int index = HashFunction(data.key);
    table[index].push_front(data);             // 在第index個slot, 串上data
}


int HashChain_std::PreHashing(string key_str){
    
    int exp = 9,
        key_int = 0,
        p = 1;
    
    for (int i = (int)key_str.size()-1; i >= 0 ; i--) {
        key_int += key_str[i]*p;
        p *= exp;
    }
    return key_int;
}

int HashChain_std::HashFunction(string key_str){
    
    int key_int = PreHashing(key_str);
    
    return ( key_int % this->size);
}

void HashChain_std::DisplayTable(){
    
    for (int i = 0; i < table.size(); i++) {
        cout << "slot#" << i+1 << ": ";
        for (list<dict>::iterator itr = table[i].begin(); itr != table[i].end(); itr++) {
            cout << "(" << (*itr).key << "," << (*itr).value << ") ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    
    HashChain_std hash(5);
    hash.Insert(dict("T-Mac","Magic"));
    hash.Insert(dict("Bryant","Lakers"));
    hash.Insert(dict("Webber","Kings"));
    hash.Insert(dict("Arenas", "Wizards"));
    hash.Insert(dict("Davis","Clippers"));
    hash.Insert(dict("Kidd","Nets"));
    hash.DisplayTable();
    
    cout << "T-Mac is in " << hash.Search("T-Mac") << ". " << endl;
    cout << "Arenas is in " << hash.Search("Arenas") << ". " << endl;
    
    cout << "\nAfter deleing Kidd and T-Mac:\n";
    hash.Delete("Kidd");
    hash.Delete("T-Mac");
    
    hash.DisplayTable();
    
    return 0;
}
```

output:

```cpp
slot#1: (Kidd,Nets) (Bryant,Lakers) 
slot#2: (Arenas,Wizards) 
slot#3: (Webber,Kings) 
slot#4: (T-Mac,Magic) 
slot#5: (Davis,Clippers) 

T-Mac is in Magic. 
Arenas is in Wizards. 

After deleing Kidd and T-Mac:
slot#1: (Bryant,Lakers) 
slot#2: (Arenas,Wizards) 
slot#3: (Webber,Kings) 
slot#4: 
slot#5: (Davis,Clippers) 
```



</br>    

<a name="ll"></a>

###不偷懶：實作Linked list


// 強調 Rehashing(), TableDoubling(), TableShrinking()


```cpp
// C++ code
#include <iostream>
#include <vector>
#include <string>
#include <math.h>       // floor()

using std::vector;
using std::string;
using std::cout;
using std::endl;

struct Node{
    int key;                    // number
    string value;               // genre
    Node *next;
    Node():key(0),value(""),next(0){};
    Node(int Key, string Value):key(Key),value(Value),next(0){};
    Node(Node const &data):key(data.key),value(data.value),next(data.next){};
};

class HashChainNode{
private:
    int size, count;                // size: size of table, count: #data
    Node **table;
    
    int HashFunction(int key);      // Multiplication method
    void TableDoubling();           // load factor
    void TableShrinking();          // load factor
    void Rehashing(int size_orig);  // change size
    
public:
    HashChainNode(){};
    HashChainNode(int m):size(m),count(0){
        table = new Node *[size];
        for (int i = 0; i < size; i++) {    // initialization
            table[i] = 0;
        }
    }
    ~HashChainNode();
    
    void Insert(Node data);         // consider TableDoubling()
    void Delete(int key);           // consider TableShrinking()
    string Search(int key);
    void DisplayTable();
};

HashChainNode::~HashChainNode(){
    
    for (int i = 0; i < size; i++) {
        
        Node *current = table[i];
        while (current != NULL) {
            Node *previous = current;
            current = current->next;
            delete previous;
            previous = 0;
        }
    }
    delete [] table;
}

void HashChainNode::Insert(Node data){
    
    count++;
    if (count > size) {
        TableDoubling();
    }
    
    int index = HashFunction(data.key);
    
    Node *newNode = new Node(data);     // 吃data創造新node
    Node *current = table[index];           // 取得table的slot的head pointer
    
    // push_front()
    if (current == NULL) {
        table[index] = newNode;
    }
    else {
        Node *next = current->next;
        current->next = newNode;
        newNode->next = next;
    }
    
    // push_back()
//    while (current != NULL) {                   // assume key is unique
//        previous = current;
//        current = current->next;
//    }
//    
//    if ( previous == NULL) {              // 表示table[index]沒東西
//        table[index] = newNode;
//    }
//    else {
//        previous->next = newNode;
//    }
    
}

void HashChainNode::Delete(int key){
    
    int index = HashFunction(key);
    Node *current = table[index],
         *previous = NULL;
    
    while (current != NULL && current->key != key) {
        previous = current;
        current = current->next;
    }
    
    if (current == NULL) {
        cout << "data not found.\n\n";
        return;
    }
    else {
        if (previous == NULL) {
            table[index] = current->next;
        }
        else {
            previous->next = current->next;
        }
        delete current;
        current = 0;
    }
    
    count--;
    if (count < size/4) {
        TableShrinking();
    }
}

string HashChainNode::Search(int key){
    
    int index = HashFunction(key);
    Node *current = table[index];
    
    while (current != NULL) {
        if ( current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return "...\nno such data";
}

int HashChainNode::HashFunction(int key){
    // Multiplication method
    double A = 0.6180339887,
           frac = key*A-floor(key*A);
    
    return floor(size*frac);
}

void HashChainNode::TableDoubling(){
    
    int size_orig = size;
    size *= 2;
    Rehashing(size_orig);;
}

void HashChainNode::TableShrinking(){
    
    int size_orig = size;
    size /= 2;
    Rehashing(size_orig);
}

void HashChainNode::Rehashing(int size_orig){
    
    Node **newtable = new Node *[size];
    for (int i = 0; i < size; i++) {        // 這三行超關鍵！因為 新new 出來的 newtable, 每個node可能不是NULL
        newtable[i] = 0;
    }
    
    for (int i = 0; i < size_orig; i++) {
        Node *curr_orig = table[i], *previous = NULL;
        while (curr_orig != NULL) {
            
            previous = curr_orig->next;  // 因為curr_orig會被移到newtable上, 所以要previous記住原先table
            
            int index = HashFunction(curr_orig->key);
            
            // push_front(), 把原來的table中的node接到newtable上, 不需要配置新的記憶體
            if (newtable[index] == NULL) {
                newtable[index] = curr_orig;
                newtable[index]->next = 0;      // equivalent to curr_orig->next = 0;
            }
            else {                // 如果沒有對 newtable 的元素做initialization, 可能跑進來這個 else{}, 會爆炸
                Node *next = newtable[index]->next;
                newtable[index]->next = curr_orig;
                curr_orig->next = next;
            }
            curr_orig = previous;
        }
    }
    delete [] table;
    table = newtable;
}

void HashChainNode::DisplayTable(){
    
    for (int i = 0; i < size; i++) {
        cout << "#slot#" << i << ": ";
        Node *current = table[i];
        while (current != NULL) {
            cout << "(" << current->key << "," << current->value << ") ";
            current = current->next;
        }
        cout << endl;
    }
    cout << endl;
}

int main(){
    
    HashChainNode hash(2);

    hash.Insert(Node(12,"post rock"));
    hash.Insert(Node(592,"shoegaze"));
    hash.Insert(Node(6594,"blues"));        // evoke TableDoubling()
    hash.Insert(Node(49593,"folk"));
    cout << "After inserting key(12),key(592),key(6594),key(49593):\n";
    hash.DisplayTable();
    hash.Insert(Node(123596,"hiphop"));     // evoke TableDoubling()
    cout << "After inserting key(123596):\n";
    hash.DisplayTable();
    hash.Insert(Node(93,"soul"));
    hash.Insert(Node(2288,"indie"));
    hash.Insert(Node(793,"jazz"));
    cout << "After inserting key(93),key(2288),key(793):\n";
    hash.DisplayTable();
    hash.Insert(Node(8491,"electro"));      // evoke TableDoubling()
    cout << "After inserting key(8491):\n";
    hash.DisplayTable();
    hash.Insert(Node(323359,"pop"));
    cout << "After inserting key(323359):\n";
    hash.DisplayTable();
    
    cout << "Searching: genre(8491) is " << hash.Search(8491) << ".\n\n";
    
    hash.Delete(49593);
    cout << "After deleting key(49593):\n";
    cout << "Searching: genre(49593) is " << hash.Search(49593) << ".\n\n";
    
    hash.Delete(592);
    cout << "After deleting key(592):\n";
    hash.DisplayTable();
    
    cout << "Want to  delete key(592) again:\n";
    hash.Delete(592);
    
    hash.Delete(123596);
    hash.Delete(323359);
    hash.Delete(793);
    hash.Delete(93);
    cout << "After deleting key(123596),key(323359),key(793),key(93):\n";
    hash.DisplayTable();
    
    hash.Delete(6594);      // evoke TableShrinking()
    cout << "After deleting key(6594):\n";
    hash.DisplayTable();
    
    return 0;
}
```

output:

```cpp
After inserting key(12),key(592),key(6594),key(49593):
#slot#0: (49593,folk) 
#slot#1: (12,post rock) (6594,blues) 
#slot#2: 
#slot#3: (592,shoegaze) 

After inserting key(123596):
#slot#0: 
#slot#1: (49593,folk) 
#slot#2: (6594,blues) 
#slot#3: (12,post rock) 
#slot#4: (123596,hiphop) 
#slot#5: 
#slot#6: 
#slot#7: (592,shoegaze) 

After inserting key(93),key(2288),key(793):
#slot#0: (2288,indie) (793,jazz) 
#slot#1: (49593,folk) 
#slot#2: (6594,blues) 
#slot#3: (12,post rock) (93,soul) 
#slot#4: (123596,hiphop) 
#slot#5: 
#slot#6: 
#slot#7: (592,shoegaze) 

After inserting key(8491):
#slot#0: (2288,indie) 
#slot#1: (793,jazz) 
#slot#2: (49593,folk) 
#slot#3: 
#slot#4: 
#slot#5: (6594,blues) 
#slot#6: (12,post rock) 
#slot#7: (93,soul) 
#slot#8: (123596,hiphop) 
#slot#9: 
#slot#10: 
#slot#11: (8491,electro) 
#slot#12: 
#slot#13: 
#slot#14: (592,shoegaze) 
#slot#15: 

After inserting key(323359):
#slot#0: (2288,indie) 
#slot#1: (793,jazz) 
#slot#2: (49593,folk) 
#slot#3: 
#slot#4: 
#slot#5: (6594,blues) 
#slot#6: (12,post rock) 
#slot#7: (93,soul) 
#slot#8: (123596,hiphop) 
#slot#9: 
#slot#10: 
#slot#11: (8491,electro) 
#slot#12: 
#slot#13: (323359,pop) 
#slot#14: (592,shoegaze) 
#slot#15: 

Searching: genre(8491) is electro.

After deleting key(49593):
Searching: genre(49593) is ...
no such data.

After deleting key(592):
#slot#0: (2288,indie) 
#slot#1: (793,jazz) 
#slot#2: 
#slot#3: 
#slot#4: 
#slot#5: (6594,blues) 
#slot#6: (12,post rock) 
#slot#7: (93,soul) 
#slot#8: (123596,hiphop) 
#slot#9: 
#slot#10: 
#slot#11: (8491,electro) 
#slot#12: 
#slot#13: (323359,pop) 
#slot#14: 
#slot#15: 

Want to  delete key(592) again:
data not found.

After deleting key(123596),key(323359),key(793),key(93):
#slot#0: (2288,indie) 
#slot#1: 
#slot#2: 
#slot#3: 
#slot#4: 
#slot#5: (6594,blues) 
#slot#6: (12,post rock) 
#slot#7: 
#slot#8: 
#slot#9: 
#slot#10: 
#slot#11: (8491,electro) 
#slot#12: 
#slot#13: 
#slot#14: 
#slot#15: 

After deleting key(6594):
#slot#0: (2288,indie) 
#slot#1: 
#slot#2: 
#slot#3: (12,post rock) 
#slot#4: 
#slot#5: (8491,electro) 
#slot#6: 
#slot#7: 
```





[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/HashTable%20series/Chaining/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/HashTable%20series/Chaining/f2.png?raw=true



</br>  


 
以上是以**Chaining**解決**Collision**之介紹。



</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch11](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch8](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Abdullah Ozturk：Simple Hash Map (Hash Table) Implementation in C++](https://medium.com/@aozturk/simple-hash-map-hash-table-implementation-in-c-931965904250#.du6lwge1u)
* [Pumpkin Programmer：C++ Tutorial: Intro to Hash Tables](http://pumpkinprogrammer.com/2014/06/21/c-tutorial-intro-to-hash-tables/)
* [Adnan Aziz：Hash Tables](http://users.ece.utexas.edu/~adnan/360C/hash.pdf)
* [Linked List: Intro(簡介)](http://alrightchiu.github.io/SecondRound/linked-list-introjian-jie.html)





<a name="series"></a>

</br>

###Hash Table系列文章

[Hash Table：Intro(簡介)](http://alrightchiu.github.io/SecondRound/hash-tableintrojian-jie.html)   


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


