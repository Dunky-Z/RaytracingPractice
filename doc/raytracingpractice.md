# RayTracingPractice
#### Output an image 以PPM格式输出一个简单图像
本次实验用PPM格式的文件存储图像的信息，该格式文件大致分为三部分。
- 通常为P3或者P6指明PPM的编码格式
- 图像的宽度和高度
- 最大像素值，后面跟的是每个像素具体rgb值

一般看图软件不支持PPM格式，所以需要用支持PPM格式的软件打开存储的文件才能看到图像，本次实验用的IrfanView

生成一个最简单的图像：
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (1).jpg"  width = "30%"/>
</div>

#### The vec3 class 熟悉三维坐标类
因为平时`Eigen`库用的比较多，配置到项目中也很快捷，所以坐标都是用的`Eigen`库，没有使用作者提供的`vec3.h`。但是可以学习一下C++基础知识。  
作者在该头文件中使用了大量的内联函数`inline`。因为内联函数不会引入任何函数调用，所以可以提高执行效率。但是也有一些限制，一般内联函数的实现不超过10行，也一般不会内联循环体。
```cpp
inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
```

作者的代码很规范，值得学习。习惯使用`const`关键字。
`const`关键字的作用：
    - 修饰变量，说明该变量不可以被改变；  
    - 修饰指针，分为指向常量的指针（pointer to const）和自身是常量的指针（常量指针，const pointer）；  
    - 修饰引用，指向常量的引用（reference to const），用于形参类型，即避免了拷贝，又避免了函数对值的修改；  
    - 修饰成员函数，说明该成员函数内不能修改成员变量。 

#### Rays, a simple camera, and background 光线 相机 背景
所有的ray tracers 都是以ray类为基础，计算颜色 $p(t) = A + t*B$ 其中$A$是光源点，$B$是ray的方向，$t$是具体float值，空间中确定一条线，不同的$t$，可以到达不同距离。
比如下图就表示了$C=p(2)$的光线，$t$越大打出的光能照射的距离越远。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (2).jpg"  width = "30%"/>
</div>

原理分析：  光线起点（也就是eye或者camera）固定的情况下，光线的方向向量的变动范围既形成光线束。光线束即是eye或者camera看到画面。  也就意味着：方向向量的变动范围决定着所能看到画面的范围。  另外，光线中每个光子的频率（颜色）决定这画面的内容。

要通过光线追踪画图的话，首先需要做两件事：
1. 确定光线的方向向量的活动范围
2. 对每一条光线设置颜色

Ray tracing的本质是通过发射射线，计算像素点的颜色。在ray tracing之前需要有个摄像机，建立坐标系，显示背景色，以及ray hit的点的颜色。假设摄像机的位置就是眼睛位置，看到的内容为ppm显示的东西，建立坐标系，$z$轴正方向，垂直平面向外，$x$向右，$y$向上。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (3).jpg"  width = "40%"/>
</div>


交点坐标可以表示为向量：`lower_left_corner + u*horizontal + v*vertical`
```cpp
lower_left_corner(-2.0, -1.0, -1.0);
horizontal(4.0, 0.0, 0.0);
vertical(0.0, 2.0, 0.0);
```
颜色函数根据y坐标线性混合白色和红色：`blended_value = (1-t)*start_value + t*end_value`

光线的方向向量 = 交点的向量 - 起点向量，由于起点为原点，所以，方向向量 = 交点向量。每个交点的$u$，$v$的值即为该像素点在整个画面中的位置。

#### Adding a sphere 绘制一个小球
加入一个球，球体公式:


$$x*x + y*y +z*z = R*R$$ 

对于任意$xyz$如果满足球面公式，$(x,y,z)$为球面的一个点。如果球心位置为$(cx,cy,cz)$,公式为:

$$
(x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) = R*R.
$$

用向量表示，球面点`P`，球心点`C`，半径可以表示为向量`PC`:

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



#### Surface normals and multiple objects 表面法向与绘制多个球
对球面点单位法向量进行着色。

法线是垂直与物体表面的一个向量，对于上一节提到的球，他的法线方向是从球心出发，射向`hitpoint`的。就像在地球上，地面的法向是从地心出发，射向你站立的点的。
假设N是长度在[-1，1]之间的单位向量，映射到[0,1]之间，再映射$x/y/z$到$r/g/b$。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (12).jpg"  width = "30%"/>
</div>


