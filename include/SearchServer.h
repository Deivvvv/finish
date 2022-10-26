#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include "invertedindex.h"

struct RelativeIndex{
size_t doc_id;
float rank;
bool operator ==(const RelativeIndex& other) const {
return (doc_id == other.doc_id && rank == other.rank);
}
};
class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/

std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input){
    std::vector<std::vector<RelativeIndex>> result = std::vector<std::vector<RelativeIndex>>(queries_input.size());

    //ConverterJSON conv = ConverterJSON(_index.path);

    for(int i=0;i< queries_input.size();i++){
        std::vector<RelativeIndex> resultCase =std::vector<RelativeIndex>();
        std::vector<std::string> words = _index.split(queries_input[i], " ");
        for(int j =0; j< words.size();j++){
            std::vector<Entry> entry =_index.GetWordCount(words[j]);
            RelativeIndex index;
            for(int l=0; l< _index.docsSize;l++)
                for(int k=0;k<entry.size();k++){
                    index = RelativeIndex();
                    index.doc_id = l;
                    if(entry[k].doc_id == l){
                        index.rank = entry[k].count;
                        resultCase.push_back(index);
                        break;
                    }
                }
            }
        float resultMax=0;
        for(int j=0;j<resultCase.size();j++)
            if(resultCase[j].rank> resultMax)
                 resultMax = resultCase[j].rank;

        if(resultMax ==0)
            resultCase = std::vector<RelativeIndex>();
        else
        {
            ConverterJSON conv = ConverterJSON(_index.path);
            int c = conv.GetResponsesLimit();
            c = (resultCase.size()>c)? c:resultCase.size();
            for(int k=0;k<c;k++){
                /*
                int b=k;
                for(int j=k;j<resultCase.size();j++)
                    if(resultCase[b].rank < resultCase[j].rank)
                        b=j;
                float g =resultCase[k].rank;
                resultCase[k].rank = resultCase[b].rank;
                resultCase[b].rank=g;
*/
                resultCase[k].rank /=resultMax;
            }
        }
        result[i] = resultCase;

    }
    return result;
};
private:
InvertedIndex _index;
};


#endif // SEARCHSERVER_H
