#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>
#include <chrono>
#include <vector>

void clear();

void pwd();

void ls();

void cd(const std::wstring &path);
void mkdir(const std::wstring &path);
void rmdir(const std::wstring &path);

void echo(const std::string& word);

void exitHell();

void execute(const std::string &pathWithArgs);

#endif