Title: Red Black Tree: Insert(新增資料)與Fixup(修正)    
Date: 2016-1-27 20:09  
Category: 演算法與資料結構  
Tags: C++, Red Black Tree  
Summary: 介紹於Red Black Tree(紅黑樹)Insert(新增資料)與Insert後的Fixup(修正)。
 


</br>
######**先備知識與注意事項**

RBT也是一棵BST，而RBT之Insert(新增資料)方法便是先沿用`InsertBST()`，再對顏色進行修正。而修正的方法將用上`Rotation()`，因此，建議在閱讀本篇文章之前，先熟悉[Binary Search Tree: Search(搜尋資料)、Insert(新增資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-searchsou-xun-zi-liao-insertxin-zeng-zi-liao.html)與[Red Black Tree: Rotation(旋轉)](http://alrightchiu.github.io/SecondRound/red-black-tree-rotationxuan-zhuan.html)會很有幫助。 

***

##目錄

* [先滿足BST的Key特徵](#bst)
* [修正：InsertFixUpRBT()](#fixup)
    * [Case1](#case1)
    * [Case3](#case3)
    * [Case2](#case2)
    * [幾個範例](#example)
    * [程式碼](#code)
* [參考資料](#ref)



<a name="bst"></a>

##**先滿足BST的Key特徵**

RBT也是一棵BST，在Insert(新增資料)時，必須滿足：$Key(L)<Key(Current)<Key(R)$，因此，RBT的`InsertRBT()`前半部演算法與[BST的函式：InsertBST()大同小異](http://alrightchiu.github.io/SecondRound/binary-search-tree-searchsou-xun-zi-liao-insertxin-zeng-zi-liao.html#insert)。  
需要修改的部分有兩處：

1. `NIL`：所有原先在BST中指向`NULL`的pointer，在RBT中需要修正成指向`NIL`，包括條件式與新增node的`child pointer`。
2. 顏色：如同在[RBT：Rotation(旋轉)](http://alrightchiu.github.io/SecondRound/red-black-tree-rotationxuan-zhuan.html#fail)所說，一般預設新增node為紅色，因此，若新增node接在黑色node之後，仍能滿足RBT的特徵，但是若新增node接在紅色node之後，則違反了[RBT之第四點特徵](http://alrightchiu.github.io/SecondRound/red-black-tree-introjian-jie.html#rbt)，必須進行修正。


`InsertRBT()`的程式範例如下：

```cpp
// C++ code
void RBT::InsertRBT(TreeNode &new_node){
    // 前半部與 InsertBST()之邏輯完全相同, 僅僅需要修改 NULL <-> NIL
    TreeNode *y = neel;
    TreeNode *x = root;
    TreeNode *insert_node = new TreeNode(new_node); // default copy constructor

    while (x != neel) {     // 把root初始化成neel, 這裡就可以用neel來做判斷
        y = x;
        if (insert_node->GetKey() < x->GetKey())
            x = x->leftchild;
        else
            x = x->rightchild;
    }

    insert_node->parent = y;
    if (y == neel)
        this->root = insert_node;
    else if (insert_node->GetKey() < y->GetKey())
        y->leftchild = insert_node;
    else
        y->rightchild = insert_node;
    
    // 以下是對RBT之node的設定, 將child pointer指向NIL, 顏色設為紅色
    insert_node->leftchild = neel;
    insert_node->rightchild = neel;
    insert_node->color = 0;             // 顏色可以在constructor中預設
        
    InsertFixedUpRBT(insert_node);      // 對可能出現紅色與紅色node相連之情形做修正
}
```


<a name="fixup"></a>

##**修正：InsertFixUpRBT()**

什麼情況需要對`InsertRBT()`做修正？  
當新增node接在紅色的node的`child pointer`，形成紅色與紅色相連時。

考慮以下情況，如圖一(a)，新增的node將要接在node(X)上：  

* node(X)為其`parent`，顏色為紅色；
* node(Y)為其`uncle`，其顏色**可能為紅色或黑色**。
* node(Z)為其grandparent(`parent->parent`)，顏色必定為黑色；
* node(W)的顏色**可能是紅色或黑色**。
* 所有灰色node(如node(a)、node(b)、node(c)、node(d))表示：只要不影響RBT之特徵，是否實際攜帶資料或為`NIL`並不影響結果。

<center>
![bst][f1]

**圖一(a)：。**  
</center>

根據`uncle`的顏色是紅色或者黑色，可以將修正(FixUp)分成三種情形(case)：

1. Case1：`uncle`是紅色，不論新增的node是node(X)的`leftchild`或`rightchild`；
2. Case2：`uncle`是黑色，而且新增的node為node(X)的`rightchild`；
3. Case3：`uncle`是黑色，而且新增的node為node(X)的`leftchild`。


<a name="case1"></a>

####**Case1**

圖一(b)左，此時`current`指向新增的node(A)，而node(A)成為node(X)的`rightchild`，其`uncle`node(Y)是紅色的。  
修正的方法就是「把債還給上一代的上一代」：

* 將`parent`塗成黑色：node(X)塗成黑色；
* 將`uncle`塗成黑色：node(Y)塗成黑色；
* 將`parent->parent`塗成紅色：node(Z)塗成紅色：
* 將`current`從node(A)移到node(Z)。

此時，如圖一(b)右，從node(Z)出發往其descendant leaves的任一path上之黑色node數皆相同，這個subtree便滿足了RBT的特徵。  

接著必需根據node(W)的顏色採取不同行動：

* 若node(W)為黑色，就不需要再做調整；
* 若node(W)為紅色，則node(Z)與node(W)再次形成紅色node與紅色node相連，必須重複同樣的判斷流程。

<center>
![bst][f2]

**圖一(b)：。**  
</center>


若node(A)成為node(X)的`leftchild`，如圖一(c)，修正的方法同上。


<center>
![bst][f3]

**圖一(c)：。**  
</center>


<a name="case3"></a>

####**Case3**

圖一(d)，新增的node(A)成為node(X)的`leftchild`，其`uncle`node(Y)是黑色。 

<center>
![bst][f4]

**圖一(d)：。**  
</center>

事實上，若`current`指向之node(此為node(A))是新增的node，則根據RBT之第五點特徵，其`uncle`node(Y)必定是`NIL`，如圖一(e)左。

不過，在稍後的範例中將會看到，`current`不一定是「剛剛新增的node」，也
有可能是「修正到一半，出現紅色與紅色相連的node」，但因為是「修正到一半」，尚未調整node(Z)的顏色，因此所有從node(Z)往其descendant leaves的任意path上之黑色node數必定不變，此時，若node(Y)不為`NIL`，則node(X)以及node(A)必定還有黑色的`child pointer`，如圖一(e)右所示，node(a)、node(b)與node(c)皆為黑色。

<center>
![bst][f5]

**圖一(e)：。**  
</center>


修正方法如下，見圖一(f)：

* 將`parent`塗成黑色：node(X)塗成黑色；
* 將`parent->parent`塗成紅色：node(Z)塗成紅色；
* 對`parent->parent`node(Z)進行**Right Rotation(向右旋轉)**


<center>
![bst][f6]

**圖一(f)：。**  
</center>


經過**Case3**的修正，必定會滿足RBT之規則，原因在於：

* 先考慮圖一(e)，若從node(A)往其descendant leaves的任意path上之黑色node數為$M$，則從node(Z)往其descendant leaves的任意path上之黑色node數為$M+1$；
* 再看圖一(g)，因為在修正的過程中，node(Z)從黑色被修改成紅色，因此從node(Z)往其descendant leaves的任意path上之黑色node數下修為$M$，與node(A)相同，使得整棵樹滿足RBT之特徵。

<center>
![bst][f7]

**圖一(g)：。**  
</center>



<a name="case2"></a>

####**Case2**

圖一(h)，新增的node(A)成為node(X)的`rightchild`，其`uncle`node(Y)是黑色。

<center>
![bst][f8]

**圖一(h)：。**  
</center>

如同Case3，圖一(h)的`uncle`node(Y)同樣有兩種可能：攜帶實際資料的黑色node，或者`NIL`，如圖一(i)：

<center>
![bst][f9]

**圖一(i)：。**  
</center>


而修正Case2的方法就是將其轉換成Case3，再利用上述Case3的方法調整成正確的RBT。  
從Case2調整成Case3，如圖一(j)：

* 將`current`移至`current->parent`：將`current`從node(A)移到node(X)；
* 對新的`current`進行Left Rotation：對node(X)進行Left Rotation。

圖一(j)右符合Case3：「`current`成為其`parent`的`leftchild`，且其`uncle`node(Y)是黑色」，因此，只要再進行如同圖一(f)之修正流程即可。

<center>
![bst][f10]

**圖一(j)：。**  
</center>

<a name="example"></a>

####幾個範例

**Example1**

考慮一棵RBT如圖二(a)：

<center>
![bst][f11]

**圖二(a)：。**  
</center>

若想新增node(75)，由於其將接在node(80)的`leftchild`位置上，而node(80)為紅色，因此需要進行修正。  
接著觀察，node(75)之`uncle`為node(60)，同樣是紅色，因此可以使用Case1的方法，如圖二(b)：

* 將`parent`與`uncle`塗黑：node(80)與node(60)塗黑；
* 將grandparent塗紅：node(70)塗紅；
* 將`current`移至node(70)；
* **進入下一個迴圈**判斷node(70)是否與其`parent`形成紅色與紅色node相連。

恰好，node(50)為`root`，一定是黑色，因此新增node(75)便算是完成。

<center>
![bst][f12]

**圖二(b)：。**  
</center>

若想繼續新增node(25)，由於其將接在node(30)的`leftchild`，而node(30)為紅色，因此需要修正。  
接著觀察，node(25)之`uncle`為`NIL`是黑色，而node(25)本身是`leftchild`，因此可以使用Case3的方法，如圖二(c)：

* 將`parent`塗成黑色：node(30)塗成黑色；
* 將`parent->parent`塗成紅色：node(40)塗成紅色；
* 對`parent->parent`node(40)進行**Right Rotation(向右旋轉)**

<center>
![bst][f13]

**圖二(c)：。**  
</center>

若想繼續新增node(79)，由於其將接在node(75)的`rightchild`，而node(75)為紅色，因此需要修正。  
接著觀察，node(79)之`uncle`為`NIL`是黑色，而node(79)本身是`rightchild`，因此可以使用Case2的方法，先將問題從Case2轉換成Case3，再由Case3之方法修正，如圖二(d)：

<center>
![bst][f14]

**圖二(d)：。**  
</center>


**Example2**

考慮一棵RBT如圖三(a)：

<center>
![bst][f15]

**圖三(a)：。**  
</center>

要在其中新增node(4)，則會依序經歷case1、case2直到case3完成修正，如圖三(b)、圖三(c)、圖三(d)與圖三(e)所示。

<center>
![bst][f16]

**圖三(b)：。**  

![bst][f17]

**圖三(c)：。**

![bst][f18]

**圖三(d)：。**

![bst][f19]

**圖三(e)：。**
</center>


根據以上說明，可以歸納出對於`InsertRBT()`的修正(Fix-Up)之情形(Case)間的循環圖，如圖四：

* 當新增node之`parent`為紅色時，需要對RBT進行修正；
* 若進入Case1，有可能執行一次即完成，也有可能再次出現紅色與紅色相連的情況，如圖三(b)-(c)；
* 若進入Case2，就轉換成Case3的情境；
* 一旦進入Case3，經過修正後必定能滿足RBT之特徵限制。


<center>
![bst][f20]

**圖四：。**  
</center>


最後還有一點需要說明。

圖五中：

* 左圖是本篇文章介紹修正(Fix-Up)的出發點：將新增的node接在node(X)上，而node(X)是node(Z)的`leftchild`(`parent`是`parent->parent`的`leftchild`)；
* 還有另外一半的情況就如圖五之右圖：將新增的node接在node(Y)上，而node(Y)是node(Z)的`rightchild`(`parent`是`parent->parent`的`rightchild`)。

<center>
![bst][f21]

**圖五：。**  
</center>

必須要區分這兩者的原因有二：

* 一是`uncle`：因為`parent`與`uncle`分別為`parent->parent`之`leftchild`與`rightchild`，若`parent`是`left-`，`uncle`就要是`right-`，反之亦然，兩者屬於互斥(exclusive)的、不能同時發生的情境；
* 二是Rotation(旋轉)：在Case2與Case3中必須使用Left/Right Rotation，因此，延續第一點原因，考慮到`parent`是`leftchild`或是`rightchild`的不同，Left/Right Rotation的方向也會相反。


<a name="code"></a>

##**程式碼**

`InsertFixedUpRBT()`之程式範例如下，其中分成幾個部分：

* 定義`color`：0為紅色，1為黑色；
* 如同圖四所示，修正的過程可能歷經不止一個Case，因此利用`while`迴圈實現，條件式便是判斷當前`current`之`parent`是否為紅色；
* 分別進行Case1(圖一(b)與圖一(c))、Case2(圖一(j))與Case3(圖一(f))之修正；
* 最後，在**line50**出現一行`root->color=1`，將`root`之顏色塗黑，這是為了Case1所設，由於Case1之修正方法是把「紅色與紅色node相連」之可能性往`root`方向傳遞，有可能`root`恰好是`current`的grandparent而被塗成紅色，如圖六，但因為RBT之第二點特徵要求`root`一定是黑色，因此必須作此預防。

<center>
![bst][f22]

**圖六：。**  
</center>



```cpp
// C++ code
void RBT::InsertFixedUpRBT(TreeNode *current){
    // Case0: parent是黑色, 就不用進迴圈
    while (current->parent->color == 0) {   // 若parent是紅色即進入迴圈
        // 上半部：parent為grandparent的leftchild之情況，如圖五左
        if (current->parent == current->parent->parent->leftchild) { 
            TreeNode *uncle = current->parent->parent->rightchild;
            // 若uncle是紅色：Case1
            if (uncle->color == 0) {
                current->parent->color = 1;
                uncle->color = 1;
                current->parent->parent->color = 0; //grandparent改成紅色
                current = current->parent->parent;
            }
            // 若uncle是黑色：Case2 or Case3
            else {
                // 若current是rightchild：Case2  
                if (current == current->parent->rightchild){        
                    current = current->parent;
                    LeftRotation(current);
                }
                // 若current是leftchild：Case3
                current->parent->color = 1;     
                current->parent->parent->color = 0;     
                RightRotation(current->parent->parent);
            }
        }
        // 下半部：parent為grandparent的rightchild之情況，如圖五右
        else {  
            TreeNode *uncle = current->parent->parent->leftchild;
            // 若uncle是紅色：Case1
            if (uncle->color == 0) {
                current->parent->color = 1;
                uncle->color = 1;
                current->parent->parent->color = 0; //grandparent改成紅色
                current = current->parent->parent;
            }
            // 若uncle是黑色：Case2 or Case3
            else {
                if (current == current->parent->leftchild) {
                    current = current->parent;
                    RightRotation(current);
                }
                current->parent->color = 1;
                current->parent->parent->color = 0;
                LeftRotation(current->parent->parent);
            }
        }
    }
    root->color = 1;    
}
```




[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert13.png?raw=true
[f14]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert14.png?raw=true
[f15]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert15.png?raw=true
[f16]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert16.png?raw=true
[f17]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert17.png?raw=true
[f18]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert18.png?raw=true
[f19]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert19.png?raw=true
[f20]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert20.png?raw=true
[f21]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert21.png?raw=true
[f22]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Insert/insert22.png?raw=true


</br>
<a name="ref"></a>

######**參考資料**：

* [Introduction to Algorithms, Ch13](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [GeeksforGeeks：Red Black Tree](http://code.geeksforgeeks.org/NtLnIk)


 

</br>






