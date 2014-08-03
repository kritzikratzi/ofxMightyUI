


#include "ofxFBOTexture.h"

#ifdef TARGET_OPENGLES
#include "glu.h"
#define ofxFBO_FBO "GL_OES_framebuffer_object"
#define ofxFBO_BLIT "GL_OES_framebuffer_blit"
#define ofxFBO_MULTISAMPLE "GL_OES_framebuffer_multisample"
#define ofxFBO_MAX_RENDERBUFFER_SIZE GL_MAX_RENDERBUFFER_SIZE_OES

#else
#define ofxFBO_FBO "GL_EXT_framebuffer_object"
#define ofxFBO_BLIT "GL_EXT_framebuffer_blit"
#define ofxFBO_MULTISAMPLE "GL_EXT_framebuffer_multisample"
#define ofxFBO_MAX_RENDERBUFFER_SIZE GL_MAX_RENDERBUFFER_SIZE_EXT
#endif

//NIMOY static vars
bool ofxFBOTexture::bSupportChecked = false;
bool ofxFBOTexture::bSupportsFBO;
bool ofxFBOTexture::bSupportsBlit;
bool ofxFBOTexture::bSupportsMulti;
GLint ofxFBOTexture::maxTextureSize;
GLint ofxFBOTexture::maxRenderBufferSize;
GLint ofxFBOTexture::maxSamples;

ofxFBOTexture::ofxFBOTexture() {
	screenFov		= 60;
	oldFramebuffer	= 0;
	//	oldRenderbuffer = 0;
	pixels			= NULL;
	bReading = false;
	supportCheck();
}

ofxFBOTexture::~ofxFBOTexture() {
	clean();
	supportCheck();
}

