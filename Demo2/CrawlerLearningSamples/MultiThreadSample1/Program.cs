using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace MultiThreadSample1
{
    class Program
    {
        static long Sum = 0;
        static object mylock;
        static void Main(string[] args)
        {
            long n = 0;
            long s1 = DateTime.Now.Ticks;
            for (int i = 0; i <= 100; i++)
            {
                n += i;
                Thread.Sleep(10);
            }
            long s2 = DateTime.Now.Ticks;
            Console.Write("----------------single thread-------------------\nsum:" + n + "\n\rTime(ns):" + (s2 - s1) + "\n");
            Console.Write("----------------multi thread-------------------\n");
            Thread t1 = new Thread(add1);
            Thread t2 = new Thread(add2);
            mylock = new object();
            s1 = DateTime.Now.Ticks;
            t1.Start();
            t2.Start();
            t1.Join();
            t2.Join();
            s2 = DateTime.Now.Ticks;
            Console.Write("sum:" + n + "\n\rTime(ns):" + (s2 - s1));
            Console.ReadKey();
        }
        static void add1()
        {
            long n = 0;
            for (int i = 0; i <= 50; i++)
            {
                n += i;
                Thread.Sleep(10);
            }
            Monitor.Enter(mylock);
            Sum += n;
            Console.Write("thread 1 finish, Sum:" + Sum + "\n");
            Monitor.Exit(mylock);


        }
        static void add2()
        {
            long n = 0;
            for (int i = 51; i <= 100; i++)
            {
                n += i;
                Thread.Sleep(10);
            }
            Monitor.Enter(mylock);
            Sum += n;
            Console.Write("thread 2 finish, Sum:" + Sum + "\n");
            Monitor.Exit(mylock);

        }
    }
}

