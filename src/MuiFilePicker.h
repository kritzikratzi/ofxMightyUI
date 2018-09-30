//
//  MuiFilePicker.hpp
//  Oscilloscope
//
//  Created by Hansi on 30.05.18.
//
//

#ifndef MuiFilePicker_hpp
#define MuiFilePicker_hpp

#include "MuiCore.h"
#include "Button.h"

namespace mui{
	
	class FilePicker : public mui::Container{
	public:
		enum class Operation{LOAD,SAVE};
		
		FilePicker(Operation op = Operation::LOAD);
		~FilePicker();
		
		
		void setOperation(Operation op);
		Operation getOperation();
		
		const std::filesystem::path & getFile() const;
		void setFile(const std::filesystem::path & file);
		void setFileAndNotify(const std::filesystem::path & file);
		
		void setDefaultSaveName(string name);
		string getDefaultSaveName();
		
		
		
		void touchDown(ofTouchEventArgs & args) override;
		void touchUp(ofTouchEventArgs & args) override;
		void touchUpOutside(ofTouchEventArgs & args) override;
		
		void drawBackground() override; 
		void layout() override;
		
		bool fileDragged(ofDragInfo & evt) override;
		
		

		mui::EventHandler<std::filesystem::path> onFileChanged{this};
		
		
		void resetFormatter();
		void setFormatter(const function<std::string(const std::filesystem::path & file)> & formatter);
		function<string(const std::filesystem::path & file)> getFormatter() const;
		
	private:
		function<std::string(const std::filesystem::path & file)> formatter;
		
		mui::Label * label;
		mui::Button * clearButton;
		std::filesystem::path selectedPath;
		bool pressed;
		Operation operation;
		string defaultSaveName;
		bool wantsPick = false; 
	};
}


#endif /* MuiFilePicker_hpp */
