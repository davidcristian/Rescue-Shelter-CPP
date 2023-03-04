#include <algorithm>
#include <fstream>
#include <sstream>
#include <QSqlQuery>

#include "Repository.h"
#include "Validator.h"
#include "Config.h"

/// <summary>
/// Adds a dog to the vector of dogs
/// </summary>
/// <param name="dog">the dog to add</param>
void Repository::add(const Dog& dog, int index)
{
	Dog d{};
	try
	{
		d = this->findByNameAndBreed(dog.getName(), dog.getBreed());
		throw DuplicateDogException();
	}
	catch (InexistenDogException&) {}

	if (index < 0 || index > this->size()) index = this->size();
	this->dogs.insert(this->dogs.begin() + index, dog);
}

/// <summary>
/// Removes a dog from the vector of dogs
/// </summary>
/// <param name="dog">the dog to remove</param>
void Repository::remove(const Dog& dog)
{
	auto it = std::find(this->dogs.begin(), this->dogs.end(), dog);
	if (it == this->dogs.end())
		throw InexistenDogException{};

	this->dogs.erase(it);
}

/// <summary>
/// Updates a dog in the vector of dogs
/// </summary>
/// <param name="oldDog">the old dog</param>
/// <param name="newDog">the new dog</param>
void Repository::update(const Dog& oldDog, const Dog& newDog)
{
	auto it = std::find(this->dogs.begin(), this->dogs.end(), oldDog);
	if (it == this->dogs.end())
		throw InexistenDogException{};
	
	this->dogs.at(it - this->dogs.begin()) = newDog;
}

/// <summary>
/// returns the index of a dog
/// </summary>
/// <param name="dog">the dog to look for</param>
/// <returns>the index of the found dog,
///			 -1 if the dog is not found</returns>
int Repository::indexOf(const Dog& dog) const
{
	auto it = std::find(this->dogs.begin(), this->dogs.end(), dog);

	return it == this->dogs.end() ? -1 : static_cast<int>(it - this->dogs.begin());
}

/// <summary>
/// Searches for a dog in the vector of dogs, throws an error if it doesn't exist
/// </summary>
/// <param name="name">the name of the dog</param>
/// <param name="breed">the breed of the dog</param>
/// <returns>the found dog</returns>
Dog& Repository::findByNameAndBreed(const std::string& name, const std::string& breed)
{
	for (auto& dog : this->dogs)
	{
		if (dog.getName() == name && dog.getBreed() == breed)
			return dog;
	}

	throw InexistenDogException{};
}

/// <summary>
/// Constructor for the class, automatically reads
/// from the .txt file when created if init is true
/// </summary>
FileRepository::FileRepository(const bool& init, const std::string& fileName) : Repository{}, fileName{ fileName }
{
	if (init)
		this->read();
}

/// <summary>
/// Reads all the dogs from the TXT file into the repository
/// </summary>
void FileRepository::read()
{
	if (this->fileName.empty()) return;

	std::ifstream f(this->fileName);
	if (!f.is_open())
		throw FileException("The file could not be opened!");

	Dog dog{};
	while (f >> dog)
		this->add(dog);

	f.close();
}

/// <summary>
/// Writes a TXT file of the dogs in the repository
/// </summary>
void FileRepository::write()
{
	if (this->fileName.empty()) return;

	std::ofstream f(this->fileName);
	if (!f.is_open())
		throw FileException("The file could not be opened!");

	for (const Dog& dog : this->dogs)
	{
		f << dog;
	}

	f.close();
}

void FileRepository::add(const Dog& dog, int index)
{
	Repository::add(dog, index);
	this->write();
}

void FileRepository::remove(const Dog& dog)
{
	Repository::remove(dog);
	this->write();
}

void FileRepository::update(const Dog& oldDog, const Dog& newDog)
{
	Repository::update(oldDog, newDog);
	this->write();
}

