#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

class A : public enable_shared_from_this<A>
{
public:
	A(string name)
		: name(name)
	{
		cout << "Resource Acquisition" << endl;
	}

	~A()
	{
		cout << "Resource Destroyed" << endl;
	}

public:
	FORCEINLINE void Func() { cout << "나는 메소드다!!" << endl; }
	FORCEINLINE void SetOther(weak_ptr<A> InOther) { other = InOther; }
	
	FORCEINLINE void PrintOtherName() 
	{
		shared_ptr<A> other = this->other.lock();
		if (other != nullptr)
		{
			cout << other->GetName() << endl;
			return;
		}

		cout << "Already Destroyed" << endl;
	}

private:
	string GetName() { return name; }

private:
	string name;
	weak_ptr<A> other;
};

int main()
{
	auto obj1 = make_shared<A>("Resource1");
	auto obj2 = make_shared<A>("Resource2");

	obj1->SetOther(obj2);
	obj2->SetOther(obj1);

	cout << obj1.use_count() << endl;
	cout << obj2.use_count() << endl;

	obj2.reset();
	obj1->PrintOtherName();

	system("pause");
	return 0;
}