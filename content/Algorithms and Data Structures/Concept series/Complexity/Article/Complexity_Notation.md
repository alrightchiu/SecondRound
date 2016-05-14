Title: Complexity：Asymptotic Notation(漸進符號)  
Date: 2016-5-11 20:31 
Category: 演算法與資料結構  
Tags: C++, Intro, Complexity, Big-O,  
Summary: 介紹用來分析Complexity(複雜度)的Asymptotic Notation(漸進符號)。


</br>
###先備知識與注意事項

// dominant term





***

##目錄

* [Complexity(複雜度)](#com)
* [使用Asymptotic Notation的優點](#an)
    * [$\Theta-$Notation](#tight)
    * [$O-$Notation，Big-O](#bo)
    * [$\Omega-$Notation](#bw)
    * [$o-$Notation](#so)
    * [$\omega-$Notation](#sw)
* [參考資料](#ref)
* [Complexity系列文章](#series)



</br>

<a name="com"></a>

##Complexity(複雜度)

<center>
![cc][f1]

**圖一：。**
</center>


假設某間出版社想要印書，找到一間印刷廠，廠房內共有六台機器，每台機器的工作效率分別如圖一的六個函數，橫軸表示「書的數量」，縱軸表示「需要的工作天」。

<center>

|       |&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**$N!$** |&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**$2^N$**|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**$N^2$**|&nbsp;&nbsp;**$N\ln{N}$**|&nbsp;&nbsp;&nbsp;**$N$**  |&nbsp;&nbsp;**$\ln{N}$**|
|:---   |:---:|:---:|:---:|:---:|:---:|:---:|
|$N=5$  |$120$  |$32$ |$25$|$8.05$ |$5$  |$1.6$|
|$N=100$|&nbsp;&nbsp;&nbsp;&nbsp;$9\times10^{157}$|&nbsp;&nbsp;$1.3\times10^{30}$|&nbsp;&nbsp;$10000$|$460$ |&nbsp;$100$|$4.6$|

**表一**
</center>

根據表一可以看出，若要印$N=5$本書：

* 使用「機器**$\ln{N}$**」需要$1.6$個工作天。
* 使用「機器**$N^2$**」需要$25$個工作天；
* 使用「機器**$N!$**」需要$120$個工作天；

當問題的資料量不大時，「有效率」的機器和「沒有效率」的機器在成本消耗上可能差異不大，但是當資料量增加時，成本會變得非常可觀。

若考慮印$N=100$本書：

* 使用「機器**$\ln{N}$**」需要$4.6$個工作天。
* 使用「機器**$N^2$**」需要$10000$個工作天，大約是$27$年；
* 使用「機器**$N!$**」需要$9\times10^{157}$個工作天，是個天文數字；

由以上可以看出：

1. 成本(包含運算時間與記憶體空間)，通常會和「待處理的資料量」有關，當資料量越大，成本會以某種關係(線性、指數等等，見圖一)跟著提高。
2. 當資料量大時，演算法的效率很重要。

而圖一中，描述每一種**演算法**(一台機器代表一種**演算法**)的「工作效率之函數」，就稱為此演算法之**Complexity(複雜度)**。




</br>

<a name="an"></a>

##使用Asymptotic Notation的優點

若能得知各個演算法的**Complexity(複雜度)**，便能決定哪個演算法較有效率。 
 
但是並非所有演算法都像[只有兩個迴圈的Insertion Sort](https://www.youtube.com/watch?v=i-SKeOcBwko)那麼簡潔而容易分析，所以在評估演算法之**Complexity(複雜度)**時，常使用**Asymptotic Notation(漸進符號)**，其概念為：

* 希望能以簡單的函數(例如：$N^{2}、\ln{N}$等等)來描述**Complexity(複雜度)**的「趨勢」，特別是針對資料量非常大的時候。

以下分別介紹五個**Asymptotic Notation(漸進符號)**。


</br>  

<a name="tight"></a>

###$\Theta-$Notation


**Asymptotic Notation(漸進符號)**是所有能夠描述演算法趨勢的「函數之集合」，給定：

* 非負函數$f(n)$：描述演算法之趨勢。
* 非負函數$g(n)$：簡單函數。

若滿足以下定義：

$$
\Theta(g(n))=\{\,f(n)：存在正整數\:c_1,c_2,n_0 \,,並且對於所有n\geq n_0,\,滿足 0\leq c_{1}g(n)\leq f(n) \leq c_{2}g(n)\:\}
$$

表示$g(n)$為$f(n)$趨勢之「邊界」(bound)，即可使用$g(n)$來描述$f(n)$之趨勢，以$f(n)\in\Theta(g(n))$表示。


<center>
![cc][f2]

**圖二：。**
</center>


舉例來說，若現有一個演算法之趨勢可以用$f(n)=6n+4$代表，那麼以下兩個$g(n)$都能夠在$\Theta(g(n))$的定義下視為$f(n)$的「邊界」：

* 若$g(n)=2n$，取$c_{1}=2,c_{2}=4,n_{0}=2$，則滿足：
    * $0\leq 2(2n)\leq 6n+4 \leq 4(2n) \,, \forall n\geq 2$，見圖二左。  
    也就是說，當資料量$n\geq 2$，$f(n)=6n+4$的值往上不會超過$8n$，往下不會低於$4n$。

* 若$g(n)=5n$，取$c_{1}=1,c_{2}=2,n_{0}=1$，則滿足：
    * $0\leq 1(5n)\leq 6n+4 \leq 2(5n) \,, \forall n\geq 1$，見圖二右。  
    同理，當資料量$n\geq 1$，$f(n)=6n+4$的值往上不會超過$10n$，往下不會低於$5n$。


根據定義，既然係數($c_{1},c_{2}$)可以任選，那麼以上兩個$g(n)$函數其實可以視為同一個函數：$g(n)=n$。  
因此，若一個演算法之「趨勢」為$f(n)=6n+4$，那麼其複雜度即為$\Theta(n)$，可以表示成：

* $f(n)\in\Theta(n)$，或者
* $f(n)=\Theta(n)$。



</br>    

<a name="bo"></a>

###$O-$Notation，Big-O



</br>  

<a name="bw"></a>

###$\Omega-$Notation



</br>

<a name="so"></a>

###$o-$Notation


</br>

<a name="sw"></a>

###$\omega-$Notation




[f1]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Concept%20series/Complexity/Asymptotic%20Notation/f1.png?raw=true
[f2]: https://github.com/alrightchiu/SecondRound/blob/master/content/Algorithms%20and%20Data%20Structures/Concept%20series/Complexity/Asymptotic%20Notation/f2.png?raw=true



</br>  


 
以上便是以Array表示Set之介紹。


</br>

***

<a name="ref"></a>

###參考資料：

* [Introduction to Algorithms, Ch3](http://www.amazon.com/Introduction-Algorithms-Edition-Thomas-Cormen/dp/0262033844) 
* [Fundamentals of Data Structures in C++, Ch1](http://www.amazon.com/Fundamentals-Data-Structures-Ellis-Horowitz/dp/0929306376)
* [Big-O Algorithm Complexity Cheat Sheet](http://bigocheatsheet.com/)
* [Infinite Loop：複雜度分析 - Complexity Analysis](http://program-lover.blogspot.tw/2008/10/complexity-analysis.html)
* [MyCodeSchool：Insertion sort algorithm](https://www.youtube.com/watch?v=i-SKeOcBwko)




<a name="series"></a>

</br>

###Complexity系列文章

[Complexity：Asymptotic Notation(漸進符號)]()  


回到目錄：

[目錄：演算法與資料結構](http://alrightchiu.github.io/SecondRound/mu-lu-yan-suan-fa-yu-zi-liao-jie-gou.html)

</br>


