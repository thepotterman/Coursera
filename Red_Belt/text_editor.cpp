#include <string>
#include <list>
#include "test_runner.h"
using namespace std;

class Editor {
public:
    Editor() {}
    void Left() {
        if(cur == _text.begin()) {
            return;
        } else {
            cur--;
        }
    }
    void Right() {
        if(cur == _text.end()) {
            return;
        } else {
            cur++;
        }
    }
    void Insert(char token) {
        cur = _text.insert(cur, token);
        cur++;
    }
    void Cut(size_t tokens = 1) {
        buffer.clear(); 
        for(size_t i = 0; i < tokens; ++i) {
            if(cur == _text.end()) {
                break;
            } else {
                buffer.push_back(*cur);
                cur = _text.erase(cur);
            }
        }
    }
    void Copy(size_t tokens = 1) {
        buffer.clear(); 
        for(size_t i = 0; i < tokens; ++i) {
            if(cur == _text.end()) {
                break;
            } else {
                buffer.push_back(*cur);
                cur++; 
            }
        }
    }
    void Paste() {
        for(const auto & c : buffer) {
            this->Insert(c);
        }
    }
    string GetText() const {
        string answer;
        auto pos = _text.begin();
        while(pos != _text.end()) {
            answer.push_back(*pos);
            pos++;
        }
        return answer;
    }
private:
    list<char> _text;
    list<char>::iterator cur = _text.begin();
    string buffer;
};

void TypeText(Editor& editor, const string& text) {
    for(char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;
      
        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for(size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for(size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);
        
        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;
        
        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();
        
        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;
    
    const string text = "esreveR";
    for(char c : text) {
        editor.Insert(c);
        editor.Left();
    }
    
    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");
    
    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "");
}    

void TestEmptyBuffer() {
    Editor editor;
    
    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();
  
    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    Editor editor;
    const string text = "hello, world";
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3); // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}
