#pragma once

#include <string>
#include "Repository.h"
#include "AdoptionList.h"
#include "Validator.h"

class Service
{
private:
	Repository& repo;
	AdoptionList* adoptionList;
	DogValidator& validator;

public:
	Service(Repository& repo, AdoptionList* adoptionList, DogValidator& validator, bool generate = false);
	Repository& getRepo() { return this->repo; };

	void add(const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	void remove(const std::string& name, const std::string& breed);
	void update(const std::string& oldName, const std::string& oldBreed, const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	
	Repository filterByBreedAndAge(const std::string& breed, const int& age);

	void adopt(const Dog& dog);
	AdoptionList* getAdoptionList() { return this->adoptionList; };
};
