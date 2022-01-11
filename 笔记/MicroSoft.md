# MS开发tips

## 格式要求

1. csharp文件头部、函数前均需要注释，parameter注释中间需要有空格

```c#
// ---------------------------------------------------------------------------
// <copyright file="GenLoadWorkflow.cs" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
// ---------------------------------------------------------------------------

namespace Microsoft.Office.Datacenter.DirectoryWorkflow.Workflows
{

        /// <summary>
        /// Main entry point of the workflow
        /// </summary>
        /// <param name="runtime">IWorkflowRuntime instance</param>
        /// <returns>The default continuation</returns>
```

2. while if等关键字后，须有空格

3. 同步方法转异步

```c#
        public void Start(DirProxyTestOptions op)
        {
            Func<Task<DirProxyTestResult>> func = () =>
            {
                DirProxyTestResult result = DirProxyTestResult.GenResult(op);
                Console.Write(result);
                return Task.FromResult(result);
            };

            _ = Task.Run(func);
        }
    }
```

## 开发步骤

1. DMS使用命令
   1. Update-TorusClient 16.00.8671.000 更新版本，其中版本号在pr上能找到
   2. Get-TorusClientVersion 获取当前版本
   3. Request-ADLoadGeneration.ps1 -TargetMachines SN6SPR01MB0055 -Induration 00:00:01 -EmailTo t-yuhangdu@microsoft.com -Duration 00:01:00 -RunType FuncLdap -NumThreads 3 -QPS -1 -Label "test1"  生成对应task
   4. Get-OperationTrace.ps1 \\YT1MU02\a039fad0-d531-4c54-bc9c-4f1cfe34f803 获取task信息
2. csproj中项目引用方法
   1. 不能直接引用本地路径，需要去特定repo中查询相关dll路径，直接复制然后getdeps

# C#语法

## Barrier简单介绍

1. barrier的注释为：使多个任务能够采用并行方式依据某种算法在多个阶段中协同工作。

```c#
 static void Main(string[] args)
 {
     Barrier barrier = new Barrier(4, it => {
         Console.WriteLine("再次集结，友谊万岁，再次开跑");
     });
     string[] names = { "张三", "李四", "王五", "赵六" };
     Random random = new Random();
     foreach(string name in names)
     {
         Task.Run(() => {
             Console.WriteLine($"{name}开始跑");
             int t = random.Next(1, 10);
             Thread.Sleep(t * 1000);
             Console.WriteLine($"{name}用时{t}秒，跑到友谊集结点");
             barrier.SignalAndWait();
             Console.WriteLine($"友谊万岁，{name}重新开始跑");
         });
     }
     Console.ReadKey();
 }
```

2. 执行结果为：

```c++
张三开始跑
李四开始跑
王五开始跑
赵六开始跑
王五用时4秒，跑到友谊集结点
赵六用时6秒，跑到友谊集结点
张三用时7秒，跑到友谊集结点
李四用时7秒，跑到友谊集结点
再次集结，友谊万岁，再次开跑
友谊万岁，李四重新开始跑
友谊万岁，王五重新开始跑
友谊万岁，赵六重新开始跑
友谊万岁，张三重新开始跑
```

3. 简单来说，假定有一个4人参加的友谊赛，4人开始跑，有的人跑的快，有的人跑的慢，但是都会在SignalAndWait处停下来，等4个人都到达SignalAndWait处后，又都开始继续往下执行了。

## StopWatch的使用（计时、时间间隔）

1. Stopwatch 可以测量一个时间间隔的运行时间，也可以测量多个时间间隔的总运行时间。一般用来测量代码执行所用的时间或者计算性能数据，在优化代码性能上可以使用Stopwatch来测量时间。
2. 引用的空间名:使用的时候需要引用 System.Diagnostics 命名空间。
3. 先调用 Start 方法，然后调用 Stop 方法，最后使用 Elapsed 属性或者使用 ElapsedMilliseconds 属性得到运行时间（这两个属性的区别是前者得到的是TimeSpan实例，后者得到的是毫秒）。使用 IsRunning 可以确定 Stopwatch 的当前状态是正在运行还是已经停止。每次调用 Start 时开始累计运行时间计数；每次调用 Stop 时结束当前时间间隔测量，并冻结累计运行时间值。 使用 Reset 方法可以清除现有 Stopwatch 实例中的累计运行时间。

