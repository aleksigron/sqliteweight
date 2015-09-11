#pragma once

#include <cstdint>
#include <string>

struct sqlite3_stmt;

namespace sqlw
{
	class ResultSet
	{
	public:
		enum class Status
		{
			Uninitialized,
			Initialized,
			RowAvailable,
			Done,
			Invalid
		};

	private:
		sqlite3_stmt* statement;
		Status status;

		void SetStatement(sqlite3_stmt* statement);

		friend class Statement;

	public:
		ResultSet();

		bool NextRow();

		bool GetIntAsBool(int index);
		int GetInt(int index);
		int64_t GetInt64(int index);
		float GetFloat(int index);
		double GetDouble(int index);
		std::string GetString(int index);
	};
}
