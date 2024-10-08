#pragma once

class ElementBuffer
{
public:
	ElementBuffer();
	ElementBuffer(const void* data, unsigned int count);
	~ElementBuffer();

	unsigned int getID() const;
	unsigned int getCount() const;
	void bind() const;
	void unbind() const;

	void setElements(const void* data, unsigned int count);

private:
	unsigned int m_ID;
	unsigned int m_count;
};