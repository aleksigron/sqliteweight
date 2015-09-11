#include "Statement.h"

#include "sqlite3.h"

#include "Database.h"

namespace sqlw
{
	Statement::Statement() : handle(nullptr), status(Status::Uninitialized)
	{
	}

	Statement::~Statement()
	{
		this->Finalize();
	}

	Statement::Status Statement::GetStatus() const
	{
		return this->status;
	}

	bool Statement::Prepare(Database& database, const char* statement)
	{
		if (this->status == Status::Uninitialized && database.IsOpen())
		{
			int result = sqlite3_prepare_v2(database.handle, statement,
											-1, &(this->handle), nullptr);

			//assert(result == SQLITE_OK);

			if (result == SQLITE_OK)
			{
				this->status = Status::Prepared;
				return true;
			}
		}

		return false;
	}

	bool Statement::Prepare(Database& database, const std::string& statement)
	{
		if (this->status == Status::Uninitialized && database.IsOpen())
		{
			int result = sqlite3_prepare_v2(database.handle, statement.c_str(),
											static_cast<int>(statement.size()),
											&(this->handle), nullptr);

			//assert(result == SQLITE_OK);

			if (result == SQLITE_OK)
			{
				this->status = Status::Prepared;
				return true;
			}
		}

		return false;
	}

	bool Statement::BindParameter(int parameterIndex, bool value)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_int(this->handle, parameterIndex, value ? 1 : 0);
			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::BindParameter(int parameterIndex, int value)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_int(this->handle, parameterIndex, value);
			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::BindParameter(int parameterIndex, int64_t value)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_int64(this->handle, parameterIndex, value);
			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::BindParameter(int parameterIndex, float value)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_double(this->handle, parameterIndex, double(value));
			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::BindParameter(int parameterIndex, double value)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_double(this->handle, parameterIndex, value);
			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::BindParameter(int parameterIndex, const char* value)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_text(this->handle, parameterIndex,
										   value, -1, SQLITE_STATIC);

			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::BindParameter(int parameterIndex, const char* value, int length)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_text(this->handle, parameterIndex,
										   value, length, SQLITE_STATIC);

			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::BindParameter(int parameterIndex, const std::string& value)
	{
		if (this->status == Status::Prepared)
		{
			int result = sqlite3_bind_text(this->handle, parameterIndex, value.c_str(),
										   static_cast<int>(value.size()),
										   SQLITE_STATIC);

			return result == SQLITE_OK;
		}
		else
			return false;
	}

	bool Statement::ExecuteNoResult()
	{
		bool succeeded = false;

		if (this->status == Status::Prepared)
		{
			int result = sqlite3_step(this->handle);

			if (result == SQLITE_DONE)
			{
				this->status = Status::Done;
				succeeded = true;
			}
		}

		this->Reset();

		return succeeded;
	}

	bool Statement::ExecuteDidMatchRows()
	{
		bool succeeded = false;

		if (this->status == Status::Prepared)
		{
			int sqliteStatus = sqlite3_step(this->handle);

			this->status = Status::Done;

			if (sqliteStatus == SQLITE_ROW)
				succeeded = true;
		}

		this->Reset();

		return succeeded;
	}

	bool Statement::ExecuteResultSet(ResultSet& resultOutput)
	{
		if (this->status == Status::Prepared)
		{
			resultOutput.SetStatement(this->handle);
			return true;
		}
		else
			return false;
	}

	int64_t Statement::GetDatabaseLastInsertId()
	{
		if (this->status == Status::Prepared || this->status == Status::Done)
		{
			sqlite3* db = sqlite3_db_handle(this->handle);

			if (db != nullptr)
				return sqlite3_last_insert_rowid(db);
		}

		return 0;
	}

	void Statement::Reset()
	{
		//assert(this->status != Status::Invalid);

		if (this->status == Status::Prepared || this->status == Status::Done)
		{
			int sqliteStatus = sqlite3_reset(this->handle);

			if (sqliteStatus == SQLITE_OK)
			{
				sqliteStatus = sqlite3_clear_bindings(this->handle);

				if (sqliteStatus == SQLITE_OK)
					this->status = Status::Prepared;
			}
		}
	}

	void Statement::ResetRetainBindings()
	{
		//assert(this->status != Status::Invalid);

		if (this->status == Status::Prepared || this->status == Status::Done)
		{
			int sqliteStatus = sqlite3_reset(this->handle);

			if (sqliteStatus == SQLITE_OK)
				this->status = Status::Prepared;
		}
	}
	
	void Statement::Finalize()
	{
		if (this->status == Status::Prepared ||
			this->status == Status::Done ||
			this->status == Status::Invalid)
		{
			sqlite3_finalize(this->handle);
			this->status = Status::Finalized;
		}
	}
}
