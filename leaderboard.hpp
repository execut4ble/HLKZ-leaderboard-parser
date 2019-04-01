#include <string>
#include <vector>

using namespace std;

class run {
  private:
    string playerID;
    string playerName;
    int checkpoints;
    int teleports;
    string runTime;
    int runDate;
  public:
    void setPlayerID(string read)
    {
      playerID = read;
    }
    string getPlayerID()
    {
      return playerID;
    }
    void setPlayerName(string read)
    {
      playerName = read;
    }
    string getPlayerName()
    {
      return playerName;
    }
    void setCheckpoints(int read)
    {
      checkpoints = read;
    }
    int getCheckpoints()
    {
      return checkpoints;
    }
    void setTeleports(int read)
    {
      teleports = read;
    }
    int getTeleports()
    {
      return teleports;
    }
    void setRunTime(string read)
    {
      runTime = read;
    }
    string getRunTime()
    {
      return runTime;
    }
    void setRunDate(int read)
    {
      runDate = read;
    }
    int getRunDate()
    {
      return runDate;
    }
};
class player {
  private:
    string playerID;
    string playerName;
    int runCount = 0;
    int recordCount = 0;
    vector<string> recordMaps;

  public:
    void setPlayerID(string read)
    {
      playerID = read;
    }
    string getPlayerID()
    {
      return playerID;
    }
    void setPlayerName(string read)
    {
      playerName = read;
    }
    string getPlayerName()
    {
      return playerName;
    }
    int getRunCount()
    {
      return runCount;
    }
    int getRecordCount()
    {
      return recordCount;
    }
    void incRunCount()
    {
      runCount++;
    }
    void incRecordCount()
    {
      recordCount++;
    }
    void insRecMap(string fileName)
    {
      recordMaps.push_back(fileName);
    }
    int getRecMapSize()
    {
      return recordMaps.size();
    }
    string getRecMapAt(int i)
    {
      return recordMaps.at(i);
    }
};
