#pragma once

#include <vector>
#include <string>
#include <QSqlDatabase>

#include "Dog.h"

class Repository
{
protected:
	std::vector<Dog> dogs;

public:
	Repository() = default;
	virtual ~Repository() = default;

	virtual void add(const Dog& dog, int index = -1);
	virtual void remove(const Dog& dog);
	virtual void update(const Dog& oldDog, const Dog& newDog);

	int indexOf(const Dog& dog) const;
	Dog& findByNameAndBreed(const std::string& name, const std::string& breed);
	
	std::vector<Dog>& getDogs() { return this->dogs; };
	Dog& operator[](const int& index) { return this->dogs[index]; };

	int size() const { return static_cast<int>(this->dogs.size()); };
};

class FileRepository : public Repository
{
private:
	std::string fileName;

	void read();
	void write();
	void setFileName(const std::string& fileName) { this->fileName = fileName; }

public:
	FileRepository(const bool& init = false, const std::string& fileName = "");

	void add(const Dog& dog, int index = -1) override;
	void remove(const Dog& dog) override;
	void update(const Dog& oldDog, const Dog& newDog) override;
};

class PostgreSQLRepository : public Repository
{
private:
	void loadDogs();
	
public:
	PostgreSQLRepository(const bool& init = false);
	~PostgreSQLRepository();

	void add(const Dog& dog, int index = -1) override;
	void remove(const Dog& dog) override;
	void update(const Dog& oldDog, const Dog& newDog) override;
};
