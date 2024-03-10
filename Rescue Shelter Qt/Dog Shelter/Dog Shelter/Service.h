#pragma once

#include <vector>
#include <string>
#include "Repository.h"
#include "AdoptionList.h"
#include "Validator.h"
#include "Action.h"

class Service
{
private:
	Repository& repo;
	DogValidator& validator;

	AdoptionList* adoptionList;

	std::vector<std::unique_ptr<Action>> repoUndoStack;
	std::vector<std::unique_ptr<Action>> repoRedoStack;

	std::vector<std::unique_ptr<Action>> adoptionUndoStack;
	std::vector<std::unique_ptr<Action>> adoptionRedoStack;

public:
	Service(Repository& repo, AdoptionList* adoptionList, DogValidator& validator, bool generate = false);
	Repository& getRepo() { return this->repo; };

	void add(const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	void remove(const std::string& name, const std::string& breed);
	void update(const std::string& oldName, const std::string& oldBreed, const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	
	void undoRepo();
	void redoRepo();
	void clearRepoUndoRedo();

	int undoAdoption();
	int redoAdoption();
	void clearAdoptionUndoRedo();

	Repository filterByBreedAndAge(const std::string& breed, const int& age);
	Repository filterByString(const std::string& text);

	void adopt(const Dog& dog, Repository& dogsToShow);
	AdoptionList* getAdoptionList() { return this->adoptionList; };
};
