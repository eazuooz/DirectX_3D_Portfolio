#include "DATATYPE.h"
#include <iostream>


DataType::DataType() : m_Info(nullptr)
{
}


DataType::~DataType()
{
}

// 기초.. 
void DataType::TypeSetting()
{
	
	// 문법적으로 봤을때 
	m_Info = &typeid(*this);




	std::cout << m_Info->name() << std::endl;
}