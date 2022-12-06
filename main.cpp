// References: Lecture Slides 8b Graph Traversals and Algorithms, Lecture Slides 6 Sorting

#include "AdjacencyList.h"
#include "Artist.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

void merge(std::vector<Artist> &artists, int left, int mid, int right,
           int sortBy) {
  int n1 = mid - left + 1;
  int n2 = right - mid;
  Artist X[n1], Y[n2];

  for (int i = 0; i < n1; i++) {
    X[i] = artists[left + i];
  }
  for (int j = 0; j < n2; j++) {
    Y[j] = artists[mid + 1 + j];
  }
  int i, j, k;
  i = 0;
  j = 0;
  k = left;

  while (i < n1 && j < n2) {
    switch (sortBy) {
    case 1:
      if (X[i].getArtistName() <= Y[j].getArtistName()) {
        artists[k] = X[i];
        i++;
      } else {
        artists[k] = Y[j];
        j++;
      }
      k++;
      break;
    case 2:
      if (X[i].getArtistFollowers() <= Y[j].getArtistFollowers()) {
        artists[k] = X[i];
        i++;
      } else {
        artists[k] = Y[j];
        j++;
      }
      k++;
      break;
    case 3:
      if (X[i].getArtistPopularity() <= Y[j].getArtistPopularity()) {
        artists[k] = X[i];
        i++;
      } else {
        artists[k] = Y[j];
        j++;
      }
      k++;
      break;
    }
  }
  while (i < n1) {
    artists[k] = X[i];
    i++;
    k++;
  }
  while (j < n2) {
    artists[k] = Y[j];
    j++;
    k++;
  }
}

std::vector<Artist> mergeSort(std::vector<Artist> &artists, int sortBy,
                              int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergeSort(artists, sortBy, left, mid);
    mergeSort(artists, sortBy, mid + 1, right);

    merge(artists, left, mid, right, sortBy);
  }
  return artists;
}

std::vector<Artist> shellSort(std::vector<Artist> &artists, const int &sortBy) {

  int gap = artists.size() / 2;
  switch (sortBy) {
  
  case 1: {
    // sort by name
    while (gap > 0) {
      for (int i = gap; i < artists.size(); i++) {
          Artist temp = artists[i];
          int j;
          for (j = i; j >= gap; j-= gap) {
            if (temp.getArtistName() < artists[j - gap].getArtistName()) {
              artists[j] = artists[j - gap];
						}
						else {
							break;
						}
          }
          artists[j] = temp;
        }

      gap = gap / 2;
    }

    break;
  }
  case 2: {
    // sort by followers
    while (gap > 0) {
      for (int i = gap; i < artists.size(); i++) {
        Artist temp = artists[i];
        int j;
        for (j = i; artists[j - gap].getArtistFollowers() > temp.getArtistFollowers() && j >= gap; j-= gap) {
          artists[j] = artists[j - gap];
        }
        artists[j] = temp;
      }
      
      gap = gap / 2;
    }

    break;
  }
  case 3: {
    while (gap > 0) {
      for (int i = gap; i < artists.size(); i++) {
        Artist temp = artists[i];
        int j;
        for (j = i; artists[j - gap].getArtistPopularity() > temp.getArtistPopularity() && j >= gap; j-= gap) {
          artists[j] = artists[j - gap];
        }
        artists[j] = temp;
      }
      
      gap = gap / 2;
    }

    break;
  }
  }

  return artists;
}