void ofxFBOTexture::allocate(int w, int h, int internalGlDataType, int numSamples) {
	_isActive = false;


	//NIMOY OpenGL supported technologies checking.
	
	if(!bSupportsFBO){
		ofLog(OF_LOG_ERROR, "ofxFBOTexture: GL_EXT_framebuffer_object not supported by current OpenGL renderer. "
							" For Apple machines, more info at http://developer.apple.com/graphicsimaging/opengl/capabilities");
		return;
	}
	
	if(!bSupportsMulti && numSamples){
		ofLog(OF_LOG_WARNING, "ofxFBOTexture: GL_EXT_framebuffer_multisample not supported by current OpenGL renderer."
							" Falling back to non-multisampled mode. To disable this message, specify 0 samples."
							" Apple machines, more info at http://developer.apple.com/graphicsimaging/opengl/capabilities");
		numSamples = 0;
	}
	
	if(!bSupportsBlit && numSamples){
		
		//NIMOY if there's no blit, then it's not worth using multi
		
		ofLog(OF_LOG_WARNING, "ofxFBOTexture: GL_EXT_framebuffer_blit not supported by current OpenGL renderer."
			  " Falling back to non-multisampled mode. To disable this message, specify 0 samples."
			  " Apple machines, more info at http://developer.apple.com/graphicsimaging/opengl/capabilities");
		numSamples = 0;
	}
	
	
	if(maxSamples < numSamples){
		ofLog(OF_LOG_WARNING, "ofxFBOTexture: requested samples too high. Using GL_SAMPLES instead.");
		numSamples = maxSamples;
	}
	
	
	//NIMOY validate requested dimensions to see that they are within limits.
	
	if(maxTextureSize < w || maxRenderBufferSize < w){
		ofLog(OF_LOG_WARNING, "ofxFBOTexture: requested width was too large. Using largest allowed value instead.");
		if(maxTextureSize < w) w = maxTextureSize;
		if(maxRenderBufferSize < w) w = maxRenderBufferSize;
	}
	
	if( maxTextureSize < h || maxRenderBufferSize < h){
		ofLog(OF_LOG_WARNING, "ofxFBOTexture: requested height was too large. Using largest allowed value instead.");
		if(maxTextureSize < h) h = maxTextureSize;
		if(maxRenderBufferSize < h) h = maxRenderBufferSize;
	}
	

	/**
		 NIMOY validates arg3 for legacy function calling style
		 i dont check for 1 because 1 is a valid internal color format
		 so we're not sure if the person is passing a boolean or a color format
		 so we can't accuse them of doing the legacy boolean thing.
		 Even if that stops the FBO, the other color format error will help them out.
	 */
	if(internalGlDataType==0){
		ofLog(OF_LOG_WARNING, "ofxFBOTexture::allocate( _ , _ , HERE , _ ) "
			  "The calling statement passed a boolean value like an older version of FBO object. "
			  "This argument has changed to be a color format "
			  "like GL_RGB, GL_RGBA, GL_RGBA16F_ARB, and GL_RGBA32F_ARB. "
			  "Defaulting to GL_RGBA.");
		
		internalGlDataType = GL_RGBA;
	}
	
	
	
	// attempt to free the previous bound texture, if we can:
	clean();
	
	texData.width = w;
	texData.height = h;
	this->numSamples = numSamples;
	
#ifndef TARGET_OPENGLES	
    if (GLEE_ARB_texture_rectangle){
        texData.tex_w = w;
        texData.tex_h = h;
        texData.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    } else
#endif	
	{	
        texData.tex_w = ofNextPow2(w);
        texData.tex_h = ofNextPow2(h);
    }
	
#ifndef TARGET_OPENGLES	
	if (GLEE_ARB_texture_rectangle){
		texData.tex_t = w;
		texData.tex_u = h;
	} else
#endif	
	{
		texData.tex_t = w/texData.tex_w;
		texData.tex_u = h/texData.tex_h;
	}
	
	texData.width = w;
	texData.height = h;
	texData.bFlipTexture = true;
	texData.glTypeInternal = internalGlDataType;
	
#ifndef TARGET_OPENGLES	
	// hansi
	/*switch(texData.glTypeInternal) {
		case GL_RGBA32F_ARB:
		case GL_RGBA16F_ARB:
			texData.glType		= GL_RGBA;
			texData.pixelType	= GL_FLOAT;
			pixels				= new float[w * h * 4];
			break;			
		default:
			texData.glType		= texData.glTypeInternal;
			texData.pixelType	= GL_UNSIGNED_BYTE;
			pixels				= new float[w * h * 4];
			break;
	}*/
#else
	// hansi
//	texData.glType		= GL_RGBA;
//	texData.pixelType	= GL_UNSIGNED_BYTE;
	pixels				= new unsigned char[w * h * 4];
#endif			
	
	
	// create & setup texture
	glGenTextures(1, (GLuint *)&texData.textureID);   // could be more then one, but for now, just one
	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// hansi: changed from texData.pixelType to GL_UNSIGNED_BYTE
	glTexImage2D(texData.textureTarget, 0, texData.glTypeInternal, texData.tex_w, texData.tex_h, 0, texData.glTypeInternal, GL_UNSIGNED_BYTE, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	
#ifndef TARGET_OPENGLES
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, (GLint *) &oldFramebuffer);
	
	if(numSamples ){
		// MULTISAMPLE //
		
		//THEO Create the render buffer for depth
		glGenRenderbuffersEXT(1, &depthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
		glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, numSamples, GL_DEPTH_COMPONENT, texData.tex_w, texData.tex_h);
		
		//THEO multi sampled color buffer
		glGenRenderbuffersEXT(1, &colorBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, colorBuffer);
		glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, numSamples, texData.glTypeInternal, texData.tex_w, texData.tex_h);
		
		//THEO create fbo for multi sampled content and attach depth and color buffers to it
		glGenFramebuffersEXT(1, &mfbo);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfbo);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colorBuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
		
	}else{
		//THEO Create the render buffer for depth
		glGenRenderbuffersEXT(1, &depthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, texData.tex_w, texData.tex_h);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	}
	
	// create & setup FBO
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
	// attach it to the FBO so we can render to it
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, texData.textureTarget, (GLuint)texData.textureID, 0);
	
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		
		//NIMOY validate arg3 after the fact because it's worth letting them try that format.
		
		if(internalGlDataType != GL_RGBA &&
		   internalGlDataType != GL_RGB &&
		   internalGlDataType != GL_RGBA16F_ARB &&
		   internalGlDataType != GL_RGBA32F_ARB){
			
			ofLog(OF_LOG_ERROR,  "ofxFBOTexture: Failed to initialize. "
				  "I noticed that the calling statement did not passed an expected color format "
				  "like GL_RGB, GL_RGBA, GL_RGBA16F_ARB, and GL_RGBA32F_ARB. You might try one of those. "
				  "ofxFBOTexture::allocate( _ , _ , HERE , _ ) ");
			
		}else{
			ofLog(OF_LOG_ERROR, "ofxFBOTexture: Failed to initialize.");
		}
		
		
		
		std::exit(1);
	}
	clear(0, 0, 0, 0);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFramebuffer);
