#pragma once

#include <CommandClass.h>

class AttachScriptClass : public CommandClass
{
public:
	//CommandClass
	virtual const char* GetName() const override
	{
		return "AttachScript";
	}

	virtual const wchar_t* GetUIName() const override;

	virtual const wchar_t* GetUICategory() const override;

	virtual const wchar_t* GetUIDescription() const override;

	virtual void Execute(WWKey dwUnk) const override;

};
