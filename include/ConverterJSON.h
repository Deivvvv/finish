#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;

class ConverterJSON {
    std::string path;
public:
ConverterJSON() = default;

ConverterJSON(std::string _path){
    path= _path;
}
/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/

json OpenJson(std::string str){
    //std::cout << path+"\\"+str<< std::endl;
    std::ifstream file(path+"\\"+str);
    json j = json::parse(file);
    //std::cout << j << std::endl;

    file.close();
    return j;
};

std::string OpenFile(std::string str){
    //std::vector<std::string> wordList;
    std::string wordFull =" ";
    std::string word;
    std::ifstream file(path+"\\resources\\"+str);
    while(!file.eof()){
        file >> word;
        if(wordFull == " ")
            wordFull = word;
        else
            wordFull +=" "+ word;
        //wordList.push_back(word);
    }

    file.close();
    return   wordFull;
};

std::vector<std::string> GetTextDocuments(){
    std::vector<std::string> list;
    std::vector<std::string> vec = OpenJson("config.json")["files"];
    for(int i=0;i<vec.size();i++)
        list.push_back(OpenFile(vec[i]));

    return list;
};


/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
int GetResponsesLimit(){
    std::string str=  OpenJson("config.json")["config"]["max_responses"];
    return std::stoi(str);
};
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
std::vector<std::string> GetRequests(){
    return OpenJson("requests.json")["requests"];
};
/**
* Положить в файл answers.json результаты поисковых запросов
*/
void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers){

    auto requestStr =[](int a){
       std::string str = "request";
       std::string end =std::to_string(a);
      if(a>99)
          return str+end;
      if(a>9)
          return str+"0"+end;

      return str+"00"+end;
  };
    std::ofstream file;
    //file.open(path+"\\answers.json", std::ofstream::out | std::ofstream::trunc);
    //file.close();
    file.open(path+"\\answers.json", std::ofstream::out | std::ofstream::trunc);

    std::string str;
    json j;
    for(int i=0;i<answers.size();i++){
        str = requestStr(i);
        bool use = answers[i].size()>0;
        j["answers"][str]["result"] = use;

        if(use){
            std::vector<std::pair<int, float>> vec =answers[i];
            for(int k=0;k<vec.size();k++){
                j["answers"][str]["relevance"]["doc_id"+std::to_string(k)]=vec[k].first;
                j["answers"][str]["relevance"]["rank"+std::to_string(k)]=vec[k].second;
            }
        }
    }

    std::cout << j << std::endl;
    file<<j;
    file.close();
};

};
#endif // CONVERTERJSON_H