int main() {

  // input parsing here:
  std::ifstream file("nodes.csv");
  std::string line;

  // get each line of the .csv file and create an artist object to add to vector
  // of artists
  std::vector<Artist> artists;
  std::unordered_map<std::string, std::string> idToArtistName;

  // first line has column descriptions
  std::getline(file, line);

  while (std::getline(file, line)) {
    std::istringstream s(line);
    std::string column;

    // get each column in the line (6 columns)
    std::getline(s, column, ',');
    std::string spotifyID = column;
    std::getline(s, column, ',');
    std::string artistName = column;
    std::getline(s, column, ',');

    int followers;

    // deal with bad data
    try {
      followers = (int)std::stod(column);
    } catch (const std::invalid_argument &e) {
      followers = 0;
    }
    std::getline(s, column, ',');

    int popularity;
    try {
      popularity = (int)std::stod(column);
    } catch (const std::invalid_argument &e) {
      popularity = 0;
    }
    std::getline(s, column, ']');
    std::string genreList = column;

    // remove bracket, quotations
    genreList.erase(std::remove(genreList.begin(), genreList.end(), '['),
                    genreList.end());
    genreList.erase(std::remove(genreList.begin(), genreList.end(), ']'),
                    genreList.end());
    genreList.erase(std::remove(genreList.begin(), genreList.end(), '\"'),
                    genreList.end());
    genreList.erase(std::remove(genreList.begin(), genreList.end(), '\''),
                    genreList.end());

    // clean up data
    artistName.erase(std::remove(artistName.begin(), artistName.end(), '['),
                     artistName.end());
    artistName.erase(std::remove(artistName.begin(), artistName.end(), ']'),
                     artistName.end());
    artistName.erase(std::remove(artistName.begin(), artistName.end(), '\"'),
                     artistName.end());
    artistName.erase(std::remove(artistName.begin(), artistName.end(), '\''),
                     artistName.end());

    std::istringstream iss(genreList);
    std::string element;

    // add to vector
    std::vector<std::string> genres;
    while (std::getline(iss, element, ',')) {
      genres.push_back(element);
    }

    std::getline(s, column);

    // create artist
    Artist newArtist(spotifyID, artistName, followers, popularity, genres);
    artists.push_back(newArtist);

    // place id --> name in map
    idToArtistName[spotifyID] = artistName;
  }

  // graph to store artist connections
  AdjacencyList graph;

  // graph input parsing here:
  std::ifstream edgeFile("edges.csv");
  std::string edgeLine;

  while (std::getline(edgeFile, edgeLine)) {
    std::istringstream stream(edgeLine);
    std::string edgeColumn;

    // get each column in the line (2 columns)
    std::getline(stream, edgeColumn, ',');
    std::string artist1Id = edgeColumn;
    std::getline(stream, edgeColumn);
    std::string artist2Id = edgeColumn;

    std::string artist1Name = idToArtistName[artist1Id];
    std::string artist2Name = idToArtistName[artist2Id];

    graph.addConnection(artist1Name, artist2Name);
  }

  // menu here:

  // welcome message
  std::cout << "_______________________________________________________________"
               "____________"
            << std::endl;
  std::cout << " _    _      _                             _               "
            << std::endl;
  std::cout << "| |  | |    | |                           | |               "
            << std::endl;
  std::cout << "| |  | | ___| | ___ ___  _ __ ___   ___   | |_ ___          "
            << std::endl;
  std::cout
      << "| |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\  | __/ _ \\         "
      << std::endl;
  std::cout << "\\  /\\  /  __/ | (_| (_) | | | | | |  __/  | || (_) |        "
            << std::endl;
  std::cout
      << " \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|   \\__\\___/         "
      << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << " _____             _   _  __         _____            _   _ "
            << std::endl;
  std::cout << "/  ___|           | | (_)/ _|       /  ___|          | | | |"
            << std::endl;
  std::cout << "\\ `--. _ __   ___ | |_ _| |_ _   _  \\ `--.  ___  _ __| |_| |"
            << std::endl;
  std::cout
      << " `--. \\ '_ \\ / _ \\| __| |  _| | | |  `--. \\/ _ \\| '__| __| |"
      << std::endl;
  std::cout << "/\\__/ / |_) | (_) | |_| | | | |_| | /\\__/ / (_) | |  | |_|_|"
            << std::endl;
  std::cout
      << "\\____/| .__/ \\___/ \\__|_|_|  \\__, | \\____/ \\___/|_|   \\__(_)"
      << std::endl;
  std::cout << "      | |                     __/ |                         "
            << std::endl;
  std::cout << "      |_|                    |___/                          "
            << std::endl;

  // user input choice
  int choice = 0;

  while (choice != 4) {
    // print choices
    std::cout << "_____________________________________________________________"
                 "______________"
              << std::endl;
    std::cout << std::endl
              << "Input the number of the menu choice you'd like to explore!"
              << std::endl
              << std::endl;
    std::cout << "1. Explore collabs between artists" << std::endl;
    std::cout << "2. Explore a genre" << std::endl;
    std::cout << "3. Explore an artist" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "_____________________________________________________________"
                 "______________"
              << std::endl
              << std::endl;
    std::cin >> choice;
    std::cout << "_____________________________________________________________"
                 "______________"
              << std::endl;

    switch (choice) {
    case 1: {
      // choice 1 -- collabs
      std::cout << std::endl
                << "Explore collabs selected." << std::endl
                << std::endl;
      std::cout << "1. See if two artists have a collab" << std::endl;
      std::cout << "2. See the number of collabs an artist has" << std::endl;
      std::cout << "3. See the list of collabs an artist has" << std::endl;
      std::cout << "4. Find the path between two artists" << std::endl;
      std::cout << "___________________________________________________________"
                   "________________"
                << std::endl
                << std::endl;
      int collabType;
      std::cin >> collabType;
      std::cout << "___________________________________________________________"
                   "________________"
                << std::endl;

      switch (collabType) {
      case 1: {
        std::cin.ignore();
        std::cout << std::endl
                  << "Do these artists have a collab? Lets find out!"
                  << std::endl;
        std::cout << std::endl << "Input the first artist: " << std::endl;
        std::string artist1;
        getline(std::cin, artist1);

        std::cout << std::endl << "Input the second artist: " << std::endl;
        std::string artist2;
        getline(std::cin, artist2);
        std::cout << "_________________________________________________________"
                     "__________________"
                  << std::endl;

        // finds if two artists in the graph have a collab
        bool collab = graph.haveCollab(artist1, artist2);
        std::cout << std::endl << artist1 << " and " << artist2 << " ";

        if (collab) {
          std::cout << "have a collab." << std::endl;
        } else {
          std::cout << "don't have a collab." << std::endl;
        }

        break;
      }
      case 2: {
        // print number of collabs an artist has
        std::cin.ignore();
        std::cout << std::endl
                  << "How many collabs? Input an artist to find out!"
                  << std::endl
                  << std::endl;
        std::string artist1;
        getline(std::cin, artist1);
        std::cout << "_________________________________________________________"
                     "__________________"
                  << std::endl;

        unsigned long long numOfCollabs = graph.numberOfCollabs(artist1);
        std::cout << std::endl
                  << artist1 << " has " << numOfCollabs << " collabs!"
                  << std::endl;
        break;
      }
      case 3: {
        // print list of collabs an artist has
        std::cin.ignore();
        std::cout
            << std::endl
            << "Who has have they collabed with? Input an artist to find out!"
            << std::endl
            << std::endl;
        std::string artist1;
        getline(std::cin, artist1);
        std::cout << "_________________________________________________________"
                     "__________________"
                  << std::endl;

        auto artistCollabs = graph.collabsList(artist1);
        // no collabs
        if (artistCollabs.empty()) {
          std::cout << std::endl << artist1 << " has no collabs." << std::endl;
          break;
        }

        std::cout << std::endl
                  << artist1 << " has collabed with: " << std::endl;

        int j = 1;
        for (auto &person : artistCollabs) {
          std::cout << j << ". " << person << std::endl;
          j++;
        }

        break;
      }
      case 4: {
        // path between two artists
        std::cin.ignore();
        std::cout << std::endl
                  << "Are these artists connected? Lets find out!" << std::endl;
        std::cout << std::endl << "Input the first artist: " << std::endl;
        std::string artist1;
        getline(std::cin, artist1);

        std::cout << std::endl << "Input the second artist: " << std::endl;
        std::string artist2;
        getline(std::cin, artist2);
        std::cout << "_________________________________________________________"
                     "__________________"
                  << std::endl;

        // path between artists BFS
        std::chrono::steady_clock::time_point bfsBegin =
            std::chrono::steady_clock::now();
        std::list<std::string> path =
            graph.pathBetweenArtistsBFS(artist1, artist2);
        std::chrono::steady_clock::time_point bfsEnd =
            std::chrono::steady_clock::now();

        std::cout << std::endl
                  << "Time elapsed for BFS: "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                         bfsEnd - bfsBegin)
                         .count()
                  << " ns" << std::endl;
        std::cout << "BFS path distance: " << path.size() - 1 << std::endl;

        // path between artists DFS
        std::chrono::steady_clock::time_point dfsBegin =
            std::chrono::steady_clock::now();
        std::list<std::string> pathDFS =
            graph.pathBetweenArtistsDFS(artist1, artist2);
        std::chrono::steady_clock::time_point dfsEnd =
            std::chrono::steady_clock::now();

        std::cout << std::endl
                  << "Time elapsed for DFS: "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                         dfsEnd - dfsBegin)
                         .count()
                  << " ns" << std::endl;
        std::cout << "DFS path distance: " << pathDFS.size() - 1 << std::endl;

        std::cout << std::endl
                  << "BFS shortest path between " << artist1 << " and "
                  << artist2 << ": " << std::endl;
        for (auto &element : path) {
          if (element != path.back()) {
            std::cout << element << " --> ";
          } else {
            std::cout << element;
          }
        }
        std::cout << std::endl;
      }
      }

      break;
    }
    case 2: {
      // choice 2
      std::cin.ignore();
      std::cout << std::endl
                << "Which genre would you like to explore? (ex: pop, rap, etc.)"
                << std::endl
                << std::endl;
      std::string
          userGenre; // it may be easier to select from a predetermined list
      getline(std::cin, userGenre);

      std::cout << "___________________________________________________________"
                   "________________"
                << std::endl;
      std::cout << std::endl
                << "How would you like to sort the artists in this genre?"
                << std::endl;
      std::cout << "1. Name" << std::endl;
      std::cout << "2. Followers" << std::endl;
      std::cout << "3. Popularity" << std::endl;
      std::cout << "4. Number of Collabs" << std::endl;
      std::cout << "___________________________________________________________"
                   "________________"
                << std::endl
                << std::endl;
      int sortBy;
      std::cin >> sortBy;

      if (sortBy != 4) {
        std::vector<Artist> artistsGenre;

        for (auto artist : artists) {
          // if artist has user inputted genre in their genres, add them to the
          // vector
          if (std::find(artist.getArtistGenres().begin(),
                        artist.getArtistGenres().end(),
                        userGenre) != artist.getArtistGenres().end()) {
            artistsGenre.push_back(artist);
          }
        }

        // if no artists in genre, break
        if (artistsGenre.empty()) {
          std::cout << "_______________________________________________________"
                       "____________________"
                    << std::endl
                    << std::endl;
          std::cout << "No artists in selected genre: " << userGenre << "."
                    << std::endl;

          break;
        }

        std::cout << "_________________________________________________________"
                     "__________________"
                  << std::endl;
        std::cout << std::endl << "Choose a sort:" << std::endl;
        std::cout << "1. Merge Sort" << std::endl;
        std::cout << "2. Shell Sort" << std::endl;
        std::cout << "_________________________________________________________"
                     "__________________"
                  << std::endl
                  << std::endl;
        int sortType;
        std::cin >> sortType;

        if (sortType == 1) {
          // merge sorting the artists by user inputted sort type
          std::chrono::steady_clock::time_point mergeBegin =
              std::chrono::steady_clock::now();
          std::vector<Artist> mergeSortedArtists =
              mergeSort(artistsGenre, sortBy, 0, artistsGenre.size() - 1);
          std::chrono::steady_clock::time_point mergeEnd =
              std::chrono::steady_clock::now();
          std::cout << "_______________________________________________________"
                       "____________________"
                    << std::endl
                    << std::endl;
          std::cout << "Time elapsed: "
                    << std::chrono::duration_cast<std::chrono::nanoseconds>(
                           mergeEnd - mergeBegin)
                           .count()
                    << " ns" << std::endl
                    << std::endl;
        } else {
          // shell sorting the artists by user inputted sort type
          std::chrono::steady_clock::time_point begin =
              std::chrono::steady_clock::now();
          std::vector<Artist> shellSortedArtists =
              shellSort(artistsGenre, sortBy);
          std::chrono::steady_clock::time_point end =
              std::chrono::steady_clock::now();
          std::cout << "_______________________________________________________"
                       "____________________"
                    << std::endl
                    << std::endl;
          std::cout << "Time elapsed: "
                    << std::chrono::duration_cast<std::chrono::nanoseconds>(
                           end - begin)
                           .count()
                    << " ns" << std::endl
                    << std::endl;
        }

        // if sorting by name:
        if (sortBy == 1) {
          int elementsToPrint =
              (int)std::min((unsigned long)15, artistsGenre.size());

          for (int i = 0; i < elementsToPrint; i++) {
            std::cout << i + 1 << ". " << artistsGenre[i].getArtistName()
                      << ", Followers: " << artistsGenre[i].getArtistFollowers()
                      << ", Popularity: "
                      << artistsGenre[i].getArtistPopularity() << std::endl;
          }
        }
        // if sorting by followers or popularity:
        else if (sortBy == 2 || sortBy == 3) {
          int elementsToPrint =
              (int)std::min((unsigned long)16, artistsGenre.size());

          int j = 1;
          for (int i = artistsGenre.size() - 1;
               i > artistsGenre.size() - elementsToPrint; i--) {
            std::cout << j << ". " << artistsGenre[i].getArtistName()
                      << ", Followers: " << artistsGenre[i].getArtistFollowers()
                      << ", Popularity: "
                      << artistsGenre[i].getArtistPopularity() << std::endl;
            j++;
          }
        }
      } else {
        // 4. Number of collabs
        std::vector<std::string> artistsGenre;

        for (auto &artist : artists) {
          // if artist has user inputted genre in their genres, add them to the
          // vector
          if (std::find(artist.getArtistGenres().begin(),
                        artist.getArtistGenres().end(),
                        userGenre) != artist.getArtistGenres().end()) {
            artistsGenre.push_back(artist.getArtistName());
          }
        }

        // if no artists in genre, break
        if (artistsGenre.empty()) {
          std::cout << "No artists in selected genre: " << userGenre << "."
                    << std::endl;

          break;
        }

        std::cout << "_________________________________________________________"
                     "__________________"
                  << std::endl
                  << std::endl;
        std::cout << "Most popular artist(s) in the " << userGenre
                  << " genre by # of collabs: " << std::endl;
        std::list<std::string> popularArtists =
            graph.mostPopularByGenre(artistsGenre);
        int i = 1;
        for (auto &artist : popularArtists) {
          std::cout << i << ". " << artist << std::endl;
          i++;
        }
      }
      break;
    }
    case 3: {
      // choice 3
      std::cin.ignore();
      std::cout << std::endl
                << "What artist would you like to explore information on?"
                << std::endl
                << std::endl;
      std::string artistName;
      getline(std::cin, artistName);
      std::cout << "___________________________________________________________"
                   "________________"
                << std::endl
                << std::endl;
      int artistIndex = -1;

      // get artist object
      for (int i = 0; i < artists.size(); i++) {
        if (artists[i].getArtistName() == artistName) {
          artistIndex = i;
        }
      }
      // if artist doesn't exist
      if (artistIndex == -1) {
        std::cout << "Artist not found." << std::endl;
        break;
      }

      // print
      std::cout << "Stats on " << artistName << ": " << std::endl << std::endl;

      // popularity score
      std::cout << "Popularity score: "
                << artists[artistIndex].getArtistPopularity() << std::endl;

      // follower count
      std::cout << "Follower count: "
                << artists[artistIndex].getArtistFollowers() << std::endl
                << std::endl;

      // genres
      std::cout << "Genres: " << std::endl;
      for (auto &genre : artists[artistIndex].getArtistGenres()) {
        std::cout << genre << std::endl;
      }

      // # of collabs
      auto numOfCollabs = graph.numberOfCollabs(artistName);

      std::cout << std::endl
                << "Number of collabs: " << numOfCollabs << std::endl;

      // collab list
      if (numOfCollabs == 0) {
        break;
      }

      auto artistCollabs = graph.collabsList(artistName);
      std::cout << std::endl
                << artistName << " has collabed with: " << std::endl;

      int j = 1;
      for (auto &person : artistCollabs) {
        std::cout << j << ". " << person << std::endl;
        j++;
      }

      break;
    }
    case 4: {
      // choice 4
      std::cout << std::endl
                << "Thanks for using Spotify Sort. Exiting..." << std::endl;
      break;
      // exit
    }
    default: {
      std::cout << "Please input valid menu option." << std::endl;
    }
    }
  }

  return 0;
}