#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include "leaderboard.hpp"
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/system.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

map <string,map<int,run>> board;
map <string,player> spelare;

void DownloadSingle(const char* url, const char* fname)
{
  CURL *curl;
  FILE *fp;
  CURLcode res;
  curl = curl_easy_init();
  if (curl){
      fp = fopen(fname, "wb");
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      fclose(fp);
  }
}

void ParseFiles()
{

  ifstream list("maplist");
  int lbCount = 1;
  bool dl = true;

  int selection;
  cout << "1. Download files" << endl;
  cout << "2. Read local files" << endl;

  cin >> selection;
  switch (selection)
  {
    case 1: dl = true;
    break;
    case 2: dl = false;
    break;
  }

  while(!list.eof()) {

    string fileName;
    list >> fileName;

    if (list.eof()) {
        break;
    }

    const char *webroot = "https://play.sourceruns.org/leaderboard/";
    string fullURL = webroot + fileName;
    const char *url = fullURL.c_str();
    string savePath = "./data/" + fileName;
    const char *fileNameChar = savePath.c_str();

    cout << "Parsing leaderboards: " << lbCount << " (" << fileName << ")" << endl;

    if (dl) {
    DownloadSingle(url, fileNameChar);
    }

    ifstream leaderboard("./data/" + fileName);

    int pos = 1;

    while(!leaderboard.eof()) {
      string read;
      int readint;

      leaderboard >> quoted(read);
      board[fileName][pos].setPlayerID(read);
      leaderboard >> quoted(read);
      board[fileName][pos].setPlayerName(read);
      leaderboard >> readint;
      board[fileName][pos].setCheckpoints(readint);
      leaderboard >> readint;
      board[fileName][pos].setTeleports(readint);
      leaderboard >> quoted(read);
      board[fileName][pos].setRunTime(read);
      leaderboard >> readint;
      board[fileName][pos].setRunDate(readint);

      string pID = board[fileName][pos].getPlayerID();

      spelare[pID].setPlayerID(board[fileName][pos].getPlayerID());
      spelare[pID].setPlayerName(board[fileName][pos].getPlayerName());
      spelare[pID].incRunCount();

      if (pos == 1) {
        spelare[pID].incRecordCount();
        spelare[pID].insRecMap(fileName);
      }

      pos++;

    }

    lbCount++;
  }
}

void ShowMapStats()
{
  string mapName;
  string fName;
  string lbType;
  int typeChoice;
  cout << "Enter the map name: ";
  cin >> mapName;

  cout << endl;
  cout << "1. Pure leaderboards" << endl;
  cout << "2. Pro leaderboards" << endl;

  cin >> typeChoice;
  switch (typeChoice)
  {
    case 1: lbType = "pure";
    break;
    case 2: lbType = "pro";
    break;
  }

  fName = mapName + "_" + lbType + ".dat";

  system("clear");
  cout << "POS | NAME | TIME | DATE" << endl;
  for (int i = 1; i < board[fName].size(); i++) {
    cout << i << ". ";
    cout << board[fName][i].getPlayerName() << " ";
    cout << board[fName][i].getRunTime() << " ";

    time_t t = board[fName][i].getRunDate();
    struct tm * p = localtime(&t);
    char s[1000];
    strftime(s, 1000, "%F", p);
    cout << s << endl;
  }
  return;
}

void PlayerStats()
{
  string pID;
  cout << "Enter player ID: ";
  cin >> pID;

  cout << endl;
  cout << "Name: " << spelare[pID].getPlayerName() << endl;
  cout << "Runs done: " << spelare[pID].getRunCount() << endl;
  cout << "World Records: " << spelare[pID].getRecordCount() << endl;
  if (spelare[pID].getRecordCount() > 0) {
    cout << "In the following leaderboards: " << endl;
    for (int i = 0; i < spelare[pID].getRecMapSize(); i++) {
      cout << i+1 << ". "<< spelare[pID].getRecMapAt(i) << endl;
    }
  }
  return;
}

void menu()
{
  while(1){
  int mode;
  cout << endl;
  cout << "1. Individual map leaderboards" << endl;
  cout << "2. Player stats" << endl;
  cout << "3. Exit" << endl;

  cin >> mode;

    switch (mode)
    {
      case 1: ShowMapStats();
      break;
      case 2: PlayerStats();
      break;
      case 3: exit(1);
    }
  }
}

int main() {

    system("mkdir ./data");
    system("clear");

    ParseFiles();

    menu();
    return 0;
}
