#include <iostream>
#include <functional>
#include<vector>
#include<mutex>
#include<string>
#include<chrono>
#include<atomic>

using namespace std;

class Thread_Pool
{
public:
	Thread_Pool()//+
	{
		this->max_thread_number = 0;
		this->stopped = false;
	}

	~Thread_Pool()
	{
		cancel();
	}

	void add(const function<void()>& task)//+
	{
		lock_guard<mutex>LOCK(this->_vec_mtx);
		this->tasks.emplace_back(Task(task, this->status[0]));
	}

	void start(const int& number = 0)//+
	{
		if (number < 1)
		{
			cout << "Number of threads can`t be less than 1!\n";
			return;
		}
		set_thread_number(number);
		for (int i = 0; i <this->max_thread_number; i++)
		{
			pool.push_back(thread());
			free.push_back(atomic<bool>(true));
		}
		do
		{
			for (int i = 0; i <this->max_thread_number; i++)
			{
				if (this->free[i] && this->tasks.size() > 0)
				{
					for(int j=0;j<this->tasks.size();j++)
						if(tasks[j].status == this->status[0])
						{
							this->free[i] = false;
							pool[i] = thread([this, i]()
								{
									mutex mtx;
									unique_lock<mutex>LOCK(mtx);
									for (int j = 0; j < tasks.size(); j++)
										if (tasks[j].status == this->status[0])
										{
											tasks[j].status = this->status[2];
											LOCK.unlock();
											tasks[j].task();
											LOCK.lock();
											tasks[j].status = this->status[3];
											free[i] = true;
											break;
										}
								});
							pool[i].detach();
							break;
						}
				}
			}
		} while (!stopped);
		while (true)
		{
			string ch;
			cout << "Do you want to finish your tasks?(Y/N)\n";
			cin >> ch;
			if (ch == "Y")
			{
				unique_lock<mutex>LOCK(this->_vec_mtx);
				for (int i = 0; i < tasks.size(); i++)
					if (tasks[i].status == status[0]) tasks[i].status = status[1];
				LOCK.unlock();
				finish();
				break;
			}
			else if (!cin.good())
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid symbols found!\n";
			}
			else
			{
				cancel();
				break;
			}
		}
	}

	void cancel()//+
	{
		lock_guard<mutex>LOCK(this->_vec_mtx);
		for (int i = 0; i < this->tasks.size(); i++)
			if (this->tasks[i].status != this->status[3])this->tasks[i].status = this->status[4];
	}

	void stop()//+
	{
		this->stopped = true;
	}

	vector<string> get_statuses()
	{
		vector<string>G;
		lock_guard<mutex>LOCK(this->_vec_mtx);
		for (int i = 0; i < this->tasks.size(); i++)
			G.push_back(this->tasks[i].status);
		return G;
	}
private:
	struct Task//+
	{
		function<void()>task;
		string status;
		decltype(thread().get_id()) ID;
		Task(const function<void()>& task, const string& status)
		{
			this->task = task;
			this->status = status;
		}
	};
	
	bool stopped;

	mutex _vec_mtx;

	vector<Task>tasks;

	vector<thread> pool;

	vector<string>status = { "ADDED","FINISHING","THREADED","DONE","CANCELED" };

	vector<bool>free;

	unsigned int max_thread_number;

	void set_thread_number(const unsigned int& number = thread::hardware_concurrency());//+

	void finish()
	{
		while (this->stopped)
		{
			for (int i = 0; i < this->max_thread_number; i++)
			{
				if (this->free[i] && this->tasks.size() > 0)
				{
					for (int j = 0; j < this->tasks.size(); j++)
						if (tasks[j].status == this->status[1])
						{
							this->free[i] = false;
							pool[i] = thread([this, i]()
								{
									mutex mtx;
									unique_lock<mutex>LOCK(mtx);
									for (int j = 0; j < tasks.size(); j++)
										if (tasks[j].status == this->status[1])
										{
											tasks[j].status = this->status[2];
											LOCK.unlock();
											tasks[j].task();
											LOCK.lock();
											tasks[j].status = this->status[3];
											free[i] = true;
											break;
										}
									if (!free[i])free[i] = true;
								});
							pool[i].detach();
							break;
						}	
				}
			}
			this->stopped = false;
			for (int i = 0; i < this->free.size(); i++)
				if (!free[i])this->stopped = true;
		}
		cout << "Finished\n";
		cancel();
	}
};

void Thread_Pool::set_thread_number(const unsigned int& number)//+
{
	int max = thread::hardware_concurrency();
	if (number >= max)
	{
		while (true)
		{
			cout << "WARNING! ALL " << max << " CPU`S WILL BE USED! DO YOU WANT TO CONTINUE?(Y/N)\n";
			if (cin.get() == 'Y')
			{
				this->max_thread_number = max;
				break;
			}
			else if (!cin.good())
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid symbols found!\n";
			}
			else
			{
				this->max_thread_number = number;
				break;
			}
		}
	}
	else
	{
		this->max_thread_number = number;
	}
	cout << "SET COMPLETED\n";
}

int f1()
{
	//this_thread::sleep_for(chrono::seconds(6));
	for (int i = 0; i < 1000; i++)
		cout << 2;
	cout << "F1\n";
	return 10;
}

int f2(int a)
{
	//this_thread::sleep_for(chrono::seconds(3));
	for (int i = 0; i < 1000; i++)
		cout << 1;
	cout << " F2\n";
	return a;
}


int main()
{
	int a;
	Thread_Pool tp;
	function<void()>ff1 = []() {f1(); };
	function<void()>ff2 = [&]() {a = f2(10); };
	tp.add(ff1);
	tp.add(ff2);
	tp.add(ff1);
	tp.add(ff2);
	tp.add(ff1);
	tp.add(ff2);
	thread t(&Thread_Pool::start, &tp,4);
	tp.add(function<void()>([]() {f1(); }));
	tp.add(function<void()>([]() {f2(5); }));
	
	
	tp.stop();
	this_thread::sleep_for(chrono::seconds(12));
	vector<string>v = tp.get_statuses();
	t.join();
	
	cout << a<<endl;
	for (auto au : v)
		cout << au<<endl;
	return 0;
}