#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#define MAX_LINES 1000
using namespace std;
//convert time string to minutes since midnight for HH:MM
int timeToMinutes(string time) {
  int hours = stoi(time.substr(0, 2));
  int minutes = stoi(time.substr(3));
  return hours * 60 + minutes;
}
// convert minutes since midnight to time string
string minutesToTime(int minutes) {
  int hours = minutes / 60; //find hrs
  int mins = minutes % 60; //usee modulus to find leftovers for MM time
  string time = "";
  if (hours < 10)
    time += "0";
  time += to_string(hours) + ":";
  if (mins < 10)
    time += "0";
  time += to_string(mins);
  return time;
}
// find the available times for a meeting
vector<pair<int, int>> findAvailableTimes(vector<pair<int, int>> schedule,
                                          vector<int> dailyActivity,
                                          int duration) {
  vector<pair<int, int>> availableTimes;
  int lastEnd = dailyActivity[0];
  for (int i = 0; i < schedule.size(); i++) {
    int start = schedule[i].first;
    int end = schedule[i].second;
    if (start - lastEnd >= duration) {
      availableTimes.push_back(make_pair(lastEnd, start));
    }
    lastEnd = max(lastEnd, end);
  }
  if (dailyActivity[1] - lastEnd >= duration) {
    availableTimes.push_back(make_pair(lastEnd, dailyActivity[1]));
  }
  return availableTimes;
}
// find the common available times for two people
vector<pair<int, int>> findCommonTimes(vector<pair<int, int>> schedule1,
                                       vector<int> dailyActivity1,
                                       vector<pair<int, int>> schedule2,
                                       vector<int> dailyActivity2,
                                       int duration) {
  //make vector for the 2 diff schedules/times
  vector<pair<int, int>> availableTimes1 =
      findAvailableTimes(schedule1, dailyActivity1, duration);
  vector<pair<int, int>> availableTimes2 =
      findAvailableTimes(schedule2, dailyActivity2, duration);
  vector<pair<int, int>> commonTimes;
  //prepare a while loop with 2 counters for the common times available
  int i = 0, j = 0;
  while (i < availableTimes1.size() && j < availableTimes2.size()) {
    int start1 = availableTimes1[i].first;
    int end1 = availableTimes1[i].second;
    int start2 = availableTimes2[j].first;
    int end2 = availableTimes2[j].second;
    if (end1 <= start2) {
      i++;
    } else if (end2 <= start1) {
      j++;
    } else {
      int start = max(start1, start2);
      int end = min(end1, end2);
      if (end - start >= duration) {
        commonTimes.push_back(make_pair(start, end));
      }
      if (end1 < end2) {
        i++;
      } else {
        j++;
      }
    }
  }
  return commonTimes;
}
int main() {
  // Used Sample input provided, had too many errors with original method of  while ( cin >> starttime >> comma >> endtime) approach
  vector<pair<int, int>> person1Schedule = {
      {timeToMinutes("7:00"), timeToMinutes("8:30")},
      {timeToMinutes("12:00"), timeToMinutes("13:00")},
      {timeToMinutes("16:00"), timeToMinutes("18:00")}};
  vector<int> person1DailyAct = {timeToMinutes("9:00"), timeToMinutes("19:00")};
  vector<pair<int, int>> person2Schedule = {
      {timeToMinutes("9:00"), timeToMinutes("10:30")},
      {timeToMinutes("12:20"), timeToMinutes("14:30")},
      {timeToMinutes("14:00"), timeToMinutes("15:00")},
      {timeToMinutes("16:00"), timeToMinutes("17:00")}};
  vector<int> person2DailyAct = {timeToMinutes("9:00"), timeToMinutes("18:30")};
  int duration = 30;

  // Find common available times for the two people 
  vector<pair<int, int>> commonTimes =
      findCommonTimes(person1Schedule, person1DailyAct, person2Schedule,
                      person2DailyAct, duration);

  // Convert the common available times to time strings
  vector<string> availableTimes;
  for (int i = 0; i < commonTimes.size(); i++) {
    availableTimes.push_back("[" + minutesToTime(commonTimes[i].first) + " , " +
                             minutesToTime(commonTimes[i].second) + "]");
    
  }

  // Open the output file for writing
  ofstream outputFile("output.txt");
  if (!outputFile) {
      cerr << "Failed to open the output file!" << endl;
      return 1;
  }

  if (availableTimes.empty()) {
    outputFile << "No common available times found" << endl;
  } else {
    outputFile << "Common available times:" << endl;
    for (int i = 0; i < availableTimes.size(); i++) {
      outputFile << availableTimes[i] << endl;
    }
  }

  // Close the output file
  outputFile.close();

  return 0;
}