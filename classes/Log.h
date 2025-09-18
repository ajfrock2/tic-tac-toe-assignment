#pragma once
#include <string>
#include <fstream>
class Log {
public:

    enum LevelSeverity{
        INFO,
        WARNING,
        ERROR,
        DEATH,
    };

    //Sets intial severity level and opens file to be written to
    void initialize(LevelSeverity issue);

    //Logs error message to file and console if level is valid
    void log(LevelSeverity issue, const std::string& message);

    void setLevel(LevelSeverity issue);

    std::string levelToString(LevelSeverity error);

private: 
    std::string logFile{"log.txt"};
    std::ofstream file {};
    LevelSeverity currentLogLevel {};

    bool isAuthorized(LevelSeverity testError);
    void printToFile(const std::string& message);
};