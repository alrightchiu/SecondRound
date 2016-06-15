Title: Red Black Tree: Delete(刪除資料)與Fixup(修正)    
Date: 2016-1-30 14:11  
Category: 演算法與資料結構  
Tags: C++, Red Black Tree  
Summary: 介紹於Red Black Tree(紅黑樹)中Delete(刪除資料)與Delete後的Fixup(修正)。
 


</br>
###先備知識與注意事項

如同[Red Black Tree: Insert(新增資料)與Fixup(修正)](http://alrightchiu.github.io/SecondRound/red-black-tree-insertxin-zeng-zi-liao-yu-fixupxiu-zheng.html)，RBT之Delete(刪除資料)方法同樣是先沿用`DeleteBST()`，再對顏色利用`Rotation`進行修正。  

建議讀者在閱讀本篇文章之前，先復習[BST::DeleteBST(刪除資料)](http://alrightchiu.github.io/SecondRound/binary-search-tree-sortpai-xu-deleteshan-chu-zi-liao.html#delete)與[Red Black Tree: Rotation(旋轉)](http://alrightchiu.github.io/SecondRound/red-black-tree-rotationxuan-zhuan.html)，會比較容易上手。


***


##目錄

* [於RBT中Delete(刪除資料)可能違反RBT之特徵](#violate)
* [如同於BST中Delete(刪除資料)](#bst)
* [修正：DeleteFixUpRBT()](#fixup)
    * [Case1](#case1)
    * [Case2](#case2)
    * [Case3](#case3)
    * [Case4](#case4)
    * [範例](#example)
* [程式碼](#code)
* [參考資料](#ref)
* [RBT系列文章](#tree_series)

</br>

<a name="violate"></a>

##於RBT中Delete(刪除資料)可能違反RBT之特徵


在RBT中執行Delete(刪除資料)時，若刪除之node為黑色，有可能違反三點RBT特徵：

1. 圖一(a)：若要刪除的node恰好為`root`，而刪除後恰好是紅色的node遞補成為新的`root`，此時便違反RBT之第二點特徵：`root`一定要是黑色；
2. 圖一(b)：若刪除node後，出現紅色與紅色node相連之情形，則違反RBT之第四點特徵：紅色node之`child`一定要是黑色；
3. 圖一(b)：若刪除之node是黑色，而且恰好不是`root`，那麼所有包含被刪除node的path上之黑色node數必定會減少，將會違反RBT之第五點特徵：「站在任何一個node上，所有從該node走到其任意descendant leaf的path上之黑色node數必定相同」。
    * 圖一(b)左：從`root`:node(B)出發至任意leaf的path上都有三個黑色node(包含`NIL`)；
    * 圖一(b)右：刪除node(D)後，path:node(B)-node(E)-node(C)上之黑色node數剩下$2$個(包含`NIL`)。

因此，需要對顏色進行修正，以滿足RBT特徵。


<center>
![violate][f1]

**圖一(a)：違反RBT之第二點與第四點特徵。**

![violate][f2]

**圖一(b)：違反RBT之第四點與第五點特徵。**
</center>

</br>

<a name="bst"></a>

##如同於BST中Delete(刪除資料)


`RBT::DeleteRBT()`之範例程式碼分成兩個部分：

1. 第一部分，如同`DeleteBST()`，依照欲刪除之node的child個數分成三種情形處理：
    1. 先確認BST中有沒有要刪除的node；
    2. 把要刪除的node調整成「至多只有一個child」；
    3. 把要刪除的node的child指向新的`parent`；
    4. 把要刪除的node的`parent`指向新的child；
    5. 若實際上刪除的是「替身」，再把替身的資料放回BST中；
2. 第二部分，若刪除的node是黑色，需要進行修正(Fix-Up)，引進函式：`DeleteFixedUpRBT()`。   

</br>  


```cpp
// C++ code
void RBT::DeleteRBT(int KEY){              // 要刪除具有KEY的node
    
    TreeNode *delete_node = Search(KEY);   // 先確認RBT中是否存在具有KEY的node
    if (delete_node == NULL) {
        std::cout << "data not found.\n";
        return;
    }

    TreeNode *y = 0;     // 真正要被刪除並釋放記憶體的node
    TreeNode *x = 0;     // 要被刪除的node的"child"

    if (delete_node->leftchild == neel || delete_node->rightchild == neel){
        y = delete_node;
    }
    else{
        y = Successor(delete_node);         // 將y設成delete_node的Successor
    }                                       // 經過這組if-else, y調整成至多只有一個child
    

    if (y->leftchild != neel){              // 將x設成y的child, 可能有實際資料, 也有可能是NIL
        x = y->leftchild;
    }
    else{
        x = y->rightchild;
    }

    x->parent = y->parent;                 // 即使x是NIL也要把x的parent指向有效的記憶體位置
                                           // 因為在FixUp時需要藉由x->parent判斷x為leftchild或是rightchild

    if (y->parent == neel){                // 接著再把要被釋放記憶體的node之"parent"指向新的child
        this->root = x;                    // 若刪除的是原先的root, 就把x當成新的root 
    }
    else if (y == y->parent->leftchild){   // 若y原本是其parent之left child
        y->parent->leftchild = x;          // 便把x皆在y的parent的left child, 取代y
    }
    else{                                  // 若y原本是其parent之right child
        y->parent->rightchild = x;         // 便把x皆在y的parent的right child, 取代y
    }

    if (y != delete_node) {                // 針對case3
        delete_node->key = y->key;         // 若y是delete_node的替身, 最後要再將y的資料
        delete_node->element = y->element; // 放回delete_node的記憶體位置, 並將y的記憶體位置釋放
    }

    if (y->color == 1) {                   // 若刪除的node是黑色, 要從x進行修正, 以符合RBT的顏色規則
        DeleteFixedUpRBT(x);
    }
}
```

</br>

<a name="fixup"></a>

##修正：DeleteFixUpRBT()

考慮在圖二之RBT中刪除node(B)，由於node(B)是黑色，必定違反RBT之特徵，因此需要修正。  
(以下圖示中，白色的node表示顏色可能為黑色也可能為紅色，而且可能是一棵subtree或是`NIL`，需視情況而定。)

<center>
![original][f3]

**圖二：。**
</center>

根據`sibling`之顏色與`sibling`之`child`之顏色，可以分為下列四種情形(Case)，如圖三：

* Case1：`sibling`為紅色；
* Case2：`sibling`為黑色，而且`sibling`的兩個`child`都是黑色；
* Case3：`sibling`為黑色，而且`sibling`的`rightchild`是黑色，`leftchild`是紅色；
* Case4：`sibling`為黑色，而且`sibling`的`rightchild`是紅色。
    * 雖然不太有人討論Case4中，`sibling`的`leftchild`顏色是否一定是黑色，不過根據code回推的話，應該是。不過不是很重要。


<center>
![4cases][f4]

**圖三：。**
</center>


`DeleteFixUpRBT()`的情形(Case)較為複雜，圖四是所有情形之循環圖：  
(`current`即是被刪除的node之`child`)

* 若`current`是黑色的，而且`current`不為`root`，則依情況進入四個Case；
* 若進入Case1，修正後，將進入Case2、Case3或Case4；
* 若進入Case2，有可能修正後即符合RBT特徵，也有可能根據新的`current`之情形重新判斷起；
* 若進入Case3，修正後必定進入Case4；
* 若進入Case4，修正後必定符合RBT之特徵。


<center>
![flow][f5]

**圖四：。**
</center>


<a name="case1"></a>

###Case1

若`sibling`為紅色，修正方法如下，見圖五(a)：

* 將`sibling`塗成黑色：node(E)塗成黑色；
* 將`current`之`parent`塗成紅色：node(C)塗成紅色；
* 對`current`之`parent`做Left Rotation：對node(C)做Left Rotation；
* 將`sibling`移動到`current->parent`的`rightchild`：將`sibling`指向node(D)。

<center>
![case1][f6]

**圖五(a)：。**
</center>

在上述步驟中，並沒有更改`current`之記憶體位置和顏色，`current`仍為黑色。不過其`sibling`必定會變成黑色，因此將進入Case2、Case3或Case4。

為什麼Case1經過以上修正還沒有結束？原因要回到刪除node之前的RBT。  

圖五(b)左，展示了刪除node之前，以node(C)為`root`的RBT(或是更大的RBT之subtree)的其中一種可能情況。  
從node(C)往任何一個descendant leaf的path上之黑色node數為$3$，刪除node(B)後，使得其中一條path的黑色node數減少，經過上述方法之調整，仍然無法使得所有path之黑色node數相同，如圖五(b)右。  

不過Case1所提出的修正方法能夠將情況調整成Case2、Case3或Case4，並且修正至滿足RBT之特徵。


<center>
![case1][f7]

**圖五(b)：圖中的「Original」僅代表其中一種可能的情形。**
</center>


<a name="case2"></a>

###Case2

若`sibling`為黑色，並且`sibling`之兩個`child`皆為黑色，修正的方法如下，見圖五(c)：

* 將`sibling`塗成紅色：node(E)塗成紅色；
* 將`current`移至`currnet`的`parent`：`current`移至node(C)。

<center>
![case2][f8]

**圖五(c)：。**
</center>

經過上述步驟，根據新的`current`:8node(C)之顏色，可以分成兩種情形：

* 若node(C)為紅色，則跳出迴圈，把node(C)塗黑，即可滿足RBT之特徵，如圖五(d)，其邏輯便是：將從node(C)出發往`leftchild`與`rightchild`path的黑色數目調整成與刪除之前(Original)相同；
* 若node(C)為黑色，且node(C)不是`root`，則繼續下一輪迴圈，重新判斷其屬於四種情況之何者並修正，如圖五(e)，從node(G)出發至任意descendant leaf之path上的黑色node數並不完全相同。


<center>
![case2][f9]

**圖五(d)：圖中的「Original」僅代表其中一種可能的情形。**

![case2][f10]

**圖五(e)：圖中的「Original」僅代表其中一種可能的情形。  
此時的RBT還要繼續修正，重新回到Case2。**
</center>


<a name="case3"></a>

###Case3

若`sibling`為黑色，並且`sibling`之`rightchild`為黑色，`leftchild`為紅色，修正的方法如下，見圖五(f)：

* 將`sibling`之`leftchild`塗成黑色：node(D)塗成黑色；
* 將`sibling`塗成紅色：node(E)塗成紅色；
* 對`sibling`進行Right Rotation：對node(E)進行Right Rotation；
* 將`sibling`移至`current->parent`的`rightchild`：將`sibling`移至node(D)。

<center>
![case3][f11]

**圖五(f)：。**
</center>

經過以上修正步驟，`sibling`之`rightchild`成為紅色，便進入Case4。


<a name="case4"></a>

###Case4

若`sibling`為黑色，並且`sibling`之`rightchild`為紅色，修正的方法如下，見圖五(g)：

* 將`sibling`塗成`current`之`parent`的顏色：
    * 若node(C)是紅色，則將node(E)塗成紅色；
    * 若node(C)是黑色，則將node(E)塗成黑色；
* 將`parent`塗成黑色：node(C)塗成黑色；
* 將`sibling`之`rightchild`塗成黑色：node(F)塗成黑色；
* 對`parent`進行Left Rotation：對node(C)做Left Rotation；
* 將`current`移至`root`，把`root`塗黑。  
(注意：圖五(d)之node(E)未必是RBT之`root`。)


<center>
![case4][f12]

**圖五(g)：。**
</center>

如圖五(h)所示，Case4修正方法的邏輯便是：在刪除node(B)之後的RBT(或是subtree)中，將所有從`root`位置(調整前是node(C)，調整後是node(E))出發往任意descendant leaf之path上的黑色數目調整成與刪除之前(Original)相同，因此，經過Case4的修正一定能夠滿足RBT之特徵。

<center>
![case4][f13]

**圖五(h)：圖中的「Original」僅代表其中一種可能的情形。**
</center>


<a name="example"></a>

***

###範例

接著以一個簡單的範例(圖六(a)之RBT)操作上述四種Case的修正方法。

<center>
![example][f14]

**圖六(a)：。**
</center>


###Case3->Case4

若考慮刪除node(19)，由於node(19)是黑色，需要修正。  
接著判斷，node(19)的`child`(為黑色的`NIL`)之`sibling`為黑色，且`sibling`之`rightchild`為黑色，符合Case3的描述，因此利用Case3之修正方法，見圖六(b)：

* 將`sibling`之`leftchild`塗成黑色：node(24)塗成黑色；
* 將`sibling`塗成紅色：node(27)塗成紅色；
* 對`sibling`進行Right Rotation：對node(27)進行Right Rotation；
* 將`sibling`移至`current->parent`的`rightchild`：將`sibling`移至node(24)；

接著進入Case4：`subling`為黑色，而且`sibling`之`rightchild`為紅色，進行修正：

* 將`sibling`塗成`current`之`parent`的顏色：node(22)是黑色，則將node(24)塗成黑色；
* 將`parent`塗成黑色：node(22)塗成黑色；
* 將`sibling`之`rightchild`塗成黑色：node(27)塗成黑色；
* 對`parent`進行Left Rotation：對node(22)做Left Rotation；
* 將`current`移至`root`，把`root`塗黑。

<center>
![example][f15]

**圖六(b)：。**
</center>

如此一來便再次滿足RBT之特徵限制，如圖六(c)。

<center>
![example][f16]

**圖六(c)：。**
</center>


###Case4

再考慮刪除黑色的node(45)，判斷：node(45)之`child`(為黑色的`NIL`)之`sibling`為黑色，且`sibling`之`rightchild`為紅色，符合Case4的描述，並利用Case4方法修正，見圖六(d)：

* 將`sibling`塗成`current`之`parent`的顏色：node(22)是黑色，則將node(24)塗成黑色；
* 將`parent`塗成黑色：node(22)塗成黑色；
* 將`sibling`之`rightchild`塗成黑色：node(27)塗成黑色；
* 對`parent`進行Left Rotation：對node(22)做Left Rotation；
* 將`current`移至`root`，把`root`塗黑。

<center>
![example][f17]

**圖六(d)：。**
</center>

如此一來便再次滿足RBT之特徵限制，如圖六(e)。

<center>
![example][f18]

**圖六(e)：。**
</center>

###Case1->Case4

接著考慮刪除黑色的node(39)，判斷：node(45)之`child`(為黑色的`NIL`)之`sibling`為紅色，符合Case1之描述，便利用Case1之方法，調整成Case4，見圖六(f)：

Case1調整：

* 將`sibling`塗成黑色：node(52)塗成黑色；
* 將`current`之`parent`塗成紅色：node(41)塗成紅色；
* 對`current`之`parent`做Left Rotation：對node(41)做Left Rotation；
* 將`sibling`移動到`current->parent`的`rightchild`：將`sibling`移動至node(48)；  

再利用Case4的方法修正，便能滿足RBT之特徵，見圖六(g)。


<center>
![example][f19]

**圖六(f)：。**

![example][f20]

**圖六(g)：。**

</center>

###Case2->Case4

若要刪除黑色的node(7)，由於其`child`之`sibling`為黑色，且具有兩個黑色的`child`(都是`NIL`)，符合Case2的情況，便修正如下，見圖六(h)：

* 將`sibling`塗成紅色：node(10)塗成紅色；
* 將`current`移至`currnet`的`parent`：`current`移至node(9)；
* 若新的`current`node(9)為紅色，即跳出迴圈，並將`current`塗黑。

經修正後，便符合RBT之特徵，見圖六(i)。

<center>
![example][f21]

**圖六(h)：。**

![example][f22]

**圖六(i)：。**

</center>

###Case0: current is red or current is root

最後，若要刪除黑色的node(3)呢？由於node(3)的`child`node(1)為紅色，並不需要考慮到Case1(`sibling`為紅色)，只要將node(1)塗黑即可，如圖六(j)。  

<center>
![example][f23]

**圖六(j)：。**
</center>

</br>

<a name="code"></a>

##程式碼


`RBT::DeleteFixedUpRBT()`之範例程式碼分成以下幾個部分：


如圖四所示，修正的過程可能經歷不止一個Case，因此利用`while`來實現，條件式為：`current`不是`root`，而且`current`為黑色；

若`current`是其`parent`之`leftchild`，其`sibling`就必須是`rightchild`，反之亦然，而兩種情形之`Rotation`修正之方向正好相反，因此，如同`InsertFixedUpRBT()`，必須區分出「`current`是其`parent`之`leftchild`」與「`current`是其`parent`之`rightchild`」兩種情況，彼此結構對稱。

分別進行Case1、Case2、Case3與Case4之修正。

最後，當跳出迴圈後，`current->color = 1`將`current`之顏色塗黑，有可能在Case2用上，見圖五(d)與圖六(h)。


```cpp
// C++ code
void RBT::DeleteFixedUpRBT(TreeNode *current){
    // Case0:(i)  current是紅色的, 不影響黑色在path上的數量
    //       (ii) current是root,
    while (current != root && current->color == 1) {
        // current是leftchild
        if (current == current->parent->leftchild) {    
            TreeNode *sibling = current->parent->rightchild;
            // Case1: 如果sibling是紅色
            if (sibling->color == 0) {
                sibling->color = 1;
                current->parent->color = 0;
                LeftRotation(current->parent);
                sibling = current->parent->rightchild;
            }
            // 進入 Case2、3、4: sibling是黑色
            // Case2: sibling的兩個child都是黑色
            if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
                sibling->color = 0;
                current = current->parent;           // 若current更新到root, 即跳出迴圈
            }
            // Case3 & 4: current只有其中一個child是黑色
            else {
            	// case3: sibling的right child是黑的, left child是紅色
                if (sibling->rightchild->color == 1){
                    sibling->leftchild->color = 1;
                    sibling->color = 0;
                    RightRotation(sibling);
                    sibling = current->parent->rightchild;
                }
                // 經過Case3後, 一定會變成Case4
                // Case 4: sibling的right child 是紅色的, left child是黑色
                sibling->color = current->parent->color;
                current->parent->color = 1;
                sibling->rightchild->color = 1;
                LeftRotation(current->parent);
                current = root;     // 將current移動到root, 一定跳出迴圈
            }
        }
        // current是rightchild
        else {  
            TreeNode *sibling = current->parent->leftchild;
            // Case1: 如果sibling是紅色
            if (sibling->color == 0) {
                sibling->color = 1;
                current->parent->color = 0;
                RightRotation(current->parent);
                sibling = current->parent->leftchild;
            }
            // 進入 Case2、3、4: sibling是黑色
            // Case2: sibling的兩個child都是黑色
            if (sibling->leftchild->color == 1 && sibling->rightchild->color == 1) {
                sibling->color = 0;
                current = current->parent;             // 若current更新到root, 即跳出迴圈
            }
            // Case3 & 4: current只有其中一個child是黑色
            else {
            	// case3: sibling的left child是黑的, right child是紅色
                if (sibling->leftchild->color == 1){
                    sibling->rightchild->color = 1;
                    sibling->color = 0;
                    LeftRotation(sibling);
                    sibling = current->parent->leftchild;
                }
                // 經過Case3後, 一定會變成Case4
                // Case 4: sibling的left child 是紅色的, rightt child是黑色
                sibling->color = current->parent->color;
                current->parent->color = 1;
                sibling->leftchild->color = 1;
                RightRotation(current->parent);
                current = root;     // 將current移動到root, 一定跳出迴圈
            }
        }
    }
    current->color = 1;
}
```


[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete2.png?raw=true
[f3]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete3.png?raw=true
[f4]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete4.png?raw=true
[f5]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete5.png?raw=true
[f6]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete6.png?raw=true
[f7]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete7.png?raw=true
[f8]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete8.png?raw=true
[f9]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete9.png?raw=true
[f10]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete10.png?raw=true
[f11]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete11.png?raw=true
[f12]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete12.png?raw=true
[f13]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete13.png?raw=true
[f14]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete14.png?raw=true
[f15]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete15.png?raw=true
[f16]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete16.png?raw=true
[f17]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete17.png?raw=true
[f18]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete18.png?raw=true
[f19]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete19.png?raw=true
[f20]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete20.png?raw=true
[f21]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete21.png?raw=true
[f22]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete22.png?raw=true
[f23]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Tree%20series/RBT_fig/Delete/delete23.png?raw=true

</br>

以上便是於Red Black Tree(紅黑樹)中進行Delete(刪除資料)與Delete後的Fixup(修正)之介紹。

</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch13](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch10](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [GeeksforGeeks：Red Black Tree](http://code.geeksforgeeks.org/NtLnIk)


</br>

<a name="tree_series"></a>

###RBT系列文章


[Red Black Tree: Intro(簡介)](http://alrightchiu.github.io/SecondRound/red-black-tree-introjian-jie.html)  
[Red Black Tree: Rotation(旋轉)](http://alrightchiu.github.io/SecondRound/red-black-tree-rotationxuan-zhuan.html)  
[Red Black Tree: Insert(新增資料)與Fixup(修正)](http://alrightchiu.github.io/SecondRound/red-black-tree-insertxin-zeng-zi-liao-yu-fixupxiu-zheng.html)  
[Red Black Tree: Delete(刪除資料)與Fixup(修正)](http://alrightchiu.github.io/SecondRound/red-black-tree-deleteshan-chu-zi-liao-yu-fixupxiu-zheng.html)


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)


</br>






