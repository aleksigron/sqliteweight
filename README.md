# sqliteweight
A simple sqlite3 C++ wrapper.

## Get started
```cpp
sqlw::Database db;
db.Open("example.db");

sqlw::Statement stmt;
stmt.Prepare(db, "SELECT * FROM test");

sqlw::ResultSet result;
if (stmt.ExecuteResultSet(result))
{
  while (result.NextRow())
  {
    printf("row_id: %lld, some_int: %d\n",
      result.GetInt64(0),
      result.GetInt(1));
  }
}
```

## Design rationale
The design goal of this project is to make a simple, but usable C++ wrapper
for sqlite while keeping the code as small and clear as possible.

Sometimes giving a more verbose API helps users better understand the
performance implications of various routines.

## Details on use
It is recommended to check that sqlite is up-to-date. Download the newest version of sqlite at [sqlite.org](https://www.sqlite.org/download.html).

## Version compatibility
The code is tested with sqlite version 3.8.11.1, but it should work exactly the same with any 3.8.x version.