```c#
Stopwatch stopwatch = new Stopwatch();
            //第一次计时
            stopwatch.Start();  //启动Stopwatch
            Console.WriteLine("Stopwatch is running:{0}",stopwatch.IsRunning);//获取当前Stopwatch的状态
            System.Threading.Thread.Sleep(2000);//耗时操作
            stopwatch.Stop();  //停止Stopwatch
            Console.WriteLine("Using Elapsed output runTime:{0}", stopwatch.Elapsed.ToString());//这里使用时间差来输出,如：时：分：秒
            Console.WriteLine("Using ElapsedMilliseconds output runTime:{0}", stopwatch.ElapsedMilliseconds);//这里面使用毫秒来输出
            Console.WriteLine("===================================================");
            //第二次计时
            stopwatch.Start();
            System.Threading.Thread.Sleep(1000);//耗时操作
            stopwatch.Stop();
            Console.WriteLine("The second RunTime:{0}", stopwatch.ElapsedMilliseconds);//这里面使用毫秒来输出
            Console.WriteLine("===================================================");
            //第三次计时（这里使用了Restart）
            stopwatch.Restart();//这里使用Restart来启动计时（会把前面的时间清空）
            System.Threading.Thread.Sleep(1000);//耗时操作
            stopwatch.Stop();
            Console.WriteLine("After Restart, so runTime:{0}", stopwatch.ElapsedMilliseconds);//这里面使用毫秒来输出
            Console.ReadKey();  //等待输入
```

## region的使用

1. #region是c#预处理器指令。
   #region是一个分块预处理命令，它主要用于编辑代码的分段，在编译时会被自动删除。
   #region使你可以在使用VS代码编辑器的大纲显示功能时指定可展开或折叠的代码块。

```c#
#region  MyClass definition

public class MyClass 
{
    static void Main() 
    {
    }
}
#endregion
```

2. #region块必须以#endregion指令终止。
   #region块不能与#if块重叠。但是，可以将#region块嵌套在#if块内，或将#if块嵌套在#region块内。

## dispose的使用

1. C#中的资源
在我们的程序中，使用资源后，需要释放。那么在C#中的每一种资源，可以分为两类：

- 托管资源：由CLR管理分配和释放的资源，即由CLR里new出来的对象；
- 非托管资源：不受CLR管理的对象，windows内核对象，如文件、数据库连接、套接字、COM对象等；

在我们的程序中，使用到了非托管资源，或者托管资源，最后都需要释放。针对托管资源，DotNet的垃圾回收器会自动地回收托管资源，而非托管的资源，则需要自己进行处理。

那么，我们可以使用C#的Dispose模式来方便地释放这些资源。

    <Reference Include="$(PkgMicrosoft_Skype_ECS_Core)\lib\net472\Microsoft.Skype.ECS.Core.dll" />

**---------------------------------------------------------------------------------------**

## HashSet

1. .NET 3.5在System.Collections.Generic命名空间中包含一个新的集合类：HashSet<T>。这个集合类包含不重复项的无序列表。这种集合称为“集(set)”。集是一个保留字，所以该类有另一个名称HashSet<T>。这个名称很容易理解，因为这个集合基于散列值，插入元素的操作非常快，不需要像List<T>类那样重排集合。
    HashSet<T>类提供的方法可以创建合集和交集。表10-12列出了改变集的值的方法。
2. HashSet<T>的修改方法　　　　　　　　　　说    明

    Add()　　　　　　　　　　　　　　　　　　 如果某元素不在集合中，Add()方法就把该元素添加到集合中。在其返回值Boolean中，返回元素是否添加的信息

    Clear()　　　　　　　　　　　　　　　　　　方法Clear()删除集合中的所有元素

    Remove()　　　　　　　　　　　　　　　　  Remove()方法删除指定的元素

    RemoveWhere()　　　　　　　　　　　　　 RemoveWhere()方法需要一个Predicate<T>委托作为参数。删除满足谓词条件的所有元素

    CopyTo()     　　　　　　　　　　　　　　   CopyTo()把集合中的元素复制到一个数组中

    ExceptWith()　　　　　　　　　　　　　　   ExceptWith()方法把一个集合作为参数，从集中删除该集合中的所有元素

    IntersectWith() 　　　　　　　　　　　　　  IntersectWith()修改了集，仅包含所传送的集合和集中都有的元素

    UnionWith()   　　　　　　　　　　　　　　  UnionWith()方法把传送为参数的集合中的所有元素添加到集中

