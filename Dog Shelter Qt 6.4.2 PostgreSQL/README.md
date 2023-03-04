# Dog Shelter
This project is the Database continuation of the GUI version found in the parent directory.

PostgresSQL version `15.2` is used as the database system.

The `dogs` table can be created with the query file found in the `PostgreSQL` directory.

The `libpq.dll` file must be added to the `PATH` environment variable.

There is a `Config_template.h` file in the `Utils` header directory. Create a copy named `Config.h` and update the database connection details for your database.

### 1
In addition to the file-based implementation for the repository, implement a true database-backed repository. For this, use inheritance and polymorphism. You are free to choose any type of database management system (e.g. `MySQL`, `SQLite`, `PostgreSQL`, `Couchbase` etc.).
