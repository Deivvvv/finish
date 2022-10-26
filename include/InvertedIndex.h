#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include "converterjson.h"
#include <vector>
#include <map>
#include <string>
//#include <gtest/gtest.h>

struct Entry {
    size_t doc_id, count =1;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
    return (doc_id == other.doc_id &&
            count == other.count);
    }
};
class InvertedIndex {
public:
InvertedIndex() = default;
std::string path;
/**
* Обновить или заполнить базу документов, по которой будем совершать
поиск
* @param texts_input содержимое документов
*/
void SetPath(std::string _path){
    path = _path;
};

std::vector<std::string> split(std::string str, std::string token){
    std::vector<std::string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

void UpdateDocumentBase(std::vector<std::string> input_docs){
    std::vector<std::string> new_docs;
    for(int i=0;i<input_docs.size();i++){
        bool add =false;
        for(int j=0;j<docs.size() && !add;j++)
            add =(input_docs[i] == docs[j]);

        if(!add){
            new_docs.push_back(input_docs[i]);
        }
    }

    Entry wordCase;
    for(int i=0;i<new_docs.size();i++){
        std::vector<std::string> textList = split(new_docs[i], " ");
        int a;
        for(int j=0;j<textList.size();j++)
        {
            a =docs.size();
            auto search = freq_dictionary.find(textList[j]);
            if(search != freq_dictionary.end()){
                bool add =false;
                for(int k=0;k<search->second.size() && !add;k++)
                    if(search->second[k].doc_id == a){
                        add =true;
                        search->second[k].count++;
                    }
                if(!add){
                    Entry wordCase = Entry();
                    wordCase.doc_id=a;
                    search->second.push_back(wordCase);
                }
            }else{
                Entry wordCase = Entry();
                wordCase.doc_id=a;
                freq_dictionary[textList[j]].push_back(wordCase);
            }
        }

        docs.push_back(new_docs[i]);
    }

    //std::cout << freq_dictionary.size() << std::endl;
    docsSize = docs.size();
};
/**
* Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
std::vector<Entry> GetWordCount(const std::string& word){
    auto search = freq_dictionary.find(word);
    if(search != freq_dictionary.end())
        return search->second;
    return std::vector<Entry>();

};
int docsSize;
private:
std::vector<std::string> docs; // список содержимого документов
std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};


#endif // INVERTEDINDEX_H
