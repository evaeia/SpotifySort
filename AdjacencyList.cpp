#include "AdjacencyList.h"
#include <algorithm>
#include <queue>
#include <stack>

void AdjacencyList::addConnection(const std::string& from, const std::string& to) {
    // add "to" artist to "from"'s list of adjacent vertices
    this->adjArtists[from].push_back(to);

    // since this is an undirected graph, add "from" to "to"'s list of adjacent vertices
    this->adjArtists[to].push_back(from);
}

bool AdjacencyList::haveCollab(const std::string& firstArtist, const std::string& secondArtist) {
    auto& firstArtistVector = this->adjArtists[firstArtist];

    // search for secondArtist in firstArtist's collaboration vector
    if (std::find(firstArtistVector.begin(), firstArtistVector.end(), secondArtist) != firstArtistVector.end()) {
        return true;
    }

    return false;
}

unsigned long long AdjacencyList::numberOfCollabs(const std::string& artist) {
    return this->adjArtists[artist].size();
}

std::vector<std::string> AdjacencyList::collabsList(const std::string &artist) {
    return this->adjArtists[artist];
}

std::list<std::string> AdjacencyList::pathBetweenArtistsBFS(const std::string& artist1, const std::string& artist2) {
    std::unordered_map<std::string, int> distance;
    std::unordered_map<std::string, std::string> predecessor;
    std::unordered_map<std::string, bool> visited;
    bool found = false;
    std::list<std::string> artistPath;

    // initially every node is unvisited, no predecessor, and distance hasn't been calculated
    for (auto& it : this->adjArtists) {
        distance[it.first] = 2147483647;
        visited[it.first] = false;
        predecessor[it.first] = "";
    }

    std::queue<std::string> queue;

    // start with the source node
    visited[artist1] = true;
    distance[artist1] = 0;
    queue.push(artist1);

    // breadth first search for artist2
    while (!queue.empty()) {
        std::string front = queue.front();
        queue.pop();

        // look through all the front node's adjacent nodes
        for (auto & i : this->adjArtists[front]) {
            // if the adj node isn't visited yet, visit adj node
            if (!visited[i]) {
                visited[i] = true;
                distance[i] = distance[front] + 1;
                predecessor[i] = front;
                queue.push(i);

                if (i == artist2) {
                    found = true;
                    break;
                }
            }
        }
    }

    // if node isn't found return empty list
    if (!found) {
        return artistPath;
    }

    // find the path by backtracking through the predecessor map
    artistPath.push_back(artist2);
    std::string last = artist2;
    while (!predecessor[last].empty()) {
        artistPath.push_front(predecessor[last]);
        last = predecessor[last];
    }

    return artistPath;
}

std::list<std::string> AdjacencyList::mostPopularByGenre(const std::vector<std::string> &artists) {
    int collabs = 0;
    std::list<std::string> popularArtists;

    for (auto& artist : artists) {
        int artistCollabs = this->adjArtists[artist].size();

        // if an artist has more collabs than the current max, make that artist the most popular
        if (artistCollabs > collabs) {
            popularArtists.clear();
            popularArtists.push_front(artist);
            collabs = artistCollabs;
        }
            // if an artist has the same amount of collabs as the max, they are added to popular artist list
        else if (artistCollabs == collabs) {
            popularArtists.push_front(artist);
        }
    }

    return popularArtists;
}

std::list<std::string> AdjacencyList::pathBetweenArtistsDFS(const std::string &artist1, const std::string &artist2) {

    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, std::string> predecessor;
    std::list<std::string> artistPath;
    std::stack<std::string> stack;

    // initially every node is unvisited
    for (auto& it : this->adjArtists) {
        visited[it.first] = false;
        predecessor[it.first] = "";
    }

    // start with the source node
    visited[artist1] = true;
    stack.push(artist1);

    // depth first search for artist2
    while (!stack.empty()) {
        std::string front = stack.top();
        stack.pop();

        // check to see if the node was visited
        if (!visited[front]) {
            visited[front] = true;
        }

        // look through all the front node's adjacent nodes
        for (auto & i : this->adjArtists[front]) {
            // if the adj node isn't visited yet, visit adj node
            if (!visited[i]) {
                stack.push(i);
                predecessor[i] = front;

                if (i == artist2) {
                    break;
                }
            }
        }
    }
    // find the path by backtracking through the predecessor map
    artistPath.push_back(artist2);
    std::string last = artist2;
    while (!predecessor[last].empty()) {
        artistPath.push_front(predecessor[last]);
        last = predecessor[last];
    }

    return artistPath;
}
