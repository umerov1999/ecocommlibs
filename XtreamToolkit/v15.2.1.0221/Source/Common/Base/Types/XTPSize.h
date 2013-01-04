class _XTP_EXT_CLASS CXTPSizeF
{
public:

	CXTPSizeF()
		: Width (0)
		, Height(0)
	{
	}

	CXTPSizeF(float width, float height)
		: Width (width)
		, Height(height)
	{
	}

	CXTPSizeF(const CXTPSizeF &other)
		: Width (other.Width)
		, Height(other.Height)
	{
	}

	CXTPSizeF operator+(const CXTPSizeF &other) const
	{
		return CXTPSizeF(Width + other.Width, Height + other.Height);
	}

	CXTPSizeF operator-(const CXTPSizeF &other) const
	{
		return CXTPSizeF(Width - other.Width, Height - other.Height);
	}

	bool operator ==(const CXTPSizeF &other) const
	{
		return (Width == other.Width) && (Height == other.Height);
	}

	bool IsEmpty() const
	{
		return (float(0.0) == Width && float(0.0) == Height);
	}

public:
	float Width;
	float Height;
};
