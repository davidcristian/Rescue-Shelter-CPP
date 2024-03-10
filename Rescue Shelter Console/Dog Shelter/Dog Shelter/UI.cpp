#include <iostream>
#include <sstream>
#include <string>
#include "UI.h"
#include "Validator.h"

/// <summary>
/// Constructs the UI class
/// </summary>
/// <param name="serv">the service</param>
UI::UI(Service& serv) : serv{ serv } {}

/// <summary>
/// Prints the user interface
/// </summary>
/// <param name="menu_options">the menu options</param>
/// <param name="menu_length">the number of menu options</param>
void UI::printMenu(const std::string menu_options[], const int& menu_length)
{
	for (int i = 0; i < menu_length; i++)
	{
		std::cout << menu_options[i] << std::endl;
	}
}

/// <summary>
/// Reads an integer from the user
/// </summary>
/// <param name="prompt">the prompt to be displayed</param>
/// <returns>the integer that was read from the user</returns>
int UI::readInteger(const std::string& prompt)
{
	std::cout << prompt;
	int input = 0;

	std::string line;
	while (std::getline(std::cin >> std::ws, line))
	{
		std::stringstream stream(line);
		if (stream >> input)
			if (stream.eof())
				break;

		std::cout << std::endl << "ERROR: Invalid input!" << std::endl << prompt;
	}

	return input;
}

/// <summary>
/// Reads a string from the user
/// </summary>
/// <param name="prompt">the prompt to be displayed</param>
/// <returns>the string read from the user</returns>
std::string UI::readString(const std::string& prompt)
{
	std::string line;
	std::cout << prompt;

	std::getline(std::cin >> std::ws, line);
	return line;
}

/// <summary>
/// Reads a dog from the user
/// </summary>
/// <returns>the dog that has been read</returns>
Dog UI::readDog()
{
	//std::string name = readString("Name: ");
	//std::string breed = readString("Breed: ");
	//int age = readInteger("Age: ");
	//std::string link = readString("URL: ");

	//Dog dog{ breed, name, age, link };

	Dog dog;

	std::cout << "Dog (name,breed,age,link): ";
	std::cin >> dog;

	return dog;
}

/// <summary>
/// Starts the loop for the admin mode
/// </summary>
void UI::adminLoop()
{
	system("cls");
	std::string menuOptions[] = {
	"0. Exit",
	"1. Change mode",
	"2. List all dogs",
	"3. Add a dog",
	"4. Delete a dog",
	"5. Update a dog"
	};

	int menuLength = sizeof(menuOptions) / sizeof(menuOptions[0]);
	int menuSelection = -1;

	std::cout << "Administrator Mode" << std::endl << std::endl;
	while (menuSelection != 0)
	{
		printMenu(menuOptions, menuLength);
		menuSelection = readInteger("Option: ");

		switch (menuSelection)
		{
		case 0:
			std::cout << "INFO: Quitting." << std::endl;
			break;
		case 1:
			start();
			return;
		case 2:
			listDogs();
			break;
		case 3:
			addDog();
			break;
		case 4:
			deleteDog();
			break;
		case 5:
			updateDog();
			break;
		default:
			std::cout << "ERROR: Invalid menu option!" << std::endl;
		}

		std::cout << std::endl;
	}
}

/// <summary>
/// The menu option to list the dogs
/// </summary>
void UI::listDogs()
{
	if (this->serv.getRepo().getDogs().size() == 0)
	{
		std::cout << "INFO: There are no dogs." << std::endl;
		return;
	}

	for (const Dog& dog : this->serv.getRepo().getDogs())
	{
		std::cout << dog.toString() << std::endl;
	}
}

