#pragma once

#include "const.h"

struct SectionInfo
{
	SectionInfo() {};
	~SectionInfo() { _section_data.clear(); };

	SectionInfo(const SectionInfo& src)
	{
		_section_data = src._section_data;
	};

	SectionInfo& operator = (const SectionInfo& src)
	{
		// �������Լ������Լ�
		if (&src == this)
		{
			return *this;
		}
		this->_section_data = src._section_data;
		return *this;
	};

	std::map<std::string, std::string> _section_data;

	// ����[]�������Ϊ�˷�ֹû�ж�Ӧ��key����
	std::string operator[](const std::string& key)
	{
		if (_section_data.find(key) == _section_data.end())
		{
			return "";
		}
		return _section_data[key];
	}
};

class ConfigMgr
{
public:
	~ConfigMgr()
	{
		_config_map.clear();
	};

	// ����[],Ҳ��Ϊ�˷�ֹ��key������ʱ����
	SectionInfo operator[](const std::string& section)
	{
		if (_config_map.find(section) == _config_map.end())
		{
			return SectionInfo();
		}
		return _config_map[section];
	};

	static ConfigMgr& Inst()
	{
		// �ֲ���̬�������������������ͬ������֤�̰߳�ȫ�����̷߳��ʿ���ȷ������һ��
		static ConfigMgr cfg_mgr;
		return cfg_mgr;
	};

	ConfigMgr(const ConfigMgr& src)
	{
		_config_map = src._config_map;
	};

	ConfigMgr& operator = (const ConfigMgr& src)
	{
		// �������Լ������Լ�
		if (&src == this)
		{
			return *this;
		}
		this->_config_map = src._config_map;
	};

private:
	ConfigMgr();
	std::map<std::string, SectionInfo> _config_map;
};