以上的画出的球是只考虑与光线相交最近的点，即跟较小的点。因为我们只考虑球不透明的情况。球的后面我们是看不见的。但是现在要画多个球，那一条光线可能和$N$个球相交那就可能有$2N$个交点。我们现在取根大于0，且最小的那个点。因为其余点可能是球的背面也可能被其他球遮挡。

设置颜色思路如下：（多个球的各自的颜色和背景颜色）

- 光线是否撞上球？
    - 是：设置为球的颜色；
    - 否：设置为背景颜色
- 怎么找？一个球一个球依次找。每个球可能有2个撞点，选出较近的有效的撞点。
- 从所有这些每个球的较近的有效的撞点中，选出最近的那个撞点。


下图是增加一个巨型绿色球在蓝色球下方，通过上述方法生成的图像：
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (13).jpg"  width = "30%"/>
</div>

#### Antialiasing 消除锯齿

上图渲染出来的结果方法后有明显的锯齿。因为之前是每个像素点设置一个颜色，如果两个像素颜色相差比较大，那就会有明显的锯齿。那么如何消除这些锯齿呢，准确来说不应该是消除，是如何柔和这些锯齿。让他们看起来不那么显眼。这里对求边缘的每个`像素点内进行多次采样`（进行光线追踪），然后`平均色彩值`，使得边缘模糊。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (1).png"  width = "20%"/>
</div>

每个像素点采样100次后得到的结果：
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (2).png"  width = "20%"/>
</div>


#### Diffuse Materials 漫反射
接下来为小球添加一些比较真实的材质。首先研究漫反射材料。漫反射材料自身不会发光，但是会吸收和反射环境光。包括其他物体反射出来的光。反射的光的特点是方向是随机的。
- 如何模拟随机方向？
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (5).jpg"  width = "70%"/>
</div>

在撞击点`P`处，已知法向量`PS`，如何产生一个随机的方向向量呢？借助辅助单位球`O`。球`O`圆心位于原点`(0,0,0)`。在球`O`内部找一个随机点`E`或者`F`。那么`OP + PS = OS, OS + OE = OM, OM - OP = PM`，由此可得`PM = OP + PS + OE - OP = PS + OE`。因为`PS`是已知的撞击点法向量，所以只需要获得一个`OE`即可。

- 如何产生随机的`OE`单位向量呢？
首先产生随机数$x \in {(0,1)}$：`x = (rand() % 100) / (float)(100)`
$\Rightarrow$ $2x \in{(0, 2)}$ $\Rightarrow$$2x-1 \in{(-1,1)}$$\Rightarrow$`2 * x - 1 = 2 * (rand() % 100) / (float)(100) - 1`。这是对于一维的情况，由这段代码就可以产生一个`(-1,1)`的随机浮点数。对于方向向量，三维的情况只需要对每一维随机产生一个浮点数即可，原理是一样的。
`a = (x, y, z)`,`a' = (2*x-1, 2*y-1, 2*z-1)`。因为`a`在单位球内，所以$x,y,z \in {(0,1)}$ $\Rightarrow$ $2x - 1,2y - 1,2z - 1 \in{(-1,1)}$$\Rightarrow$$(2x-1)^2 + (2y-1)^2 + (2z-1)^2 \in{(0,3)}$$\Rightarrow$$|a'| \in{(0,\sqrt{3})}$
因为我们只需要大小为$(0,1)$之间的向量，所以就采用原文中提到的rejection method。就是只取目标范围呢的，超过$(0,1)$的就丢弃，重新获取。
```cpp
RowVector3d random_in_unit_disk() {
	RowVector3d p;
	do
	{
		p = 2.0*RowVector3d((raznd() % (100) / (double)(100)),
         (rand() % (100) / (double)(100)), 0) - RowVector3d(1, 1, 0);
	} while (p.dot(p) >= 1.0);
	return p;
}
```
- 如何模拟吸收光线？
光线在反射过程中会发生衰减，每撞击一次，光线颜色衰减一半。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (8).jpg"  width = "40%"/>
</div>
第一次渲染的结果看起来很不正常。因为浮点数存储精度的问题，求根的时候，并不是精确的0，但是代码中根不为0就意味着有相交。所以导致光线会在交点处漫反射很多次。只要将判断根的大小条件修改一下就可以，忽略特别小的根。
<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (9).jpg"  width = "40%"/>
</div>

