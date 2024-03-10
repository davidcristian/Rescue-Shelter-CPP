#pragma once

#include "Service.h"

class UI
{
private:
	Service& serv;

	void printMenu(const std::string menu_options[], const int& menu_length);
	Dog readDog();

	void adminLoop();

	void listDogs();
	void addDog();
	void deleteDog();
	void updateDog();

	void userLoop();

	void prepareAdoption(Repository dogsToShow);
	void filterDogs();
	void seeAdopted();

public:
	UI(Service& serv);

	void start();

	static int readInteger(const std::string& prompt);
	static std::string readString(const std::string& prompt);
};