/// <summary>
/// Constructor for the class, automatically reads
/// from the database when created if init is true
/// </summary>
/// <param name="init"></param>
PostgreSQLRepository::PostgreSQLRepository(const bool& init) : Repository{}
{
	if (!QSqlDatabase::isDriverAvailable(DB_DRIVER))
		throw DatabaseException("Could not find the PostgreSQL database driver!");

	QSqlDatabase db = QSqlDatabase::addDatabase(DB_DRIVER, DB_STRING);
	db.setHostName(DB_HOSTNAME);
	db.setPort(DB_PORT);
	db.setDatabaseName(DB_DATABASE);
	db.setUserName(DB_USER);
	db.setPassword(DB_PASSWORD);

	if (!db.open())
		throw DatabaseException("Could not connect to the database!");

	if (init)
		this->loadDogs();
}

/// <summary>
/// Loads all the dogs from the database into the repository
/// </summary>
void PostgreSQLRepository::loadDogs()
{
	QSqlQuery query{ QSqlDatabase::database(DB_STRING) };
	query.prepare("SELECT * FROM dogs");
	if (!query.exec())
		throw DatabaseException("Could not load the dogs from the database!");

	while (query.next())
	{
		//int id = query.value(0).toInt();
		std::string name = query.value(1).toString().toStdString();
		std::string breed = query.value(2).toString().toStdString();
		int age = query.value(3).toInt();
		std::string photograph = query.value(4).toString().toStdString();

		Dog dog{ name, breed, age, photograph };
		this->add(dog);
	}
}

void PostgreSQLRepository::add(const Dog& dog, int index)
{
	Repository::add(dog, index);

	QSqlQuery query{ QSqlDatabase::database(DB_STRING) };
	query.prepare("INSERT INTO dogs (name, breed, age, photograph) VALUES (:name, :breed, :age, :photograph)");
	
	query.bindValue(":name", QString::fromStdString(dog.getName()));
	query.bindValue(":breed", QString::fromStdString(dog.getBreed()));
	query.bindValue(":age", dog.getAge());
	query.bindValue(":photograph", QString::fromStdString(dog.getPhotohraph()));

	if (!query.exec())
		throw DatabaseException("Could not add the dog to the database!");
}

void PostgreSQLRepository::remove(const Dog& dog)
{
	Repository::remove(dog);

	QSqlQuery query{ QSqlDatabase::database(DB_STRING) };
	query.prepare("DELETE FROM dogs WHERE name = :name AND breed = :breed");

	query.bindValue(":name", QString::fromStdString(dog.getName()));
	query.bindValue(":breed", QString::fromStdString(dog.getBreed()));
	
	if (!query.exec())
		throw DatabaseException("Could not remove the dog from the database!");
}

void PostgreSQLRepository::update(const Dog& oldDog, const Dog& newDog)
{
	Repository::update(oldDog, newDog);
	
	QSqlQuery query{ QSqlDatabase::database(DB_STRING) };
	query.prepare("UPDATE dogs SET name = :newName, breed = :newBreed, age = :newAge, photograph = :newPhotograph WHERE name = :oldName AND breed = :oldBreed");
	
	query.bindValue(":oldName", QString::fromStdString(oldDog.getName()));
	query.bindValue(":oldBreed", QString::fromStdString(oldDog.getBreed()));

	query.bindValue(":newName", QString::fromStdString(newDog.getName()));
	query.bindValue(":newBreed", QString::fromStdString(newDog.getBreed()));
	query.bindValue(":newAge", newDog.getAge());
	query.bindValue(":newPhotograph", QString::fromStdString(newDog.getPhotohraph()));
	
	if (!query.exec())
		throw DatabaseException("Could not update the dog in the database!");
}

PostgreSQLRepository::~PostgreSQLRepository()
{
	QSqlDatabase::database(DB_STRING).close();
	QSqlDatabase::removeDatabase(DB_STRING);
}
