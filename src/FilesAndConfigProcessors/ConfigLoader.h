#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
using json = nlohmann::json;

namespace ConfigLoader
{
	/*
	struct SkillGroup
	{
		std::string name;
		int bonusCap = 0;

		std::vector<std::string> skills;
		std::vector<std::string> bless;
		std::vector<std::string> restrict;
	};

	struct Config
	{
		bool grouping = true;
		int nonGroupingBonusCap = 100;

		std::vector<SkillGroup> groups;
	};

	inline void to_json(json& j, const SkillGroup& g)
	{
		j = {
			{ "Name", g.name },
			{ "Bonus Cap", g.bonusCap },
			{ "Skills", g.skills },
			{ "Bless", g.bless },
			{ "Restrict", g.restrict }
		};
	}

	inline void to_json(json& j, const Config& c)
	{
		j = {
			{ "Grouping", c.grouping },
			{ "Non-grouping Bonus Cap", c.nonGroupingBonusCap },
			{ "Groups", c.groups }
		};
	}

	inline void from_json(const json& j, SkillGroup& g)
	{
		g.name = j.value("Name", "");
		g.bonusCap = j.value("Bonus Cap", 0);
		g.skills = j.value("Skills", std::vector<std::string>{});
		g.bless = j.value("Bless", std::vector<std::string>{});
		g.restrict = j.value("Restrict", std::vector<std::string>{});
	}

	inline void from_json(const json& j, Config& c)
	{
		c.grouping = j.value("Grouping", true);
		c.nonGroupingBonusCap = j.value("Non-grouping Bonus Cap", 100);
		c.groups = j.value("Groups", std::vector<SkillGroup>{});
	}
	*/

	struct Config
    {
        float blessOverride = 0.0f;
        std::vector<std::string> restricted{};	//the second value (int) is to store the current XP of the player at the time of restriction
        std::unordered_map<std::string, float> blessed{};
    };

	inline void to_json(json& j, const Config& c)
	{
		j = {
			{ "Blessing Multiplier Override", c.blessOverride },
			{ "Restrict", c.restricted },
			{ "Bless", c.blessed }	//The restrict array only needs the skill name
		};
	}

	inline void from_json(const json& j, Config& c)
	{
		c.blessOverride = j.value("Blessing Multiplier Override", 1.0f);
		c.restricted = j.value("Restrict", std::vector<std::string>{});		
		c.blessed = j.value("Bless", std::unordered_map<std::string, float>{});
	}

	const std::string filePath = "Data/SKSE/Plugins/Divine_Restriction_Configs.json";
	static std::filesystem::file_time_type lastModifiedTime = std::filesystem::file_time_type::min();
	//whether or not the config file is changed and saved
	bool IfConfigChanged();

	//Gets general data from config JSON in JSON format
	void LoadConfig(Config& config);
}