Title: Binary Tree: Construct Binary Tree from char array 
Date: 2015-12-27 16:53  
Category: 演算法與資料結構  
Tags: 今天不寫明天就忘了  
Summary: 介紹Binary Tree中的Traversal(尋訪)。
 
 
</br>
######**先備知識與注意事項**

在[Binary Tree：Traversal](http://alrightchiu.github.io/SecondRound/binary-tree-traversal.html#ex_code)中，非常沒誠意地用暴力方式建了一棵Binary Tree，在這裡至上深深歉意，因此，在本篇文章將提供一種方法，由一個字串陣列(char array)輸入字母，並按照[Complete Binary Tree](http://alrightchiu.github.io/SecondRound/binary-tree-intro.html#fullcomplete)之順序重新建立**那顆樹**。(一種不健康的雪恥心態)

其中，問題情境之原始資料是一個字串陣列(char array)，為了方便處理(偷懶)選擇使用C++語言中的神器：[stringstream](http://www3.ntu.edu.sg/home/ehchua/programming/cpp/cp10_io.html)，這裡礙於篇幅與主題(與筆者自己也還在摸索)，就不多談避免誤導，點進連結中有非常詳細的說明，關於`istringstream`、`ostringstream`、`stringstream`等等template class之繼承關係(inheritance)。  
重點是，`stringstream`就是神，就是行，學起來簡直光宗耀祖。

以及，以下提供的Binary Tree之建立方法，基本上是在[Binary Tree：Traversal介紹過的level-order traversal](http://alrightchiu.github.io/SecondRound/binary-tree-traversal.html#ex_code)上加油添醋，因此[queue(佇列)](https://en.wikipedia.org/wiki/Queue_%28abstract_data_type%29)的概念會再次出現。

</br>
  
  
##目錄
* [問題描述](#description)
* [程式碼](#code)  
 
    * [定義class TreeNode、class BinaryTree](#def) 
    * [Constructor of BinaryTree](#constructor) 
    * [Function：LevelorderConstruct()](#func1)
    * [Function：insertLevelorder()](#func2)

* [參考資料](#ref)
* 

<a name="description"></a>

##**問題描述**

問題描述如下：

* 給定一個字串陣列，欲按照Complete Binary Tree之位置規則建立一棵Binary Tree，若陣列元素之資料為大寫字母(ASCII：65~90)，則將其建立成Tree的node，若陣列元素為 ' x ' 則表示該位置沒有node。

以[Binary Tree：Traversal](http://alrightchiu.github.io/SecondRound/binary-tree-traversal.html#ex_code)中所提到的Binary Tree為例，如圖一：
</br>  
<center>
![binary tree][bt_0]

**圖一：。**  
</center>   


其所對應的字串陣列即為：`A B C D E F x x x G H x I`，如圖二所示：
  
<center>
![binary tree of char array][bt_char]

**圖二：。**  
</center> 


以下程式範例的目的就是要以如此文明的方式建立出如圖一的Binary Tree。

[bt_0]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/ex.png?raw=true
[bt_char]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/ex_char.png?raw=true

<a name="code"></a>

##**程式碼**

先看看`main()`中，上半部分別為

* 原始資料：字串陣列
* 以該字串陣列實體化(instantiate)一棵Binary Tree
* 以inorder traversal印出樹的資料

溫馨小提醒：純粹以inorder traversal之結果並無法驗證樹之結構正如圖一(舉例來說：以inorder traversal對某一Linked list也可能得出相同結果)，因此，建議還是使用IDE的debug功能把pointer全部攤開。

```cpp
// C++ code
int main() {
    const char *a = "A B C D E F x x x G H x I";
    BinaryTree T(a);
    T.Inorder_by_parent();
    std::cout << std::endl;
    
    T.insertLevelorder('K');
    T.insertLevelorder('L');
    T.insertLevelorder('M');
    T.insertLevelorder('N');
    T.Inorder_by_parent();
    std::cout << std::endl;

    return 0;
}
```
在經過一連串程式碼後，預期得到的output：

```cpp 
D B G E H A F I C   
L D M B G E H A N F I C K    
```
</br> 

<a name="def"></a>

####**定義class TreeNode、class BinaryTree**

幾點說明：

* 這裡對`class TreeNode`與`class BinaryTree`之定義，與[上一篇文章](http://alrightchiu.github.io/SecondRound/binary-tree-traversal.html#ex_code)之最大不同在於資料的隱蔽性，因為在此不需要於`main()`中存取任何pointer(`root`、`leftchild`、`rightchild`)，因此將之放進**private**區塊。

* 在許多traversal中，時常以pointer不為NULL作為`while`的判斷式，因此在constructor中，多做一步，將pointer指向NULL(eg：`leftchild = 0`)避免埋地雷在日後炸自己。

* 在`class BinaryTree`除了上一篇介紹過的inorder traversal外，多了兩個新朋友`LevelorderConstruct()`與`insertLevelorder()`，前者即是本篇主角，吃進`stringstream`後，把樹建出來；後者純粹是好玩，其功能為「以Complete Binary Tree之位置規則，在理應出現node的位置，新增node」。


看下去。

```cpp
// C++ code
class BinaryTree;
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    char data;
public:
    TreeNode(){ leftchild = 0; rightchild = 0; parent = 0; };
    TreeNode(char s):data(s){leftchild = 0; rightchild = 0; parent = 0; };
    
    friend class BinaryTree;
};

class BinaryTree{
private:
    TreeNode *root;
public:
    BinaryTree(){ root = 0;};
    BinaryTree(const char* str);
    
    void LevelorderConstruct(std::stringstream &ss);
    void insertLevelorder(char data);
    
    TreeNode* leftmost(TreeNode *current);
    TreeNode* InorderSuccessor(TreeNode *current);
    void Inorder_by_parent();
};
```
</br>  

<a name="constructor"></a>

####**Constructor of BinaryTree**

`class BinaryTree`的constructor很直觀，拿到一個字串陣列，先送進`stringstream`後，再由`stringstream`放進樹中，先對樹的`root`進行記憶體配置以及賦值，接著以level-order的方式建立Binary Tree。

```cpp
// C++ code
BinaryTree::BinaryTree(const char* str){
    std::stringstream  ss;
    ss << str;
    
    root = new TreeNode;
    ss >> root->data;
    
    LevelorderConstruct(ss);
}
```
</br>

<a name="func1"></a>

####**Function：LevelorderConstruct()**

* 在看`LevelorderConstruct()`的函式主體之前，再看一眼[level-order traversal](http://alrightchiu.github.io/SecondRound/binary-tree-traversal.html#level)，概念上即是藉著`queue`的「先排隊就先購票」的特性，在同一個level中，只要確保由左至右將node放進`queue`中，便能確保在進入下一個level後，以先前放入node之順序進行visiting。  
* 在`while`內，新增條件用來判斷從`stringstream`中輸出的字母是「大寫字母」(ASCII：65~90)還是「x」，前者要放入樹中建成node，後者則忽略不計。  
* 整份程式碼的關鍵在於神器`stringstream &ss`，只要不斷地透過`ss >> data`，`ss`便會自動尋找下一筆資料餵進`data`。
* 最後，當`stringstream`不再更新`data`時，也就是字串陣列已全數讀取完畢，即跳出`while`迴圈。

步驟如下：

* 首先，在Binary Tree的constructor中，先配置`root`的記憶體位置，並透過第一次`ss >> root->data`將第一個字母放進`root`中，如圖三(a)。
 
<center>
![construct_0][construct_0]

**圖三(a)：從ss取出第一個字母'A'放進`root`。**  
</center> 
 

接著進入`while`迴圈。  

* 條件式：`ss >> data`若為真，表示成功從`ss`中取出字母，傳進`data`。  
* 進入迴圈後，先判斷取出的字母若為大寫字母(在此為**'B'**)，即生成一個新的`new_node`，將B放進`new_node`中，並將`CurrentNode`(在此為A)的left child指向`new_node`，如圖三(b)。  
* 在`queue`	的部分，若成功建立出新的node(此為B)，便把B放進`queue`的隊伍中，表示之後將要把`CurrentNode`移到B，繼續往下建立新的node。

  
<center> 
![construct_1][construct_1]

**圖三(b)：。**  
</center> 


在同一個迴圈裡，建立完`CurrentNode`的left child後，接著嘗試建立right child。  

* 條件式：`if( !(ss >> data) )`若為真，表示`ss`中的字母已經讀取完畢，即跳出迴圈(`break`)。若否，則繼續從`ss`中讀取字母。
* 判斷字母是否為大寫字母(此為**'C'**)，便如同生成left child之方法，建立新的`new_node`、配置記憶體、將字母**'C'**放進`new_node`中，並將`CurrentNode`之right child指向`new_node`，如圖三(c)。
* 已成功建立新的node(C)，便把C放進`queue`的隊伍中，表示之後將要把`CurrentNode`移到B，繼續往下建立新的node。
此時，`queue`裡有兩個node，分別為B與C，要注意的是，排隊時，先進入隊伍的人會代表隊伍的前方，因此B為`queue`的**Front**，C為`queue`的**End**。
  
<center>
![construct_2][construct_2]  

**圖三(c)：。**  
</center> 

在建立完`CurrentNode`的left child與right child後，接著要移動`CurrentNode`，作為下一個`while`迴圈的起點。  
`queue`的功能便是提供`CurrentNode`移動的依據：

* 一律將`queue`隊伍的第一個node視作新的`CurrentNode`：`CurrentNode = q.front()`。
* 將`CurrentNode`移動至B後，便把B從`queue`移除：`q.pop()`，如圖三(d)。

如此便能保證，`CurrentNode`的移動會依照level-order「由上至下、由左至右」之順序。
</br>  
<center>
![construct_3][construct_3]  

**圖三(d)：。**  
</center> 

進入第二次`while`迴圈後，重複以上之步驟：

* 從`ss`取出字母，放進`data`。
* 判斷`data`是否為大寫，若是，便依序在`CurrentNode`之left child與right child建立新的node。
* 並且，將成功建立之node放進`queue`隊伍中，用作之後`CurrentNode`移動之用。

仔細觀察圖三(e)至圖三(h)之`ss`與`CurrentNode`之移動，與`queue`的變化：
</br>  
<center>
![construct_4][construct_4]  

**圖三(e)：。**  
</center> 

  
<center>
![construct_5][construct_5]  

**圖三(f)：。**  
</center> 

  
<center>
![construct_6][construct_6]  

**圖三(g)：。**  
</center> 

 
<center>
![construct_7][construct_7]  

**圖三(h)：。**  
</center>


* 在建立完C的left child後，從`ss`讀取到字母**'x'**，因為其並非大寫字母，表示C沒有right child，因此跳過生成新的node之步驟，如圖三(i)。  

 
<center>
![construct_8][construct_8]  

**圖三(i)：。**  
</center> 

* 若沒有生成新的node，便沒有新的node進入`queue`排隊。
* 接著要繼續將`CurrentNode`移動到`queue`的第一個元素，也就是D，並把D從`queue`中移除，如圖三(j)。


<center>
![construct_9][construct_9]  

**圖三(j)：。**  
</center>

* 當`CurrentNode`移動到D之後，`ss`連續放兩個**'x'**進入`data`，表示D的兩個child pointer皆指向`NULL`。
* 由於沒有新的node產生，`queue`的隊伍便沒有更新，如圖三(k)。


<center>
![construct_10][construct_10]

**圖三(k)：。**  
</center> 

接著，重複步驟：

* 移動`CurrentNode`至`queue`的第一個元素所指示的node。
* 從`ss`讀取字母，判斷其為大寫字母，生成新的node接在`CurrentNode`的child pointer上。
* 若有生成新的node，則將該node推入`queue`的隊伍。

  
<center>
![construct_11][construct_11]  

**圖三(l)：。**  
</center> 

* 直到`ss`輸出最後一個字母**'I'**後，這棵樹便建立完成。
* 由於，`ss`已全數讀取完畢，敘述句：`ss >> data`不成立，因此結束迴圈。

  
<center>
![construct_12][construct_12]  

**圖三(m)：。**  
</center> 

[construct_0]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct0.png?raw=true
[construct_1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct1.png?raw=true
[construct_2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct2.png?raw=true
[construct_3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct3.png?raw=true
[construct_4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct4.png?raw=true
[construct_5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct5.png?raw=true
[construct_6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct6.png?raw=true
[construct_7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct7.png?raw=true
[construct_8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct8.png?raw=true
[construct_9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct9.png?raw=true
[construct_10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct10.png?raw=true
[construct_11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct11.png?raw=true
[construct_12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/construct12.png?raw=true
</br>  

```cpp
// C++ code
void BinaryTree::LevelorderConstruct(std::stringstream &ss){
    std::queue<TreeNode*> q;
    TreeNode *current = root;
    char data = 'x';
    while (ss >> data) {
        if (data >= 65 && data <= 90){
            TreeNode *new_node = new TreeNode(data);  // call constructor TreeNode(char s)
            new_node->parent = current;
            current->leftchild = new_node;
            q.push(new_node);
        }
        if (!(ss >> data))
            break;
            
        if (data >= 65 && data <= 90){
            TreeNode *new_node = new TreeNode;        // call constructor TreeNode()
            new_node->parent = current;
            current->rightchild = new_node;
            new_node->data = data;
            q.push(new_node);
        }
        current = q.front();
        q.pop();
    }
}
```
</br>
<a name="func2"></a>

####**Function：insertLevelorder()**

函式`insertLevelorder()`的功能是，能夠按照Complete Binary Tree的位置順序放置新增的node，例如，若要在圖三之樹上新增帶有字母**'K'**的node，則`T.insertLevelorder('K')`便會將**'K'**建成C的right child，如圖四(a)：

<center>
![insertK][insertK]  

**圖四(a)：。**  
</center> 

再依序新增L、M、N：

* ```T.insertLevelorder('L')```  
* ```T.insertLevelorder('M')```  
* ```T.insertLevelorder('N')```

即會得到如圖四(b)的樹：

<center>
![insertLMN][insertLMN]  

**圖四(b)：。**  
</center> 

[insertK]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/insertK.png?raw=true
[insertLMN]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/BinaryTree_fig/Construct_from_char_array/insertLMN.png?raw=true

程式碼之邏輯與`LevelorderConstruct`大同小異，最主要的部分就是利用`queue`來記錄`CurrentNode`移動的順序：

* 首先，將`current`設成`root`，若樹存在，則進入`while`迴圈。
* 接著判斷，若`current`之left child已經有node，則將之放入`queue`中，再下次迴圈將以此node作為`current`，若left child還沒有node，便產生帶有`data`之新node，並將其建立成`current`之left child。  
pointer連接完成後，結束迴圈。
* 對`current`之right child進行相同之步驟。

如此便能有效控制Binary Tree之樹高(height)，使pointer所配置之記憶體空間有效利用，亦能夠減少traversal(以及其他操作)所需的時間。


```cpp
// C++ code
void BinaryTree::insertLevelorder(char data){    
    std::queue<TreeNode*> q;
    TreeNode *current = root;
    
    while (current) {
        if (current->leftchild != NULL)
            q.push(current->leftchild);
        else{
            TreeNode *new_node = new TreeNode(data);
            new_node->parent = current;
            current->leftchild = new_node;
            break;
        }
        if (current->rightchild != NULL)
            q.push(current->rightchild);
        else{
            TreeNode *new_node = new TreeNode(data);
            new_node->parent = current;
            current->rightchild = new_node;
            break;
        }
        current = q.front();
        q.pop();
    }
}
```
</br>  
以上便是利用`queue`執行level-order方式建立Binary Tree之範例。  
另外，[有些方法是利用遞迴的方式，外帶一個迴圈來進行level-order traversal](http://www.geeksforgeeks.org/level-order-tree-traversal/)，也能夠完成相同的功能。


</br>
<a name="ref"></a>

######**參考資料**：

* [Introduction to Algorithms](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [C++ Programming Language：Stream IO and File IO](http://www3.ntu.edu.sg/home/ehchua/programming/cpp/cp10_io.html)
* [GeeksforGeeks：Level Order Tree Traversal](http://www.geeksforgeeks.org/level-order-tree-traversal/)


 

</br>






