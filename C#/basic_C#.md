# Barrier简单介绍

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
    ```
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

# StopWatch的使用（计时、时间间隔）
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