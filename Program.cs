using System;
using System.Runtime.Serialization.Formatters;

namespace ConsoleApp1
{
    interface IArray
    {
        /// <summary>
        /// добавляет новый элемент в конец массива
        /// </summary>
        /// <param name="newValue">новый элемент</param>
        void Add(int newValue);
        /// <summary>
        /// находит элемент по индексу
        /// </summary>
        /// <param name="index">индекс искомого элемента</param>
        /// <returns>значение элемента</returns>
        int Find(int index);
        /// <summary>
        /// удаляет элемент по индексу
        /// </summary>
        /// <param name="index">индекс удаляемого элемента</param>
        void Remove(int index);
        void Update(int index, int newValue);
        public void Print();
    }
    class MyIntArray: IArray 
    {
        private int[] storage;
        private int size;
        public MyIntArray(int size)
        {
            this.storage = new int[size];
            this.size = size;
        }
        public MyIntArray(int[] arr)
        {
            this.storage = arr;
            this.size = arr.Length;
        }
        public void Add(int newValue)
        {
            int[] temp = storage;
            storage = new int[temp.Length+1];
            storage[storage.Length-1]= newValue;
            for (int i = 0; i < temp.Length; i++)
                storage[i] = temp[i];
            size++;
        }
        public int Find(int index)
        {
            for (int i = 0; i < size; i++)
                if (i == index)
                    return storage[i];
            Console.WriteLine("There is no such position!");
            return 0;
        }
        public void Remove(int index)
        {
            if (index < 0 || index >= this.size) 
            {
                Console.WriteLine("There is no such position!");
                return;
            }
            int[] temp = storage;
            storage = new int[temp.Length -1];
            for (int i = 0; i < index; i++)
                storage[i] = temp[i];
            for (int i = index + 1; i < temp.Length; i++)
                storage[i - 1] = temp[i];
            size--;
        }
        public void Update(int index, int newValue)
        {
            if(index>=this.size||index<0)
            {
                Console.WriteLine("There is no such position!");
                return;
            }
            for (int i = 0; i < size; i++)
                if (i == index)
                    storage[i] = newValue;
        }
        public void Print()
        {
            for (int i = 0; i < storage.Length; i++)
                Console.Write(storage[i].ToString() + " ");
            Console.WriteLine();
        }
    }
    class Program                                                                                   
    {
        static void Main(string[] args)
        {
            IArray arr = new MyIntArray(5);
            for (int i = 0; i < 5; i++)
                arr.Update(i, i);
            arr.Print();
            Console.WriteLine(arr.Find(3));
            arr.Add(100);
            arr.Print();
            arr.Remove(3);
            arr.Print();
        }
    }
}
