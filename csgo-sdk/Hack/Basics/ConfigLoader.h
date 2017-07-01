#pragma once

template <typename T>
class ConfigValue {
public:
	ConfigValue(std::string category_, std::string name_, T* value_) {
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

#define ConfigLoader CConfigLoader::GetInstance()

class CConfigLoader {
protected:
	std::vector<ConfigValue<int>*> ints;
	std::vector<ConfigValue<bool>*> bools;
	std::vector<ConfigValue<float>*> floats;
private:
	void SetupValue(int&, int, std::string, std::string);
	void SetupValue(bool&, bool, std::string, std::string);
	void SetupValue(float&, float, std::string, std::string);

	IMPLEMENT_SINGLETON_INITIALIZED(CConfigLoader);

	void Initialize() { Setup(); Load("Default"); }
public:

	void Setup();

	void Save(const char* filename);
	void Load(const char* filename);
};
