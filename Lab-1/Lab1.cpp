//===============================================================================
//                                Task 1.2.1
//===============================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
using namespace std;

void Thread1() {
	cout << "1";
}

void Thread2() {
	cout << "2";
}

int main()
{
	thread t1(Thread1);
	thread t2(Thread2);
}
//===============================================================================
//                                Task 1.2.2
//===============================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
using namespace std;

void Thread1() {
	cout << "1";
}

void Thread2() {
	cout << "2";
}

int main()
{
	thread t1(Thread1);
	thread t2(Thread2);
	t1.detach();
	t2.detach();
}
//===============================================================================
//                                Task 1.2.3
//===============================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <list>
using namespace std;

list<int> l;

void AddToListFunction(int n) {
	for (int i = 0; i < 10; i++) {
		l.push_back(n + i);
	}
	cout << "All items added\n";
}

void ListContainsFunction(int x) {
	list<int>::iterator it = l.begin();

	for (int i = 0; i < 10; i++) {
		if (distance(l.begin(), it) >= l.size() || l.size() == 0) {
			break;
		}
		if (x == *it) {
			cout << "x found\n";
			return;
		}
		it++;
	}
	cout << "x not found\n";
}

int main()
{
	thread AddToList(AddToListFunction, 3);
	thread ListContains(ListContainsFunction, 4);

	AddToList.join();
	ListContains.join();
}

//===============================================================================
//                                Task 1.2.4
//===============================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <list>
using namespace std;

list<int> l;
mutex m;

void AddToListFunction(int n) {

	m.lock();

	for (int i = 0; i < 10; i++) {
		l.push_back(n + i);
	}

	cout << "All items added\n";
	m.unlock();
}

void ListContainsFunction(int x) {
	m.lock();

	list<int>::iterator it = l.begin();

	for (int i = 0; i < 10; i++) {
		if (distance(l.begin(), it) >= l.size() || l.size() == 0) {
			break;
		}
		if (x == *it) {
			cout << "x found\n";
			m.unlock();
			return;
		}
		it++;
	}
	cout << "x not found\n";

	m.unlock();
}

int main()
{
	thread AddToList(AddToListFunction, 3);
	thread ListContains(ListContainsFunction, 4);

	AddToList.join();
	ListContains.join();
}

//===============================================================================
//                                Task 1.2.5
//===============================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <list>
using namespace std;

list<int> l;
mutex m;

void AddToListFunction(int n) {
	lock_guard<mutex> lock(m);
	l.push_back(n + 1);
	cout << "Item added\n";
}

void ListContainsFunction(int x) {
	lock_guard<mutex> lock(m);

	list<int>::iterator it = l.begin();
	while (it != l.end()) {
		if (x == *it) {
			cout << "x found\n";
			return;
		}
		it++;
	}
	cout << "x not found\n";
}

int main()
{
	int n = 3;
	int m = 5;
	for (int i = 0; i < 10; i++) {
		thread AddToList(AddToListFunction, n + i);
		thread ListContains(ListContainsFunction, m);

		AddToList.join();
		ListContains.join();
	}
}

//===============================================================================
//                                Task 1.2.6
//===============================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <list>

using namespace std;


class someData {
public:
	string name;
	string surname;
	string address;
	int age;

	void printInfo() {
		cout << "Name - " << this->name << " " << this->surname << " Address - " << this->address << " Age - " << this->age << "\n";
	}
};

class exchangePerson {

	someData infoAboutPerson;
	mutex m;

public:

	static  void JohnDoe(exchangePerson& exchangePerson) {
		lock_guard<mutex> lock(exchangePerson.m);

		exchangePerson.infoAboutPerson.name = "John";
		exchangePerson.infoAboutPerson.surname = "Doe";
		exchangePerson.infoAboutPerson.address = "Unknown";
		exchangePerson.infoAboutPerson.age = 120;
	}