#### Metal 金属材质 镜面反射
在一个场景里不仅有漫反射，还有镜面反射，折射等。现在来分析如何模拟镜面发射。

要模拟镜面发射，最重要的就是计算镜面反射的反射光线的方向向量。

<div  align="center">    
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/jinmianfanshe.jpg"  width = "30%"/>
</div>


假设光线$V$与交点法向量$N$的夹角为$\theta$，反射光线的方向向量$F=V+2B$

$$
\vert B \vert = \vert V \vert \cos\theta\\
V \cdot N = \vert V \vert \vert N \vert \cos(\pi - \alpha)\\
\cos\theta = -\frac{V \cdot N}{\vert N \vert}=-V\cdot N\\
\vert B \vert = -\frac{V\cdot N}{\vert N \vert} = -V\cdot N\\
B = -(V\cdot N)\times N\\
F = V - 2\times (V\cdot N) \times N
$$

除了计算反射向量以外，还有一点需要考虑--衰减系数。
衰减系数的作用是什么呢？一方面，每次反射后的光强度肯定是弱了；另一方面每反射一次都会加入被撞物体的颜色。我们可以这样理解：像漫反射球，原始光线碰撞漫反射球后经过多次无规则的反射进入环境，漫反射球的颜色就是环境颜色（最后一次反射光线方向向量的映射）和所有被碰撞到的其他物体颜色（衰减向量中隐含该信息）的叠加。

镜面反射的清晰度可以加一个模糊系数调节。
我们可以在反射向量的方向向量上添加一个“起点在原点，长度小于1，方向随机”的向量（和漫反射的随机向量一样 ）。



<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/sphere1.jpg"  width = "30%"/> 
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/sphere2.jpg"  width = "30%"/>
</div>


#### Dielectrics 模拟一些介质的折射
有一定透明度的物体会发生折射现象。模拟折射就得知道折射光的方向。现在的目的是怎样能用入射光线和法线的信息求得折射光的方向。
<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/4869464adsa (3).png"  width = "30%"/>
</div>

