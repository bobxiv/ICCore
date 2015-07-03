#include "stdafx.h"

#include "ISerializable.h"

//================================================
/////////////Serializable Interface	//////////////
//================================================

void ISerializable::Serialize(std::string fileName, BaseFormat format) throw(std::exception)
{
	std::string extension = "";
	switch(format)
		{
		case BaseFormat::CSV:
			extension = ".csv";
			break;
		case BaseFormat::Own:
			extension = ".txt";
			break;
		case BaseFormat::XML:
			extension = ".xml";
			break;
		}

	size_t pos = fileName.find('.'); 
	if( pos == std::string::npos )
		fileName += extension;
	else
		if( fileName.substr(pos) != extension )
			fileName.replace(pos, fileName.length(), extension);

	std::ofstream fileStream(fileName.c_str());

	if( !fileStream.is_open() )
		throw std::exception( std::string( std::string("Serialize::Serialize(std::string fileName) => The file ") + 
											fileName + std::string(" cannot be created/open to be Serialized") ).c_str()
							);

	try
	{
		this->Serialize(fileStream, format);
	}catch(std::exception* e)
	{
		throw e;
	}

	fileStream.close();
}

void ISerializable::Load(std::string fileName, BaseFormat format) throw(std::exception)
{
	std::string extension = "";
	switch(format)
		{
		case BaseFormat::CSV:
			extension = ".csv";
			break;
		case BaseFormat::Own:
			extension = ".txt";
			break;
		case BaseFormat::XML:
			extension = ".xml";
			break;
		}

	size_t pos = fileName.find('.'); 
	if( pos == std::string::npos )
		fileName += extension;
	else
		if( fileName.substr(pos) != extension )
			fileName.replace(pos, fileName.length(), extension);

	std::ifstream fileStream(fileName.c_str());

	if( !fileStream.is_open() )
		throw (new std::exception( std::string(std::string("Serialize::Load(std::string fileName) => The file ")
								+ fileName + std::string(" cannot be open to be Loaded") ).c_str() )
								);

	try
	{
		this->Load(fileStream, format);
	}catch(std::exception* e)
	{
		throw e;
	}

	fileStream.close();
}


//================================================
//  Serializable SupervisedLearning Interface	//
//================================================

void ISerializableSupervisedLearning::SerializeLearnData(std::string fileName, BaseFormat format) throw(std::exception)
{
	std::string extension = "";
	switch(format)
		{
		case BaseFormat::CSV:
			extension = ".csv";
			break;
		case BaseFormat::Own:
			extension = ".txt";
			break;
		case BaseFormat::XML:
			extension = ".xml";
			break;
		}

	size_t pos = fileName.find('.'); 
	if( pos == std::string::npos )
		fileName += extension;
	else
		if( fileName.substr(pos) != extension )
			fileName.replace(pos, fileName.length(), extension);

	std::ofstream fileStream(fileName.c_str());

	if( !fileStream.is_open() )
		throw std::exception( std::string( std::string("ISerializableSupervisedLearning::SerializeLearnData(std::string fileName) => The file ") + 
											fileName + std::string(" cannot be created/open to be Serialized") ).c_str()
							);

	try
	{
		this->SerializeLearnData(fileStream, format);
	}catch(std::exception* e)
	{
		throw e;
	}

	fileStream.close();
}

void ISerializableSupervisedLearning::LoadLearnData(std::string fileName, BaseFormat format) throw(std::exception)
{
	std::string extension = "";
	switch(format)
		{
		case BaseFormat::CSV:
			extension = ".csv";
			break;
		case BaseFormat::Own:
			extension = ".txt";
			break;
		case BaseFormat::XML:
			extension = ".xml";
			break;
		}

	size_t pos = fileName.find('.'); 
	if( pos == std::string::npos )
		fileName += extension;
	else
		if( fileName.substr(pos) != extension )
			fileName.replace(pos, fileName.length(), extension);

	std::ifstream fileStream(fileName.c_str());

	if( !fileStream.is_open() )
		throw (new std::exception( std::string(std::string("ISerializableSupervisedLearning::LoadLearnData(std::string fileName) => The file ")
								+ fileName + std::string(" cannot be open to be Loaded") ).c_str() )
								);

	try
	{
		this->LoadLearnData(fileStream, format);
	}catch(std::exception* e)
	{
		throw e;
	}

	fileStream.close();
}