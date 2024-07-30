#pragma once

class UIComponent
{
public:
	UIComponent() = default;
	virtual ~UIComponent() = default;

	virtual void render() = 0;
};