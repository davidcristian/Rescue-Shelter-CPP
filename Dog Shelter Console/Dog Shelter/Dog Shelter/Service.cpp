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
	Dog d{ name, breed, age, photograph };
	this->validator.validate(d);
	this->repo.add(d);
}

/// <summary>
/// Removes a dog from the repository
/// </summary>
/// <param name="name">the name of the dog</param>
/// <param name="breed">the age breed the dog</param>
void Service::remove(const std::string& name, const std::string& breed)
{
	Dog d = this->repo.findByNameAndBreed(name, breed);
	this->repo.remove(d);
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

	for (const Dog& dog : this->getRepo().getDogs())
	{
		if ((breed.length() == 0 || dog.getBreed().compare(breed) == 0) && dog.getAge() < age)
			newRepo.add(dog);
	}

	return newRepo;
}

/// <summary>
/// Adds a dog to the adoption list
/// </summary>
/// <param name="dog">the dog to adopt</param>
void Service::adopt(const Dog& dog)
{
	this->repo.remove(dog);
	adoptionList->add(dog);
}
