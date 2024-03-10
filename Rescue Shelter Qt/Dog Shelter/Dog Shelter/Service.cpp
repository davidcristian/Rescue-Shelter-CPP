#include <algorithm>
#include "Service.h"

/// <summary>
/// Constructs the Service class
/// </summary>
/// <param name="repo">the repository</param>
/// <param name="adoptionList">the adoption list</param>
/// <param name="validator">the validator</param>
/// <param name="generate">whether or not to generate 10 startup entries</param>
Service::Service(Repository& repo, AdoptionList* adoptionList, DogValidator& validator, bool generate) : repo{ repo }, adoptionList{ adoptionList }, validator { validator }
{
	if (generate)
	{
		repo.add(Dog{ "mec", "poodle", 4, "https://upload.wikimedia.org/wikipedia/commons/thumb/f/f8/Full_attention_%288067543690%29.jpg/330px-Full_attention_%288067543690%29.jpg" });
		repo.add(Dog{ "aydo", "beagle", 2, "https://upload.wikimedia.org/wikipedia/commons/thumb/5/55/Beagle_600.jpg/330px-Beagle_600.jpg" });
		repo.add(Dog{ "clyde", "landseer", 2, "https://upload.wikimedia.org/wikipedia/commons/thumb/0/04/Landseer.jpg/330px-Landseer.jpg" });
		repo.add(Dog{ "fixy", "barbet", 6, "https://upload.wikimedia.org/wikipedia/commons/c/cf/Chien_de_race_Barbet.jpg" });
		repo.add(Dog{ "ossi", "poodle", 3, "https://upload.wikimedia.org/wikipedia/commons/thumb/f/f8/Full_attention_%288067543690%29.jpg/330px-Full_attention_%288067543690%29.jpg" });

		repo.add(Dog{ "zani", "pug", 3, "https://upload.wikimedia.org/wikipedia/commons/thumb/f/f0/Mops_oct09_cropped2.jpg/330px-Mops_oct09_cropped2.jpg" });
		repo.add(Dog{ "geno", "shikoku", 3, "https://upload.wikimedia.org/wikipedia/commons/6/69/Shikokuken.jpg" });
		repo.add(Dog{ "dary", "english mastiff", 8, "https://upload.wikimedia.org/wikipedia/commons/thumb/c/cc/Westgort_Anticipation_17_months.JPG/330px-Westgort_Anticipation_17_months.JPG" });
		repo.add(Dog{ "ikas", "dachshund", 6, "https://upload.wikimedia.org/wikipedia/commons/thumb/2/27/Short-haired-Dachshund.jpg/330px-Short-haired-Dachshund.jpg" });
		repo.add(Dog{ "flapp", "maltese", 4, "https://upload.wikimedia.org/wikipedia/commons/thumb/9/94/Maltese_600.jpg/330px-Maltese_600.jpg" });
	}
}

/// <summary>
/// Adds a dog to the repository
/// </summary>
/// <param name="name">the name of the dog</param>
/// <param name="breed">the breed of the dog</param>
/// <param name="age">the age of the dog</param>
/// <param name="photograph">the photograph of the dog</param>
void Service::add(const std::string& name, const std::string& breed, const int& age, const std::string& photograph)
{
	Dog dog{ name, breed, age, photograph };
	this->validator.validate(dog);
	this->repo.add(dog);

	std::unique_ptr<Action> p = std::make_unique<ActionAdd>(dog, repo, this->repo.size());
	repoUndoStack.push_back(std::move(p));
	repoRedoStack.clear();
}

/// <summary>
/// Removes a dog from the repository
/// </summary>
/// <param name="name">the name of the dog</param>
/// <param name="breed">the age breed the dog</param>
void Service::remove(const std::string& name, const std::string& breed)
{
	Dog dog = this->repo.findByNameAndBreed(name, breed);
	int index = repo.indexOf(dog);
	this->repo.remove(dog);

	std::unique_ptr<Action> p = std::make_unique<ActionRemove>(dog, repo, index);
	repoUndoStack.push_back(std::move(p));
	repoRedoStack.clear();
}

/// <summary>
/// Updates a dog in the repository
/// </summary>
/// <param name="name">the old name of the dog</param>
/// <param name="breed">the old breed of the dog</param>
/// <param name="name">the name of the dog</param>
/// <param name="breed">the breed of the dog</param>
/// <param name="age">the age of the dog</param>
/// <param name="photograph">the photograph of the dog</param>
void Service::update(const std::string& oldName, const std::string& oldBreed, const std::string& name, const std::string& breed, const int& age, const std::string& photograph)
{
	Dog newDog{ name, breed, age, photograph };
	this->validator.validate(newDog);

	Dog oldDog = repo.findByNameAndBreed(oldName, oldBreed);
	this->repo.update(oldDog, newDog);

	std::unique_ptr<Action> p = std::make_unique<ActionUpdate>(oldDog, newDog, repo);
	repoUndoStack.push_back(std::move(p));
	repoRedoStack.clear();
}

