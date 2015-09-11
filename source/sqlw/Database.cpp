#include "Database.h"

#include "sqlite3.h"

namespace sqlw
{
	Database::Database() : handle(nullptr)
	{
	}

	Database::~Database()
	{
		if (this->handle != nullptr)
			this->Close();
	}

	bool Database::Open(const char* filename)
	{
		if (this->handle == nullptr)
		{
			int statusCode = sqlite3_open(filename, &(this->handle));

			if (statusCode == SQLITE_OK)
				return true;

			// Open did not succeed, but resources were allocated
			if (this->handle != nullptr)
			{
				sqlite3_close(this->handle);
				this->handle = nullptr;
			}
		}

		return false;
	}

	bool Database::Close()
	{
		if (this->handle != nullptr)
		{
			int statusCode = sqlite3_close(this->handle);

			if (statusCode == SQLITE_OK)
			{
				this->handle = nullptr;
				return true;
			}
		}

		return false;
	}

	bool Database::IsOpen() const
	{
		return this->handle != nullptr;
	}

	int64_t Database::GetLastInsertId() const
	{
		if (this->handle != nullptr)
			return sqlite3_last_insert_rowid(this->handle);
		else
			return 0;
	}
}
