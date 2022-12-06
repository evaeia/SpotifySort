#ifndef PROJECT3B_ADJACENCYLIST_H
#define PROJECT3B_ADJACENCYLIST_H
#include <unordered_map>
#include <vector>
#include <string>
#include <list>

class AdjacencyList {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjArtists;

public:
    void addConnection(const std::string& from, const std::string& to);
    bool haveCollab(const std::string& firstArtist, const std::string& secondArtist);
    unsigned long long numberOfCollabs(const std::string& artist);
    std::vector<std::string> collabsList(const std::string& artist);
    std::list<std::string> pathBetweenArtistsBFS(const std::string& artist1, const std::string& artist2);
    std::list<std::string> pathBetweenArtistsDFS(const std::string& artist1,  const std::string& artist2);
    std::list<std::string> mostPopularByGenre(const std::vector<std::string>& artists);
};


#endif //PROJECT3B_ADJACENCYLIST_H
