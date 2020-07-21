RayTracingPractice
#### 以PPM格式输出一个简单图像
本次实验用PPM格式的文件存储图像的信息，该格式文件大致分为三部分。
- 通常为P3或者P6指明PPM的编码格式
- 图像的宽度和高度
- 最大像素值，后面跟的是每个像素具体rgb值

一般看图软件不支持PPM格式，所以需要用支持PPM格式的软件打开存储的文件才能看到图像，本次实验用的IrfanView

生成一个最简单的图像：
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (1).jpg"  width = "30%"/>
</div>

#### 光线，背景
所有的ray tracers 都是以ray类为基础，计算颜色 $p(t) = A + t*B$ 其中$A$是光源点，$B$是ray的方向，$t$是具体float值，空间中确定一条线，不同的$t$，可以到达不同距离。
比如下图就表示了$C=p(2)$的光线，$t$越大打出的光能照射的距离越远。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (2).jpg"  width = "30%"/>
</div>

原理分析：  光线起点（也就是eye或者camera）固定的情况下，光线的方向向量的变动范围既形成光线束。光线束即是eye或者camera看到画面。  也就意味着：方向向量的变动范围决定着所能看到画面的范围。  另外，光线中每个光子的频率（颜色）决定这画面的内容。

要通过光线追踪画图的话，首先需要做两件事：
1. 确定光线的方向向量的活动范围
2. 对每一条光线设置颜色

Ray tracing的本质是通过发射射线，计算像素点的颜色。在ray tracing之前需要有个摄像机，建立坐标系，显示背景色，以及ray hit的点的颜色。假设摄像机的位置就是眼睛位置，看到的内容为ppm显示的东西，建立坐标系，$z$轴正方向，垂直平面向外，$x$向右，$y$向上。Ray tracing的本质是通过发射射线，计算像素点的颜色。在ray tracing之前需要有个摄像机，建立坐标系，显示背景色，以及ray hit的点的颜色。假设摄像机的位置就是眼睛位置，看到的内容为ppm显示的东西，建立坐标系，$z$轴正方向，垂直平面向外，$x$向右，$y$向上。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (3).jpg"  width = "40%"/>
</div>


交点坐标可以表示为向量：$lower\_left\_corner + u*horizontal + v*vertical$
颜色函数根据y坐标线性混合白色和红色：$blended\_value = (1-t)*start\_value + t*end\_value$


光线的方向向量 = 交点的向量 - 起点向量，由于起点为原点，所以，方向向量=交点向量。  每个交点的$u$，$v$的值即为该像素点在整个画面中的位置。

#### 绘制一个小球
加入一个球，球体公式:


$$x*x + y*y +z*z = R*R$$ 

对于任意$xyz$如果满足球面公式，$(x,y,z)$为球面的一个点。如果球心位置为$(cx,cy,cz)$,公式为:

$$
(x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) = R*R.
$$

用向量表示，球面点P，球心点C，半径可以表示为向量$PC$:

$$
dot((p-C)(p-C)) = (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz)
$$

等价于

$$
dot((A + t*B - C),(A + t*B - C)) = R*R
$$


展开之后
$$
t*t*dot(B,B) + 2*t*dot(A-C,A-C) + dot(C,C) - R*R = 0
$$

$ABC$已知，这里是一个关于$t$的一元二次方程：
- $a = (B·B)=dot(B,B)$
- $b=2*(B·(A-C))=2*dot(B,(A-C))$ 
- $c= (A-C)·(A-C) - R2=dot((A-C),(A-C))- R2$



根据判别式来判断根的情况对于$t$无解，有一个解，有两个解的情况，即为下左图。

如果光线和球有交点，那就给像素设置一个颜色，比如红色。如果没有碰上那就设置成背景色，如下右图。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (4).jpg"  width = "30%"/>
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (11).jpg"  width = "30%"/>
</div>



#### 绘制多个球
对球面点单位法向量进行着色。

法线是垂直与物体表面的一个向量，对于上一节提到的球，他的法线方向是从球心出发，射向hitpoint的。就像在地球上，地面的法向是从地心出发，射向你站立的点的。
假设N是长度在[-1，1]之间的单位向量，映射到[0,1]之间，再映射$x/y/z$到$r/g/b$。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (12).jpg"  width = "30%"/>
</div>


以上的画出的球是只考虑与光线相交最近的点，即跟较小的点。因为我们只考虑球不透明的情况。球的后面我们是看不见的。但是现在要画多个球，那一条光线可能和$N$个球相交那就可能有$2N$个交点。我们现在取根大于0，且最小的那个点。因为其余点可能是球的背面也可能被其他球遮挡。

设置颜色思路如下：（多个球的各自的颜色和背景颜色）

- 光线是否撞上球？是：设置为球的颜色；否：设置为背景颜色
- 如果光线撞上了球。$N$个球，可能有$2N$撞点，到底哪一个撞点是有效的呢？可以被最终设置颜色呢？原则是：找出最近的撞点。
- 怎么找？一个球一个球依次找。每个球可能有2个撞点，选出较近的有效的撞点。
- 然后，从所有这些每个球的较近的有效的撞点中，选出最近的那个撞点。


下图是增加一个绿色球，通过上述方法生成的图像：
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (13).jpg"  width = "30%"/>
</div>

#### 消除锯齿

上图渲染出来的结果方法后有明显的锯齿。因为之前是每个像素点设置一个颜色，如果两个像素颜色相差比较大，那就会有明显的锯齿。那么如何消除这些锯齿呢，准确来说不应该是消除，是如何柔和这些锯齿。让他们看起来不那么显眼。这里对求边缘的每个像素点内进行多次采样（进行光线追踪），然后平均色彩值，使得边缘模糊。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (1).png"  width = "20%"/>
</div>

每个像素点采样100次后得到的结果：
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (2).png"  width = "20%"/>
</div>


