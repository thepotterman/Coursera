#include "ini.h"

using namespace std;

namespace Ini {

    pair<string_view, string_view> Split(string_view line, char by) {
        size_t pos = line.find(by);
        string_view left = line.substr(0, pos);

        if(pos < line.size() && pos + 1 < line.size()) {
            return {left, line.substr(pos + 1)};
        } else {
            return {left, string_view()};
        }
    }

    string_view Unfigure(string_view line) {
        if(!line.empty() && line.front() == '[') {
            line.remove_prefix(1);
        }
        if(!line.empty() && line.back() == ']') {
            line.remove_suffix(1);
        }
        return line;
    }

    Section & Document::AddSection(string name) {
        sections.insert({name, {}});
        return sections[name];
    }

    const Section & Document::GetSection(const string & name) const {
        return sections.at(name);
    }

    size_t Document::SectionCount() const {
        return sections.size();
    }

    Document Load(istream & input) {
        Document answer;
        string line;
        Section * section;
        string name;
        while(getline(input, line)) {
            if(line.front() == '[') {
                name = Unfigure(line);
                section= &answer.AddSection(move(name));
                name.clear();
                continue;
            }
            if(line.empty()) {
                //answer.sections.insert({move(name), move(section)});
                //section.insert();
                //name.clear();
                //section.clear();
                continue;
            }
            pair<string, string> cat_am;
            cat_am = Split(line, '=');
            section->insert({move(cat_am.first), move(cat_am.second)});
        }
    return answer;
    }
}
