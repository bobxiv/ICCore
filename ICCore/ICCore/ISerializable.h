#pragma once

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#ifdef IC_EXPORTS
#define IC_API __declspec(dllexport)
#else
#define IC_API __declspec(dllimport)
#endif

#include <string>
#include <iostream>
#include <fstream>

// Base class of a Serializable object
class IC_API ISerializable
{
public:	

	virtual ~ISerializable()
	{}

	enum BaseFormat{ CSV, Own, XML };

	virtual void Serialize(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void Serialize(std::ostream& fileStream, BaseFormat format = BaseFormat::CSV) =0;//make output to file or cout for example
	virtual void Load(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void Load(std::istream& fileStream, BaseFormat format = BaseFormat::CSV)      =0;//get input from file or cin for example

};

// Base class of a Serializable object
class IC_API ISerializableSupervisedLearning: public ISerializable
{
public:	

	virtual ~ISerializableSupervisedLearning()
	{}

	virtual void SerializeLearnData(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void SerializeLearnData(std::ostream& fileStream, BaseFormat format = BaseFormat::CSV) =0;//make output to file or cout for example
	virtual void LoadLearnData(std::string fileName, BaseFormat format = BaseFormat::CSV);
	virtual void LoadLearnData(std::istream& fileStream, BaseFormat format = BaseFormat::CSV)      =0;//get input from file or cin for example

};

#endif