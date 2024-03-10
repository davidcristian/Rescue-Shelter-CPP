#include <iostream>
#include <crtdbg.h>

#include "Test.h"
#include "Repository.h"
#include "AdoptionList.h"
#include "Service.h"
#include "Validator.h"
#include "UI.h"

/// <summary>
/// Checks for memory leaks
/// </summary>
void checkLeaks()
{
	if (_CrtDumpMemoryLeaks() == false)
		std::cout << "INFO: No memory leaks were detected. The program executed correctly." << std::endl;
	else
		std::cout << "WARNING: Memory leaks were detected. See the Output tab for more information." << std::endl;
}

/// <summary>
/// Program entry point
/// </summary>
/// <returns>0 if the execution is successful</returns>
int main()
{
	// Run the tests
	{
		Test test{};
		test.runAllTests();
	}

	// Run the main program
	{
		int repoType = -1;
		std::cout << "Select the type of file to save the adoption list to." << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cout << "1. CSV" << std::endl;
		std::cout << "2. HTML" << std::endl;

		while (repoType < 0 || repoType > 2)
		{
			repoType = UI::readInteger("Option: ");

			if (repoType < 0 || repoType > 2)
				std::cout << "ERROR: Invalid option!" << std::endl << std::endl;
		}

		if (repoType == 0)
		{
			std::cout << "INFO: Quitting." << std::endl << std::endl;
			
			checkLeaks();
			return 0;
		}

		AdoptionList* adoptionList = nullptr;
		switch (repoType)
		{
		case 1:
			adoptionList = new CSVAdoptionList;
			break;
		case 2:
			adoptionList = new HTMLAdoptionList;
			break;
		default:
			std::cout << "ERROR: An unexpected error has occurred!" << std::endl;
			return 1;
		}

		Repository repo{ true, "Dogs.txt"};
		DogValidator validator;
		Service serv{ repo, adoptionList, validator, repo.size() == 0 };

		UI ui{ serv };
		ui.start();
		
		delete adoptionList;
	}

	checkLeaks();
	return 0;
}