	static  void JacobSmith(exchangePerson& exchangePerson) {
		lock_guard<mutex> lock(exchangePerson.m);

		exchangePerson.infoAboutPerson.name = "Jacob";
		exchangePerson.infoAboutPerson.surname = "Smith";
		exchangePerson.infoAboutPerson.address = "Known";
		exchangePerson.infoAboutPerson.age = 1;
	}

	static void Swap(exchangePerson& person1, exchangePerson& person2) {

		if (&person1 == &person2) {
			return;
		}

		lock(person1.m, person2.m);

		lock_guard<mutex> lock1(person1.m, adopt_lock);
		lock_guard<mutex> lock2(person2.m, adopt_lock);

		someData tempObject = person1.infoAboutPerson;

		//Before
		cout << "Person 1\n";
		person1.infoAboutPerson.printInfo();
		cout << "Person 2\n";
		person2.infoAboutPerson.printInfo();

		//Swap
		person1.infoAboutPerson = person2.infoAboutPerson;
		person2.infoAboutPerson = tempObject;

		//After
		cout << "Person 1\n";
		person1.infoAboutPerson.printInfo();
		cout << "Person 2\n";
		person2.infoAboutPerson.printInfo();
	}
};

int main()
{
	exchangePerson person1;
	exchangePerson person2;

	thread t1(exchangePerson::JohnDoe, ref(person1));
	thread t2(exchangePerson::JohnDoe, ref(person2));
	thread t3(exchangePerson::JacobSmith, ref(person1));
	thread t4(exchangePerson::JacobSmith, ref(person2));

	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();

	thread t5(exchangePerson::Swap, ref(person1), ref(person2));

	t5.join();
}

//===============================================================================
//                                Завдання 1.2.7
//============================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <list>

using namespace std;

class someData {
public:
	string name;
	string surname;
	string address;
	int age;

	void printInfo() {
		cout << "Name - " << this->name << " " << this->surname << " Address - " << this->address << " Age - " << this->age << "\n";
	}
};

class exchangePerson {

	someData infoAboutPerson;
	mutex m;

public:

	static  void JohnDoe(exchangePerson& exchangePerson) {
		lock_guard<mutex> lock(exchangePerson.m);

		exchangePerson.infoAboutPerson.name = "John";
		exchangePerson.infoAboutPerson.surname = "Doe";
		exchangePerson.infoAboutPerson.address = "Unknown";
		exchangePerson.infoAboutPerson.age = 120;
	}

	static  void JacobSmith(exchangePerson& exchangePerson) {
		lock_guard<mutex> lock(exchangePerson.m);

		exchangePerson.infoAboutPerson.name = "Jacob";
		exchangePerson.infoAboutPerson.surname = "Smith";
		exchangePerson.infoAboutPerson.address = "Known";
		exchangePerson.infoAboutPerson.age = 1;
	}

	static void Swap(exchangePerson& person1, exchangePerson& person2) {

		if (&person1 == &person2) {
			return;
		}

		unique_lock<mutex> lock1(person1.m, defer_lock);
		unique_lock<mutex> lock2(person2.m, defer_lock);

		lock(lock1, lock2);

		someData tempObject = person1.infoAboutPerson;

		//Before
		cout << "Person 1\n";
		person1.infoAboutPerson.printInfo();
		cout << "Person 2\n";
		person2.infoAboutPerson.printInfo();

		//Swap
		person1.infoAboutPerson = person2.infoAboutPerson;
		person2.infoAboutPerson = tempObject;

		//After
		cout << "Person 1\n";
		person1.infoAboutPerson.printInfo();
		cout << "Person 2\n";
		person2.infoAboutPerson.printInfo();
	}
};

int main()
{
	exchangePerson person1;
	exchangePerson person2;

	thread t1(exchangePerson::JohnDoe, ref(person1));
	thread t2(exchangePerson::JohnDoe, ref(person2));
	thread t3(exchangePerson::JacobSmith, ref(person1));
	thread t4(exchangePerson::JacobSmith, ref(person2));

	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();

	thread t5(exchangePerson::Swap, ref(person1), ref(person2));

	t5.join();
}