#else
	
	if(numSamples ){
		ofLog(OF_LOG_WARNING, "Multi-sampling not supported on OpenGL ES");
	}else{
	}
	
	
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
	
	// create & setup FBO
	glGenFramebuffersOES(1, &fbo);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo);
	
	// attach it to the FBO so we can render to it
	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, texData.textureTarget, (GLuint)texData.textureID, 0);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFramebuffer);
	
#endif	
	
	
	texData.bAllocated = true;
}

void ofxFBOTexture::setupScreenForMe(){
	
	int w = texData.width;
	int h = texData.height;
	
	float halfFov, theTan, aspect;
	
	float eyeX              = (float)w / 2.0;
	float eyeY              = (float)h / 2.0;
	halfFov                 = PI * screenFov / 360.0;
	theTan                  = tanf(halfFov);
	float dist              = eyeY / theTan;
	float nearDist  = dist / 10.0;  // near / far clip plane
	float farDist   = dist * 10.0;
	aspect                  = (float)w/(float)h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
	
	
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
	
    glViewport(0,0,texData.width, texData.height);
	
}

void ofxFBOTexture::setupScreenForThem(){
	//      glPopAttrib();
	//      return;
	
    int w, h;
	
	w = ofGetWidth();
	h = ofGetHeight();
	
	float halfFov, theTan, aspect;
	
	float eyeX              = (float)w / 2.0;
	float eyeY              = (float)h / 2.0;
	halfFov                 = PI * screenFov / 360.0;
	theTan                  = tanf(halfFov);
	float dist              = eyeY / theTan;
	float nearDist			= dist / 10.0;  // near / far clip plane
	float farDist			= dist * 10.0;
	aspect                  = (float)w/(float)h;
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
	
	
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
	
	
    glViewport(0,0,w, h);
}

void ofxFBOTexture::swapIn() {
	_isActive = true;
	
#ifndef TARGET_OPENGLES
	//	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, (GLint *) &oldFramebuffer);
	if(numSamples){
		// THEO - we bind to the multisampled FBO 
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, bReading?fbo:mfbo);
		glPushAttrib(GL_VIEWPORT_BIT);
	}else{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // bind the FBO to the screen so we can draw to it
	}
#else
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo); // bind the FBO to the screen so we can draw to it
#endif	
	
}


void ofxFBOTexture::swapOut() {
	_isActive = false;
	
#ifndef TARGET_OPENGLES
	if(numSamples){
		// WE THEN COPY THE MULTISAMPLED FBO TO THE TEXTURED FBO
		glPopAttrib();
		glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, mfbo);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo);
		glBlitFramebufferEXT(0, 0, texData.tex_w, texData.tex_h, 0, 0, texData.tex_w, texData.tex_h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFramebuffer); //unbind the FBO
#else
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFramebuffer);
	
#endif
}


