#include "Artist.h"

Artist::Artist() {}

Artist::Artist(std::string &ID, std::string &name, int followerCount,
               int popularityScore, std::vector<std::string> &genres) {
  this->spotifyID = ID;
  this->artistName = name;
  this->followers = followerCount;
  this->popularity = popularityScore;
  this->artistGenres = genres;
}

const std::string &Artist::getArtistName() const { return artistName; }

const std::vector<std::string> &Artist::getArtistGenres() const {
  return artistGenres;
}

int Artist::getArtistFollowers() const { return followers; }

int Artist::getArtistPopularity() const { return popularity; }