/// <summary>
/// Filter the dogs based on a given breed and age
/// </summary>
/// <param name="breed">the breed to filter by</param>
/// <param name="age">the age to filter by</param>
/// <returns>the filtered repo</returns>
Repository Service::filterByBreedAndAge(const std::string& breed, const int& age)
{
	Repository newRepo;

	for (const Dog& dog : this->repo.getDogs())
	{
		if ((breed.length() == 0 || dog.getBreed().compare(breed) == 0) && dog.getAge() < age)
			newRepo.add(dog);
	}

	return newRepo;
}

/// <summary>
/// Filter the dogs based on a given name
/// </summary>
/// <param name="text">the string to filter by</param>
/// <returns>the filtered repo</returns>
Repository Service::filterByString(const std::string& text)
{
	Repository newRepo;

	for (const Dog& dog : this->repo.getDogs())
	{
		if (dog.getName().find(text) != std::string::npos) // || dog.getBreed().find(text) != std::string::npos)
			newRepo.add(dog);
	}

	return newRepo;
}

/// <summary>
/// Adds a dog to the adoption list
/// </summary>
/// <param name="dog">the dog to adopt</param>
void Service::adopt(const Dog& dog, Repository& dogsToShow)
{
	int index = repo.indexOf(dog);
	
	this->repo.remove(dog);
	adoptionList->add(dog);

	std::unique_ptr<Action> p = std::make_unique<ActionAdopt>(
		dog, repo, index,
		dogsToShow, dogsToShow.indexOf(dog),
		adoptionList, adoptionList->size() + 1);
	adoptionUndoStack.push_back(std::move(p));
	adoptionRedoStack.clear();
}

/// <summary>
/// Undo the previous repo operation
/// </summary>
void Service::undoRepo()
{
	if (repoUndoStack.size() == 0)
		throw UndoException("There is nothing to undo!");

	std::unique_ptr<Action> action = std::move(repoUndoStack.back());
	repoUndoStack.pop_back();

	action.get()->executeUndo();
	repoRedoStack.push_back(std::move(action));
}

/// <summary>
/// Redo the previous repo operation
/// </summary>
void Service::redoRepo()
{
	if (repoRedoStack.size() == 0)
		throw RedoException("There is nothing to redo!");

	std::unique_ptr<Action> action = std::move(repoRedoStack.back());
	repoRedoStack.pop_back();

	action.get()->executeRedo();
	repoUndoStack.push_back(std::move(action));
}

/// <summary>
/// Clears the repo undo and redo stacks
/// </summary>
void Service::clearRepoUndoRedo()
{
	repoUndoStack.clear();
	repoRedoStack.clear();
}

/// <summary>
/// Undo the adoption list previous operation
/// <returns>the old index of the dog in the action</returns>
/// </summary>
int Service::undoAdoption()
{
	if (adoptionUndoStack.size() == 0)
		throw UndoException("There is nothing to undo!");

	std::unique_ptr<Action> action = std::move(adoptionUndoStack.back());
	adoptionUndoStack.pop_back();

	Action* tempBase = action.get();
	ActionAdopt* tempDerived = static_cast<ActionAdopt*>(tempBase);

	int index = tempDerived->getDogsToShowIndex();

	action.get()->executeUndo();
	adoptionRedoStack.push_back(std::move(action));

	return index;
}

/// <summary>
/// Redo the adoption list previous operation
/// <returns>the old index of the dog in the action</returns>
/// </summary>
int Service::redoAdoption()
{
	if (adoptionRedoStack.size() == 0)
		throw RedoException("There is nothing to redo!");

	std::unique_ptr<Action> action = std::move(adoptionRedoStack.back());
	adoptionRedoStack.pop_back();

	Action* tempBase = action.get();
	ActionAdopt* tempDerived = static_cast<ActionAdopt*>(tempBase);

	int index = tempDerived->getDogsToShowIndex();

	action.get()->executeRedo();
	adoptionUndoStack.push_back(std::move(action));

	return index;
}

/// <summary>
/// Clears the adoption list undo and redo stacks
/// </summary>
void Service::clearAdoptionUndoRedo()
{
	adoptionUndoStack.clear();
	adoptionRedoStack.clear();
}
