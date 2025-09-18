#include "Log.h"
#include <iostream>
#include <fstream>

//Sets intial severity level and opens file to be written to
void Log::initialize(LevelSeverity issue){
    currentLogLevel = issue;
    std::cout << "Logging Initialized" << std::endl;
    file.open(logFile, std::ios::app); 
    printToFile("Logging Initialized");
}

//Logs error message to file and console if level is valid
void Log::log(LevelSeverity issue, const std::string& message){
    if(isAuthorized(issue)){
        std::cout << message << std::endl;
        printToFile(message);
    }
}

void Log::setLevel(LevelSeverity issue){
    currentLogLevel = issue;
}

std::string Log::levelToString(LevelSeverity error){
    switch(error){
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        case DEATH: return "DEATH";
        default: return "OTHER"; 
    }
}

bool Log::isAuthorized(LevelSeverity testError){
    return testError >= currentLogLevel;
}

void Log::printToFile(const std::string& message){
    if (file.is_open()) {
        file << message << std::endl;
    }
}