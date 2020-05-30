#include <iostream>
#include <functional>
#include<vector>
#include<mutex>
#include<string>
#include<chrono>
#include<atomic>

using namespace std;

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

class Thread_Pool
{
public:
	const vector<string>status = { "ADDED","FINISHING","THREADED","DONE","CANCELED" };

	Thread_Pool()//+
	{
		this->max_thread_number = 0;
		this->stopped = false;
	}

	~Thread_Pool()
	{
		cancel();
	}

	const Task& add(const function<void()>& task)//+
	{
		lock_guard<mutex>LOCK(this->_vec_mtx);
		this->tasks.emplace_back(Task(task, this->status[0]));
		const Task& ref = this->tasks[this->tasks.size() - 1];
		return ref;
	}

	void start(const unsigned int& number = thread::hardware_concurrency())//+
	{
		if (number < 1)
		{
			cout << "Number of threads can`t be less than 1!\n";
			return;
		}
		set_thread_number(number);
		for (int i = 0; i <this->max_thread_number; i++)
		{
			free.push_back(atomic<bool>(true));
			vector<Task>& v_ref = this->tasks;
			mutex& mtx = this->_vec_mtx;
			pool.push_back(thread([this,&v_ref,&mtx]() 
				{
					while (!stopped)
					{
						for (int j = 0; j < v_ref.size(); j++)
						{
							unique_lock<mutex>LOCK(this->_vec_mtx);
							if (v_ref[j].status == this->status[0])
							{
								v_ref[j].status = this->status[2];
								LOCK.unlock();
								v_ref[j].task();
								LOCK.lock();
								tasks[j].status = this->status[3];
							}
						}
					}
				}));
		}
		for (auto& ref : this->pool)
			ref.join();
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
		lock_guard<mutex>LOCK(this->_vec_mtx);
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
	
	atomic<bool>stopped;

	mutex _vec_mtx;

	vector<Task>tasks;

	vector<thread> pool;

	vector<bool>free;

	unsigned int max_thread_number;

	void set_thread_number(const unsigned int& number = thread::hardware_concurrency());//+

	void finish()
	{
		for (int i = 0; i < this->max_thread_number; i++)
		{
			vector<Task>& v_ref = this->tasks;
			mutex& mtx = this->_vec_mtx;
			pool[i] = thread([this, &v_ref, &mtx]()
				{
					for (int j = 0; j < v_ref.size(); j++)
					{
						unique_lock<mutex>LOCK(this->_vec_mtx);
						if (v_ref[j].status == this->status[1])
						{
							v_ref[j].status = this->status[2];
							LOCK.unlock();
							v_ref[j].task();
							LOCK.lock();
							tasks[j].status = this->status[3];
						}
					}
				});
		}
		for (auto& ref : this->pool)
			ref.join();
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
	for (int i = 0; i < 10; i++)
	{
		tp.add(ff1);
		tp.add(ff2);
		tp.add(ff1);
		tp.add(ff2);
		tp.add(ff1);
		tp.add(ff2);
	}
	tp.add(function<void()>([&tp]() {tp.stop(); }));
	//thread t(&Thread_Pool::start, &tp,4);
	tp.start();
	tp.add(function<void()>([]() {f1(); }));
	tp.add(function<void()>([]() {f2(5); }));
	//tp.stop();
	vector<string>v = tp.get_statuses();
	//t.join();
	
	cout << a<<endl;
	for (auto au : v)
		cout << au<<endl;
	return 0;
}