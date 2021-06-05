#ifndef LAB_2_SHANNON_FANO_H
#define LAB_2_SHANNON_FANO_H

#include <map>
#include <string>
#include <vector>

using namespace std;

map<char, string> shannon_tree;
map<string, char> decode_shannon_tree;

typedef map<int, char>::iterator __it;  // для обозначения позиции в map

int find_average(multimap<int, char>& alphabet, __it start_pos, __it end_pos) {
    int sum = 0;
    for (__it iter = start_pos; iter != end_pos; iter++)
        sum += iter->first;
    return sum / 2;
}

// num - цифра кода
void Shannon_Fano_Codes(multimap<int, char>& alphabet, char num,  __it start_pos, __it end_pos) {
    __it temp = start_pos;
    if (start_pos == end_pos || ++temp == alphabet.end()) {
        if (start_pos != alphabet.end())
            shannon_tree[start_pos->second] += num;
        return;   // достигли конца
    }

    int average = find_average(alphabet, start_pos, end_pos);

    __it i = start_pos;
    __it m = start_pos;
    int left_sum = 0;

    // Находим середину в последовательности символов
    while ((left_sum + i->first < average) && (i != end_pos)) {
        left_sum += i->first;
        i++;
        m++;
    }

    if (start_pos != alphabet.begin() || end_pos != alphabet.end()) {
        // Самая первая итерация - создаём дерево
        for (__it iter = start_pos; iter != alphabet.end(); iter++) {
            shannon_tree[iter->second] += num;  // каждому элементу ссумируем код
            // map[char] = '000111';
            if (iter == end_pos) break;
        }
    }

    // Рекурсия левая ветка дерева
    Shannon_Fano_Codes(alphabet, '1', start_pos, m);
    // Правая ветка дерева
    Shannon_Fano_Codes(alphabet, '0', ++m, end_pos);
}

int parse_text(map<char, int>& alphabet,const vector<string>& text) {

    int prob_sum = 0;

    for (const auto & line : text) {
        for (char i : line) {
            alphabet[i]++;
            prob_sum++;
        }
    }

    return prob_sum;
}

vector<string> code_text(const vector<string>& text) {
    vector<string> coded_text;
    string coded_line;
    for (const auto & line : text) {
        coded_line = "";
        for (char symbol : line) {
            coded_line += shannon_tree[symbol];
        }
        coded_text.push_back(coded_line);
    }
    return coded_text;
}

vector<string> decode_text(const vector<string>& coded_text) {
    vector<string> decoded_text;
    string decoded_line;
    for (const auto & line : coded_text) {
        decoded_line = "";
        string code_sequence = "";
        for (char symbol : line) {
            code_sequence += symbol;
            auto decode_value = decode_shannon_tree.find(code_sequence);
            if (decode_value != decode_shannon_tree.end()) {
                decoded_line += (*decode_value).second;
                code_sequence = "";
            }
        }
        decoded_text.push_back(decoded_line);
    }
    return decoded_text;
}


void make_work() {
    // Ввод через консоль
    map<char, int> alphabet;

    string line;
    vector<string> text;
    while (getline(cin, line)) {
        if (line[0] == '$')
            break;   // строки в вектор и в новую функцию кодирования/декодирования
        text.push_back(line);
    }

    int prob_sum = parse_text(alphabet, text);

    for(auto& var : alphabet) {
        var.second = 100 * var.second / prob_sum; // находим вероятности
        // 100% * x / sum
    }

    // Переворачиваем для отсортировки по убыванию частот
    multimap<int, char> alphabet_by_prob;
    for (auto& pair : alphabet) {
        alphabet_by_prob.insert({pair.second, pair.first});
    }

    Shannon_Fano_Codes(alphabet_by_prob, '1', alphabet_by_prob.begin(), alphabet_by_prob.end());

    cout << "Alphabet: \n";
    for (const auto& var : shannon_tree) {
        cout << "\'" << var.first << "\'" << " : " << var.second <<
             " : " << alphabet[var.first] / 100.0 << endl;
    }

    vector<string> coded_text = code_text(text);

    cout << "Coded text: \n";
    for (auto & line : coded_text) {
        cout << line << endl;
    }

    // Переворачиваем shannon_tree для декодинга
    for (auto& pair : shannon_tree) {
        decode_shannon_tree.insert({pair.second, pair.first});
    }

    vector<string> decoded_text = decode_text(coded_text);

    cout << "Decoded text: \n";
    for (auto & line : decoded_text) {
        cout << line << endl;
    }
}


#endif //LAB_2_SHANNON_FANO_H
