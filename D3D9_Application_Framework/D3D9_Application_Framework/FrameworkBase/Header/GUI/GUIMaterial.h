////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////					GUI							  //////////////////////////////////////
///////////////////////////////////					Materials					  //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class MyApp;

namespace GUIFramework
{
	//This class contains all the nesesary data to specify of to render a 
	//GUIObject's representation.
	//Its main component is the effect, but could have data to use with the effect
	class GUIMaterial
		{
		public:

			GUIMaterial(): pEffect(NULL)
			{}

			~GUIMaterial();

			//Makes the initializations for the material, could be loading data,
			//loading the effect, etc
			virtual void Initialize()
			{}

			LPD3DXEFFECT pEffect;
		};

	//This material only can contain an effect file that will be the basic one for all
	//the GUI objects. It have a few techniques, like grayscale, sepia, negative, etc
	class GUIBasicEffect: public GUIMaterial
		{
		public:
			GUIBasicEffect();

			~GUIBasicEffect();

			virtual void Initialize();

		};
}



