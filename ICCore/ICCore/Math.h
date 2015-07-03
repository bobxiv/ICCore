namespace Math
{
	// Summatory => sum an array of values
	template<class _InIt, class _ValueType> 
	inline void Sum(_InIt ArrayBase, unsigned int Count, _ValueType& Result)
	{	 
		// sum( First+i + Second+i ) where 0<=i<Count to Result

		Result = _ValueType();
		for(int i=0; i < Count; (++ArrayBase,++i))
			Result += *ArrayBase;
		return;
	}

	// Productory => multiply an array of values
	template<class _InIt, class _ValueType> 
	inline void Mult(_InIt ArrayBase, unsigned int Count, _ValueType& Result)
	{	 
		// mult( First+i * Second+i ) where 0<=i<Count to Result

		Result = _ValueType();
		for(int i=0; i < Count; (++First,++Second,++i))
			Result *= *ArrayBase;
		return;
	}

	// Dot Product between 2 arrays
	template<class _InIt, class _ValueType> 
	inline void Dot(_InIt First, _InIt Second, unsigned int Count, _ValueType& Result)
	{	 
		// Dot( First+i , Second+i ) where 0<=i<Count to Result

		Result = _ValueType();
		for(int i=0; i < Count; (++First,++Second,++i))
			Result += (*First) * (*Second);
		return;
	}

	// Norm 2 of the two vectors
	template<class _InIt, class _ValueType> 
	inline void Norm(_InIt First, _InIt Second, unsigned int Count, _ValueType& Result)
	{	 
		// Norm( First+i , Second+i ) where 0<=i<Count to Result

		Result = _ValueType();
		for(int i=0; i < Count; (++First,++Second,++i))
		{
			_ValueType aux = (*First) - (*Second);
			Result += aux * aux;
		}
		Result = sqrt(Result);
		return;
	}

	// Mean Square Error Product between 2 arrays
	template<class _InIt, class _ValueType> 
	inline void MeanSquareErr(_InIt First, _InIt Second, unsigned int Count, _ValueType& Result)
	{	 
		// MeanSquareErr( First+i , Second+i ) where 0<=i<Count to Result

		Result = _ValueType();
		for(int i=0; i < Count; (++First,++Second,++i))
		{
			_ValueType aux = (*First) - (*Second);
			Result += aux * aux;
		}
		Result /= Count;
		return;
	}

	// Linear Interpolation between value A and B with coeficient coef
	// coef = 0 -> res = A			coef = 1 -> res = B
	template<class _ValueType> 
	inline _ValueType LInterpolate(_ValueType A, _ValueType B, float coef)
	{	 
		// A * (1-coef) + B * coef

		return (A * (1-coef) + B * coef);
	}

}