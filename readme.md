终于写好这个游戏咯~~好好玩啊·

一直都很像写个数独游戏，这次终于完成了~~很有成就感

游戏还没有彻底完成，还有一些地方可以完善~

这些我想等到下一版本时再推出~~这样可以让自己有进步~呵呵



游戏编程还真的很有意思，根据自己的思路，想法

完成一个属于自己的游戏~~这过程真的很享受

虽然有时也会很痛苦，调试啊，突然出错啊，等等的·

记得这次在快完成的时候，debug版明明非常正常，想换到release版，

结果是0 errors, 0 warning，可以！！运行的时候居然一开始程序就死了··

这可直接把我雷到了········

检查了好久啊··因为代码不短，不好发现，

心慌地上网查资料，看debug和release版的区别啊之类··还是搞不定·

最后··我无意间发现开始程序时有某个整形变量（记录游戏状态）没初始化··我把它初始化后·

一切居然都正常了····天啊·经过这次，也告诫大家，

记得常常编译下release版，不然等代码臃肿起来后，可不好检错啊。



呵呵~~~这次蛮满意的是界面，虽达不到一般商业游戏的水平，

但自己看看还是不错的，怎么说也花了我好多心思，

每一张都是自己用fireworks做的或者是合成的~~而且

兔斯基版本的数独，我觉得很配啊，很有感觉。

可以说，这次的编程与做图的时间是一半一半。



对游戏编程有兴趣的朋友可以联系我啊

或者你对这个程序有什么意见看法也可以

本人的联系方式：E-Mail: gouki04@126.com

欢迎大家多多支持··



如果你对本程序有兴趣，想去看懂这个程序的，可以接着看，否则就不要看了

=========================================================================

## 类简介：
1. 程序的主类是CMapManager类，与所有的类关联，基本上所有数据和操作都保存在这里。

2. 实现图片的显示，合成的是CDibSection类，主要用于载入位图和输出位图。

3. CSprite类可以说是封装了CDibSection类的操作，简化了位图的显示以及合成操作。


## 程序算法简介：
1. 设定窗口大小为固定的288*288，不可最大化。方面实现定位显示的操作。

2. 整个窗口其实是被分为9*9个小格（这也就是注释里格子的含义，是格子坐标，同时也可以看出，9*9正是数独的大小），每个格子大小均为32*32像素。这样定位操作就更简单了。程序里用的图片都严格遵守了这个规格。每个图片的大小都是由N个这样的格子组成。大家可以用画图软件打开游戏的图片，再设定网格为32*32，即可以看出，每一个按钮或光标可移动的位置，都是在一个或几个格子之内，这样定位就更加简单了。

3. 程序里的所有事件（包括鼠标事件，键盘事件）都是通过Process函数来处理，它根据状态和事件来分配不同的事件处理函数，有人会问，为什么不把鼠标单击事件都写在同一个函数里，反而要根据状态的不同而分别写多个函数，这样是麻烦了，但其实，到游戏的状态增多的时候，更改函数比添加函数来说，更加会让程序的复杂度增加和更加容易出错，特别是要更新程序的时候，要对某个状态进行更改，只要改响应函数即可，不会更改到其他状态的函数，程序健壮性就提高了。

4. 这里简单介绍图片处理类CDibSection类，该类重用性很大，如果有需要，可以直接装到自己的程序中。而且该类提供的接口不错，推荐大家可以试用一下。

    1. CDibSection类，主要是Copy函数和Mix函数，同样是图片合成功能，但Copy是简单的图片复制（直接用API函数BitBlt），而Mix则是加上了对透明色的操作，实现部分复制（相当于底色透明），因为bmp没有真正的透明色，所以要预设某一颜色为透明色（这种颜色一定要很少用到），这里默认使用的是RGB（0，255，0）亮绿色，所以看到所有图片的底色都是亮绿色的。

5. 程序里比较巧的地方有两点：

    1. 第一点是Map数组的应用，可以看到，该二维数组保存了数独各位置的数字，然而，在玩家输入数字后，我们修改的是Map的数据，并不是输出数字。但实际上，因为输出是与Map数组关联的，这样给人的错觉就是：我改了Map相应位置的数字，画面就有相应的改变，所以不用管数字的显示与更新，只要关注于这个数组就可以了。

    2. 第二点就是程序里将处理和显示分开，这样的好处很多，很多时候，在处理的同时进行画面的显示，这样比较符合逻辑，但代码却会显得很臃肿，不好修改，因为改处理的同时，相应的显示也要修改。程序的大概思想就是这样，Show函数主管显示（这里的显示不是重绘全部图片，这样太费时，只重绘要重绘的地方），在Process结束的时候调用Show，这样每当有事件发生后都重绘一次，这样就不用再处理时加入显示部分。当然，这种办法不能解决所有问题，某些时候还是要在处理时加入显示。

6. 程序在进行答案正确性时，自己想了一个不错的方法，因为是位运算，速度有保证。
  下面以检验行正确性为例说明。
```c
	DWORD check = 0x01FF;
	DWORD tmp = 0;
	for (int i=0; i<9; i++)
	{
		tmp = 1 << (Map[row][i].num - 1);
		check &= ~tmp;
	}
	return check;
```
    如果某一行是正确的，那它必然包括123456789，9个数字，且每个数字出现且仅出现一次
    这里采用位与运算检查
    其中check = 0000 0001 1111 1111,后9位为1
    假设当前位置的数字为3，则tmp = 1 << 2 = 0000 0000 0000 0100
    则check &= ~tmp =
    0000 0001 1111 1111 & 
    1111 1111 1111 1011 =  
    0000 0001 1111 1011,则第3位变为0，这样只有当9个数刚好是1-9的时候，check才为0
    其他检验列和小九宫格的正确性的方法本质跟这个一样。

7. 按钮按下效果的实现，这个最简单，用两个图就行了，一大一小的，按下时显示小的，按下后显示大的，看上去就像按钮被按下去了。

8. 程序的注释大部分是用VC小助手自动生成的。这次但自己也写不少注释，值得嘉贺。

还有什么不懂的就直接发邮件问我吧。批评建议也可以···
