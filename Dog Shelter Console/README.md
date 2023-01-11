# Dog Shelter
The continuation of the project with a GUI implementation can be found in the parent directory.

### 1

The dog shelter needs a software application to help them find adoptive parents for the dogs  they are taking care of. The application can be used in two modes: `administrator` and `user`. When the application is started, it will offer the option to select the mode.\
**Administrator mode:** The application will have a database that holds all the dogs in the shelter at a given moment. The shelter employees must be able to update the database, meaning: add a new dog, delete a dog (when the dog is adopted) and update the information of a dog. Each **Dog** has a `breed`, a `name`, an `age` and a `photograph`. The photograph is memorised as a link towards an online resource (the photograph on the presentation site of the centre). The administrators will also have the option to see all the dogs in the shelter.\
**User mode:** A user can access the application and choose one or more dogs to adopt. The application will allow the user to:
- See the dogs in the database, one by one. When the user chooses this option, the data of the first dog (breed, name, age) is displayed, along with its photograph.
- Choose to adopt the dog, in which case the dog is added to the user’s adoption list.
- Choose not to adopt the dog and to continue to the next. In this case, the information corresponding to the next dog is shown and the user is again offered the possibility to adopt it. This can continue as long as the user wants, as when arriving to the end of the list, if the user chooses next, the application will again show the first dog.
- See all the dogs of a given breed, having an age less than a given number. If no breed is provided, then all the dogs will be considered. The functionalities outlined above apply again in this case.
- See the adoption list.

### 2
1. Use STL algorithms wherever possible in your application (e.g. in your filter function you could use `copy_if`, `count_if`). Replace all your for loops either with STL algorithms, or with C++11's ranged-based for loop.

2. Store data in a text file. When the program starts, entities are read from the file. Modifications made during program execution are stored in the file. Implement this using the `iostream` library. Create insertion and extraction operators for your entities and use these when reading/writing to files or the console.

3. Use exceptions to signal errors:
    - from the repository;
    - validation errors – validate your entities using Validator classes;
    - create your own exception classes.
    - validate program input.

4.	Store your adoption list in a file. When the application starts, the user should choose the type of file between `CSV` or `HTML`. Depending on the type, the application will save the list in the correct format.

    **Indications**\
    The CSV file will contain each entity on one line and the attributes will be separated by comma.\
    The HTML file will contain a table, in which each row holds the data of one entity. The columns of the table will contain the names of the data attributes.\
    `CSV` and `HTML` files are used to save and display data to the user; they act as program outputs, so data should not be read from them!

5. Add a new command, allowing the user to see the adoption list.\
Displaying the list means opening the saved file (`CSV` or `HTML`) with the correct application (`CSV` files using Notepad, Notepad++, Microsoft Excel etc. and `HTML` files using a browser)

6. Create a UML class diagram for your entire application. For this, you can use any tool that you like ([StarUML](https://staruml.io/) or [LucidChart](https://www.lucidchart.com/) are only some examples. Many other options exist.

### 3
Implement the following:

1. A class template Comparator, which has a pure virtual function, compare, that receives two generic elements; the function return type is bool.

2. Two concrete classes which inherit from Comparator and implement the compare function, in different ways (e.g. ComparatorAscendingByTitle, ComparatorDescendingByAge, etc…)

3. Implement a generic sort function, which receives an STL vector and a Comparator (pointer or reference, for polymorphism) and sorts the elements in the vector, according to the compare function in the Comparator.

4. Test the function.

5. Make sure you have no memory leaks.
