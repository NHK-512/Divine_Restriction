#include "ConfigLoader.h"
using namespace ConfigLoader;
using json = nlohmann::json;

Config InitDefaultConfig()
{
	namespace fs = std::filesystem;
	Config config;
    //test configs for blessed skills
    // config.blessed.insert({"test", 999});
    // config.blessed.insert({"block", 12});

    //WIP for future updates
    /*
	config.grouping = true;
    config.nonGroupingBonusCap = 100;

    config.groups =
    {
        {
            "Physical",
            80,
            {
                "One-Handed",
                "Two-Handed",
                "Archery"
            },
            {
                "One-Handed"
            },
            {}
        },

        {
            "Defense",
            50,
            {
                "Light Armor",
                "Heavy Armor",
                "Block"
            },
            {},
            {
                "Heavy Armor"
            }
        },

        {
            "Magic",
            100,
            {
                "Alteration",
                "Conjuration",
                "Destruction",
                "Illusion",
                "Restoration"
            },
            {},
            {
                "Alteration",
                "Destruction"
            }
        },

        {
            "Crafting",
            30,
            {
                "Enchanting",
                "Smithing",
                "Alchemy"
            },
            {
                "Smithing",
                "Enchanting"
            },
            {}
        },

        {
            "Utility",
            40,
            {
                "Lockpicking",
                "Pickpocket",
                "Sneak",
                "Speech"
            },
            {},
            {
                "Lockpicking",
                "Pickpocket",
                "Speech"
            }
        }
    };
    */

	if (!fs::exists(filePath))
	{
        REX::INFO("[Config Loader] Config doesn't exist. Creating defaults...");

		// Write to file
		fs::create_directories("Data/SKSE/Plugins/");
		std::ofstream file(filePath);

		if (file.is_open()) {
			json j = config;
			//file << std::setw(4) << j;		// pretty print with indent = 4
			file << j.dump(4); 
			file.close();

            REX::INFO("[Config Loader] Default Config created successfully");
		}
		else
		{
			REX::ERROR("[Config Loader] Failed to open Config file.");
		}
	}
    else    REX::INFO("[Config Loader] Existing config found. Using...");

	return config;
}

bool ConfigLoader::IfConfigChanged()
{

    
	std::filesystem::file_time_type currentWriteTime{};

	try {
		currentWriteTime = std::filesystem::last_write_time(filePath);
	}
	catch (...) {
		REX::ERROR("[Config Loader] Failed to read file modification time.");
        if (!std::filesystem::exists(filePath))  return true; //Exception to create new config file if it doesn't exist

		return false;
	}

	if (currentWriteTime != lastModifiedTime)
	{
		lastModifiedTime = currentWriteTime;
		return true;
	}

	return false;
}

void ConfigLoader::LoadConfig(Config& cfg)
{
	cfg = InitDefaultConfig();

	// Reload JSON
	std::ifstream file(filePath);
	if (file.is_open()) {
		try {
			json j;
			file >> j;
			cfg = j.get<Config>();
			REX::INFO("[Config Loader] User configs loaded successfully.");
		}
		catch (const json::parse_error& e) {
			REX::ERROR("[Config Loader] JSON parse error: {}", e.what());
		}
	}
	else {
		REX::ERROR("[Config Loader] Failed to open config file.");
	}
}
