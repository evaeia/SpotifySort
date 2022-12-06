#ifndef PROJECT3B_ARTIST_H
#define PROJECT3B_ARTIST_H
#include <string>
#include <vector>

class Artist {
private:
  std::string spotifyID;
  std::string artistName;
  int followers;
  int popularity;
  std::vector<std::string> artistGenres;

public:
  Artist();
  Artist(std::string &ID, std::string &name, int followerCount,
         int popularityScore, std::vector<std::string> &genres);
  const std::vector<std::string> &getArtistGenres() const;
  const std::string &getArtistName() const;
  int getArtistFollowers() const;
  int getArtistPopularity() const;
};

#endif // PROJECT3B_ARTIST_H