### 一、实验内容

设计一个应用软件，以实现SLR(1)分析器的生成。

1、要提供一个源程序编辑界面，让用户输入文法规则（可保存、打开存有文法规则的文件）

2、检查该文法是否需要进行文法的扩充。

3、求出该文法各非终结符号的first集合与follow集合，并提供窗口以便用户可以查看这些集合结果。

4、需要提供窗口以便用户可以查看文法对应的LR(0) DFA图。（可以用画图的方式呈现，也可用表格方式呈现该图点与边的关系数据）

5、需要提供窗口以便用户可以查看该文法是否为SLR(1)文法。（如果非SLR(1)文法，可查看其原因）

6、需要提供窗口以便用户可以查看文法对应的SLR(1)分析表。（如果该文法为SLR(1)文法时）

### 二、实验目的

通过实验掌握SLR文法的原理，了解SLR分析方法的分析过程。

### 三、实验思路

1. 将文法拆分为产生式。如`E->E+n|n`拆分为`E->E+n 和 E->n`两条产生式。

2. 将产生式的**左部和右部进行分离**。其中，右部的每一个终结符和非终结符也是单独保存的。产生式类的结构如下：

   ```C++
   class Production
   {
   public:
       int number;
       QString leftPart;
       QList<QString> rightParts;
   }
   ```

3. 将左部和右部分离后的文法中的**终结符和非终结符识别**出来。一般左部都是非终结符，右部除去非终结符就是终结符。

4. **检查文法是否需要扩充。**查看文法的第一条是否有两个入口，即检查是否含有或运算符`|`，如`E->E+n|n`则需要扩充文法`START->E`。

5. **计算FIRST集。**设置一个变量`_ALTERED_`，在每次有新的终结符添加到FIRST集中时，就将`_ALTERED_`置为`true`。计算FIRST集的方法如下：如果产生式右部是终结符a，且这个终结符a不在FIRST集中，则添加。如果是非终结符A，则将A的FIRST集添加到自己的FIRST集中。如果A的终结符中包含ε，则继续判断下一个。算法如下：

   ```c++
   for all nonterminals A do First(A) := {};
   while there are changes to any First(a) do
       for each production choice A->X1X2...Xn do
           k := 1; Continue := true;
   		while Continue = true and k <= n do
               add First(Xk) - {ε} to First(A);
   			if ε is not in First(Xk) then Continue := false;
   			k := k + 1;
   		if Continue = true then add ε to First(A);
   ```

6. **计算FOLLOW集。**设置一个变量`_ALTERED_`，在每次有新的终结符添加到FOLLOW集中的时候，就将`_ALTERED_`置位`true`。计算FOLLOW集的方法如下：

   1. 若A是开始符号，则`$`就在`Follow(A)`中。
   2. 若存在产生式B→αAγ，则First (γ) - { }在`Follow(A)`中。
   3. 若存在产生式B→αAγ，且 在`First(γ)`中，则F o l l o w (A)包括`Follow(B)`。

   ```c++
   Follow(start-symbol) := {$};
   for all nonterminals A ≠ start-symbol do Follow(A) := {};
   while there are changes to any Follow sets do
       for each production A->X1X2...Xn do
           for each Xi that is a nonterminal do
               add First(Xi+1Xi+2...Xn) - {ε} to Follow(Xi)
               if ε is in First(Xi+1Xi+2...Xn) then
                   add Follow(A) to Follow(Xi)
   ```

7. 根据每一条产生式，**生成项目集规范族**。程序设计的是对项目集规范族中的每一个项目集中的每一条项目都进行分析，产生了新的项目集就添加到项目集规范族中。规范项目集的算法如下：

   ```c++
   items(G') {
   	C = {CLOSURE({[START -> .S]})};
   	repeat
       	for(C中的每个项集I)
           	for(每个文法符号X)
               	if(GOTO(I, X)非空且不在C中)
                   	将GOTO(I, X) 加入C中；
   	until 在某一轮中没有新的项集被加入C中；
   }
   ```

   `CLOSURE()`函数：计算给定`项目集I`的闭包

   ```c++
   CLOSURE(I) {
       J = 1;
       repeat
           for(J中的每一个项A->α.Bβ)
               for(G的每个产生式B->γ)
                   if(项B->.γ不在J中)
                       将B->.γ加入J中;
       until 在某一轮中没有新的项被加入到J中;
       return J;
   }
   ```

   `GOTo()`函数：返回`项目集I`对应于文法符号X的后继项目集闭包

   ```c++
   GOTO(I, X) {
       将J初始化为空集;
       for(I中的每个项A->α.Xβ)
           将项A->αX.β加入到集合J中;
       return CLOSURE(J);
   }
   ```

8. **构建DFA**。DFA的存储结构我才用一个二维数组，第一个数表示前驱状态的编号，第二个数表示后继状态的编号，该点存储的是转换的符号。如：`I0 --S--> I1`则存储为`DFA[0][1] = "S"`。

9. **检查是否为SLR文法。**文法为SLR(1)文法应满足两个条件：

   1. 对于在s 中的任何项目A→α.Xβ，当X是一个终结符，且X在Follow (B) 中时，s 中没有完整的项目B→γ.。
   2. 对于在s 中的任何两个完整项目A→α.和B→β.，Follow (A) ∩Follow(B)为空。

   若第1个条件不满足，就表示这是一个移进-归约冲突 。若第2个条件不满足，就表示这是一个归约-归约冲突。所以只需要检查该文法是否同时满足这两个条件则可判断他是否为SLR文法。

10. 根据项目集规范族和FOLLOW集生成SLR分析表。

    ```c++
    if A->α.aβ∈Ii and GOTO(Ii, a)=Ij then ACTION[i, a]=sj
    if A->α.Bβ∈Ii and GOTO(Ii, B)=Ij then GOTO[i, B]=j
    if A->α.∈Ii且A≠S' then for ∀a∈FOLLOW(A) do
        ACTION[i, a]=rj (j是产生式A->α的编号)
    if S'->S.∈Ii then ACTION[i, $]=acc;
    ```

#### 四、实验小结

通过这次实验，了解到了SLR分析的过程和基本算法。