//
//  TextureAtlas.h
//  OsciStudio
//
//  Created by Hansi on 23.04.15.
//
//

#ifndef __Mui__TextureAtlas__
#define __Mui__TextureAtlas__

#include <stdio.h>
#include <ofMain.h>
#include <map>

namespace mui{
	class TextureAtlas{
	public: 
		TextureAtlas();
		
		void load( string absXmlPath, string absImgPath );
		
		void bind();
		void unbind();
		
		// binding must be done manually before calling draw!
		void draw( string name, float x, float y );
		void draw( string name, float x, float y, float w, float h );
		void draw( float x, float y, float w, float h, const ofRectangle & rectInAtlas );
		void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);

		// these are for manual drawing. if you need to draw lots of thingies
		// it's faster to collect all the points and then draw everything at once.
		void addDraw( string name, float x, float y, float w, float h );
		void addDraw( float x, float y, float w, float h, const ofRectangle & rectInAtlas );
		void addSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh);
		void drawAdded();
		
		// returns a reference to the rectangle containing the coordinates.
		const ofRectangle & getRectRef( string name );
		
		static const ofRectangle NOT_FOUND;
		
		
		bool loaded;
		ofTexture tex;
		map<string,ofRectangle> rects;
		ofMesh quad;
		ofMesh add;
	};
	
}

#endif /* defined(__OsciStudio__TextureAtlas__) */
