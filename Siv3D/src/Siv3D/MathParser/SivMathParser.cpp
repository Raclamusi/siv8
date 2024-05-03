﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2024 Ryo Suzuki
//	Copyright (c) 2016-2024 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/MathParser.hpp>
# include "MathParserDetail.hpp"

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	MathParser::MathParser()
		: pImpl{ std::make_shared<MathParserDetail>() } {}

	MathParser::MathParser(const StringView expression)
		: MathParser{}
	{
		setExpression(expression);
	}

	////////////////////////////////////////////////////////////////
	//
	//	isValid
	//
	////////////////////////////////////////////////////////////////

	bool MathParser::isValid() const
	{
		return std::isfinite(pImpl->eval());
	}

	////////////////////////////////////////////////////////////////
	//
	//	operator bool
	//
	////////////////////////////////////////////////////////////////

	MathParser::operator bool() const
	{
		return std::isfinite(pImpl->eval());
	}

	////////////////////////////////////////////////////////////////
	//
	//	getErrorMessage
	//
	////////////////////////////////////////////////////////////////

	String MathParser::getErrorMessage() const
	{
		return pImpl->getErrorMessage();
	}

	////////////////////////////////////////////////////////////////
	//
	//	setExpression
	//
	////////////////////////////////////////////////////////////////

	void MathParser::setExpression(const StringView expression)
	{
		pImpl->setExpression(expression);
	}

	////////////////////////////////////////////////////////////////
	//
	//	setConstant
	//
	////////////////////////////////////////////////////////////////

	bool MathParser::setConstant(const StringView name, const double value)
	{
		return pImpl->setConstant(name, value);
	}

	////////////////////////////////////////////////////////////////
	//
	//	setVariable
	//
	////////////////////////////////////////////////////////////////

	bool MathParser::setVariable(const StringView name, double* value)
	{
		return pImpl->setVariable(name, value);
	}

	////////////////////////////////////////////////////////////////
	//
	//	setFunction
	//
	////////////////////////////////////////////////////////////////

	bool MathParser::setFunction(const StringView name, Fty0 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty1 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty2 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty3 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty4 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty5 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty6 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty7 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty8 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty9 f)
	{
		return pImpl->setFunction(name, f);
	}

	bool MathParser::setFunction(const StringView name, Fty10 f)
	{
		return pImpl->setFunction(name, f);
	}

	////////////////////////////////////////////////////////////////
	//
	//	setPrefixOperator
	//
	////////////////////////////////////////////////////////////////

	bool MathParser::setPrefixOperator(const StringView name, Fty1 f)
	{
		return pImpl->setPrefixOperator(name, f);
	}

	////////////////////////////////////////////////////////////////
	//
	//	setPostfixOperator
	//
	////////////////////////////////////////////////////////////////

	bool MathParser::setPostfixOperator(const StringView name, Fty1 f)
	{
		return pImpl->setPostfixOperator(name, f);
	}

	////////////////////////////////////////////////////////////////
	//
	//	removeVariable
	//
	////////////////////////////////////////////////////////////////

	void MathParser::removeVariable(const StringView name)
	{
		pImpl->removeVariable(name);
	}

	////////////////////////////////////////////////////////////////
	//
	//	clearVariables
	//
	////////////////////////////////////////////////////////////////

	void MathParser::clearVariables()
	{
		pImpl->clearVariables();
	}

	////////////////////////////////////////////////////////////////
	//
	//	clearFunctions
	//
	////////////////////////////////////////////////////////////////

	void MathParser::clearFunctions()
	{
		pImpl->clearFunctions();
	}

	////////////////////////////////////////////////////////////////
	//
	//	clearOperators
	//
	////////////////////////////////////////////////////////////////

	void MathParser::clearOperators()
	{
		pImpl->clearOperators();
	}

	////////////////////////////////////////////////////////////////
	//
	//	clear
	//
	////////////////////////////////////////////////////////////////

	void MathParser::clear()
	{
		pImpl->clear();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getExpression
	//
	////////////////////////////////////////////////////////////////

	String MathParser::getExpression() const
	{
		return pImpl->getExpression();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getUsedVariables
	//
	////////////////////////////////////////////////////////////////

	HashTable<String, double*> MathParser::getUsedVariables() const
	{
		return pImpl->getUsedVariables();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getVariables
	//
	////////////////////////////////////////////////////////////////

	HashTable<String, double*> MathParser::getVariables() const
	{
		return pImpl->getVariables();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getConstants
	//
	////////////////////////////////////////////////////////////////

	HashTable<String, double> MathParser::getConstants() const
	{
		return pImpl->getConstants();
	}

	////////////////////////////////////////////////////////////////
	//
	//	validNameCharacters
	//
	////////////////////////////////////////////////////////////////

	String MathParser::validNameCharacters() const
	{
		return pImpl->validNameCharacters();
	}

	////////////////////////////////////////////////////////////////
	//
	//	validPrefixCharacters
	//
	////////////////////////////////////////////////////////////////

	String MathParser::validPrefixCharacters() const
	{
		return pImpl->validPrefixCharacters();
	}

	////////////////////////////////////////////////////////////////
	//
	//	validPostfixCharacters
	//
	////////////////////////////////////////////////////////////////

	String MathParser::validPostfixCharacters() const
	{
		return pImpl->validPostfixCharacters();
	}

	////////////////////////////////////////////////////////////////
	//
	//	eval
	//
	////////////////////////////////////////////////////////////////

	double MathParser::eval() const
	{
		return pImpl->eval();
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalOr
	//
	////////////////////////////////////////////////////////////////

	double MathParser::evalOr(const double defaultValue) const
	{
		const double result = pImpl->eval();

		if (std::isfinite(result))
		{
			return result;
		}
		else
		{
			return defaultValue;
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalOpt
	//
	////////////////////////////////////////////////////////////////

	Optional<double> MathParser::evalOpt() const
	{
		const double result = pImpl->eval();

		if (std::isfinite(result))
		{
			return result;
		}
		else
		{
			return none;
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalArray
	//
	////////////////////////////////////////////////////////////////

	Array<double> MathParser::evalArray() const
	{
		return pImpl->evalArray();
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalVec2
	//
	////////////////////////////////////////////////////////////////

	Vec2 MathParser::evalVec2() const
	{
		Vec2 xy{};
		pImpl->eval(xy.getPointer(), 2);
		return xy;
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalVec3
	//
	////////////////////////////////////////////////////////////////

	Vec3 MathParser::evalVec3() const
	{
		Vec3 xyz{};
		pImpl->eval(xyz.getPointer(), 3);
		return xyz;
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalVec4
	//
	////////////////////////////////////////////////////////////////

	Vec4 MathParser::evalVec4() const
	{
		Vec4 xyzw{};
		pImpl->eval(xyzw.getPointer(), 4);
		return xyzw;
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalColorF
	//
	////////////////////////////////////////////////////////////////

	ColorF MathParser::evalColorF() const
	{
		ColorF rgba{};
		pImpl->eval(rgba.getPointer(), 4);
		return rgba;
	}

	////////////////////////////////////////////////////////////////
	//
	//	evalHSV
	//
	////////////////////////////////////////////////////////////////

	HSV MathParser::evalHSV() const
	{
		HSV hsva{};
		pImpl->eval(&hsva.h, 4);
		return hsva;
	}

	////////////////////////////////////////////////////////////////
	//
	//	Eval
	//
	////////////////////////////////////////////////////////////////

	double Eval(const StringView expression)
	{
		return MathParser{ expression }.eval();
	}

	////////////////////////////////////////////////////////////////
	//
	//	EvalOpt
	//
	////////////////////////////////////////////////////////////////

	Optional<double> EvalOpt(const StringView expression)
	{
		return MathParser{ expression }.evalOpt();
	}
}
