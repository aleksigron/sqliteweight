#include "ResultSet.h"

#include "sqlite3.h"

namespace sqlw
{
	ResultSet::ResultSet() : statement(nullptr), status(Status::Uninitialized)
	{
	}

	void ResultSet::SetStatement(sqlite3_stmt* statement)
	{
		this->statement = statement;

		if (statement != nullptr)
			this->status = Status::Initialized;
		else
			this->status = Status::Uninitialized;
	}

	bool ResultSet::NextRow()
	{
		if (this->status == Status::Initialized || this->status == Status::RowAvailable)
		{
			int status = sqlite3_step(this->statement);

			if (status == SQLITE_ROW)
			{
				this->status = Status::RowAvailable;
				return true;
			}
			else if (status == SQLITE_DONE)
				this->status = Status::Done;
			else
				this->status = Status::Invalid;

			return false;

		}
		else
			return false;
	}

	bool ResultSet::GetIntAsBool(int index)
	{
		return sqlite3_column_int(this->statement, index) != 0;
	}

	int ResultSet::GetInt(int index)
	{
		return sqlite3_column_int(this->statement, index);
	}

	int64_t ResultSet::GetInt64(int index)
	{
		return sqlite3_column_int64(this->statement, index);
	}

	float ResultSet::GetFloat(int index)
	{
		return float(sqlite3_column_double(this->statement, index));
	}

	double ResultSet::GetDouble(int index)
	{
		return sqlite3_column_double(this->statement, index);
	}

	std::string ResultSet::GetString(int index)
	{
		return std::string((const char*)sqlite3_column_text(this->statement, index));
	}
}
