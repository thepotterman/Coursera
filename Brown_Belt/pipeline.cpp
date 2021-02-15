#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};

ostream& operator<< (ostream& out, const Email& a) {
    out << a.from << endl << a.to << endl << a.body << endl; // << endl
    return out;
}

istream& operator>> (istream& in, Email& a) {
    getline(in, a.from); 
    getline(in, a.to); 
    getline(in, a.body); 
    /*string s;
    getline(in, s);
    if(s == "") {
        return in;
    } else {
        throw std::runtime_error("error");
    }
    */
    return in;
}

bool operator< (const Email& lhs, const Email& rhs) {
    return tie(lhs.from, lhs.to, lhs.body) < tie(rhs.from, rhs.to, rhs.body);
}

class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if(next_) {
            next_.get()->Process(move(email));
        } 
    }

public:
    unique_ptr<Worker> next_;
    void SetNext(unique_ptr<Worker> next) {
        next_ = move(next);
    }
};


class Reader : public Worker {
public:
    // реализуйте класс
    Reader(istream& in) :
        in_(in) {}
    void Run() {
        Email current_email;
        while(in_ >> current_email) {
            PassOn(make_unique<Email>(move(current_email))); 
        }
    }
    void Process(unique_ptr<Email> current) {
        PassOn(move(current));
    }
private:
    istream& in_;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
    
    Filter(Function func) : func_(func) {}
    void Process(unique_ptr<Email> current) {
        if(func_(*current.get())) {
            PassOn(move(current));
        }
    }
private:
    Function func_;
};


class Copier : public Worker {
public:
    Copier(string adress) : adress_(move(adress)){}
    void Process(unique_ptr<Email> current) {
        Email current_email = *current.get();
        if(current_email.to == adress_) {
            PassOn(move(current)); 
        } else {
            current_email.to = adress_;
            PassOn(move(current));
            PassOn(make_unique<Email>(current_email));
        }
    }
private:
    string adress_;
};


class Sender : public Worker {
public:
    Sender(ostream& out) : out_(out) {}
    void Process(unique_ptr<Email> current) {
        out_ << *current.get();
        PassOn(move(current));
    }
private:
    ostream& out_;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        //Reader first(in);
        //answer = make_unique<Worker>(move(first));
        answer = make_unique<Reader>(in);
    }

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        //Filter next(filter);
        Worker* current = answer.get();
        while(current->next_.get()) {
            current = current->next_.get();
        } 
        //После прохода цикла current указывает на последний Worker в цепочке
        current->SetNext(make_unique<Filter>(filter));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        //Copier next(recipient);
        Worker* current = answer.get();
        while(current->next_.get()) {
            current = current->next_.get();
        } 
        //После прохода цикла current указывает на последний Worker в цепочке
        current->SetNext(make_unique<Copier>(recipient));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        //Sender next(out);
        Worker* current = answer.get();
        while(current->next_.get()) {
            current = current->next_.get();
        } 
        //После прохода цикла current указывает на последний Worker в цепочке
        current->SetNext(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        return move(answer);
    }
private:
    unique_ptr<Worker> answer;
};


void TestSanity() {
    string input = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "ralph@example.com\n"
        "erich@example.com\n"
        "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "erich@example.com\n"
        "richard@example.com\n"
        "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
