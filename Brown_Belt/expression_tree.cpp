#include "Common.h"
#include "test_runner.h"

#include <sstream>
#include <memory>

using namespace std;

class Operation : public Expression {
    public:
        Operation(bool type, ExpressionPtr left, ExpressionPtr right):
            type_(type),
            left_(move(left)),
            right_(move(right)) {}
        int Evaluate() const {
            if(type_) {
                return left_->Evaluate() * right_->Evaluate();
            } else {
                return left_->Evaluate() + right_->Evaluate();
            }
        }
        string ToString() const {
            if(type_) {
                return "(" + left_->ToString() + ")" + "*" + "(" + right_->ToString() + ")";
            } else {
                return "(" + left_->ToString() + ")" + "+" + "(" + right_->ToString() + ")";
            }
        }
    private:
        bool type_;
        //1 - "*"
        //0 - "+"
        ExpressionPtr left_;
        ExpressionPtr right_;
};

class Number : public Expression {
    public:
        Number(int value) : value_(value) {}
        int Evaluate() const {
            return value_;
        }
        string ToString() const {
            return to_string(value_);
        }
    private:
        int value_;
};

ExpressionPtr Value(int value) {
    return make_unique<Number>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Operation>(0, move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Operation>(1, move(left), move(right));
}

string Print(const Expression* e) {
    if (!e) {
        return "Null expression provided";
    }
    stringstream output;
    output << e->ToString() << " = " << e->Evaluate();
    return output.str();
}

void Test() {
    ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
    ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

    ExpressionPtr e2 = Sum(move(e1), Value(5));
    ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

    ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);
    return 0;
}
