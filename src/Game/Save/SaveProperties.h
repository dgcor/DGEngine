#pragma once

namespace Save
{
	struct Properties
	{
		bool saveDefaults{ false };
		bool saveCurrentPlayer{ false };
		bool saveQuests{ false };
		void* customProperty{ nullptr };
	};
}
