#pragma once

#include <cstdint>

struct sqlite3;

namespace sqlw
{
	class Database
	{
	private:
		sqlite3* handle;

		friend class Statement;

	public:
		Database();
		~Database();

		bool Open(const char* filename);
		bool Close();

		bool IsOpen() const;
		int64_t GetLastInsertId() const;
	};
}
