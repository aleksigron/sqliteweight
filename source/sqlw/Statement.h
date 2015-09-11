#pragma once

#include <cstdint>
#include <string>

#include "ResultSet.h"

struct sqlite3_stmt;

namespace sqlw
{
	class Database;

	class Statement
	{
	public:
		enum class Status
		{
			Uninitialized,
			Prepared,
			Done,
			Finalized,
			Invalid
		};

	private:
		sqlite3_stmt* handle;
		Status status;

	public:
		Statement();
		~Statement();

		/**
		 Get the current status of the statement
		 */
		Status GetStatus() const;

		/**
		 Prepare an SQL statement from a null terminated string
		 */
		bool Prepare(Database& database, const char* statement);

		/**
		 Prepare an SQL statement from a std::string
		 */
		bool Prepare(Database& database, const std::string& statement);

		/**
		 Bind a boolean (as int) value to a placeholder parameter in the
		 prepared statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, bool value);

		/**
		 Bind an int value to a placeholder parameter in the prepared
		 statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, int value);

		/**
		 Bind an int64 value to a placeholder parameter in the prepared
		 statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, int64_t value);

		/**
		 Bind a float value to a placeholder parameter in the prepared
		 statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, float value);

		/**
		 Bind a double value to a placeholder parameter in the prepared
		 statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, double value);

		/**
		 Bind a c-string value to a placeholder parameter in the prepared
		 statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, const char* value);

		/**
		 Bind a c-string (with specified length) value to a placeholder
		 parameter in the prepared statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, const char* value, int length);

		/**
		 Bind a std::string value to a placeholder parameter in the prepared
		 statement. Parameter indexing starts from 1.
		 */
		bool BindParameter(int parameterIndex, const std::string& value);

		/**
		 Execute a prepared statement that doesn't return anything and
		 then reset the statement.
		 */
		bool ExecuteNoResult();

		/**
		 Execute a prepared statement and check whether it returned any rows
		 and then reset the statement.
		 */
		bool ExecuteDidMatchRows();

		/**
		 Execute a prepared statement and initialize a ResultSet object
		 for row iteration.
		 */
		bool ExecuteResultSet(ResultSet& resultOutput);

		/**
		 Get the last inserted row id from the database associated with
		 this statement
		 */
		int64_t GetDatabaseLastInsertId();

		/**
		 Reset the internal state after an execution to do another execution
		 */
		void Reset();

		/**
		 Reset the internal state after an execution to do another execution,
		 but leave the paramater bindings to their previous values
		 */
		void ResetRetainBindings();

		/**
		 Release all resources related to this statement. This will also be
		 called by the destructor. It's a no-op if already finalized.
		 */
		void Finalize();
	};
}