void ofxFBOTexture::clean() {
#ifndef TARGET_OPENGLES	
	switch(texData.glTypeInternal) {
		case GL_RGBA32F_ARB:
		case GL_RGBA16F_ARB:
			texData.glType		= GL_RGBA;
			texData.pixelType	= GL_FLOAT;
			delete [](float*)pixels;
			break;
		default:
			texData.glType		= texData.glTypeInternal;
			texData.pixelType	= GL_UNSIGNED_BYTE;
			delete [](unsigned char*)pixels;
	}
#else
	// hansi:
//	texData.glType		= GL_RGBA;
//	texData.pixelType	= GL_UNSIGNED_BYTE;
	delete [](unsigned char*)pixels;
#endif		
	
	
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (texData.textureID != 0){
#ifndef TARGET_OPENGLES		
		glDeleteFramebuffersEXT(1, &fbo);
		glDeleteRenderbuffersEXT(1, &depthBuffer);
		
		if(numSamples){
			glDeleteFramebuffersEXT(1, &mfbo);
			glDeleteRenderbuffersEXT(1, &colorBuffer);
		}
#else
		glDeleteFramebuffersOES(1, &fbo);
#endif		
		
		glDeleteTextures(1, (GLuint *)&texData.textureID);
	}
	texData.width = 0;
	texData.height = 0;
	texData.bFlipTexture = false;
}

void ofxFBOTexture::clear() {
	bool alreadyIn = _isActive;
	if(!alreadyIn) begin();         // MEMO
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear Screen data of the texture to write on
	if(!alreadyIn) end();           // MEMO
}

void ofxFBOTexture::clear(float r, float g, float b, float a) {
    clearColor[0] = r;
    clearColor[1] = g;
    clearColor[2] = b;
    clearColor[3] = a;
    clear();
}

void ofxFBOTexture::bindAsTexture(){
	glBindTexture(GL_TEXTURE_2D, (GLuint)&texData.textureID);
}

void ofxFBOTexture::begin() {
	swapIn();
	setupScreenForMe();
}

void ofxFBOTexture::end() {
	swapOut();
	setupScreenForThem();
}



void *ofxFBOTexture::getPixels() {
	bool alreadyIn = _isActive;   // save current state of fbo (bound or not)
	bReading = true;
	if(!alreadyIn) begin();			// if it isn't bound, bind it
	#ifndef TARGET_OPENGLES
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT); 
	#endif
	// hansi: changed from texData.pixelType to GL_UNSIGNED_BYTE
	glReadPixels(0, 0, texData.width, texData.height, texData.glTypeInternal, GL_UNSIGNED_BYTE, pixels);
	if(!alreadyIn) end();  // if fbo wasn't bound when the function was called, unbind it
	bReading = false;
	return pixels;
}


//NIMOY checks the grand GL_EXTENSIONS string for a token
bool ofxFBOTexture::isExtensionSupported(const char *extension){
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;
	
	// Extension names should not have spaces
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')return false;
	extensions = glGetString(GL_EXTENSIONS);
	// It takes a bit of care to be fool-proof about parsing the
	// OpenGL extensions string. Don't be fooled by sub-strings,
	// etc.
	start = extensions;
	for (;;) {
		where = (GLubyte *) strstr((const char *) start, extension);
		if (!where)break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;
		start = terminator;
	}
	return false;
}


//NIMOY caches various system checks
void ofxFBOTexture::supportCheck(){
	if(!bSupportChecked){
		bSupportsFBO = isExtensionSupported( ofxFBO_FBO );
		bSupportsBlit = isExtensionSupported( ofxFBO_BLIT );
		bSupportsMulti = isExtensionSupported( ofxFBO_MULTISAMPLE );
		
		glGetIntegerv(ofxFBO_MAX_RENDERBUFFER_SIZE,&maxRenderBufferSize);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxTextureSize);
		glGetIntegerv(GL_SAMPLES,&maxSamples);
		
		bSupportChecked = true;
	}
}

