using System;
using System.Threading;

namespace TestSystem
{
    class Program
    {
        static int SLEEP_TIME = 300;
        static int NUM_REPETITIONS = 20;

        static void Main(string[] args)
        {
            Program p = new Program();
            p.bar();

            for (int i = 0; i < NUM_REPETITIONS; i++)
            {
                if (i % 2 == 0)
                {
                    p.doSomething();
                } else
                {
                    p.doSomeStuff();
                }

                Thread.Sleep(SLEEP_TIME);
            }
        }

        void doSomething()
        {
            sayHello();
        }

        void doSomeStuff()
        {
            say("Hi");
            say("What's up?");
            say("Computed: " + foo(1, 42));
            Thread.Sleep(SLEEP_TIME);
        }

        void sayHello()
        {
            say("Hello");
        }

        void say(string message)
        {
            Console.WriteLine(message);
            Thread.Sleep(SLEEP_TIME / 2);
        }

        public string foo(int i, long l)
        {
            Thread.Sleep(SLEEP_TIME);
            return "int: " + i + ", long: " + l;
        }

        public Program bar()
        {
            return this;
        }

    }
}