设：
$$
I' = \vert I'\vert I , N' = \vert N' \vert N\\
\text{令}\frac{\sin\theta_2}{\sin\theta_1}=S 
$$
由图得:
$$
 K = \frac{\vert O \vert \sin\theta_2}{\vert I' \vert \sin\theta_1} \tag{1}
$$
$$
\vert I'\vert \cos\theta_1 = \vert N'\vert = \vert O \vert \cos \theta_2 
$$
为方便计算$\vert O \vert =1 $
$$
\Rightarrow  \vert I' \vert = \vert O\vert \frac{\cos\theta_2}{\cos\theta_1} = \frac{\cos\theta_2}{\cos\theta_1} 
 \tag{2}$$
$$
\vert O\vert = -N'+K[I'-(-N')] \\
			 = KI'+(K-1)N'\\
			 =K\vert I'\vert +(K-1)\vert N' \vert N \tag{3}
$$

将(1),(2)带入(3)得：

$$
\vert O \vert = \frac{\sin\theta_2}{\sin\theta_1}I+[\frac{\sin\theta_2}{\sin\theta_1}\cos\theta_1-\cos\theta_2]N \tag{4}
$$

由$\frac{\sin\theta_2}{\sin\theta_1}=S$得
$$
\sin^2\theta_2= S^2\sin^2\theta_1 \tag{5}
$$
$$
\Rightarrow 1-\cos^2\theta_2 = S^2(1-\cos^2\theta_1)  \\
\Rightarrow \cos\theta_2=\sqrt{1+S^2(\cos^2\theta_1-1)} \tag{6}
$$

将（6）带入（5）得 
$$
\vert O \vert = SI+[S\cos\theta_1 - \sqrt{1+S^2(\cos^2\theta_1-1)}]N \tag{7}
$$
由图可得：
$$
\cos\theta_1 = -\frac{IN}{\vert I\vert \vert N \vert} \tag{8}
$$
若I，N都为单位向量。则
$$
\cos\theta_1=-IN \tag{9}
$$
将（9）带入（3）得
$$
\vert O \vert = SI+[-S(I \cdot N) - \sqrt{1+S^2((I \cdot N)-1)}]N
$$
介质球的半径为负数，它的几何形状是不受影响的，但表面法线点向内，所以它可以作来制造一个空心的玻璃球。
<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/4869464adsa (5).jpg"  width = "30%"/>
</div>

由于折射的原因，透过玻璃球看到的像的位置和玻璃球后景的位置是上下颠倒的。

#### Positionable camera 相机的位置
在此之前，我们的相机都是放在坐标系原点。接下来尝试移动相机，看看不同的位置会有怎样的变化。当我们尝试移动相机时，我们就要考虑到几个参数，移动到哪，看向何方，视角多大？
- lookfrom
相机所放的位置
- lookat
相机视线看的点
- vup
为了确定相机平面的基向量；
相机平面就是与`lookat-lookfrom`向量垂直的那个平面。类似于世界坐标系，确定一个相机坐标系也需要正交基向量，而相机坐标系一般yong`uvw`。在确定`uvw`前，要先确定一个垂直向上的向量。因为相机坐标系局部的，将它放在世界坐标系中时，就需要一个始终指向世界坐标系正向上方向的基向量，这个基向量就是`vup`。
接下来就开始建立相机坐标系，首先确定一个`w`向量。它正交于相机平面。`w = lookfrom - lookat`。由公式可以看出，它就是和视线方向相反的一个基向量。接下来确定`u`向量，它代表与相机平面平行想向量。`w`垂直于相机平面，所以`w⊥u`。它始终与世界坐标系`x`方向平行，而`vup∥y`。所以`vup⊥u`。因此得治，`u`同时垂直于`w`和`vup`。我们知道，向量叉乘的几何意义之一就是，得到与这两个向量垂直的向量。因此`w = vup x w`。最后定义向量`v`。它一定与另外两个向量`w,u`垂直，所以很容易得到`v = w x u`。
<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/20200722210205.png"  width = "50%"/>
</div>
<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/20200722210932.png"  width = "50%"/>
</div>

- vfov
它是一个角度，它分为两种：
    - 垂直方向岔开的角度（vfov）
    vfov即相机在垂直方向上从屏幕顶端扫描到底部所岔开的视角角度
    - 水平方向岔开的角度（hfov）
    hfov即相机在水平方向上从屏幕左端扫描到右端所岔开的视角角度

- aspect 屏幕宽高比
我们之前是通过直接定义屏幕的坐标位置来确定屏幕，现在，我们可以通过相机参数来确定屏幕。目前，相机在原点，屏幕中心在（0,0，-1）  
眼睛离屏幕中心的距离为dis(也就是1)。根据`tan(vfov/2) = (屏幕高/2)/dis`得到
`屏幕高 = 2 * dis * tan(vfov/2)`，则，
`屏幕上边界为 y_up = dis * tan(vfov/2)`
`屏幕下边界为 y_bottom = - y_up`　
`屏幕宽 = 屏幕高 * aspect`，则，
`屏幕左边界为 x_left = - 屏幕宽/2`
`屏幕右边界为 x_right = - x_left`
 
<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/4869464adsa (1).jpg"  width = "50%"/>
</div>
<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/4869464adsa (2).jpg"  width = "50%"/>
</div>

#### Defocus Blur 散焦模糊 更大的光圈看时间
如果玩过摄影，应该知道，大光圈就会导致景深小。景深小拍出的画面就是我们常看到的人像照片。前景的人像很清晰，背景会被模糊掉。在光线追踪中，光圈的增大，导致图片出现散焦而模糊。但是我们可以调节成像的位置，改变图片的清晰度，使图片在当前光圈条件下尽可能清晰。

所以，此时相机就得再加入两个参数`aperture`（光圈）和`focus_dist`（成像位置）。
一个真实的相机成像，需要三个部分，`film`胶片，`len`镜头，`aperture`光圈。而模拟成像，只需要去`outside`部分，因为我们并没有实际的胶片成像。相机镜头在接受光线时不时只有一束光进入镜头，所以我们模拟的镜头不再是一束光。
![](https://gitee.com//dominic_z/markdown_picbed/raw/master/img/20200722215838.png)
将光圈看做是镜头，从镜头上随机发射光线。
![](https://gitee.com//dominic_z/markdown_picbed/raw/master/img/20200722220540.png)

<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/4869464adsa (3).jpg"  width = "50%"/>
</div>

#### Where next? 最后的最后

<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/4869464adsa (4).jpg"  width = "50%"/>
</div>

<div  align="center">   
<img src="https://gitee.com//dominic_z/markdown_picbed/raw/master/img/45645161 (10).jpg"  width = "50%"/>
</div>
