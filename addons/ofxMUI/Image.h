#pragma once

namespace mui{
	
	enum ImageScaleMethod{
		ScaleMethodFitInside,
		ScaleMethodCropToFill
	};
	
	class Image : public Container{
	public:
		Image( string filename, float x_ = 0, float y_ = 0, float width_ = 200, float height_ = 20 );
		
		virtual void update();
		virtual void draw();
		
		virtual void touchDown( ofTouchEventArgs &touch );
		virtual void touchMoved( ofTouchEventArgs &touch );
		virtual void touchUp( ofTouchEventArgs &touch );
		virtual void touchDoubleTap( ofTouchEventArgs &touch );
		
		virtual void autoHeight();
		
		// assign if you take care of memory stuff yourself
		ofTexture * texRef;
		
		// assign if you want it automatically
		ofTexture tex;
		
		HorizontalAlign horizontalAlign;
		VerticalAlign verticalAlign;
		ImageScaleMethod scaleMethod;
		
	private:
		ofRectangle getDrawRect();
	};
};
