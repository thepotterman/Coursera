#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class HumanType
{
	TEACHER,
	POLICEMAN,
	STUDENT
};

const string GetType(const HumanType & type)
{
	string answer;
	if(type == HumanType::TEACHER)
	{
		return "Teacher";
	}
	else if(type == HumanType::POLICEMAN)
	{
		return "Policeman";
	}
	else if(type == HumanType::STUDENT)
	{
		return "Student";
	}
	else
	{
		return "Unknown type";
	}
}

class Human
{
public:
	Human(const string & name, const HumanType & type)
	: name_(name),
	  type_(type)
	{

	}
	virtual void Walk(const string & destination) const
	{
		cout << GetType(type_) << " " << name_ << " walks to: " << destination << endl;
	}
	string GetName() const
	{
		return name_;
	}
	HumanType GetHumanType() const
	{
		return type_;
	}

	const string name_;
	const HumanType type_;
};

class Student : public Human
{
public:

    Student(const string & name, const string & favouriteSong) 
    : Human(name, HumanType::STUDENT),
      FavouriteSong_(favouriteSong)
    {
        
    }
    void Learn() 
    {
        cout << GetType(type_) << " " << name_ << " learns" << endl;
    }

    void Walk(const string & destination) const
    {
        cout << GetType(type_) << " " << name_ << " walks to: " << destination << endl;
        SingSong();
    }

    void SingSong() const
    {
        cout << GetType(type_) << " " << name_ << " sings a song: " << FavouriteSong_ << endl;
    }

    const string FavouriteSong_;
};


class Teacher : public Human
{
public:

    Teacher(const string & name, const string & subject) 
    : Human(name, HumanType::TEACHER),
      Subject_(subject)
    {

    }

    void Teach() {
        cout << GetType(type_) << " " << name_ << " teaches: " << Subject_ << endl;
    }

    const string Subject_;
};


class Policeman : public Human
{
public:
    Policeman(const string & name)
    : Human(name, HumanType::POLICEMAN)
    {

    }

    void Check(const Human & suspect)
    {
    	cout << GetType(type_) << " " << name_ << " checks " << GetType(suspect.GetHumanType()) << ". " 
    	<< GetType(suspect.GetHumanType()) << "'s name is: " << suspect.GetName() << endl;
    }
};


void VisitPlaces(const Human & human, const vector<string> & places) 
{
    for (auto p : places) 
    {
        human.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
