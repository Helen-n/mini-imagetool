#### mini-imagetool
之前在学习一些图像相关的知识的时候，学习了opencv,所以当时花了一些时间做一些相关的练习，看了opencv的源码和一些结构，所以试着找了一些图像的应用进行实现,然后也仿照opencv进行了简单的封装，支持DLL调用。  
#### 背景
随着现在人们对美丽的向往，很多和美相关的算法和软件也随着产生。其中大家熟知的廋脸，美白，去痘痘，磨皮等美颜算法广泛的应用。
所以我之前花了一些时间去复现了其中的一部分的功能。包括基于face++ landmark点的廋脸，瘦鼻子等，还有基于局部区域美白和全局的美白，点范围祛斑，祛痘等。

这个简单工具主要具有以下比较好的点：
- 1 可以进行点和线图像形变，支持设置形变范围 
- 2 可以进行点和局部的图像修复
- 3 结合opencv可以跨平台使用
- 4 使用c++和平滑滤波速度和效果都还可以

#### 主要部分
  
```
    // 图像修复的LBP的方式：灰度，等价
	enum LbpType 
	{
		LBP_GRAY = 0,
	    LBP_UNIFORM =1
	};
	// 瘦脸的方式：全脸，左脸，右脸
	enum WarperFaceType
	{
		FACE_LEFT = 0,
		FACE_RIGHT = 1,
		FACE_ALL = 2
	};

```
主要分成：
- base,cdef,cstruct等基础类
- Blur,Inpaint是一些算法功能实现类
- ManageXX是指算法类封装
#### 一些原理
- LBP 修复
![image](https://raw.githubusercontent.com/Helen-n/mini-imagetool/master/images/lbp.jpg)
然后比较范围该点与局部其他点的差异，红色mask点内的点选择LBP纹理相似度进行替换掉进行修复
![image](https://raw.githubusercontent.com/Helen-n/mini-imagetool/master/images/lbp.gif)
- 图像形变
  - 图像偏移
    - c,s表示要移到的位置和起始位置， d表示范围半径
   ```math
   p << p+(\frac{||p-s||}d -1)(c-s)
   ```
     
    - 图像缩放
      - r表示缩放， 大于0表示放大，小于0表示缩小
    ```math
     p << c+(\frac{||p-c||}d -1)^r(c-s)
    ```
    ![image](https://raw.githubusercontent.com/Helen-n/mini-imagetool/master/images/wrap.jpg)
    - a 偏移 b-放大 c-缩小
##### 注意：
廋脸等需要脸部点的功能需要结合face++ 接口的landmark点先进行插值，进行偏移，但现在代码尚未集成，需要使用的话，需要给struct landmark进行传值   
还尚未完成，有些需要再测试
##### reference
[基于本地网格的图像变形器](https://www.codeproject.com/Articles/182242/Transforming-Images-for-Fun-A-Local-Grid-based-Ima)   
[LBP](https://blog.csdn.net/shenziheng1/article/details/72582197)   
[opencv](https://opencv.org/)