/// <summary>
/// The menu option to add a dog
/// </summary>
void UI::addDog()
{
	Dog dog = readDog();

	try
	{
		this->serv.add(dog.getName(), dog.getBreed(), dog.getAge(), dog.getPhotohraph());
		std::cout << "INFO: The dog has been added to the shelter!" << std::endl;
	}
	catch (DogException& e)
	{
		std::cout << e.getErrors() << std::endl;
	}
	catch (RepositoryException& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (FileException& e)
	{
		std::cout << e.what() << std::endl;
	}
}

/// <summary>
/// The menu option to delete a dog
/// </summary>
void UI::deleteDog()
{
	std::string name = readString("Name: ");
	std::string breed = readString("Breed: ");

	try
	{
		this->serv.remove(name, breed);
		std::cout << "INFO: The dog has been removed from the shelter!" << std::endl;
	}
	catch (DogException& e)
	{
		std::cout << e.getErrors() << std::endl;
	}
	catch (RepositoryException& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (FileException& e)
	{
		std::cout << e.what() << std::endl;
	}
}

/// <summary>
/// The menu option to update a dog
/// </summary>
void UI::updateDog()
{
	std::string oldName = readString("Name: ");
	std::string oldBreed = readString("Breed: ");
	Dog dog = readDog();

	try
	{
		this->serv.update(oldName, oldBreed, dog.getName(), dog.getBreed(), dog.getAge(), dog.getPhotohraph());
		std::cout << "INFO: The dog has been updated!" << std::endl;
	}
	catch (DogException& e)
	{
		std::cout << e.getErrors() << std::endl;
	}
	catch (RepositoryException& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (FileException& e)
	{
		std::cout << e.what() << std::endl;
	}
}

/// <summary>
/// Starts the loop for the user mode
/// </summary>
void UI::userLoop()
{
	system("cls");
	std::string menuOptions[] = {
	"0. Exit",
	"1. Change mode",
	"2. View the dogs in the shelter",
	"3. See all dogs of a given breed (empty = all) aged less than a given number",
	"4. See adoption list"
	};

	int menuLength = sizeof(menuOptions) / sizeof(menuOptions[0]);
	int menuSelection = -1;

	std::cout << "User Mode" << std::endl << std::endl;
	while (menuSelection != 0)
	{
		printMenu(menuOptions, menuLength);
		menuSelection = readInteger("Option: ");

		switch (menuSelection)
		{
		case 0:
			std::cout << "INFO: Quitting." << std::endl;
			break;
		case 1:
			start();
			return;
		case 2:
			prepareAdoption(this->serv.getRepo());
			break;
		case 3:
			filterDogs();
			break;
		case 4:
			seeAdopted();
			break;
		default:
			std::cout << "ERROR: Invalid menu option!" << std::endl;
		}

		std::cout << std::endl;
	}
}

/// <summary>
/// Shows the dogs to the user and asks if they would like
/// to adopt it. If yes, the dog is added to the adoption list
/// </summary>
/// <param name="dogsToShow">the repository of the dogs to be shown</param>
void UI::prepareAdoption(Repository dogsToShow)
{
	if (dogsToShow.size() == 0)
	{
		std::cout << "INFO: There are no dogs available for adoption." << std::endl;
		return;
	}

	std::string prompt = "Adopt this dog? (yes/next/stop): ";
	std::string option;
	int index = 0;

	while (option != "stop")
	{
		Dog dog = dogsToShow[index];
		std::cout << dog.toString() << std::endl;

		std::string command = "start ";
		system(command.append(dog.getPhotohraph()).c_str());

		option = readString(prompt);
		while (option != "yes" && option != "next" && option != "stop")
		{
			std::cout << "ERROR: Invalid selection!" << std::endl << std::endl;
			option = readString(prompt);
		}

		if (option == "yes")
		{
			dogsToShow.remove(dog);
			
			try
			{
				this->serv.adopt(dog);
			}
			catch (FileException& e)
			{
				std::cout << e.what() << std::endl;
			}

			std::cout << "INFO: The dog has been added to the adoption list." << std::endl;
			index--;
		}

		if (++index == dogsToShow.size())
			index = 0;

		if (dogsToShow.size() == 0)
		{
			std::cout << "INFO: There are no more dogs available for adoption." << std::endl;
			option = "stop";
		}

		if (option != "stop") std::cout << std::endl;
	}
}

/// <summary>
/// Shows the dogs to the user filtered by
/// a given breed and a given age
/// </summary>
void UI::filterDogs()
{
	std::string breed;
	std::cout << "Breed (empty = all): ";
	
	std::getline(std::cin, breed);
	int age = readInteger("Age: ");
	
	Repository repo = serv.filterByBreedAndAge(breed, age);
	prepareAdoption(repo);
}

/// <summary>
/// The menu option to list the dogs adopted by the user
/// </summary>
void UI::seeAdopted()
{
	try
	{
		this->serv.getAdoptionList()->open();
	}
	catch (RepositoryException& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

/// <summary>
/// Starts and handles the user interface
/// </summary>
void UI::start()
{
	system("cls");
	std::string menuOptions[] = {
	"0. Exit",
	"1. Admin Mode",
	"2. User Mode"
	};
	
	int menuLength = sizeof(menuOptions) / sizeof(menuOptions[0]);
	int mode = -1;

	std::cout << "Welcome to the dog shelter!" << std::endl;
	printMenu(menuOptions, menuLength);

	while (mode != 0)
	{
		mode = readInteger("Option: ");

		switch (mode)
		{
		case 0:
			std::cout << "INFO: Quitting." << std::endl;
			break;
		case 1:
			adminLoop();
			return;
		case 2:
			userLoop();
			return;
		default:
			std::cout << "ERROR: Invalid selection!" << std::endl;
		}

		std::cout << std::endl;
	}
}
