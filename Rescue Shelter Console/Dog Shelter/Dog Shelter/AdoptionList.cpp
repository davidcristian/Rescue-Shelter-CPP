#include <fstream>
#include <sstream>
#include "AdoptionList.h"
#include "Validator.h"

/// <summary>
/// Adds a dog to the adoption list
/// </summary>
/// <param name="dog">the dog to add</param>
void AdoptionList::add(const Dog& dog)
{
	this->dogs.push_back(dog);
	adoptionList->write();
}

/// <summary>
/// Override the write function to write
/// a CVS table of the adoption list
/// </summary>
void CSVAdoptionList::write()
{
	std::ofstream f(this->fileName + this->extension);
	if (!f.is_open())
		throw FileException("The file could not be opened!");

	for (const Dog& dog : this->dogs)
	{
		f << dog;
	}

	f.close();
}

/// <summary>
/// Override the open function to open
/// the CVS file in notepad
/// </summary>
void CSVAdoptionList::open()
{
	std::string command = "notepad ";
	system(command.append(this->fileName + this->extension).c_str());
}

/// <summary>
/// Override the write function to write
/// an HTML table of the adoption list
/// </summary>
void HTMLAdoptionList::write()
{
	std::ofstream f(this->fileName + this->extension);
	if (!f.is_open())
		throw FileException("The file could not be opened!");

	f << "<!DOCTYPE html><html><head><title>Adoption List</title></head><body><table border=\"1\" cellpadding=\"5\" cellspacing=\"5\">";
	f << "<tr>";
	f << "<th>Name</th>";
	f << "<th>Breed</th>";
	f << "<th>Age</th>";
	f << "<th>Photograph</th>";
	f << "</tr>";

	for (const Dog& dog : this->dogs)
	{
		f << "<tr>";
		f << "<td>" + dog.getName() + "</td>";
		f << "<td>" + dog.getBreed() + "</td>";
		f << "<td>" + std::to_string(dog.getAge()) + "</td>";
		f << "<td><a href=\"" + dog.getPhotohraph() + "\">link</a></td>";
		f << "</tr>";
	}

	f << "</table></body></html>";
	f.close();
}

/// <summary>
/// Override the open function to open
/// the HTML file in the browser
/// </summary>
void HTMLAdoptionList::open()
{
	std::string command = "start ";

	try
	{
		system(command.append(this->fileName + this->extension).c_str());
	}
	catch (std::exception&)
	{
		throw RepositoryException("Invalid link!");
	}
}
