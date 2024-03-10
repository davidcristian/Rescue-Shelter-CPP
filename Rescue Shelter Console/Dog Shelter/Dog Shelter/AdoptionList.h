#pragma once

#include <vector>
#include <string>
#include "Dog.h"

class AdoptionList
{
protected:
	std::vector<Dog> dogs;
	std::string fileName = "Dogs";

public:
	AdoptionList() = default;
	virtual ~AdoptionList() = default;

	void add(const Dog& dog);
	virtual void write() = 0;
	virtual void open() = 0;

	int size() const { return static_cast<int>(this->dogs.size()); };
};

class CSVAdoptionList : public AdoptionList
{
private:
	std::string extension = ".csv";

public:
	CSVAdoptionList() = default;

	void write() override;
	void open() override;
};

class HTMLAdoptionList : public AdoptionList
{
private:
	std::string extension = ".html";

public:
	HTMLAdoptionList() = default;

	void write() override;
	void open() override;
};