3. HashSet<T>的验证方法 　　　　　　　　　　说    明

    Contains()　　　　　　　　　　　　　　　　 如果所传送的元素在集合中，方法Contains()就返回true

    IsSubsetOf()   　　　　　　　　　　　　　　如果参数传送的集合是集的一个子集，方法IsSubsetOf()就返回true

    IsSupersetOf()　　　　　　　　　　　　　　如果参数传送的集合是集的一个超集，方法IsSupersetOf()就返回true

    Overlaps()　　　　　　　　　　　　　　　　 如果参数传送的集合中至少有一个元素与集中的元素相同，Overlaps()就返回true

    SetEquals()　　　　　　　　　　　　　　　　如果参数传送的集合和集包含相同的元素，方法SetEquals()就返回true

4. 在示例代码中，创建了3个字符串类型的新集，并用一级方程式汽车填充。HashSet<T>类实现了ICollection<T>接口。但是在该类中，Add()方法是显式实现的，还提供了另一个Add()方法。Add()方法的区别是返回类型，它返回一个布尔值，说明是否添加了元素。如果该元素已经在集中，就不添加它，并返回false。

```c#
HashSet < string > companyTeams =new HashSet < string > (){ "Ferrari", "McLaren", "Toyota", "BMW","Renault", "Honda" };

HashSet < string > traditionalTeams =new HashSet < string > (){ "Ferrari", "McLaren" };

HashSet < string > privateTeams =new HashSet < string > (){ "Red Bull", "Toro Rosso", "Spyker","Super Aguri" };

if (privateTeams.Add("Williams"))
    Console.WriteLine("Williams added");
if (!companyTeams.Add("McLaren"))
    Console.WriteLine("McLaren was already in this set");
```

5. ExceptWith
    1. A与B没有包含关系时， 用A.ExceptWith(B)将B中出现过的元素删除

```c#
HashSet<int> lowNumbers = new HashSet<int>();
HashSet<int> highNumbers = new HashSet<int>();

for (int i = 0; i < 6; i++)
{
    lowNumbers.Add(i);
}

for (int i = 3; i < 10; i++)
{
    highNumbers.Add(i);
}

Console.Write("lowNumbers contains {0} elements: ", lowNumbers.Count);
DisplaySet(lowNumbers);

Console.Write("highNumbers contains {0} elements: ", highNumbers.Count);
DisplaySet(highNumbers);

Console.WriteLine("highNumbers ExceptWith lowNumbers...");
highNumbers.ExceptWith(lowNumbers);

Console.Write("highNumbers contains {0} elements: ", highNumbers.Count);
DisplaySet(highNumbers);

void DisplaySet(HashSet<int> set)
{
    Console.Write("{");
    foreach (int i in set)
    {
        Console.Write(" {0}", i);
    }
    Console.WriteLine(" }");
}

/* This example provides output similar to the following:
* lowNumbers contains 6 elements: { 0 1 2 3 4 5 }
* highNumbers contains 7 elements: { 3 4 5 6 7 8 9 }
* highNumbers ExceptWith lowNumbers...
* highNumbers contains 4 elements: { 6 7 8 9 }
*/
```

## Dictionary(HashMap)

1. IDictionary<type, type> numberNames = new Dictionary<type, type>();
2. 方法：Add()，Clear()，ContainsKey()，ContainsValue() ，Equals()，GetType()，Remove()

```c#
using System;
using System.Collections.Generic;
public class Program {
   public static void Main() {
  
IDictionary<int, string> flowerNames = new Dictionary<int, string>();
flowerNames.Add(1,"Rose");
flowerNames.Add(2,"Jasmine");
flowerNames.Add(3,"Lili");
 

foreach(KeyValuePair<int, string> kvp in flowerNames)
    Console.WriteLine("Key: {0}, Value: {1}", kvp.Key, kvp.Value);
   }
}
```

## 基于范围的for循环

1. foreach ( var val in vector)
